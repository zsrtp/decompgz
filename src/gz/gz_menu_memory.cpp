#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_memory.h"
#include "gz/gz_menu_main.h"

gzMemoryMenu_c::gzMemoryMenu_c() {
    OSReport("creating gzMemoryMenu_c\n");

    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = new gzTextBox;
    }

    mTopLine = 0;
}

gzMemoryMenu_c::~gzMemoryMenu_c() {
    _delete();
}

void gzMemoryMenu_c::_delete() {
    OSReport("deleting gzMemoryMenu_c\n");

    for (int i = 0; i < LINE_NUM; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;
    }
}

void gzMemoryMenu_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();
    int current_max_line = LINE_NUM;

    if (gzPad::getTrigDown()) {
        l_cursor->y = (l_cursor->y + 1) % current_max_line;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigUp()) {
        l_cursor->y = (l_cursor->y - 1 + current_max_line) % current_max_line;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigB()) {
        l_cursor->x--;
        l_cursor->y = gzMainMenu_c::MENU_MEMORY;
        gzInfo_seStart(Z2SE_SY_EXP_WIN_CLOSE);
        g_gzInfo.mpMainMenu->startReverseTransition();
        return;
    }
}

void gzMemoryMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();

    static const f32 Y_ALIGNMENT = 100.0f;
    static const f32 LINE_SPACING = 22.0f;
    static const int VISIBLE_LINES = 15;

    u32 cursor_color = gzInfo_getCursorColor();

    int current_max_line = LINE_NUM;
    gzTextBox** currentLines = mpLines;

    if (l_cursor->y < mTopLine) {
        mTopLine = l_cursor->y;
    } else if (l_cursor->y >= mTopLine + VISIBLE_LINES) {
        mTopLine = l_cursor->y - VISIBLE_LINES + 1;
    }

    // Clamp mTopLine to valid range
    int maxTop = current_max_line - VISIBLE_LINES;
    if (maxTop < 0) maxTop = 0;
    if (mTopLine > maxTop) mTopLine = maxTop;
    if (mTopLine < 0) mTopLine = 0;

    for (int screenIdx = 0; screenIdx < VISIBLE_LINES; screenIdx++) {
        int lineIdx = mTopLine + screenIdx;
        if (lineIdx >= current_max_line) break;

        if (currentLines[lineIdx] != NULL) {
            f32 y_pos = Y_ALIGNMENT + ((screenIdx - 1) * LINE_SPACING);

            if (l_cursor->y == lineIdx && gzInfo_isSubMenuVisible()) {
                currentLines[lineIdx]->draw(mXPos, y_pos, cursor_color);
            } else {
                currentLines[lineIdx]->draw(mXPos, y_pos, COLOR_WHITE);
            }
        }
    }
}
