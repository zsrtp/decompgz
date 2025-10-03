#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "gz/gz_menu.h"

gzMenu_c::gzCursor gzSettingsMenu_c::mCursor = {0, 0};

gzSettingsMenu_c::gzSettingsMenu_c() {
    OSReport("creating gzSettingsMenu_c\n");

    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = new gzTextBox();
        mpLines[i]->setFont(mDoExt_getMesgFont());
        mpLines[i]->setFontSize(18.0f, 18.0f);
    }

    mpLines[0]->setString("area reload behavior:");
    mpLines[1]->setString("cursor color:");
    mpLines[2]->setString("font:");
    mpLines[3]->setString("drop shadows:");
    mpLines[4]->setString("swap equips:");
    mpLines[5]->setString("save card");
    mpLines[6]->setString("load card");
    mpLines[7]->setString("delete card");
    mpLines[8]->setString("command combos");
    mpLines[9]->setString("menu positions");
    mpLines[10]->setString("credits");
}

gzSettingsMenu_c::~gzSettingsMenu_c() {
    _delete();
}

void gzSettingsMenu_c::_delete() {
    OSReport("deleting gzSettingsMenu_c\n");
    for (int i = 0; i < LINE_NUM; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;
    }
}

void gzSettingsMenu_c::execute() {
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
        gzChangeMenu<gzMainMenu_c>();
        return;
    }
}

void gzSettingsMenu_c::draw() {
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
