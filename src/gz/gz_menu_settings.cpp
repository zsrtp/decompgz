#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_settings.h"
#include "gz/gz_menu_main.h"
#include "gz/gz_utility_confirm.h"
#include "umbra/umbra_usb.h"

u8 gzSettingsMenu_c::getHaihaiFlags(int i) {
    u8 haihai_flags = gzMenu_c::ARROW_LEFT | gzMenu_c::ARROW_RIGHT;

    switch (i) {
    case gzSettingsMenu_c::SETTING_BOOT_TO_MENU:
        !gzInfo_isBootToMenu() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_CURSOR_TYPE:
        break;
    case gzSettingsMenu_c::SETTING_DISPLAY_MODE:
        !gzInfo_getDisplayMode() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_DROP_SHADOW:
        !gzInfo_isDropShadows() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_MENU_PAUSES_GAME:
        !gzInfo_isMenuPausesGame() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_MENU_SFX:
        !gzInfo_isMenuSfx() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_RELOAD_TYPE:
        !gzInfo_getReloadType() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_STATE_STREAMING:
        !gzInfo_isOnline_StateStreaming() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_SWAP_EQUIPS:
        !gzInfo_isSwapEquips() ? haihai_flags &= ~gzMenu_c::ARROW_LEFT : haihai_flags &= ~gzMenu_c::ARROW_RIGHT;
        break;
    case gzSettingsMenu_c::SETTING_THEME:
        break;
    }

    return haihai_flags;
}

static void storeSettingsCallbackWrapper(void*) {
    gzInfo_storeSettings();
}

static void deleteSettingsCallbackWrapper(void*) {
    gzInfo_deleteSettings();
}

static void returnToSettings() {
    //gzChangeMenu<gzSettingsMenu_c>();
}

void gzSettingsMenu_c::updateDynamicLines() {
    mpBootToMenu->getOptionBox()->setStringf("%s", getBootToMenuText());
    mpCursorType->getOptionBox()->setStringf("%s", getCursorTypeText());
    mpDisplayMode->getOptionBox()->setStringf("%s", getDisplayModeText());
    mpDropShadows->getOptionBox()->setStringf("%s", getDropShadowsText());
    mpMenuPausesGame->getOptionBox()->setStringf("%s", getMenuPausesGameText());
    mpMenuSfx->getOptionBox()->setStringf("%s", getMenuSfxText());
    mpReloadType->getOptionBox()->setStringf("%s", getReloadTypeText());
    mpStateStreaming->getOptionBox()->setStringf("%s", getStateStreamingText());
    mpSwapEquips->getOptionBox()->setStringf("%s", getSwapEquipsText());
    mpTheme->getOptionBox()->setStringf("%s", getThemeText());
    updateLineBounds(mpLines, mLineCount);
}

int gzSettingsMenu_c::deleteConfirmCb(gzConfirm_c* i_confirm, void* i_data) {
    gzInfo_deleteSettings();
    return 1;
}

int gzSettingsMenu_c::deleteReturnCb(gzConfirm_c* i_confirm, void* i_data) {
    return 1;
}

int gzSettingsMenu_c::returnToLoaderConfirmCb(gzConfirm_c* i_confirm, void* i_data) {
    gzInfo_returnToLoader();
    return 1;
}

int gzSettingsMenu_c::returnToLoaderReturnCb(gzConfirm_c* i_confirm, void* i_data) {
    return 1;
}

gzSettingsMenu_c::gzSettingsMenu_c() {
    OSReport("creating gzSettingsMenu_c\n");
    mXPos = g_gzInfo.mBackgroundXPos + 170.0f;
    mLineYStart = gzMenuLayout::TAB_HEADER_Y_OFFSET;
    mpConfirm = NULL;

    // Options (alphabetical)
    mpBootToMenu = new (gzHeap(GZ_GROUP_MENU), 4) gzBoolOptionLine("boot to menu", "boot directly to gz menu on startup", gzInfo_isBootToMenu, gzInfo_onBootToMenu, gzInfo_offBootToMenu);
    mpCursorType = new (gzHeap(GZ_GROUP_MENU), 4) gzListOptionLine("cursor type", "sets the cursor type to classic, tp or both", gzInfo_nextCursorType, gzInfo_prevCursorType);
    mpDisplayMode = new (gzHeap(GZ_GROUP_MENU), 4) gzBoolOptionLine("display mode", "change between progressive and interlaced display modes", gzInfo_getDisplayMode, gzInfo_setDisplayModeProgressive, gzInfo_setDisplayModeInterlaced);
    mpDropShadows = new (gzHeap(GZ_GROUP_MENU), 4) gzBoolOptionLine("drop shadows", "adds drop shadows to tpgz menu text", gzInfo_isDropShadows, gzInfo_onDropShadows, gzInfo_offDropShadows);
    mpMenuPausesGame = new (gzHeap(GZ_GROUP_MENU), 4) gzBoolOptionLine("menu pauses game", "opening gz menu pauses game", gzInfo_isMenuPausesGame, gzInfo_onMenuPausesGame, gzInfo_offMenuPausesGame);
    mpMenuSfx = new (gzHeap(GZ_GROUP_MENU), 4) gzBoolOptionLine("menu sfx", "turn on/off gz menu sound effects", gzInfo_isMenuSfx, gzInfo_onMenuSfx, gzInfo_offMenuSfx);
    mpReloadType = new (gzHeap(GZ_GROUP_MENU), 4) gzBoolOptionLine("reload type", "changes reload type to last file or last area", gzInfo_isReloadArea, gzInfo_setReloadArea, gzInfo_setReloadFile);
    mpStateStreaming = new (gzHeap(GZ_GROUP_MENU), 4) gzBoolOptionLine("state streaming", "stream link state over network", gzInfo_isOnline_StateStreaming, gzInfo_onOnline_StateStreaming, gzInfo_offOnline_StateStreaming);
    mpSwapEquips = new (gzHeap(GZ_GROUP_MENU), 4) gzBoolOptionLine("swap equips", "", gzInfo_isSwapEquips, gzInfo_onSwapEquips, gzInfo_offSwapEquips);
    mpTheme = new (gzHeap(GZ_GROUP_MENU), 4) gzListOptionLine("theme", "changes tpgz menu color theme", gzInfo_nextTextColor, gzInfo_prevTextColor);
    // Actions
    mpCommandCombos = new (gzHeap(GZ_GROUP_MENU), 4) gzLine("command combos", "change default command combos");
    mpMenuPositions = new (gzHeap(GZ_GROUP_MENU), 4) gzLine("menu positions", "set positions of overlay menus");
    mpGdbServer = new (gzHeap(GZ_GROUP_MENU), 4) gzLine("start gdb server", "start listening for gdb on port 2159");
    mpSave = new (gzHeap(GZ_GROUP_MENU), 4) gzLine("save settings", "saves tpgz settings");
    mpLoad = new (gzHeap(GZ_GROUP_MENU), 4) gzLine("load settings", "loads tpgz settings");
    mpDelete = new (gzHeap(GZ_GROUP_MENU), 4) gzLine("delete settings", "deletes tpgz settings");
    mpReturnToLoader = NULL;

    mpLines[SETTING_BOOT_TO_MENU] = mpBootToMenu;
    mpLines[SETTING_CURSOR_TYPE] = mpCursorType;
    mpLines[SETTING_DISPLAY_MODE] = mpDisplayMode;
    mpLines[SETTING_DROP_SHADOW] = mpDropShadows;
    mpLines[SETTING_MENU_PAUSES_GAME] = mpMenuPausesGame;
    mpLines[SETTING_MENU_SFX] = mpMenuSfx;
    mpLines[SETTING_RELOAD_TYPE] = mpReloadType;
    mpLines[SETTING_STATE_STREAMING] = mpStateStreaming;
    mpLines[SETTING_SWAP_EQUIPS] = mpSwapEquips;
    mpLines[SETTING_THEME] = mpTheme;
    mpLines[SETTING_COMMAND_COMBOS] = mpCommandCombos;
    mpLines[SETTING_MENU_POSITIONS] = mpMenuPositions;
    mpLines[SETTING_GDB_SERVER] = mpGdbServer;
    mpLines[SETTING_SAVE] = mpSave;
    mpLines[SETTING_LOAD] = mpLoad;
    mpLines[SETTING_DELETE] = mpDelete;

    if (umbraIsNintendont()) {
        mpReturnToLoader = new (gzHeap(GZ_GROUP_MENU), 4) gzLine("return to loader", "exit game and return to homebrew channel");
        mpLines[SETTING_RETURN_TO_LOADER] = mpReturnToLoader;
        mLineCount = LINE_NUM;
    } else {
        mLineCount = LINE_NUM - 1;
    }
}

gzSettingsMenu_c::~gzSettingsMenu_c() {
    _delete();
}

void gzSettingsMenu_c::_delete() {
    OSReport("deleting gzSettingsMenu_c\n");

    // Options (alphabetical)
    delete mpBootToMenu;
    mpBootToMenu = NULL;

    delete mpCursorType;
    mpCursorType = NULL;

    delete mpDisplayMode;
    mpDisplayMode = NULL;

    delete mpDropShadows;
    mpDropShadows = NULL;

    delete mpMenuPausesGame;
    mpMenuPausesGame = NULL;

    delete mpMenuSfx;
    mpMenuSfx = NULL;

    delete mpReloadType;
    mpReloadType = NULL;

    delete mpStateStreaming;
    mpStateStreaming = NULL;

    delete mpSwapEquips;
    mpSwapEquips = NULL;

    delete mpTheme;
    mpTheme = NULL;

    // Actions
    delete mpCommandCombos;
    mpCommandCombos = NULL;

    delete mpMenuPositions;
    mpMenuPositions = NULL;

    delete mpGdbServer;
    mpGdbServer = NULL;

    delete mpSave;
    mpSave = NULL;

    delete mpLoad;
    mpLoad = NULL;

    delete mpDelete;
    mpDelete = NULL;

    delete mpReturnToLoader;
    mpReturnToLoader = NULL;

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
        case SETTING_STATE_STREAMING:
        case SETTING_SWAP_EQUIPS:
        case SETTING_MENU_PAUSES_GAME:
        case SETTING_BOOT_TO_MENU:
        case SETTING_MENU_SFX:
        case SETTING_THEME:
            gzInfo_setMenuOption(!gzInfo_isMenuOption());
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
            break;
        case SETTING_SAVE:
            gzInfo_storeSettings();
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
            return;
        case SETTING_LOAD:
            gzInfo_loadSettings();
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
            break;
        case SETTING_DELETE:
            mpConfirm = new (gzHeap(GZ_GROUP_UI), 4) gzConfirm_c(deleteConfirmCb, deleteReturnCb, this, "delete settings?");
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
            return;
        case SETTING_MENU_POSITIONS:
            gzInfo_sendNotification("test!", 1);
            gzInfo_sendNotification("test!", 2);
            gzInfo_sendNotification("test2!");
            if (umbra::usb_probe(1)) {
                char msg[] = "Hello from USBGecko!\n";
                umbra::usb_send(1, msg, sizeof(msg));
                gzInfo_sendNotification("USBGecko: Hello!");
            } else {
                gzInfo_sendNotification("USBGecko not found!");
            }
            break;
        case SETTING_GDB_SERVER:
            if (g_gzInfo.mNet.gdbStart(2159) == 0) {
                gzInfo_sendNotification("gdb server started on port 2159");
            } else {
                gzInfo_sendNotification("failed to start gdb server");
            }
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
            break;
        case SETTING_RETURN_TO_LOADER:
            mpConfirm = new (gzHeap(GZ_GROUP_UI), 4) gzConfirm_c(returnToLoaderConfirmCb, returnToLoaderReturnCb, this, "return to loader?");
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
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
            case SETTING_STATE_STREAMING:
                if (!gzInfo_isOnline_StateStreaming()) {
                    gzInfo_onOnline_StateStreaming();
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
            case SETTING_THEME:
                gzInfo_setTextColor(mpTheme->mpNext());
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
            case SETTING_STATE_STREAMING:
                if (gzInfo_isOnline_StateStreaming()) {
                    gzInfo_offOnline_StateStreaming();
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
            case SETTING_THEME:
                gzInfo_setTextColor(mpTheme->mpPrev());
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                break;
            }
        }
    }

    handleNavigation(mLineCount);
    finishExecute(mLineCount);
}

void gzSettingsMenu_c::draw() {
    if (mpConfirm != NULL) {
        mpConfirm->draw();
        return;
    }

    gzCursor* l_cursor = gzInfo_getCursor();
    updateDynamicLines();
    u8 haihai_flags = 0;

    for (int i = 0; i < mLineCount; i++) {
        if (l_cursor->y == i)
            haihai_flags = getHaihaiFlags(i);
    }

    drawLines(mpLines, mLineCount, haihai_flags, 0, mLineCount);
}
