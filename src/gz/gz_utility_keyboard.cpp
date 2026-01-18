#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_utility_keyboard.h"
#include "gz/gz_textbox.h"

#define CHARACTER_ROWS    7
#define CHARACTER_COLUMNS 13
#define CHARACTER_COUNT   (CHARACTER_ROWS * CHARACTER_COLUMNS)
#define SPACEBAR_ROW      7

// QWERTY layout - array is column-major
// Row 0: 1 2 3 4 5 6 7 8 9 0 ! ( )
// Row 1: Q W E R T Y U I O P [ ] #
// Row 2: A S D F G H J K L ; ' & %
// Row 3: Z X C V B N M , . / - _ +
// Row 4: q w e r t y u i o p { } |
// Row 5: a s d f g h j k l : " * `
// Row 6: z x c v b n m < > ? ~ = ^
// Row 7: (space bar - handled separately)
static char l_keyboard[] = {
    '1', 'Q', 'A', 'Z', 'q', 'a', 'z',  // column 0
    '2', 'W', 'S', 'X', 'w', 's', 'x',  // column 1
    '3', 'E', 'D', 'C', 'e', 'd', 'c',  // column 2
    '4', 'R', 'F', 'V', 'r', 'f', 'v',  // column 3
    '5', 'T', 'G', 'B', 't', 'g', 'b',  // column 4
    '6', 'Y', 'H', 'N', 'y', 'h', 'n',  // column 5
    '7', 'U', 'J', 'M', 'u', 'j', 'm',  // column 6
    '8', 'I', 'K', ',', 'i', 'k', '<',  // column 7
    '9', 'O', 'L', '.', 'o', 'l', '>',  // column 8
    '0', 'P', ';', '/', 'p', ':', '?',  // column 9
    '!', '[', '\'', '-', '{', '"', '~', // column 10
    '(', ']', '&', '_', '}', '*', '=',  // column 11
    ')', '#', '%', '+', '|', '`', '^',  // column 12
};

static gzCursor l_key_cursor = {0, 0};

gzKeyboard_c::gzKeyboard_c(kbCallback finishCb, kbCallback returnCb, void* cbData) {
    gzCursor* l_cursor = gzInfo_getCursor();
    OSReport("creating gzKeyboardMenu_c\n");

    for (int i = 0; i < CHARACTER_COUNT; i++) {
        mpCharacters[i] = gzTextBox_allocate();
        if (mpCharacters[i] != NULL) {
            char buf[2];
            sprintf(buf, "%c", l_keyboard[i]);
            mpCharacters[i]->setString(buf);
        }
    }

    mpSpacebarBox = gzTextBox_allocate();
    if (mpSpacebarBox != NULL) {
        mpSpacebarBox->setString("[SPACE]");
    }

    mpStringBox = gzTextBox_allocate();
    memset(mString, 0, sizeof(mString));
    mStringIndex = 0;

    setCallbacks(finishCb, returnCb, cbData);
}

gzKeyboard_c::~gzKeyboard_c() {
    _delete();
}

void gzKeyboard_c::_delete() {
    OSReport("deleting gzKeyboardMenu_c\n");

    for (int i = 0; i < CHARACTER_COUNT; i++) {
        gzTextBox_free(mpCharacters[i]);
        mpCharacters[i] = NULL;
    }

    gzTextBox_free(mpSpacebarBox);
    mpSpacebarBox = NULL;

    gzTextBox_free(mpStringBox);
    mpStringBox = NULL;
}

int gzKeyboard_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();
    const int TOTAL_ROWS = CHARACTER_ROWS + 1;  // Include spacebar row

    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return 0;
    }

    // Left/right navigation only applies to character rows, not spacebar
    if (l_key_cursor.y < CHARACTER_ROWS) {
        if (gzPad::getTrigRight()) {
            l_key_cursor.x = (l_key_cursor.x + 1) % CHARACTER_COLUMNS;
            gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
        }

        if (gzPad::getTrigLeft()) {
            l_key_cursor.x = (l_key_cursor.x - 1 + CHARACTER_COLUMNS) % CHARACTER_COLUMNS;
            gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
        }
    }

    if (gzPad::getTrigDown()) {
        l_key_cursor.y = (l_key_cursor.y + 1) % TOTAL_ROWS;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigUp()) {
        l_key_cursor.y = (l_key_cursor.y - 1 + TOTAL_ROWS) % TOTAL_ROWS;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    // Add selected character when A is pressed
    if (gzPad::getTrigA()) {
        if (mStringIndex < MAX_STRING_LEN - 1) {
            if (l_key_cursor.y == SPACEBAR_ROW) {
                mString[mStringIndex] = ' ';
            } else {
                mString[mStringIndex] = l_keyboard[l_key_cursor.y + l_key_cursor.x * CHARACTER_ROWS];
            }
            mpStringBox->setString(mString);
            mStringIndex++;
            gzInfo_seStart(Z2SE_SY_NAME_INPUT);
        }
    }

    // Remove character when B is pressed, or exit if string is empty
    if (gzPad::getTrigB()) {
        if (mStringIndex > 0) {
            mStringIndex--;
            mString[mStringIndex] = '\0';
            mpStringBox->setString(mString);
            gzInfo_seStart(Z2SE_SY_NAME_DELETE);
        } else {
            if (mReturnCb != NULL)
                mReturnCb(this, mpCbData);
            gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
            return 1;
        }
    }

    if (gzPad::getTrigStart()) {
        if (mFinishCb != NULL)
            mFinishCb(this, mpCbData);
        gzInfo_seStart(Z2SE_SY_NAME_OK);
        return 2;
    }

    return 0;
}

void gzKeyboard_c::draw() {
    const f32 x_pos_base = 160.0f;
    const f32 y_pos_base = 100.0f;
    const f32 char_spacing = 25.0f;
    const f32 line_spacing = 22.0f;

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
        y_pos += line_spacing;
    }

    // Draw spacebar row
    if (mpSpacebarBox != NULL) {
        // Center the spacebar text in the keyboard width
        f32 spacebar_x = x_pos_base + (CHARACTER_COLUMNS * char_spacing) / 2.0f - 40.0f;
        if (l_key_cursor.y == SPACEBAR_ROW) {
            mpSpacebarBox->draw(spacebar_x, y_pos, gzInfo_getCursorColor());
        } else {
            mpSpacebarBox->draw(spacebar_x, y_pos, COLOR_WHITE);
        }
    }
    y_pos += line_spacing;

    if (mpStringBox != NULL) {
        mpStringBox->draw(x_pos_base, y_pos + 10.0f, COLOR_WHITE);
    }
}
