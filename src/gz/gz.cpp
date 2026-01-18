#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "gz/gz_menu_main.h"
#include "gz/gz_utility_notification.h"
#include "d/d_select_cursor.h"
#include "m_Do/m_Do_controller_pad.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "m_Do/m_Do_MemCard.h"
#include "dolphin/card.h"

gzInfo_c g_gzInfo;

int gzPrint(int x, int y, u32 color, char const* string, ...) {
    JUTDbPrint::getManager()->setVisible(true);
    char buffer[256];

    va_list list;
    va_start(list, string);
    vsnprintf(buffer, sizeof(buffer), string, list);
    va_end(list);

    JUTDbPrint::getManager()->flush();

    static JUtility::TColor ShadowDarkColor(0, 0, 0, 0x80);
    JUTDbPrint::getManager()->setCharColor(ShadowDarkColor);

    JUTReport(x + 2, y + 2, buffer);
    JUTDbPrint::getManager()->flush();

    JUTDbPrint::getManager()->setCharColor(color);
    JUTReport(x, y, buffer);

    JUTDbPrint::getManager()->flush();
    return 1;
}

void gzDVDLoadFile(const char* filePath, void* buffer, int length, int offset) {
    DVDFileInfo ATTRIBUTE_ALIGN(32) fileInfo;
    if (DVDOpen(filePath, &fileInfo)) {
        int bytesRead = DVDReadPrio(&fileInfo, buffer, length, offset, 2);
        if (bytesRead > 0) {
            DVDClose(&fileInfo);
        } else {
            OSReport("no bytes read!\n");
        }
    } else {
        OSReport("failed to open file %s\n", filePath);
    }
}

void gzInfo_c::loadDefaultSettings() {
    mSettings.mTextColor = 0xEE8000FF;
    mSettings.mCommandCombos.mMoveLink = (PAD_TRIGGER_L | PAD_TRIGGER_R | PAD_BUTTON_Y);
    mSettings.mCommandCombos.mMoonJump = (PAD_TRIGGER_R | PAD_BUTTON_A);
    mSettings.mCommandCombos.mTeleportSave = (PAD_TRIGGER_R | PAD_BUTTON_UP);
    mSettings.mCommandCombos.mTeleportLoad = (PAD_TRIGGER_R | PAD_BUTTON_DOWN);
    mSettings.mDropShadows = true;
    mSettings.mMenuPausesGame = true;
    setCursorType(1);
    mpFont = mDoExt_getMesgFont();
    mCursor.x = 0;
    mCursor.y = 0;
    mSettings.mMenuSfx = true;

    // Initialize input state for menu navigation
    mStickTriggers = 0;
    mRepeatDirection = 0;
    mRepeatCounter = 0;

    mBackgroundXPos = 5.0f;
    mBackgroundYPos = 5.0f;
    mBackgroundWidth = 595.0f;
    mBackgroundHeight = 415.0f;

    mIconXPos = mBackgroundXPos + 5.0f;
    mIconYPos = mBackgroundYPos + 5.0f;
    mIconWidth = 30.0f;
    mIconHeight = 30.0f;

    mHeaderXPos = mBackgroundXPos + 35.0f;
    mHeaderYPos = mBackgroundYPos + 25.0f;
}


int gzInfo_c::_create() {
    OSReport("creating gzInfo_c\n");

    // Store current heap settings until gz is done allocating
    JKRHeap* oldHeap = mDoExt_getCurrentHeap();
    JKRExpHeap* archiveHeap = (JKRExpHeap*)mDoExt_getArchiveHeap();
    u8 oldGroupId = archiveHeap->mCurrentGroupId;

    // Alloc on archive heap
    // Set group ID for identifying gz allocations in heaps menu
    mDoExt_setCurrentHeap(archiveHeap);
    archiveHeap->mCurrentGroupId = mGzGroupID = 0x69;

    // load default settings. config from mem card will overwrite if it exists
    loadDefaultSettings();

    ResTIMG* icon = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "midona64.bti");
    mpIcon = new J2DPicture(icon);

    // TODO: replace this with something better or alloc it on ARAM
    void* buf = JKRHeap::alloc(108960, 32, NULL);
    gzDVDLoadFile("/gz/bg.bti", buf, 108960, 0);
    ResTIMG* bg = (ResTIMG*)buf;
    mpBackground = new J2DPicture(bg);
    mpHeader = gzTextBox_allocate();
    mpHeader->setString("tpgz v2.0.0");
    
    mpMainMenu = new gzMainMenu_c();
    if (mpMainMenu == NULL) {
        return 0;
    }

    mpNotification = new gzNotification_c();

    mpTPCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpTPCursor->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    mpTPCursor->setAlphaRate(1.0f);

    mpMenuDescription = gzTextBox_allocate();
    mMenuOption = false;
    mTopLine = 0;
    mVisibleLines = 15;

    mInputWaitTimer = 2;
    mGZInitialized = true;

    loadSettingsMemcard();

    // initialize oxygen now instead of waiting to go to the file select screen
    dComIfGp_setOxygen(600);
    dComIfGp_setNowOxygen(600);
    dComIfGp_setMaxOxygen(600);
    
    // load the default menu
    gzChangeMenu(mpMainMenu->getMenu(0));

    // Restore original group ID and heap
    archiveHeap->mCurrentGroupId = oldGroupId;
    mDoExt_setCurrentHeap(oldHeap);
    
    return 1;
}

int gzInfo_c::_delete() {
    OSReport("deleting gzInfo_c\n");
    delete mpIcon;
    mpIcon = NULL;

    gzTextBox_free(mpHeader);
    mpHeader = NULL;

    delete mpBackground;
    mpBackground = NULL;

    mpCurrentMenu = NULL;

    delete mpMainMenu;
    mpMainMenu = NULL;

    delete mpNotification;
    mpNotification = NULL;

    delete mpTPCursor;
    mpTPCursor = NULL;

    gzTextBox_free(mpMenuDescription);
    mpMenuDescription = NULL;
    return 1;
}

void gzInfo_c::updateStickTriggers() {
    static const f32 STICK_THRESHOLD = 0.5f;
    static const s16 REPEAT_DELAY = 14;  // Frames before repeat starts
    static const s16 REPEAT_RATE = 4;    // Frames between repeats

    // Get current direction from d-pad
    u32 currentDir = 0;
    u32 dpadHold = mDoCPd_c::m_gzPadInfo.mButtonFlags & (PAD_BUTTON_UP | PAD_BUTTON_DOWN | PAD_BUTTON_LEFT | PAD_BUTTON_RIGHT);
    currentDir |= dpadHold;

    // Only use analog stick for menu navigation if menu pauses game is enabled
    // (otherwise stick passes through to game)
    if (gzInfo_isMenuPausesGame()) {
        f32 stickX = gzPad::getStickX();
        f32 stickY = gzPad::getStickY();

        if (stickY > STICK_THRESHOLD) currentDir |= PAD_BUTTON_UP;
        if (stickY < -STICK_THRESHOLD) currentDir |= PAD_BUTTON_DOWN;
        if (stickX < -STICK_THRESHOLD) currentDir |= PAD_BUTTON_LEFT;
        if (stickX > STICK_THRESHOLD) currentDir |= PAD_BUTTON_RIGHT;
    }

    mStickTriggers = 0;

    if (currentDir == 0) {
        // No direction held - reset
        mRepeatDirection = 0;
        mRepeatCounter = 0;
    } else if (currentDir != mRepeatDirection) {
        // New direction - trigger immediately and start counting
        mStickTriggers = currentDir;
        mRepeatDirection = currentDir;
        mRepeatCounter = 0;
    } else {
        // Same direction held - handle repeat
        mRepeatCounter++;
        if (mRepeatCounter == REPEAT_DELAY) {
            mStickTriggers = currentDir;
        } else if (mRepeatCounter > REPEAT_DELAY && (mRepeatCounter - REPEAT_DELAY) % REPEAT_RATE == 0) {
            mStickTriggers = currentDir;
        }
    }
}

int gzInfo_c::execute() {
    if (!mGZInitialized) return 0;

    updateStickTriggers();

    if (gzPad::getHoldL() && gzPad::getHoldR() && gzPad::getTrigDown()) {
        mDisplay = !mDisplay;

        // Null out game inputs to prevent ring menu from opening
        interface_of_controller_pad& cpad = mDoCPd_c::getCpadInfo(PAD_1);
        cpad.mPressedButtonFlags = 0;
        cpad.mButtonFlags = 0;
        cpad.mMainStickPosX = 0.0f;
        cpad.mMainStickPosY = 0.0f;
        cpad.mMainStickValue = 0.0f;
        cpad.mMainStickAngle = 0;
        cpad.mCStickPosX = 0.0f;
        cpad.mCStickPosY = 0.0f;
        cpad.mCStickValue = 0.0f;
        cpad.mCStickAngle = 0;
        cpad.mAnalogA = 0.0f;
        cpad.mAnalogB = 0.0f;
        cpad.mTriggerLeft = 0.0f;
        cpad.mTriggerRight = 0.0f;

        if (mDisplay)
            mInputWaitTimer = 2;
    }

    if (mDisplay) {
        if (mInputWaitTimer != 0) {
            mInputWaitTimer--;
            return 1;
        }
        
        if (mpMainMenu != NULL && mCursor.x == 0) mpMainMenu->execute();
        if (mpCurrentMenu != NULL && mCursor.x > 0) mpCurrentMenu->execute();
    } else {
        // may need to be more selective here on what does/doesn't apply 
        // when the menu is/isn't up
        mCheatsMng.execute();
        mToolsMng.execute();
        mSaveLoaderMng.execute();
    }

    return 1;
}

int gzInfo_c::draw() {
    if (!mGZInitialized) return 0;

    if (mDisplay) {
        if (mpBackground != NULL) mpBackground->draw(mBackgroundXPos, mBackgroundYPos, mBackgroundWidth, mBackgroundHeight, false, false, false);
        if (mpIcon != NULL) mpIcon->draw(mIconXPos, mIconYPos, mIconWidth, mIconHeight, false, false, false);
        if (mpHeader != NULL) mpHeader->draw(mHeaderXPos, mHeaderYPos, mSettings.mTextColor);
        if (mpMainMenu != NULL) dComIfGd_set2DOpaTop(mpMainMenu);
        if (mpCurrentMenu != NULL) dComIfGd_set2DOpaTop(mpCurrentMenu);
    }

    // Draw any notifications
    if (mpNotification != NULL) mpNotification->draw();

    return 1;
}

int gzInfo_c::storeSettingsMemcard() {
    CARDFileInfo file;
    int ret;

    ret = CARDProbeEx(0, NULL, NULL);
    if (ret != CARD_RESULT_READY) {
        return -1;
    }

    ret = CARDCreate(0, "tpgzcfg", SECTOR_SIZE, &file);
    if (ret == CARD_RESULT_READY || ret == CARD_RESULT_EXIST) {
        ret = CARDOpen(0, "tpgzcfg", &file);
        if (ret == CARD_RESULT_READY) {
            gzConfigHeader_s cfg;
            cfg.version = GZ_SAVE_VERSION;
            cfg.settingsOffset = sizeof(gzConfigHeader_s);

            memcpy(mDoMemCd_Ctrl_c::sTmpBuf, &cfg, sizeof(gzConfigHeader_s));
            memcpy(mDoMemCd_Ctrl_c::sTmpBuf + cfg.settingsOffset, &mSettings, sizeof(gzSettings_s));

            ret = CARDWrite(&file, mDoMemCd_Ctrl_c::sTmpBuf, SECTOR_SIZE, 0);
            if (ret == CARD_RESULT_READY) {
                OSReport("stored tpgz settings to memcard!\n");
                gzInfo_sendNotification("settings saved!");
            }

            CARDClose(&file);
        }
    }

    return ret;
}

int gzInfo_c::loadSettingsMemcard() {
    CARDFileInfo file;
    int ret;

    ret = CARDProbeEx(0, NULL, NULL);
    if (ret != CARD_RESULT_READY) {
        return -1;
    }

    ret = CARDOpen(0, "tpgzcfg", &file);
    if (ret == CARD_RESULT_READY) {
        ret = CARDRead(&file, mDoMemCd_Ctrl_c::sTmpBuf, SECTOR_SIZE, 0);
        if (ret == CARD_RESULT_READY) {
            OSReport("loaded tpgz settings from memcard!\n");
            gzInfo_sendNotification("settings loaded!");

            gzConfigHeader_s cfg;
            memcpy(&cfg, mDoMemCd_Ctrl_c::sTmpBuf, sizeof(gzConfigHeader_s));
            if (cfg.version != GZ_SAVE_VERSION) {
                OSReport("outdated tpgz save version!\n");
                return -1;
            }

            gzSettings_s settings;
            memcpy(&mSettings, mDoMemCd_Ctrl_c::sTmpBuf + cfg.settingsOffset, sizeof(gzSettings_s));
        }

        CARDClose(&file);
    } else {
        gzInfo_sendNotification("no stored settings found!");
    }

    return ret;
}

int gzInfo_c::deleteSettingsMemcard() {
    CARDFileInfo file;
    int ret;

    ret = CARDProbeEx(0, NULL, NULL);
    if (ret != CARD_RESULT_READY) {
        return -1;
    }

    ret = CARDDelete(0, "tpgzcfg");
    if (ret == CARD_RESULT_READY) {
        OSReport("deleted tpgz settings from memcard!\n");
        gzInfo_sendNotification("settings deleted!");
    } else {
        OSReport_Error("failed to delete tpgz settings from memcard!\n");
    }

    return ret;
}

void gzInfo_c::sendNotification(const char* msg) { 
    if (mpNotification != NULL) mpNotification->send(msg);
}

void gzInfo_c::sendNotification(const char* msg, int i_notificationType) { 
    if (mpNotification != NULL) mpNotification->send(msg, (gzNotification_c::NotificationType)i_notificationType);
}
