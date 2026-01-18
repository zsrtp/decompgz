#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_memory.h"
#include "gz/gz_menu_main.h"
#include "d/d_select_cursor.h"

gzMemoryMenu_c::gzMemoryMenu_c() {
    OSReport("creating gzMemoryMenu_c\n");
    mXPos = g_gzInfo.mBackgroundXPos + 195.0f;

    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = gzTextBox_allocate();
    }

    gzInfo_resetTopLine();
}

gzMemoryMenu_c::~gzMemoryMenu_c() {
    _delete();
}

void gzMemoryMenu_c::_delete() {
    OSReport("deleting gzMemoryMenu_c\n");

    for (int i = 0; i < LINE_NUM; i++) {
        gzTextBox_free(mpLines[i]);
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

    updateScrolling(current_max_line);
    s32 topLine = gzInfo_getTopLine();

    for (int screenIdx = 0; screenIdx < VISIBLE_LINES; screenIdx++) {
        int lineIdx = topLine + screenIdx;
        if (lineIdx >= current_max_line) break;

        if (currentLines[lineIdx] != NULL) {
            f32 y_pos = Y_ALIGNMENT + ((screenIdx - 1) * LINE_SPACING);
            bool isSelected = (l_cursor->y == lineIdx && gzInfo_isSubMenuVisible());

            if (isSelected) {
                currentLines[lineIdx]->draw(mXPos, y_pos, cursor_color);
            } else {
                currentLines[lineIdx]->draw(mXPos, y_pos, COLOR_WHITE);
            }

            // Draw TP cursor for selected line
            if (isSelected) {
                drawTPCursorForBox(currentLines[lineIdx], mXPos, y_pos);
            }
        }
    }
}
