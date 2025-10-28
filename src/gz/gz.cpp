#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "gz/gz_menu.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "m_Do/m_Do_MemCard.h"
#include "dolphin/card.h"
#include "d/d_meter_HIO.h"
#include "f_op/f_op_camera_mng.h"
#include "d/actor/d_a_alink.h"

#include "d/d_debug_viewer.h"

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
    DVDFileInfo ALIGN_DECL(32) fileInfo;
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
    setCursorType(1);
    mpFont = mDoExt_getMesgFont();
    mCursor.x = 0;
    mCursor.y = 0;
    mSettings.mMenuSfx = true;

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

void* mogaCubeImgBuffer;

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

    mogaCubeImgBuffer = JKRHeap::alloc(1240, 32, NULL);
    gzDVDLoadFile("/gz/moga.bti", mogaCubeImgBuffer, 1240, 0);

    ResTIMG* bg = (ResTIMG*)buf;
    mpBackground = new J2DPicture(bg);
    mpHeader = new gzTextBox("tpgz v2.0.0", mSettings.mTextColor);
    
    mpMainMenu = new gzMainMenu_c();
    if (mpMainMenu == NULL) {
        return 0;
    }

    mpNotification = new gzNotification_c();

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

    delete mpHeader;
    mpHeader = NULL;

    delete mpBackground;
    mpBackground = NULL;

    mpCurrentMenu = NULL;

    delete mpMainMenu;
    mpMainMenu = NULL;

    delete mpNotification;
    mpNotification = NULL;
    return 1;
}

int gzInfo_c::execute() {
    if (!mGZInitialized) return 0;

    if (gzPad::getHoldL() && gzPad::getHoldR() && gzPad::getTrigDown()) {
        mDisplay = !mDisplay;

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
    }

    mCheatsMng.execute();
    mToolsMng.execute();
    mSaveLoaderMng.execute();

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

        // randomly crashes?
        // if (mpCurrentMenu != NULL) mpCurrentMenu->draw();

        // showHeapUsage();
    }

    // Draw any notifications
    if (mpNotification != NULL) mpNotification->draw();

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (player != NULL) {
        static u32 counter = 0;

        cXyz pos;
        cXyz offset(0.0f, 100.0f, 100.0f);
        cLib_offsetPos(&pos, &player->current.pos, player->current.angle.y, &offset);

        cXyz size(32.0f, 32.0f, 32.0f);
        csXyz rot(counter, 0, counter);
        GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
        dDbVw_drawCubeOpa(pos, size, rot, color);
        counter += 1000;
    }

    return 1;
}

void gzInfo_c::showHeapUsage() {
    if (zeldaHeap != NULL && gameHeap != NULL && archiveHeap != NULL) {
        u32 zeldaFree = zeldaHeap->getFreeSize();
        u32 gameFree = gameHeap->getFreeSize();
        u32 archiveFree = archiveHeap->getFreeSize();
        u32 zeldaTotal = zeldaHeap->getTotalFreeSize();
        u32 gameTotal = gameHeap->getTotalFreeSize();
        u32 archiveTotal = archiveHeap->getTotalFreeSize();

        gzPrint(200, 30, 0xFFFFFFFF, "  Zelda %5d / %5d\n", zeldaFree / 1024, zeldaTotal / 1024);
        gzPrint(200, 50, 0xFFFFFFFF, "   Game %5d / %5d\n", gameFree / 1024, gameTotal / 1024);
        gzPrint(200, 70, 0xFFFFFFFF, "Archive %5d / %5d\n", archiveFree / 1024, archiveTotal / 1024);
    }
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
