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

    mpLines[0]->setString("cheats");
    mpLines[1]->setString("flags");
    mpLines[2]->setString("inventory");
    mpLines[3]->setString("memory");
    mpLines[4]->setString("practice");
    mpLines[5]->setString("scene");
    mpLines[6]->setString("settings");
    mpLines[7]->setString("tools");
    mpLines[8]->setString("warping");
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
        case 6:
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

    // print method
    /* for (int i = 0; i < LINE_NUM; i++) {
        const char* lines[] = {
            "cheats",
            "flags",
            "inventory",
            "memory",
            "practice",
            "scene",
            "settings",
            "tools",
            "warping",
        };

        gzPrint(30, 90 + ((i - 1) * 22), g_gzInfo.getCursorColor(), lines[i]);
    } */
}
