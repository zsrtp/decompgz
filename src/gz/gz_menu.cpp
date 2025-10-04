#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "gz/gz_menu.h"

gzMenu_c::gzCursor gzMainMenu_c::mCursor = {0, 0};

gzMainMenu_c::gzMainMenu_c() {
    OSReport("creating gzMainMenu_c\n");

    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = new gzTextBox();
        mpLines[i]->setFont(mDoExt_getMesgFont());
        mpLines[i]->setFontSize(18.0f, 18.0f);
    }

    mpLines[MENU_CHEATS]->setString("cheats");
    mpLines[MENU_FLAGS]->setString("flags");
    mpLines[MENU_INVENTORY]->setString("inventory");
    mpLines[MENU_MEMORY]->setString("memory");
    mpLines[MENU_PRACTICE]->setString("practice");
    mpLines[MENU_SCENE]->setString("scene");
    mpLines[MENU_SETTINGS]->setString("settings");
    mpLines[MENU_TOOLS]->setString("tools");
    mpLines[MENU_WARPING]->setString("warping");
}

gzMainMenu_c::~gzMainMenu_c() {
    _delete();
}

void gzMainMenu_c::_delete() {
    OSReport("deleting gzMainMenu_c\n");
    for (int i = 0; i < LINE_NUM; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;
    }
}

void gzMainMenu_c::execute() {
    if (gzPad::getTrigDown() && mCursor.y < LINE_NUM) {
        mCursor.y++;
    }

    if (gzPad::getTrigUp() && mCursor.y >= 0) {
        mCursor.y--;
    }

    if (mCursor.y < 0) {
        mCursor.y = LINE_NUM - 1;
    } else if (mCursor.y > LINE_NUM - 1) {
        mCursor.y = 0;
    }

    if (gzPad::getTrigB()) {
        g_gzInfo.mDisplay = false;
        return;
    }

    if (gzPad::getTrigA()) {
        switch (mCursor.y) {
        case MENU_SETTINGS:
            gzChangeMenu<gzSettingsMenu_c>();
            break;
        }
    }
}

void gzMainMenu_c::draw() {
    // textbox method
    for (int i = 0; i < LINE_NUM; i++) {
        if (mpLines[i] != NULL) {
            if (mCursor.y == i) {
                mpLines[i]->draw(30.0f, 90.0f + ((i - 1) * 22.0f), g_gzInfo.getCursorColor(), true);
            } else {
                mpLines[i]->draw(30.0f, 90.0f + ((i - 1) * 22.0f), 0xFFFFFFFF, true);
            }
        }
    }
}
