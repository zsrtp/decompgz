#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_tab.h"
#include "gz/gz_textbox.h"
#include "gz/gz_menu.h"

void gzTab_c::create() {
    for (int i = 0; i < mMax; i++) {
        mpLines[i] = new gzTextBox();
        mpLines[i]->mBounds.f.x = 430.0f;
        mpLines[i]->mBounds.f.y = 10.0f;

        mpLineOptions[i] = new gzTextBox();
        mpLineOptions[i]->mBounds.f.y = 10.0f;

        if (mOptions[i].desc) {
            mpLines[i]->setStringDesc(mOptions[i].name, mOptions[i].desc);
        } else {
            mpLines[i]->setString(mOptions[i].name);
        }
    }
}

void gzTab_c::_delete() {
    for (int i = 0; i < mMax; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;

        delete mpLineOptions[i];
        mpLineOptions[i] = NULL;
    }
}

int gzTab_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();

    if (mOptionToggle) {
        if (gzPad::getTrigA()) {
            mOptionToggle = false;
            gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
        }

        if (gzPad::getTrigRight() && !mOptions[l_cursor->y].is()) {
            mOptions[l_cursor->y].on();
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }

        if (gzPad::getTrigLeft() && mOptions[l_cursor->y].is()) {
            mOptions[l_cursor->y].off();
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }
    } else {
        if (gzPad::getTrigA()) {
            mOptionToggle = true;
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK);
        }
    }

    return 1;
}

void gzTab_c::updateDynamicLines() {
    J2DTextBox::TFontSize font_size;

    for (int i = 0; i < mMax; i++) {
        mpLineOptions[i]->setStringf("%s", mOptions[i].is() ? "on" : "off");
        mpLineOptions[i]->getFontSize(font_size);
        font_size.mSizeX *= 0.5f;
        mpLines[i]->mBounds.f.x = mpLines[i]->mStringLength * font_size.mSizeX;
        mpLineOptions[i]->mBounds.f.x = mpLineOptions[i]->mStringLength * font_size.mSizeX;
    }
}
