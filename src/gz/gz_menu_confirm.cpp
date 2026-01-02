#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_confirm.h"

gzConfirmMenu_c::gzConfirmMenu_c(confirmCallback i_confirmCallback, void* i_data, returnCallback i_returnCallback) {
    gzCursor* l_cursor = gzInfo_getCursor();
    OSReport("creating gzConfirmMenu_c\n");
    mpLineConfirmPrompt = allocateTextBox();

    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = allocateTextBox();
    }

    mpLineConfirmPrompt->setString("are you sure?");
    mpLines[CONFIRM_NO]->setString("no");
    mpLines[CONFIRM_YES]->setString("yes");

    mpConfirmCallback = i_confirmCallback;
    mCallbackData = i_data;
    mpReturnCallback = i_returnCallback;
    l_cursor->x = 0; // always put the cursor on "no" when the menu pops up
}

gzConfirmMenu_c::gzConfirmMenu_c(confirmCallback i_confirmCallback, void* i_data, returnCallback i_returnCallback, const char* msg) {
    gzCursor* l_cursor = gzInfo_getCursor();
    OSReport("creating gzConfirmMenu_c\n");
    mpLineConfirmPrompt = allocateTextBox();

    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = allocateTextBox();
    }

    mpLineConfirmPrompt->setString(msg);
    mpLines[CONFIRM_NO]->setString("no");
    mpLines[CONFIRM_YES]->setString("yes");

    mpConfirmCallback = i_confirmCallback;
    mCallbackData = i_data;
    mpReturnCallback = i_returnCallback;
    l_cursor->x = 0; // always put the cursor on "no" when the menu pops up
}

gzConfirmMenu_c::~gzConfirmMenu_c() {
    _delete();
}

void gzConfirmMenu_c::_delete() {
    OSReport("deleting gzConfirmMenu_c\n");

    delete mpLineConfirmPrompt;
    mpLineConfirmPrompt = NULL;

    for (int i = 0; i < LINE_NUM; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;
    }
}

void gzConfirmMenu_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();
    
    if (gzPad::getTrigRight()) l_cursor->x = (l_cursor->x + 1) % LINE_NUM;
    if (gzPad::getTrigLeft()) l_cursor->x = (l_cursor->x - 1 + LINE_NUM) % LINE_NUM;

    if (gzPad::getTrigA()) {
        switch (l_cursor->x) {
        case CONFIRM_NO:
            if (mpReturnCallback != NULL) (*mpReturnCallback)();
            return;
        case CONFIRM_YES:
            if (mpConfirmCallback != NULL) (*mpConfirmCallback)(mCallbackData);
            if (mpReturnCallback != NULL) (*mpReturnCallback)();
            return;
        }
    }

    if (gzPad::getTrigB()) {
        if (mpReturnCallback != NULL) (*mpReturnCallback)();;
        return;
    }
}

void gzConfirmMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();
    static const f32 PROMPT_X = 0.0f;
    static const f32 PROMPT_Y = 90.0f;
    static const f32 LINE_X_BASE = 20.0f;
    static const f32 LINE_SPACING = 60.0f;
    static const f32 LINE_Y = 120.0f;

    mpLineConfirmPrompt->draw(PROMPT_X, PROMPT_Y, COLOR_WHITE, HBIND_CENTER);

    u32 cursor_color = gzInfo_getCursorColor();
    for (int i = 0; i < LINE_NUM; i++) {
        f32 x_pos = LINE_X_BASE + ((i - 1) * LINE_SPACING);
        u32 color = (l_cursor->x == i) ? cursor_color : COLOR_WHITE;
        mpLines[i]->draw(x_pos, LINE_Y, color, HBIND_CENTER);
    }
}
