#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "gz/gz_menu.h"

// gzMenu_c::gzCursor gzCreditsMenu_c::mCursor = {0, 0};

gzCreditsMenu_c::gzCreditsMenu_c() {
    OSReport("creating gzCreditsMenu_c\n");
}

gzCreditsMenu_c::~gzCreditsMenu_c() {
    _delete();
}

void gzCreditsMenu_c::_delete() {
    OSReport("deleting gzCreditsMenu_c\n");
    // for (int i = 0; i < LINE_NUM; i++) {
    //     delete mpLines[i];
    //     mpLines[i] = NULL;
    // }
}

void gzCreditsMenu_c::execute() {
    if (gzPad::getTrigB()) {
        gzChangeMenu<gzSettingsMenu_c>();
        return;
    }
}

void gzCreditsMenu_c::draw() {
    // for (int i = 0; i < LINE_NUM; i++) {
    //     if (mpLines[i] != NULL) {
    //         if (mCursor.y == i) {
    //             mpLines[i]->draw(30.0f, 90.0f + ((i - 1) * 22.0f), g_gzInfo.getCursorColor(), g_gzInfo.getDropShadows());
    //         } else {
    //             mpLines[i]->draw(30.0f, 90.0f + ((i - 1) * 22.0f), 0xFFFFFFFF, g_gzInfo.getDropShadows());
    //         }
    //     }
    // }
}
