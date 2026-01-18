#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu.h"
#include "d/d_select_cursor.h"

void gzMenu_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();
    s32 visibleLines = gzInfo_getVisibleLines();

    if (gzPad::getTrigDown() && !gzInfo_isMenuOption()) {
        l_cursor->y = (l_cursor->y + 1) % visibleLines;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigUp() && !gzInfo_isMenuOption()) {
        l_cursor->y = (l_cursor->y == 0) ? visibleLines - 1 : l_cursor->y - 1;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigB()) {
        gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
    }

    if (gzPad::getTrigA()) {
        gzInfo_isMenuOption() ? gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK) : gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
    }

    updateScrolling(visibleLines);
    mpHaihai->_execute(0);
}

void gzMenu_c::drawLines(gzTextBox** lines, gzTextBox** lineOptions, u8 haihaiFlags, s32 numLines) {
    gzCursor* l_cursor = gzInfo_getCursor();

    J2DTextBox::TFontSize font_size;
    lines[0]->getFontSize(font_size);
    mpHaihai->setScale(font_size.mSizeY * 0.04f);

    f32 lineX = mXPos;
    f32 lineY_start = g_gzInfo.mBackgroundYPos + 78.0f;
    f32 line_spacing = g_gzInfo.mBackgroundHeight / 20;

    f32 optionX = mXPos - 20.0f;
    f32 haihaiX = optionX + 305.0f;

    u32 cursorColor = gzInfo_getCursorColor();

    for (int i = 0; i < numLines; i++) {
        f32 lineY = lineY_start + ((i - 1) * line_spacing);
        f32 haihaiY = lineY - 7.0f;
        bool isSelected = (l_cursor->y == i && gzInfo_isSubMenuVisible());
        
        f32 haihaiWidth = lineOptions[i]->mBounds.f.x + 30.0f; // extra spacing so text fits inside the haihai bounds
        bool showHaihai = gzInfo_isMenuOption() && l_cursor->y == i;

        drawLineWithOption(lines[i], lineOptions[i], lineX, optionX, lineY, isSelected, cursorColor, showHaihai, haihaiFlags, haihaiX, haihaiY, haihaiWidth);
    }

    // Note: gzTextBox no longer has m_description - use gzLine-based drawLines overload for descriptions
}

void gzMenu_c::drawLines(gzLine** lines, s32 numLines, u8 haihai_flags, s32 topLine, s32 visibleLines) {
    gzCursor* l_cursor = gzInfo_getCursor();
    J2DTextBox::TFontSize font_size;

    if (numLines > 0 && lines[0] != NULL) {
        lines[0]->mText->getFontSize(font_size);
        mpHaihai->setScale(font_size.mSizeY * 0.04f);
    }

    f32 lineX = mXPos;
    f32 lineY_start = g_gzInfo.mBackgroundYPos + 78.0f;
    f32 line_spacing = g_gzInfo.mBackgroundHeight / 20;
    f32 optionX = mXPos - 20.0f;
    f32 haihaiX = optionX + 305.0f;
    u32 cursorColor = gzInfo_getCursorColor();
    s32 endLine = topLine + visibleLines;

    if (endLine > numLines) endLine = numLines;
    for (s32 i = topLine; i < endLine; i++) {
        gzLine* line = lines[i];
        if (line == NULL) continue;
        
        f32 lineY = lineY_start + ((i - topLine) * line_spacing);
        f32 haihaiY = lineY - 7.0f;
        bool isSelected = (l_cursor->y == i && gzInfo_isSubMenuVisible());
        u32 color = isSelected ? cursorColor : COLOR_WHITE;
        f32 haihaiWidth = 0.0f;
        gzTextBox* opt = line->getOptionBox();

        if (opt != NULL && opt->mStringLength != 0) {
            haihaiWidth = opt->mBounds.f.x + 30.0f;
        }

        bool showHaihai = gzInfo_isMenuOption() && l_cursor->y == i && haihai_flags != 0;
        line->mText->draw(lineX, lineY, color);

        if (opt != NULL) {
            opt->draw(optionX, lineY, color, HBIND_CENTER);
        }

        if (showHaihai && mpHaihai != NULL) {
            mpHaihai->drawHaihai(haihai_flags, haihaiX, haihaiY, haihaiWidth, 0.0f);
        }

        if (isSelected && gzInfo_isCursorTypeTP() && gzInfo_getTPCursor() != NULL) {
            f32 cursorY = lineY - 10.0f;
            gzInfo_getTPCursor()->setPos(lineX - 20.0f, cursorY, (J2DPane*)line->mText, false);
            gzInfo_getTPCursor()->draw();
        }
    }

    drawDescription((l_cursor->y < numLines && lines[l_cursor->y] != NULL) ? lines[l_cursor->y]->m_description : NULL);
}

// Draws the line, line option, haihai arrows (for option boxes), and TP cursor if enabled
void gzMenu_c::drawLineWithOption(gzTextBox* line, gzTextBox* option, f32 lineX, f32 optionX, f32 lineY, bool isSelected, u32 selectedColor, bool showHaihai, u8 haihaiFlags, f32 haihaiX, f32 haihaiY, f32 haihaiWidth) {
    u32 color = isSelected ? selectedColor : COLOR_WHITE;

    if (line) line->draw(lineX, lineY, color);

    if (option) option->draw(optionX, lineY, color, HBIND_CENTER);

    if (showHaihai && option && option->mStringLength != 0 && haihaiFlags != 0) {
        mpHaihai->drawHaihai(haihaiFlags, haihaiX, haihaiY, haihaiWidth, 0.0f);
    }

    if (isSelected && gzInfo_isCursorTypeTP() && gzInfo_getTPCursor()) {
        static const f32 TP_CURSOR_Y_OFFSET = -10.0f;
        f32 cursorY = lineY + TP_CURSOR_Y_OFFSET;
        gzInfo_getTPCursor()->setPos(lineX - 20.0f, cursorY, (J2DPane*)line, false);
        gzInfo_getTPCursor()->draw();
    }
}

void gzMenu_c::updateScrolling(s32 maxLines) {
    gzCursor* cursor = gzInfo_getCursor();
    s32 topLine = gzInfo_getTopLine();
    s32 visibleLines = gzInfo_getVisibleLines();

    if (cursor->y < topLine) {
        topLine = cursor->y;
    } else if (cursor->y >= topLine + visibleLines) {
        topLine = cursor->y - visibleLines + 1;
    }

    s32 maxTop = maxLines - visibleLines;
    if (maxTop < 0) maxTop = 0;
    if (topLine > maxTop) topLine = maxTop;
    if (topLine < 0) topLine = 0;

    gzInfo_setTopLine(topLine);
}

void gzMenu_c::drawDescription(const char* desc) {
    f32 x = 0.0f;
    f32 y = g_gzInfo.mBackgroundHeight + 25.0f;

    if (gzInfo_isSubMenuVisible() && desc != NULL && *desc != '\0' && gzInfo_getMenuDescription() != NULL) {
        gzInfo_getMenuDescription()->setString(desc);
        gzInfo_getMenuDescription()->draw(x, y, gzInfo_getCursorColor(), HBIND_CENTER);
    }
}

gzMenu_c::gzMenu_c() : mXPos(0.0f), mpHaihai(NULL) {
    mpHaihai = new dMeterHaihai_c(3);
    mpHaihai->setScale(0.04f);
}

gzMenu_c::~gzMenu_c() {
    if (mpHaihai != NULL) {
        delete mpHaihai;
        mpHaihai = NULL;
    }
}
