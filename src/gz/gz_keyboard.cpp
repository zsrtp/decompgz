#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_keyboard.h"
#include "gz/gz_textbox.h"
#include "gz/gz_menu.h"

#define CHARACTER_ROWS    5
#define CHARACTER_COLUMNS 13

static char l_keyboard[] = {
    'A', 'N', 'a', 'n', '1',
    'B', 'O', 'b', 'o', '2',
    'C', 'P', 'c', 'p', '3',
    'D', 'Q', 'd', 'q', '4',
    'E', 'R', 'e', 'r', '5',
    'F', 'S', 'f', 's', '6',
    'G', 'T', 'g', 't', '7',
    'H', 'U', 'h', 'u', '8',
    'I', 'V', 'i', 'v', '9',
    'J', 'W', 'j', 'w', '0',
    'K', 'X', 'k', 'x', ',',
    'L', 'Y', 'l', 'y', '.',
    'M', 'Z', 'm', 'z', ' ',
};

static gzCursor l_key_cursor = {0, 0};

gzKeyboard_c::gzKeyboard_c(kbCallback finishCb, kbCallback returnCb, void* cbData) {
    gzCursor* l_cursor = gzInfo_getCursor();
    OSReport("creating gzKeyboardMenu_c\n");

    for (int i = 0; i < 65; i++) {
        mpCharacters[i] = gzMenu_c::allocateTextBox();

        char buf[2];
        sprintf(buf, "%c", l_keyboard[i]);
        mpCharacters[i]->setString(buf);
    }

    mpStringBox = gzMenu_c::allocateTextBox();
    memset(mString, 0, sizeof(mString));
    mStringIndex = 0;

    setCallbacks(finishCb, returnCb, cbData);
}

gzKeyboard_c::~gzKeyboard_c() {
    _delete();
}

void gzKeyboard_c::_delete() {
    OSReport("deleting gzKeyboardMenu_c\n");

    for (int i = 0; i < 65; i++) {
        delete mpCharacters[i];
        mpCharacters[i] = NULL;
    }

    delete mpStringBox;
    mpStringBox = NULL;
}

int gzKeyboard_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();

    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return 0;
    }

    if (gzPad::getTrigRight()) {
        l_key_cursor.x = (l_key_cursor.x + 1) % CHARACTER_COLUMNS;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }
    
    if (gzPad::getTrigLeft()) {
        l_key_cursor.x = (l_key_cursor.x - 1 + CHARACTER_COLUMNS) % CHARACTER_COLUMNS;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigDown()) {
        l_key_cursor.y = (l_key_cursor.y + 1) % CHARACTER_ROWS;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigUp()) {
        l_key_cursor.y = (l_key_cursor.y - 1 + CHARACTER_ROWS) % CHARACTER_ROWS;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    // Add selected character when A is pressed
    if (gzPad::getTrigA()) {
        if (mStringIndex < MAX_STRING_LEN - 1) {
            mString[mStringIndex] = l_keyboard[l_key_cursor.y + l_key_cursor.x * CHARACTER_ROWS];
            mpStringBox->setString(mString);
            mStringIndex++;
            gzInfo_seStart(Z2SE_SY_NAME_INPUT);
        }
    }

    // Remove character when X is pressed
    // TODO: maybe should make this B with some more complex handling?
    if (gzPad::getTrigX()) {
        if (mStringIndex > 0) {
            mStringIndex--;
            mString[mStringIndex] = '\0';
            mpStringBox->setString(mString);
            gzInfo_seStart(Z2SE_SY_NAME_DELETE);
        }
    }

    if (gzPad::getTrigStart()) {
        if (mFinishCb != NULL)
            mFinishCb(this, mpCbData);
        gzInfo_seStart(Z2SE_SY_NAME_OK);
        return 2;
    }

    if (gzPad::getTrigB()) {
        if (mReturnCb != NULL)
            mReturnCb(this, mpCbData);
        gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
        return 1;
    }

    return 0;
}

void gzKeyboard_c::draw() {
    const f32 x_pos_base = 200.0f;
    const f32 y_pos_base = 100.0f;
    const f32 char_spacing = 22.0f;

    f32 x_pos = x_pos_base;
    f32 y_pos = y_pos_base;
    for (int r = 0; r < CHARACTER_ROWS; r++) {
        for (int c = 0; c < CHARACTER_COLUMNS; c++) {
            if (mpCharacters[r + c * CHARACTER_ROWS] != NULL) {
                if (l_key_cursor.y == r && l_key_cursor.x == c) {
                    mpCharacters[r + c * CHARACTER_ROWS]->draw(x_pos, y_pos, gzInfo_getCursorColor());
                } else {
                    mpCharacters[r + c * CHARACTER_ROWS]->draw(x_pos, y_pos, COLOR_WHITE);
                }
            }
            x_pos += char_spacing;
        }
        x_pos = x_pos_base;
        y_pos += char_spacing;
    }

    if (mpStringBox != NULL) {
        mpStringBox->draw(x_pos_base, y_pos_base + 150.0f, COLOR_WHITE);
    }
}
