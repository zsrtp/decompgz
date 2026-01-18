#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_utility_confirm.h"
#include "gz/gz_textbox.h"

static gzCursor l_confirm_cursor = {0, 0};

gzConfirm_c::gzConfirm_c(confirmCallback confirmCb, confirmCallback returnCb, void* cbData) {
    OSReport("creating gzConfirm_c\n");

    mpPromptBox = gzTextBox_allocate();
    mpNoBox = gzTextBox_allocate();
    mpYesBox = gzTextBox_allocate();

    if (mpPromptBox != NULL) {
        mpPromptBox->setString("are you sure?");
    }
    if (mpNoBox != NULL) {
        mpNoBox->setString("no");
    }
    if (mpYesBox != NULL) {
        mpYesBox->setString("yes");
    }

    setCallbacks(confirmCb, returnCb, cbData);
    l_confirm_cursor.x = 0;  // default to "no"
}

gzConfirm_c::gzConfirm_c(confirmCallback confirmCb, confirmCallback returnCb, void* cbData, const char* msg) {
    OSReport("creating gzConfirm_c\n");

    mpPromptBox = gzTextBox_allocate();
    mpNoBox = gzTextBox_allocate();
    mpYesBox = gzTextBox_allocate();

    if (mpPromptBox != NULL) {
        mpPromptBox->setString(msg);
    }
    if (mpNoBox != NULL) {
        mpNoBox->setString("no");
    }
    if (mpYesBox != NULL) {
        mpYesBox->setString("yes");
    }

    setCallbacks(confirmCb, returnCb, cbData);
    l_confirm_cursor.x = 0;  // default to "no"
}

gzConfirm_c::~gzConfirm_c() {
    _delete();
}

void gzConfirm_c::_delete() {
    OSReport("deleting gzConfirm_c\n");

    gzTextBox_free(mpPromptBox);
    mpPromptBox = NULL;

    gzTextBox_free(mpNoBox);
    mpNoBox = NULL;

    gzTextBox_free(mpYesBox);
    mpYesBox = NULL;
}

void gzConfirm_c::setPrompt(const char* msg) {
    if (mpPromptBox != NULL) {
        mpPromptBox->setString(msg);
    }
}

int gzConfirm_c::execute() {
    if (gzPad::getTrigRight()) {
        l_confirm_cursor.x = 1;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigLeft()) {
        l_confirm_cursor.x = 0;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigA()) {
        if (l_confirm_cursor.x == 1) {
            // Yes selected
            if (mConfirmCb != NULL) {
                mConfirmCb(this, mpCbData);
            }
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
            return 2;
        } else {
            // No selected
            if (mReturnCb != NULL) {
                mReturnCb(this, mpCbData);
            }
            gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
            return 1;
        }
    }

    if (gzPad::getTrigB()) {
        if (mReturnCb != NULL) {
            mReturnCb(this, mpCbData);
        }
        gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
        return 1;
    }

    return 0;
}

void gzConfirm_c::draw() {
    const f32 PROMPT_X = 0.0f;
    const f32 PROMPT_Y = 90.0f;
    const f32 LINE_Y = 120.0f;
    const f32 NO_X = -30.0f;
    const f32 YES_X = 30.0f;

    u32 cursor_color = gzInfo_getCursorColor();

    if (mpPromptBox != NULL) {
        mpPromptBox->draw(PROMPT_X, PROMPT_Y, COLOR_WHITE, HBIND_CENTER);
    }

    if (mpNoBox != NULL) {
        u32 no_color = (l_confirm_cursor.x == 0) ? cursor_color : COLOR_WHITE;
        mpNoBox->draw(NO_X, LINE_Y, no_color, HBIND_CENTER);
    }

    if (mpYesBox != NULL) {
        u32 yes_color = (l_confirm_cursor.x == 1) ? cursor_color : COLOR_WHITE;
        mpYesBox->draw(YES_X, LINE_Y, yes_color, HBIND_CENTER);
    }
}
