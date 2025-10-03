#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "gz/gz_menu.h"

gzMenu_c::gzCursor gzMainMenu_c::mCursor = {0, 0};

gzMainMenu_c::gzMainMenu_c() {
    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = new J2DTextBox();
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
}

void gzMainMenu_c::draw() {
    // textbox method
    for (int i = 0; i < LINE_NUM; i++) {
        if (mpLines[i] != NULL) {
            mpLines[i]->draw(30.0f, 90.0f + ((i - 1) * 22.0f), 608.0f, HBIND_LEFT);
        }

        if (mCursor.y == i) {
            mpLines[i]->setCharColor(g_gzInfo.getCursorColor());
            mpLines[i]->setGradColor(g_gzInfo.getCursorColor());
        } else {
            mpLines[i]->setCharColor(0xFFFFFFFF);
            mpLines[i]->setGradColor(0xFFFFFFFF);
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
