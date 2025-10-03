#include "dolphin/gx/GXGeometry.h"
#include "dolphin/gx/GXTexture.h"
#include "gz/draw.h"
#include "gz/texture.h"

Texture blankTex;

void begin(u16 n, u8 primitive, GXTexObj* tex) {
    GXLoadTexObj(tex, GX_TEXMAP0);
    GXBegin((GXPrimitive)primitive, GX_VTXFMT0, n);
}

void begin(u16 n, u8 primitive) {
    begin(n, primitive, &blankTex._texObj);
}

void begin(u16 n) {
    begin(n, GX_TRIANGLESTRIP);
}

void begin(u16 n, GXTexObj* tex) {
    begin(n, GX_TRIANGLESTRIP, tex);
}

void end() {
    GXEnd();
}

void add_vertex(u32 color, Vec2 point, Vec2 tex) {
    GXPosition2f32(point.x, point.y);
    GXColor1u32(color);
    GXTexCoord2f32(tex.x, tex.y);
}

void drawQuad(u32 color, Vec2 p[4], Vec2 tex[4], GXTexObj* texture) {
    begin(4, texture);
    add_vertex(color, p[0], tex[0]);
    add_vertex(color, p[1], tex[1]);
    add_vertex(color, p[3], tex[3]);
    add_vertex(color, p[2], tex[2]);
    end();
}
