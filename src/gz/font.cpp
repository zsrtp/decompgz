#include "gz/font.h"
#include "gz/disc.h"
#include "gz/draw.h"
#include "dolphin/dvd.h"
#include "m_Do/m_Do_ext.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "SSystem/SComponent/c_lib.h"
#include "string.h"

#define isWidescreen (false)

FontCode Font::loadFont(const char* path) {
    DVDFileInfo fileInfo;
    int readsize;
    if (font.loadCode == FNT_OK) {
        free_font();
    }

    if (!DVDOpen(path, &fileInfo)) {
        font.loadCode = FNT_ERR_FILE;
        return font.loadCode;
    }
    readsize = dvd_read(&fileInfo, &font.header, sizeof(_FontHeader), 0);
    if (readsize < (int)sizeof(_FontHeader)) {
        DVDClose(&fileInfo);
        font.loadCode = FNT_ERR_READ;
        return font.loadCode;
    }

    u32 size = font.header.glyph_count * sizeof(_Glyph);
    font.glyphs = new ((JKRHeap*)mDoExt_createSolidHeapFromSystem(0, 0), 0) _Glyph[font.header.glyph_count];
    if (font.glyphs == NULL) {
        DVDClose(&fileInfo);
        font.loadCode = FNT_ERR_MEM;
        return font.loadCode;
    }

    if (DVDReadPrio(&fileInfo, font.glyphs, size, sizeof(font.header), 2) < (int)size) {
        delete font.glyphs;
        DVDClose(&fileInfo);
        font.loadCode = FNT_ERR_READ;
        return font.loadCode;
    }

    if (load_texture_offset(path, &font.texture, sizeof(font.header) + size) != TEX_OK) {
        delete font.glyphs;
        DVDClose(&fileInfo);
        font.loadCode = FNT_ERR_TEXTURE;
        return font.loadCode;
    }
    DVDClose(&fileInfo);

    font.loadCode = FNT_OK;
    return font.loadCode;
}

void Font::free_font() {
    if (font.glyphs != NULL) {
        delete font.glyphs;
        font.glyphs = 0;
    }
    free_texture(&font.texture);
    cLib_memSet(&font, 0, sizeof(_Font));
    // The next line is redundant, but is still there for good measure
    font.loadCode = FNT_UNLOADED;
}

void PositionedGlyph::render(u32 color, Texture* texture) {
    drawQuad(color, vertices, tex_coords, &texture->_texObj);
}

PositionedGlyph DecodedGlyph::position(f32 _x, f32 _y, f32 factor) {
    f32 vty = _y - metrics.ascender * factor;
    f32 vby = _y + metrics.descender * factor;
    f32 vrx = _x + (width + offset) * factor * (isWidescreen ? 0.75f : 1.0f);
    f32 vlx = _x + offset * factor * (isWidescreen ? 0.75f : 1.0f);

    f32 tty = glyph->minY;
    f32 tby = glyph->maxY;
    f32 trx = glyph->maxX;
    f32 tlx = glyph->minX;

    PositionedGlyph result;
    result.vertices[0].x = vlx; result.vertices[0].y = vty;
    result.vertices[1].x = vrx; result.vertices[1].y = vty;
    result.vertices[2].x = vrx; result.vertices[2].y = vby;
    result.vertices[3].x = vlx; result.vertices[3].y = vby;
    result.tex_coords[0].x = tlx; result.tex_coords[0].y = tty;
    result.tex_coords[1].x = trx; result.tex_coords[1].y = tty;
    result.tex_coords[2].x = trx; result.tex_coords[2].y = tby;
    result.tex_coords[3].x = tlx; result.tex_coords[3].y = tby;
    result.next_x = _x + width * factor * (isWidescreen ? 0.75f : 1.0f);
    return result;
}

bool Font::lookupGlyph(char c, DecodedGlyph& glyph) {
    u8 idx = c;
    if (idx < 0 || idx >= font.header.glyph_count) {
        return false;
    }
    glyph.glyph = &font.glyphs[idx];
    glyph.width = font.glyphs[idx].width;
    glyph.offset = font.glyphs[idx].offset;
    glyph.metrics = font.header.metrics;
    return true;
}

f32 Font::renderChar(char c, f32 x, f32 y, u32 color, f32 size) {
    DecodedGlyph glyph;
    if (lookupGlyph(c, glyph)) {
        PositionedGlyph positioned = glyph.position(x, y, size / font.header.base_size);
        positioned.render(color, &font.texture);
        return positioned.next_x;
    } else {
        return x + font.glyphs[' '].width * size / font.header.base_size *
                       (isWidescreen ? 0.75f : 1.0f);
    }
}

void Font::renderChars(const char* str, f32 x, f32 y, u32 color, f32 size) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        x = renderChar(str[i], x, y, color, size);
    }
}

void Font::GZ_drawChar(char c, f32 x, f32 y, u32 color, bool drop_shadows,
                                 f32 size) {
    if (drop_shadows) {
        renderChar(c, x + 1.0f, y + 1.0f, DROP_SHADOWS_RGBA, size);
    }
    renderChar(c, x, y, color, size);
}

void Font::GZ_drawStr(const char* str, f32 x, f32 y, u32 color,
                                bool drop_shadows, f32 size) {
    if (drop_shadows) {
        renderChars(str, x + 1.0f, y + 1.0f, DROP_SHADOWS_RGBA, size);
    }
    renderChars(str, x, y, color, size);
}

f32 Font::getCharWidth(char c, f32 size) {
    DecodedGlyph glyph;
    if (lookupGlyph(c, glyph)) {
        return glyph.width * size / font.header.base_size * (isWidescreen ? 0.75f : 1.0f);
    } else {
        return font.glyphs[' '].width * size / font.header.base_size *
               (isWidescreen ? 0.75f : 1.0f);
    }
}

f32 Font::getMaxCharRangeWidth(char start, char end, f32 size) {
    f32 max_width = 0.f;
    for (char c = start; c <= end; c++) {
        max_width = MAX(max_width, getCharWidth(c, size));
    }
    return max_width;
}

f32 Font::getMaxCharWidth(f32 size) {
    f32 max_width = font.glyphs[' '].width;
    for (size_t idx = 0; idx < font.header.glyph_count; ++idx) {
        max_width = MAX(max_width, font.glyphs[idx].width);
    }
    return max_width * size / font.header.base_size * (isWidescreen ? 0.75f : 1.0f);
}

f32 Font::getStrWidth(const char* str, f32 size) {
    int len = strlen(str);
    f32 str_size = 0.f;
    for (int i = 0; i < len; i++) {
        str_size += getCharWidth(str[i], size);
    }
    return str_size;
}

// returns the width of the rendered string
f32 GZ_drawSelectChar(const char* str, f32 x, f32 y, size_t char_idx,
                                  size_t max_char, u32 color) {
    f32 pos = 0.0f;
    for (size_t i = 0; i <= max_char; ++i) {
        Font::GZ_drawChar(str[i], x + pos, y, char_idx == i ? 0xFF000000 : color,
                          true);
        pos += Font::getCharWidth(str[i]);
    }
    return pos;
}
