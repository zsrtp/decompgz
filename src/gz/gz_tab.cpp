#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_tab.h"
#include "gz/gz_textbox.h"

void gzTab_c::create() {
    for (int i = 0; i < mLineMax; i++) {
        mpLines[i] = new gzTextBox();
        mpLines[i]->mBounds.f.x = 430.0f;
        mpLines[i]->mBounds.f.y = 10.0f;

        mpLineOptions[i] = new gzTextBox();
        mpLineOptions[i]->mBounds.f.y = 10.0f;

        if (mBoolOptions[i].desc) {
            mpLines[i]->setStringDesc(mBoolOptions[i].name, mBoolOptions[i].desc);
        } else {
            mpLines[i]->setString(mBoolOptions[i].name);
        }
    }
}

void gzTab_c::_delete() {
    for (int i = 0; i < mLineMax; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;

        delete mpLineOptions[i];
        mpLineOptions[i] = NULL;
    }
}

int gzTab_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();

    if (mOptionToggle) {
        if (gzPad::getTrigRight() && !mBoolOptions[l_cursor->y].is()) {
            mBoolOptions[l_cursor->y].on();
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }

        if (gzPad::getTrigLeft() && mBoolOptions[l_cursor->y].is()) {
            mBoolOptions[l_cursor->y].off();
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }
    }

    return 1;
}

void gzTab_c::updateDynamicLines() {
    J2DTextBox::TFontSize font_size;

    for (int i = 0; i < mLineMax; i++) {
        mpLineOptions[i]->setStringf("%s", mBoolOptions[i].is() ? "on" : "off");
        mpLineOptions[i]->getFontSize(font_size);
        font_size.mSizeX *= 0.5f;
        mpLines[i]->mBounds.f.x = mpLines[i]->mStringLength * font_size.mSizeX;
        mpLineOptions[i]->mBounds.f.x = mpLineOptions[i]->mStringLength * font_size.mSizeX;
    }
}
