#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_credits.h"

gzCreditsMenu_c::gzCreditsMenu_c() {
    OSReport("creating gzCreditsMenu_c\n");

    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = gzTextBox_allocate();
        if (i % 2 == 0) mpLines[i]->setString("a");
        if (i % 2 == 1) mpLines[i]->setString("b");
        if (i > 20 && i % 2 == 1) mpLines[i]->setString("c");
    }

    mpMeterHaihai = new dMeterHaihai_c(3);
    mpMeterHaihai->setScale(0.5f);
    gzInfo_resetTopLine();
}

gzCreditsMenu_c::~gzCreditsMenu_c() {
    _delete();
}

void gzCreditsMenu_c::_delete() {
    OSReport("deleting gzCreditsMenu_c\n");

    for (int i = 0; i < LINE_NUM; i++) {
        gzTextBox_free(mpLines[i]);
        mpLines[i] = NULL;
    }

    delete mpMeterHaihai;
    mpMeterHaihai = NULL;
}

void gzCreditsMenu_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();
    if (gzPad::getTrigDown()) l_cursor->y = (l_cursor->y + 1) % LINE_NUM;
    if (gzPad::getTrigUp()) l_cursor->y = (l_cursor->y - 1 + LINE_NUM) % LINE_NUM;

    mpMeterHaihai->_execute(0);
}

void gzCreditsMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();
    static const f32 Y_ALIGNMENT = 100.0f;
    static const f32 LINE_SPACING = 22.0f;
    static const f32 HAIHAI_X_OFFSET = 6.0f;
    static const f32 HAIHAI_Y_OFFSET = 125.0f;
    static const f32 HAIHAI_Y_SIZE = 355.0f; // TODO(Pheenoh): make this dynamic based on VISIBLE_LINES and mFontSizeY
    static const int VISIBLE_LINES = 15;
    u32 cursor_color = gzInfo_getCursorColor();

    f32 x_alignment_haihai = mXPos + HAIHAI_X_OFFSET;
    f32 y_alignment_haihai = Y_ALIGNMENT + HAIHAI_Y_OFFSET;

    updateScrolling(LINE_NUM);
    s32 topLine = gzInfo_getTopLine();
    int maxTop = LINE_NUM - VISIBLE_LINES;
    if (maxTop < 0) maxTop = 0;

    for (int screenIdx = 0; screenIdx < VISIBLE_LINES; screenIdx++) {
        int lineIdx = topLine + screenIdx;
        if (lineIdx >= LINE_NUM) break;

        if (mpLines[lineIdx] != NULL) {
            f32 y_pos = Y_ALIGNMENT + ((screenIdx - 1) * LINE_SPACING);

            if (l_cursor->y == lineIdx && gzInfo_isSubMenuVisible()) {
                mpLines[lineIdx]->draw(mXPos, y_pos, cursor_color);
            } else {
                mpLines[lineIdx]->draw(mXPos, y_pos, COLOR_WHITE);
            }
        }
    }

    u32 arrows = 0;
    if (LINE_NUM > VISIBLE_LINES) {
        if (topLine > 0) {
            arrows |= ARROW_UP;
        }
        if (topLine < maxTop) {
            arrows |= ARROW_DOWN;
        }
    }

    if (arrows != 0) {
        gzSetup2DContext();
        mpMeterHaihai->drawHaihai(arrows, x_alignment_haihai, y_alignment_haihai, 0.0f, HAIHAI_Y_SIZE);
    }
}
