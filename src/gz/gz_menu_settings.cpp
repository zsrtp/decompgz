#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_settings.h"
#include "gz/gz_menu_main.h"

#define COLOR_AMETHYST 0x9966FFFF
#define COLOR_AQUAMARINE 0x71D9E2FF
#define COLOR_BANANA_MANIA 0xFAE7B5FF
#define COLOR_BOLD_CRIMSON 0xDC143CFF
#define COLOR_BUBBLEGUM_PINK 0xFF69B4FF
#define COLOR_CERULEAN 0x007BA7FF
#define COLOR_COSMIC_COBALT 0x2E2D88FF
#define COLOR_ELECTRIC_BLUE 0x7DF9FFFF
#define COLOR_FIERY_ORANGE 0xFF4500FF
#define COLOR_FLAMINGO_FEATHER 0xFC8EACFF
#define COLOR_GOLD_DROP 0xEE8000FF
#define COLOR_LEMON_YELLOW 0xFFF44FFF
#define COLOR_LIME_GREEN 0x32CD32FF
#define COLOR_MAGENTA_MAGIC 0xFF00FFFF
#define COLOR_MIDNIGHT_BLUE 0x191970FF
#define COLOR_MYSTICAL_PURPLE 0x9370DBFF
#define COLOR_NEON_CARROT 0xFFA343FF
#define COLOR_PERIWINKLE 0xCCCCFFFF
#define COLOR_SAPPHIRE_SPARKLE 0x0F52BAFF
#define COLOR_SHAMROCK_GREEN 0x009E60FF
#define COLOR_SUNNY_YELLOW 0xFFFF00FF
#define COLOR_TANGERINE_TWIST 0xFFA500FF
#define COLOR_TROPICAL_TURQUOISE 0x40E0D0FF
#define COLOR_VIVID_VIOLET 0x9F00FFFF
#define COLOR_WILD_STRAWBERRY 0xFF43A4FF
#define COLOR_ZESTY_CHARTREUSE 0x7FFF00FF

static const int COLOR_COUNT = 27;

// TODO: load these from disk
static char* l_textColorName[] = {
    "amethyst",
    "aquamarine",
    "banana mania",
    "bold crimson",
    "bubblegum pink",
    "cerulean",
    "cosmic cobalt",
    "electric blue",
    "fiery orange",
    "flamingo feather",
    "gold drop",
    "lemon yellow",
    "lime green",
    "magenta magic",
    "midnight blue",
    "mystical purple",
    "neon carrot",
    "periwinkle",
    "sapphire sparkle",
    "shamrock green",
    "sunny yellow",
    "tangerine twist",
    "tropical turquoise",
    "vivid violet",
    "white",
    "wild strawberry",
    "zesty chartreuse"
};

static u32 l_textColorValue[] = {
    COLOR_AMETHYST,
    COLOR_AQUAMARINE,
    COLOR_BANANA_MANIA,
    COLOR_BOLD_CRIMSON,
    COLOR_BUBBLEGUM_PINK,
    COLOR_CERULEAN,
    COLOR_COSMIC_COBALT,
    COLOR_ELECTRIC_BLUE,
    COLOR_FIERY_ORANGE,
    COLOR_FLAMINGO_FEATHER,
    COLOR_GOLD_DROP,
    COLOR_LEMON_YELLOW,
    COLOR_LIME_GREEN,
    COLOR_MAGENTA_MAGIC,
    COLOR_MIDNIGHT_BLUE,
    COLOR_MYSTICAL_PURPLE,
    COLOR_NEON_CARROT,
    COLOR_PERIWINKLE,
    COLOR_SAPPHIRE_SPARKLE,
    COLOR_SHAMROCK_GREEN,
    COLOR_SUNNY_YELLOW,
    COLOR_TANGERINE_TWIST,
    COLOR_TROPICAL_TURQUOISE,
    COLOR_VIVID_VIOLET,
    COLOR_WHITE,
    COLOR_WILD_STRAWBERRY,
    COLOR_ZESTY_CHARTREUSE
};

u8 gzSettingsMenu_c::getHaihaiFlags(int i) {
    u8 haihai_flags = ARROW_LEFT | ARROW_RIGHT;

    switch (i) {
    case SETTING_RELOAD_TYPE:
        !gzInfo_getReloadType() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case SETTING_CURSOR_TYPE: {
        break;
    }
    case SETTING_DISPLAY_MODE:
        !gzInfo_getDisplayMode() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case SETTING_DROP_SHADOW:
        !gzInfo_isDropShadows() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case SETTING_MENU_PAUSES_GAME:
        haihai_flags = 0;
        break;
    case SETTING_MENU_SFX:
        !gzInfo_isMenuSfx() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case SETTING_FONT:
        haihai_flags = 0;
        break;
    case SETTING_SWAP_EQUIPS:
        !gzInfo_isSwapEquips() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case SETTING_TEXT_COLOR: {
        break;
    }
    }

    return haihai_flags;
}

u32 cycleTextColor(bool forward) {
    // Find current color index
    int currentIndex = -1;
    for (int i = 0; i < COLOR_COUNT; i++) {
        if (gzInfo_getTextColor() == l_textColorValue[i]) {
            currentIndex = i;
            break;
        }
    }
    
    // If not found, default to first color
    if (currentIndex == -1) {
        return l_textColorValue[0];
    }
    
    // Calculate offset and new index with wrap around
    int offset = forward ? 1 : -1;
    int newIndex = (currentIndex + offset + COLOR_COUNT) % COLOR_COUNT;
    return l_textColorValue[newIndex];
}

static void storeSettingsCallbackWrapper(void*) {
    gzInfo_storeSettingsMemcard();
}

static void deleteSettingsCallbackWrapper(void*) {
    gzInfo_deleteSettingsMemcard();
}

static void returnToSettings() {
    //gzChangeMenu<gzSettingsMenu_c>();
}

void gzSettingsMenu_c::updateDynamicLines() {
    mpLineOptions[SETTING_RELOAD_TYPE]->setStringf("%s", getReloadTypeText());
    mpLineOptions[SETTING_CURSOR_TYPE]->setStringf("%s", getCursorTypeText());

    // Find current color name
    char* currentColorName = "unknown";

    for (int i = 0; i < COLOR_COUNT; i++) {
        if (gzInfo_getTextColor() == l_textColorValue[i]) {
            currentColorName = l_textColorName[i];
            break;
        }
    }

    mpLineOptions[SETTING_TEXT_COLOR]->setStringf("%s", currentColorName);
    mpLineOptions[SETTING_DROP_SHADOW]->setStringf("%s", getDropShadowsText());
    mpLineOptions[SETTING_SWAP_EQUIPS]->setStringf("%s", getSwapEquipsText());
    mpLineOptions[SETTING_DISPLAY_MODE]->setStringf("%s", getDisplayModeText());
    mpLineOptions[SETTING_MENU_PAUSES_GAME]->setStringf("%s", getMenuPausesGameText());
    mpLineOptions[SETTING_MENU_SFX]->setStringf("%s", getMenuSfxText());
    mpLineOptions[SETTING_FONT]->setString("rodan");

    J2DTextBox::TFontSize font_size;

    // update box bounds
    for (int i = 0; i < LINE_NUM; i++) {
        mpLineOptions[i]->getFontSize(font_size);
        // applying font_size.mSizeX against a scaling factor 
        // to create a linear relationship between string length 
        // and text box bound size
        font_size.mSizeX *= 0.5f;
        mpLines[i]->mBounds.f.x = mpLines[i]->mStringLength * font_size.mSizeX;
        mpLineOptions[i]->mBounds.f.x = mpLineOptions[i]->mStringLength * font_size.mSizeX;
    }
}

gzSettingsMenu_c::gzSettingsMenu_c() {
    OSReport("creating gzSettingsMenu_c\n");

    mpCreditsMenu = new gzCreditsMenu_c();

    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = allocateTextBox();
        mpLines[i]->mBounds.f.x = 430.0f;
        mpLines[i]->mBounds.f.y = 10.0f;
        // mpLines[i]->setLineSpace(0.0f);

        mpLineOptions[i] = allocateTextBox();
        mpLineOptions[i]->mBounds.f.y = 10.0f;
    }

    mpDescription = allocateTextBox();

    mpLines[SETTING_CURSOR_TYPE]->setStringDesc("cursor type", "sets the cursor type to classic, tp or both");
    mpLines[SETTING_DISPLAY_MODE]->setStringDesc("display mode", "change between progressive and interlaced display modes");
    mpLines[SETTING_DROP_SHADOW]->setStringDesc("drop shadows", "adds drop shadows to tpgz menu text");
    mpLines[SETTING_FONT]->setStringDesc("font", "changes tpgz menu font");
    mpLines[SETTING_MENU_PAUSES_GAME]->setStringDesc("menu pauses game", "opening gz menu pauses game");
    mpLines[SETTING_MENU_SFX]->setStringDesc("menu sfx", "turn on/off gz menu sound effects");
    mpLines[SETTING_RELOAD_TYPE]->setStringDesc("reload type", "changes reload type to last file or last area");
    mpLines[SETTING_TEXT_COLOR]->setStringDesc("text color", "changes tpgz menu text color");
    mpLines[SETTING_SWAP_EQUIPS]->setStringDesc("swap equips", "swaps equips when loading practice saves");
    mpLines[SETTING_SAVE_CARD]->setStringDesc("save card", "saves tpgz settings to memory card");
    mpLines[SETTING_LOAD_CARD]->setStringDesc("load card", "loads tpgz settings from memory card");
    mpLines[SETTING_DELETE_CARD]->setStringDesc("delete card", "deletes tpgz settings from memory card");
    mpLines[SETTING_COMMAND_COMBOS]->setStringDesc("command combos", "change default command combos");
    mpLines[SETTING_MENU_POSITIONS]->setStringDesc("menu positions", "set positions of overlay menus");
    mpLines[SETTING_CREDITS]->setStringDesc("credits", "show the tpgz credits");

    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpDrawCursor->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    mpDrawCursor->setAlphaRate(1.0f);

    mpMeterHaihai = new dMeterHaihai_c(3);
}

gzSettingsMenu_c::~gzSettingsMenu_c() {
    _delete();
}

void gzSettingsMenu_c::_delete() {
    OSReport("deleting gzSettingsMenu_c\n");
    for (int i = 0; i < LINE_NUM; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;

        delete mpLineOptions[i];
        mpLineOptions[i] = NULL;
    }

    delete mpCreditsMenu;

    delete mpDescription;
    mpDescription = NULL;

    delete mpDrawCursor;
    mpDrawCursor = NULL;

    delete mpMeterHaihai;
    mpMeterHaihai = NULL;
}

void gzSettingsMenu_c::execute() {
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return;
    }
    
    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigDown() && !mOption) {
        l_cursor->y = (l_cursor->y + 1) % LINE_NUM;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigUp() && !mOption) {
        l_cursor->y = (l_cursor->y - 1 + LINE_NUM) % LINE_NUM;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigA()) {
        switch (l_cursor->y) {
        case SETTING_RELOAD_TYPE:
        case SETTING_CURSOR_TYPE:
        case SETTING_DISPLAY_MODE:
        case SETTING_DROP_SHADOW:
        case SETTING_SWAP_EQUIPS:
        case SETTING_MENU_SFX:
        case SETTING_TEXT_COLOR:
            mOption = !mOption;
            mOption ? gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK) : gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
            break;
        case SETTING_SAVE_CARD:
            //gzChangeMenu<gzConfirmMenu_c>(storeSettingsCallbackWrapper, NULL, returnToSettings, "save settings?");
            g_gzInfo.storeSettingsMemcard(); // temp
            return;
        case SETTING_LOAD_CARD:
            gzInfo_loadSettingsMemcard();
            break;
        case SETTING_DELETE_CARD:
            //gzChangeMenu<gzConfirmMenu_c>(deleteSettingsCallbackWrapper, NULL, returnToSettings, "delete settings?");
            return;
        case SETTING_MENU_POSITIONS:
            gzInfo_sendNotification("test!", 1);
            gzInfo_sendNotification("test!", 2);
            gzInfo_sendNotification("test2!");
            break;
        case SETTING_CREDITS:
            // gzChangeMenu(mpCreditsMenu);
            return;
        }
    }

    if (gzPad::getTrigB()) {
        if (mOption) {
            mOption = false;
            gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
        } else {
            l_cursor->x--;
            l_cursor->y = gzMainMenu_c::MENU_SETTINGS;
            gzInfo_seStart(Z2SE_SY_EXP_WIN_CLOSE);
            g_gzInfo.mpMainMenu->startReverseTransition();
            return;
        }
    }

    if (gzPad::getTrigRight()) {
        if (mOption) {
            switch (l_cursor->y) {
            case SETTING_RELOAD_TYPE:
                if (gzInfo_isReloadFile()) {
                    gzInfo_setReloadArea();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case SETTING_CURSOR_TYPE:
                gzInfo_setCursorType(gzInfo_nextCursorType());
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                break;
            case SETTING_DISPLAY_MODE: {
                if (gzInfo_isDisplayModeInterlaced()) {
                    gzInfo_setDisplayModeProgressive();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            }
            case SETTING_DROP_SHADOW:
                if (!gzInfo_isDropShadows()) {
                    gzInfo_onDropShadows();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case SETTING_MENU_PAUSES_GAME:
                break;
            case SETTING_MENU_SFX:
                if (!gzInfo_isMenuSfx()) {
                    gzInfo_onMenuSfx(); 
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case SETTING_SWAP_EQUIPS:
                if (!gzInfo_isSwapEquips()) {
                    gzInfo_onSwapEquips(); 
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case SETTING_TEXT_COLOR:
                gzInfo_setTextColor(cycleTextColor(true));
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                break;
            }
        }
    }

    if (gzPad::getTrigLeft()) {
        if (mOption) {
            switch (l_cursor->y) {
            case SETTING_RELOAD_TYPE:
                if (gzInfo_isReloadArea()) {
                    gzInfo_setReloadFile();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case SETTING_CURSOR_TYPE:
                gzInfo_setCursorType(gzInfo_previousCursorType());
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                break;
            case SETTING_DISPLAY_MODE:
                if (gzInfo_isDisplayModeProgressive()) {
                    gzInfo_setDisplayModeInterlaced();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case SETTING_DROP_SHADOW:
                if (gzInfo_isDropShadows()) {
                    gzInfo_offDropShadows();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case SETTING_MENU_PAUSES_GAME:
                break;
            case SETTING_MENU_SFX:
                if (gzInfo_isMenuSfx()) {
                    gzInfo_offMenuSfx();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case SETTING_SWAP_EQUIPS:
                if (gzInfo_isSwapEquips()) {
                    gzInfo_offSwapEquips();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case SETTING_TEXT_COLOR:
                gzInfo_setTextColor(cycleTextColor(false));
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                break;
            }
        }
    }

    mpMeterHaihai->_execute(0);
}

void gzSettingsMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();

    static const f32 Y_ALIGNMENT = 100.0f;
    static const f32 OPTIONS_X_OFFSET = -20.0f;
    static const f32 HAIHAI_X_OFFSET = 305.0f;
    static const f32 HAIHAI_Y_OFFSET = -7.0f;
    static const f32 HAIHAI_SCALE_FACTOR = 0.04f;
    static const f32 HAIHAI_EXTRA_SPACING = 30.0f;
    static const f32 TP_CURSOR_X_OFFSET = 20.0f;
    static const f32 CURSOR_Y_BASE = 90.0f;
    static const f32 LINE_SPACING = 22.0f;
    static const f32 DESCRIPTION_X = 0.0f;
    static const f32 DESCRIPTION_Y = 420.0f;

    updateDynamicLines();

    J2DTextBox::TFontSize font_size;
    mpLineOptions[0]->getFontSize(font_size);  // assume that all lines have the same font size
    mpMeterHaihai->setScale(font_size.mSizeY * HAIHAI_SCALE_FACTOR);

    u32 cursor_color = gzInfo_getCursorColor();

    f32 x_alignment_opts = mXPos + OPTIONS_X_OFFSET;
    f32 x_alignment_haihai = x_alignment_opts + HAIHAI_X_OFFSET;
    f32 y_alignment_haihai = Y_ALIGNMENT + HAIHAI_Y_OFFSET;
    f32 x_alignment_tp_cursor = mXPos + TP_CURSOR_X_OFFSET;

    for (int i = 0; i < LINE_NUM; i++) {
        f32 y_pos = Y_ALIGNMENT + ((i - 1) * LINE_SPACING);
        f32 y_pos_haihai = y_alignment_haihai + ((i - 1) * LINE_SPACING);
        f32 y_pos_cursor = CURSOR_Y_BASE + ((i - 1) * LINE_SPACING);

        if (l_cursor->y == i && gzInfo_isSubMenuVisible()) {
            // Spacing between arrows determined by text box bound size
            f32 x_size_haihai = mpLineOptions[i]->mBounds.f.x + HAIHAI_EXTRA_SPACING;
            if (mpLineOptions[i]->mStringLength != 0 && mOption) {
                mpMeterHaihai->drawHaihai(getHaihaiFlags(i), x_alignment_haihai, y_pos_haihai, x_size_haihai, 0.0f);
            }

            mpLines[i]->draw(mXPos, y_pos, cursor_color);
            mpDrawCursor->setPos(x_alignment_tp_cursor, y_pos_cursor, (J2DPane*)mpLines[i], false);
            mpLineOptions[i]->draw(x_alignment_opts, y_pos, cursor_color, HBIND_CENTER);
        } else {
            mpLines[i]->draw(mXPos, y_pos, COLOR_WHITE);
            mpLineOptions[i]->draw(x_alignment_opts, y_pos, COLOR_WHITE, HBIND_CENTER);
        }
    }

    if (gzInfo_isSubMenuVisible()) {
        if (mpLines[l_cursor->y] && *mpLines[l_cursor->y]->m_description != 0) {
            f32 description_x = DESCRIPTION_X;
            f32 description_y = g_gzInfo.mBackgroundHeight + 40.0f;

            mpDescription->setString(mpLines[l_cursor->y]->m_description);
            mpDescription->draw(DESCRIPTION_X, description_y, cursor_color, HBIND_CENTER);
        }
    }

    if (gzInfo_isCursorTypeTP()) {
        if (mpDrawCursor != NULL) {
            mpDrawCursor->draw();
        }
    }
}
