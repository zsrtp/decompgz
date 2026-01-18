#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_settings.h"
#include "gz/gz_menu_main.h"
#include "gz/gz_utility_confirm.h"

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
        !gzInfo_isMenuPausesGame() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_BOOT_TO_MENU:
        !gzInfo_isBootToMenu() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
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
    mpMenuPausesGame->getOptionBox()->setStringf("%s", getMenuPausesGameText());
    mpBootToMenu->getOptionBox()->setStringf("%s", getBootToMenuText());
    mpMenuSfx->getOptionBox()->setStringf("%s", getMenuSfxText());
    mpReloadType->getOptionBox()->setStringf("%s", getReloadTypeText());
    mpTextColor->getOptionBox()->setStringf("%s", getTextColorText());
    mpSwapEquips->getOptionBox()->setStringf("%s", getSwapEquipsText());
    updateLineBounds(mpLines, LINE_NUM);
}

int gzSettingsMenu_c::deleteCardConfirmCb(gzConfirm_c* i_confirm, void* i_data) {
    gzInfo_deleteSettingsMemcard();
    gzInfo_seStart(Z2SE_SY_CONTINUE_OK);
    return 1;
}

int gzSettingsMenu_c::deleteCardReturnCb(gzConfirm_c* i_confirm, void* i_data) {
    return 1;
}

gzSettingsMenu_c::gzSettingsMenu_c() {
    OSReport("creating gzSettingsMenu_c\n");
    mXPos = g_gzInfo.mBackgroundXPos + 195.0f;
    mpConfirm = NULL;

    mpCursorType = new gzListOptionLine("cursor type", "sets the cursor type to classic, tp or both", gzInfo_nextCursorType, gzInfo_prevCursorType);
    mpDisplayMode = new gzBoolOptionLine("display mode", "change between progressive and interlaced display modes", gzInfo_getDisplayMode, gzInfo_setDisplayModeProgressive, gzInfo_setDisplayModeInterlaced);
    mpDropShadows = new gzBoolOptionLine("drop shadows", "adds drop shadows to tpgz menu text", gzInfo_isDropShadows, gzInfo_onDropShadows, gzInfo_offDropShadows);
    mpFont = new gzListOptionLine("font", "changes tpgz menu font", gzInfo_nextFont, gzInfo_prevFont);
    mpMenuPausesGame = new gzBoolOptionLine("menu pauses game", "opening gz menu pauses game", gzInfo_isMenuPausesGame, gzInfo_onMenuPausesGame, gzInfo_offMenuPausesGame);
    mpBootToMenu = new gzBoolOptionLine("boot to menu", "boot directly to gz menu on startup", gzInfo_isBootToMenu, gzInfo_onBootToMenu, gzInfo_offBootToMenu);
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
    mpLines[SETTING_BOOT_TO_MENU] = mpBootToMenu;
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

    delete mpCursorType;
    mpCursorType = NULL;

    delete mpDisplayMode;
    mpDisplayMode = NULL;

    delete mpDropShadows;
    mpDropShadows = NULL;

    delete mpFont;
    mpFont = NULL;

    delete mpMenuPausesGame;
    mpMenuPausesGame = NULL;

    delete mpMenuSfx;
    mpMenuSfx = NULL;

    delete mpReloadType;
    mpReloadType = NULL;

    delete mpTextColor;
    mpTextColor = NULL;

    delete mpSwapEquips;
    mpSwapEquips = NULL;

    delete mpSaveCard;
    mpSaveCard = NULL;

    delete mpLoadCard;
    mpLoadCard = NULL;

    delete mpDeleteCard;
    mpDeleteCard = NULL;

    delete mpCommandCombos;
    mpCommandCombos = NULL;

    delete mpMenuPositions;
    mpMenuPositions = NULL;

    delete mpCredits;
    mpCredits = NULL;

    delete mpBootToMenu;
    mpBootToMenu = NULL;

    for (int i = 0; i < LINE_NUM; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;
    }
}

void gzSettingsMenu_c::execute() {
    if (checkInputWait()) return;

    if (mpConfirm != NULL) {
        int rt = mpConfirm->execute();
        if (rt == 1 || rt == 2) {
            delete mpConfirm;
            mpConfirm = NULL;
        }
        return;
    }

    if (handleBackButton(gzMainMenu_c::MENU_SETTINGS)) return;

    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigA()) {
        switch (l_cursor->y) {
        case SETTING_RELOAD_TYPE:
        case SETTING_CURSOR_TYPE:
        case SETTING_DISPLAY_MODE:
        case SETTING_DROP_SHADOW:
        case SETTING_SWAP_EQUIPS:
        case SETTING_MENU_PAUSES_GAME:
        case SETTING_BOOT_TO_MENU:
        case SETTING_MENU_SFX:
        case SETTING_TEXT_COLOR:
            gzInfo_setMenuOption(!gzInfo_isMenuOption());
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
            break;
        case SETTING_SAVE_CARD:
            g_gzInfo.storeSettingsMemcard();
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
            return;
        case SETTING_LOAD_CARD:
            gzInfo_loadSettingsMemcard();
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
            break;
        case SETTING_DELETE_CARD:
            mpConfirm = new gzConfirm_c(deleteCardConfirmCb, deleteCardReturnCb, this, "delete settings?");
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
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

    if (gzPad::getTrigRight()) {
        if (gzInfo_isMenuOption()) {
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
                if (!gzInfo_isMenuPausesGame()) {
                    gzInfo_onMenuPausesGame();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
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
            case SETTING_BOOT_TO_MENU:
                if (!gzInfo_isBootToMenu()) {
                    gzInfo_onBootToMenu();
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
        if (gzInfo_isMenuOption()) {
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
                if (gzInfo_isMenuPausesGame()) {
                    gzInfo_offMenuPausesGame();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
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
            case SETTING_BOOT_TO_MENU:
                if (gzInfo_isBootToMenu()) {
                    gzInfo_offBootToMenu();
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

    handleNavigation(LINE_NUM);
    finishExecute(LINE_NUM);
}

void gzSettingsMenu_c::draw() {
    if (mpConfirm != NULL) {
        mpConfirm->draw();
        return;
    }

    gzCursor* l_cursor = gzInfo_getCursor();
    updateDynamicLines();
    u8 haihai_flags = 0;

    for (int i = 0; i < LINE_NUM; i++) {
        if (l_cursor->y == i)
            haihai_flags = getHaihaiFlags(i);
    }

    drawLines(mpLines, LINE_NUM, haihai_flags, 0, LINE_NUM);
}
