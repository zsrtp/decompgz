#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "gz/gz_menu_main.h"
#include "gz/gz_utility_notification.h"
#include "d/d_com_inf_game.h"
#include "d/d_drawlist.h"
#include "d/d_select_cursor.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "m_Do/m_Do_MemCard.h"
#include "dolphin/card.h"
#include "dolphin/gx/GXTexture.h"
#include "f_op/f_op_view.h"

class gzCapture_c : public dDlst_base_c {
public:
    virtual void draw() {
        if (mFlag == 0) {
            return;
        } else if (mFlag == 1) {
            mFlag = 3;
            GXSetTexCopySrc(0, 0, FB_WIDTH, FB_HEIGHT);
            GXSetTexCopyDst(FB_WIDTH / 2, FB_HEIGHT / 2, (GXTexFmt)mDoGph_gInf_c::getFrameBufferTimg()->format, GX_ENABLE);
            GXCopyTex(mDoGph_gInf_c::getFrameBufferTex(), GX_FALSE);
            GXPixModeSync();
            dComIfGp_onPauseFlag();
        } else {
            GXTexObj tex;
            GXInitTexObj(&tex, mDoGph_gInf_c::getFrameBufferTex(), FB_WIDTH / 2, FB_HEIGHT / 2,
                        (GXTexFmt)mDoGph_gInf_c::getFrameBufferTimg()->format, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXInitTexObjLOD(&tex, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            GXSetNumChans(0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            const GXColor color = {0, 0, 0, mAlpha};
            GXSetTevColor(GX_TEVREG0, color);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetZCompLoc(GX_TRUE);
            GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
            GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
            GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
            GXSetFogRangeAdj(GX_FALSE, 0, NULL);
            GXSetCullMode(GX_CULL_NONE);
            GXSetDither(GX_TRUE);
            GXLoadPosMtxImm(g_mDoMtx_identity, GX_PNMTX0);
            GXSetCurrentMtx(0);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGB8, 0);

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3s16(mDoGph_gInf_c::getMinX(), mDoGph_gInf_c::getMinY(), 0);
            GXTexCoord2s8(0, 0);
            GXPosition3s16(mDoGph_gInf_c::getMaxX(), mDoGph_gInf_c::getMinY(), 0);
            GXTexCoord2s8(1, 0);
            GXPosition3s16(mDoGph_gInf_c::getMaxX(), mDoGph_gInf_c::getMaxY(), 0);
            GXTexCoord2s8(1, 1);
            GXPosition3s16(mDoGph_gInf_c::getMinX(), mDoGph_gInf_c::getMaxY(), 0);
            GXTexCoord2s8(0, 1);
            GXEnd();
        }
    }

    virtual ~gzCapture_c() {}

    static const u8 CAPTURE_DIM_ALPHA = 128;

    gzCapture_c() {
        mFlag = 0;
        mAlpha = CAPTURE_DIM_ALPHA;
    }

    void setCaptureFlag() { mFlag = 1; }
    void disable() { mFlag = 0; }
    bool isCapturing() { return mFlag != 0; }

private:
    u8 mFlag;
    u8 mAlpha;
};

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

    mStickTriggers = 0;
    mRepeatDirection = 0;
    mRepeatCounter = 0;

    mDisplay = false;
    mpCapture = NULL;

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

    // If boot to menu is enabled, show the menu immediately
    if (mSettings.mBootToMenu) {
        mDisplay = true;
    }

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
    static const s16 REPEAT_DELAY = 12;  // Frames before repeat starts
    static const s16 REPEAT_RATE = 2;    // Frames between repeats

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

    bool wasDisplayed = mDisplay;

    updateStickTriggers();

    if (gzPad::getHoldL() && gzPad::getHoldR() && gzPad::getTrigDown()) {
        mDisplay = !mDisplay;
        gzClearControllerInput();

        if (mDisplay)
            mInputWaitTimer = 2;
    }

    // Handle menu OPEN transition
    if (!wasDisplayed && mDisplay) {
        if (isMenuPausesGame() && !dComIfGp_isPauseFlag() && mpCapture == NULL) {
            mpCapture = new gzCapture_c();
            mpCapture->setCaptureFlag();
        }
    }

    if (mDisplay) {
        if (mInputWaitTimer != 0) {
            mInputWaitTimer--;
            return 1;
        }

        if (mpMainMenu != NULL && mCursor.x == 0) mpMainMenu->execute();
        if (mpCurrentMenu != NULL && mCursor.x > 0) mpCurrentMenu->execute();

        // Handle setting toggle while menu is displayed
        if (!isMenuPausesGame() && mpCapture != NULL) {
            delete mpCapture;
            mpCapture = NULL;
            dComIfGp_offPauseFlag();
        } else if (isMenuPausesGame() && mpCapture == NULL && !dComIfGp_isPauseFlag()) {
            mpCapture = new gzCapture_c();
            mpCapture->setCaptureFlag();
        }
    } else {
        mCheatsMng.execute();
        mToolsMng.execute();
        mSaveLoaderMng.execute();
    }

    // Handle menu CLOSE transition
    if (wasDisplayed && !mDisplay) {
        if (mpCapture != NULL) {
            delete mpCapture;
            mpCapture = NULL;
        }
        dComIfGp_offPauseFlag();
    }

    return 1;
}

int gzInfo_c::draw() {
    if (!mGZInitialized) return 0;

    // Draw capture directly (dims the background game)
    if (mpCapture != NULL && mpCapture->isCapturing()) {
        mpCapture->draw();
    }

    if (mDisplay) {
        if (mpBackground != NULL) mpBackground->draw(mBackgroundXPos, mBackgroundYPos, mBackgroundWidth, mBackgroundHeight, false, false, false);
        if (mpIcon != NULL) mpIcon->draw(mIconXPos, mIconYPos, mIconWidth, mIconHeight, false, false, false);
        if (mpHeader != NULL) mpHeader->draw(mHeaderXPos, mHeaderYPos, mSettings.mTextColor);
        // Draw menus directly to avoid issues with deferred rendering
        if (mpMainMenu != NULL) mpMainMenu->draw();
        if (mpCurrentMenu != NULL) mpCurrentMenu->draw();
    }

    // Draw any notifications
    if (mpNotification != NULL) mpNotification->draw();

    return 1;
}

void gzSetup2DContext() {
    // Set up 2D orthographic projection for drawing J2DScreen-based elements.
    // This is necessary because GZ draws after the game's normal 2D render pass,
    // when the original graphics context (a stack-local ortho) is no longer valid.
    // We must also set the current graf port because J2DScreen::draw uses it.
    static J2DOrthoGraph sGzOrtho(0.0f, 0.0f, 608.0f, 448.0f, -1.0f, 1.0f);
    sGzOrtho.setPort();
    dComIfGp_setCurrentGrafPort(&sGzOrtho);
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
