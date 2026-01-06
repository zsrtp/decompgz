#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_settings.h"
#include "gz/gz_menu_main.h"

u8 gzSettingsMenu_c::getHaihaiFlags(int i) {
    u8 haihai_flags = gzMenu_c::ARROW_LEFT | gzMenu_c::ARROW_RIGHT;

    switch (i) {
    case gzSettingsMenu_c::SETTING_RELOAD_TYPE:
        !gzInfo_getReloadType() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_CURSOR_TYPE: {
        break;
    }
    case gzSettingsMenu_c::SETTING_DISPLAY_MODE:
        !gzInfo_getDisplayMode() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_DROP_SHADOW:
        !gzInfo_isDropShadows() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_MENU_PAUSES_GAME:
        haihai_flags = 0;
        break;
    case gzSettingsMenu_c::SETTING_MENU_SFX:
        !gzInfo_isMenuSfx() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_FONT:
        haihai_flags = 0;
        break;
    case gzSettingsMenu_c::SETTING_SWAP_EQUIPS:
        !gzInfo_isSwapEquips() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_TEXT_COLOR: {
        break;
    }
    }

    return haihai_flags;
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
    mpCursorType->getOptionBox()->setStringf("%s", getCursorTypeText());
    mpDisplayMode->getOptionBox()->setStringf("%s", getDisplayModeText());
    mpDropShadows->getOptionBox()->setStringf("%s", getDropShadowsText());
    mpFont->getOptionBox()->setStringf("%s", "rodan");
    mpMenuPausesGame->getOptionBox()->setStringf("%s", "no");
    mpMenuSfx->getOptionBox()->setStringf("%s", getMenuSfxText());
    mpReloadType->getOptionBox()->setStringf("%s", getReloadTypeText());
    mpTextColor->getOptionBox()->setStringf("%s", getTextColorText());
    mpSwapEquips->getOptionBox()->setStringf("%s", getSwapEquipsText());

    J2DTextBox::TFontSize font_size;
    for (s32 i = 0; i < LINE_NUM; i++) {
        gzLine* line = mpLines[i];
        line->mText->getFontSize(font_size);
        font_size.mSizeX *= 0.5f;
        line->mText->mBounds.f.x = line->mText->mStringLength * font_size.mSizeX;
        gzTextBox* opt = line->getOptionBox();
        if (opt) {
            opt->getFontSize(font_size);
            font_size.mSizeX *= 0.5f;
            opt->mBounds.f.x = opt->mStringLength * font_size.mSizeX;
        }
    }
}

gzSettingsMenu_c::gzSettingsMenu_c() {
    OSReport("creating gzSettingsMenu_c\n");

    mpCursorType = new gzListOptionLine("cursor type", "sets the cursor type to classic, tp or both", gzInfo_nextCursorType, gzInfo_prevCursorType);
    mpDisplayMode = new gzBoolOptionLine("display mode", "change between progressive and interlaced display modes", gzInfo_getDisplayMode, gzInfo_setDisplayModeProgressive, gzInfo_setDisplayModeInterlaced);
    mpDropShadows = new gzBoolOptionLine("drop shadows", "adds drop shadows to tpgz menu text", gzInfo_isDropShadows, gzInfo_onDropShadows, gzInfo_offDropShadows);
    mpFont = new gzListOptionLine("font", "changes tpgz menu font", gzInfo_nextFont, gzInfo_prevFont);
    mpMenuPausesGame = new gzBoolOptionLine("menu pauses game", "opening gz menu pauses game", gzInfo_isMenuPausesGame, gzInfo_onMenuPausesGame, gzInfo_offMenuPausesGame);
    mpMenuSfx = new gzBoolOptionLine("menu sfx", "turn on/off gz menu sound effects", gzInfo_isMenuSfx, gzInfo_onMenuSfx, gzInfo_offMenuSfx);
    mpReloadType = new gzBoolOptionLine("reload type", "changes reload type to last file or last area", gzInfo_isReloadArea, gzInfo_setReloadArea, gzInfo_setReloadFile);
    mpTextColor = new gzListOptionLine("text color", "changes tpgz menu text color", gzInfo_nextTextColor, gzInfo_prevTextColor);
    mpSwapEquips = new gzBoolOptionLine("swap equips", "", gzInfo_isSwapEquips, gzInfo_onSwapEquips, gzInfo_offSwapEquips);
    mpSaveCard = new gzLine("save card", "saves tpgz settings to memory card");
    mpLoadCard = new gzLine("load card", "loads tpgz settings from memory card");
    mpDeleteCard = new gzLine("delete card", "deletes tpgz settings from memory card");
    mpCommandCombos = new gzLine("command combos", "change default command combos");
    mpMenuPositions = new gzLine("menu positions", "set positions of overlay menus");
    mpCredits = new gzLine("credits", "show the tpgz credits");

    mpLines[SETTING_CURSOR_TYPE] = mpCursorType;
    mpLines[SETTING_DISPLAY_MODE] = mpDisplayMode;
    mpLines[SETTING_DROP_SHADOW] = mpDropShadows;
    mpLines[SETTING_FONT] = mpFont;
    mpLines[SETTING_MENU_PAUSES_GAME] = mpMenuPausesGame;
    mpLines[SETTING_MENU_SFX] = mpMenuSfx;
    mpLines[SETTING_RELOAD_TYPE] = mpReloadType;
    mpLines[SETTING_TEXT_COLOR] = mpTextColor;
    mpLines[SETTING_SWAP_EQUIPS] = mpSwapEquips;
    mpLines[SETTING_SAVE_CARD] = mpSaveCard;
    mpLines[SETTING_LOAD_CARD] = mpLoadCard;
    mpLines[SETTING_DELETE_CARD] = mpDeleteCard;
    mpLines[SETTING_COMMAND_COMBOS] = mpCommandCombos;
    mpLines[SETTING_MENU_POSITIONS] = mpMenuPositions;
    mpLines[SETTING_CREDITS] = mpCredits;
}

gzSettingsMenu_c::~gzSettingsMenu_c() {
    _delete();
}

void gzSettingsMenu_c::_delete() {
    OSReport("deleting gzSettingsMenu_c\n");

    mpCursorType = NULL;
    delete mpCursorType;

    mpDisplayMode = NULL;
    delete mpDisplayMode;

    mpDropShadows = NULL;
    delete mpDropShadows;
    
    mpFont = NULL;
    delete mpFont;
    
    mpMenuPausesGame = NULL;
    delete mpMenuPausesGame;
    
    mpMenuSfx = NULL;
    delete mpMenuSfx;
    
    mpReloadType = NULL;
    delete mpReloadType;
    
    mpTextColor = NULL;
    delete mpTextColor;

    mpSwapEquips = NULL;
    delete mpSwapEquips;
    
    mpSaveCard = NULL;
    delete mpSaveCard;
    
    mpLoadCard = NULL;
    delete mpLoadCard;
    
    mpDeleteCard = NULL;
    delete mpDeleteCard;
    
    mpCommandCombos = NULL;
    delete mpCommandCombos;
    
    mpMenuPositions = NULL;
    delete mpMenuPositions;
    
    mpCredits = NULL;
    delete mpCredits;

    for (int i = 0; i < LINE_NUM; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;
    }
}

void gzSettingsMenu_c::execute() {
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return;
    }
    
    gzCursor* l_cursor = gzInfo_getCursor();

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
                gzInfo_setTextColor(mpTextColor->mpNext());
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
                gzInfo_setCursorType(gzInfo_prevCursorType());
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
                gzInfo_setTextColor(mpTextColor->mpPrev());
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                break;
            }
        }
    }

    gzMenu_c::execute();
}

void gzSettingsMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();
    updateDynamicLines();
    u8 haihai_flags = 0;

    for (int i = 0; i < LINE_NUM; i++) {
        if (l_cursor->y == i)
            haihai_flags = getHaihaiFlags(i);
    }

    drawLines(mpLines, LINE_NUM, haihai_flags, 0, LINE_NUM);
}
