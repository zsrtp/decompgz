#ifndef GZ_H
#define GZ_H

#include "c/c_damagereaction.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "gz/gz_manager_cheats.h"
#include "gz/gz_manager_practice.h"
#include "gz/gz_manager_tools.h"
#include "JSystem/J2DGraph/J2DPicture.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_machine.h"
#include "SSystem/SComponent/c_API_controller_pad.h"

class gzMenu_c;
class gzTextBox;
class gzMainMenu_c;
class gzNotification_c;
class gzToolsMng_c;
class dSelect_cursor_c;
class gzCapture_c;

#define COLOR_WHITE 0xFFFFFFFFu
#define COLOR_RED 0xFF0000FFu
#define COLOR_BLUE 0x0000FFFFu
#define COLOR_GREEN 0x00FF00FFu

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

struct gzCommandCombos_s {
    u32 mMoveLink;
    u32 mMoonJump;
    u32 mTeleportSave;
    u32 mTeleportLoad;
};

struct gzSettings_s {
    u32 mTextColor;  // todo: just make this an index?
    bool mDropShadows;
    bool mSwapEquips;
    bool mReloadType;
    u8 mCursorType;
    bool mDisplayMode;
    bool mMenuPausesGame;
    bool mBootToMenu;
    bool mMoveLink;
    gzCommandCombos_s mCommandCombos;
    bool mMenuSfx;
    BOOL mCoroTD;
    bool mDisableItemTimer;
    bool mDisableWalls;
    bool mElevatorEscape;
    bool mEndingBlowMoonBoots;
    bool mGorgeVoid;
    bool mInfiniteAir;
    bool mInfiniteArrows;
    bool mInfiniteBombs;
    bool mInfiniteHearts;
    bool mInfiniteOil;
    bool mInfiniteRupees;
    bool mInfiniteSlingshot;
    bool mInvincibleLink;
    bool mInvincibleEnemies;
    bool mLadderFreezardCancel;
    bool mMoonJump;
    bool mRolling;
    bool mSuperClawshot;
    bool mTransformAnywhere;
    bool mUniversalMapDelay;
    bool mUnrestrictedItems;
    bool mAbMashRate;
    bool mFastBonkRecovery;
    bool mFastMovement;
    bool mInGameTimer;
    bool mInputViewer;
    bool mLinkDebugInfo;
    bool mLoadTimer;
    bool mNoSinkingInSand;
    bool mStageInfo;
    bool mTeleport;
    bool mTimer;
};

// tpgz config file header
struct gzConfigHeader_s {
    u32 version;
    u32 settingsOffset;

    u8 reserved[0x20 - 0x8];
};

struct gzCursor {
    int x;
    int y;
};

class gzInfo_c {
public:
    static const int GZ_SAVE_VERSION = 1;

    gzInfo_c() { mGZInitialized = false; };

    enum gzInfoMenu_CursorType_e {
        CURSOR_CLASSIC = 1,
        CURSOR_TP,
        CURSOR_BOTH,
    };

    int _create();
    int _delete();
    int execute();
    int draw();
    void updateStickTriggers();

    void loadDefaultSettings();
    int storeSettingsMemcard();
    int loadSettingsMemcard();
    int deleteSettingsMemcard();
    void showHeapUsage();
    void sendNotification(const char* msg);
    void sendNotification(const char* msg, int i_notificationType);
    void setButtonFlags();
    void executeTools();
    void executeMoveLink();

    gzCursor* getCursor() { return &mCursor; }
    dSelect_cursor_c* getTPCursor() { return mpTPCursor; }
    gzTextBox* getMenuDescription() { return mpMenuDescription; }
    bool isMenuOption() { return mMenuOption; }
    void setMenuOption(bool i_opt) { mMenuOption = i_opt; }
    s32 getTopLine() { return mTopLine; }
    void setTopLine(s32 i_topLine) { mTopLine = i_topLine; }
    s32 getVisibleLines() { return mVisibleLines; }
    void setVisibleLines(s32 i_visibleLines) { mVisibleLines = i_visibleLines; }
    u32 getCursorColor() { return getCursorType() & CURSOR_CLASSIC ? getTextColor() : COLOR_WHITE; }
    u8 getCursorType() const { return mSettings.mCursorType; }
    bool getDisplayMode() const { return mSettings.mDisplayMode; }
    JUTFont* getFont() { return mpFont; }
    bool getReloadType() const { return mSettings.mReloadType; }
    u32 getTextColor() const { return mSettings.mTextColor; }
    bool isAbMashRate() { return mSettings.mAbMashRate; }
    u8 getBossFlag() { return cDmr_SkipInfo; }

    bool isCoroTD() { 
        mSettings.mCoroTD = dComIfGs_isTmpBit(0x0002);
        return mSettings.mCoroTD;
    }
    bool isCursorTypeClassic() { return getCursorType() & CURSOR_CLASSIC; }
    bool isCursorTypeTP() { return getCursorType() & CURSOR_TP; }
    bool isDisableItemTimer() { return mSettings.mDisableItemTimer; }
    bool isDisableWalls() { return mSettings.mDisableWalls; }
    bool isDisplay() { return mDisplay; }
    bool isDropShadows() { return mSettings.mDropShadows; }
    bool isElevatorEscape() { return mSettings.mElevatorEscape; }
    bool isEponaStolen() { return dComIfGs_isEventBit(0x0580); }
    bool isEponaTamed() { return dComIfGs_isEventBit(0x0601); }
    bool isEndingBlowMoonBoots() { return mSettings.mEndingBlowMoonBoots; }
    bool isFastBonkRecovery() { return mSettings.mFastBonkRecovery; }
    bool isFastMovement() { return mSettings.mFastMovement; }
    bool isGorgeVoid() { return mSettings.mGorgeVoid; }
    bool isInfiniteAir() { return mSettings.mInfiniteAir; }
    bool isInfiniteArrows() { return mSettings.mInfiniteArrows; }
    bool isInfiniteBombs() { return mSettings.mInfiniteBombs; }
    bool isInfiniteHearts() { return mSettings.mInfiniteHearts; }
    bool isInfiniteOil() { return mSettings.mInfiniteOil; }
    bool isInfiniteRupees() { return mSettings.mInfiniteRupees; }
    bool isInfiniteSlingshot() { return mSettings.mInfiniteSlingshot; }
    bool isInGameTimer() { return mSettings.mInGameTimer; }
    bool isInputViewer() { return mSettings.mInputViewer; }
    bool isInvincibleEnemies() { return mSettings.mInvincibleEnemies; }
    bool isInvincibleLink() { return mSettings.mInvincibleLink; }
    bool isLadderFreezardCancel() { return mSettings.mLadderFreezardCancel; }
    bool isLinkDebugInfo() { return mSettings.mLinkDebugInfo; }
    bool isLoadTimer() { return mSettings.mLoadTimer; }
    bool isMaloMartCT() { return dComIfGs_isEventBit(0x2210);}
    bool isMapWarping() { return dComIfGs_isEventBit(0x0604);}
    bool isMenuPausesGame() const { return mSettings.mMenuPausesGame; }
    bool isBootToMenu() const { return mSettings.mBootToMenu; }
    bool isMenuSfx() const { return mSettings.mMenuSfx; }
    bool isMidnaCharge() { return dComIfGs_isEventBit(0x0501);}
    bool isMidnaHealthy() { return dComIfGs_isEventBit(0x1E08);}
    bool isMidnaOnBack() { return dComIfGs_isTransformLV(3);}
    bool isMidnaOnZ() { return dComIfGs_isEventBit(0x0C10);}
    bool isMoonJump() { return mSettings.mMoonJump; }
    bool isMoveLink() { return mSettings.mMoveLink; }
    bool isNoSinkingInSand() { return mSettings.mNoSinkingInSand; }
    bool isRolling() { return mSettings.mRolling; }
    bool isRuslTD() { return dComIfGs_isTmpBit(0x0006);}
    bool isStageInfo() { return mSettings.mStageInfo; }
    bool isSuperClawshot() { return mSettings.mSuperClawshot; }
    bool isSwapEquips() const { return mSettings.mSwapEquips; }
    bool isTeleport() { return mSettings.mTeleport; }
    bool isTimer() { return mSettings.mTimer; }
    bool isTransformAnywhere() { return mSettings.mTransformAnywhere; }
    bool isTransformWarp() { return dComIfGs_isEventBit(0x0D04);}
    bool isUniversalMapDelay() { return mSettings.mUniversalMapDelay; }
    bool isUnrestrictedItems() { return mSettings.mUnrestrictedItems; }
    bool isWolfSense() { return dComIfGs_isEventBit(0x4308);}

    void setAbMashRate(bool i_opt) { mSettings.mAbMashRate = i_opt; }
    void setBossFlag(u8 i_value) { cDmr_SkipInfo = i_value; }
    void setCoroTD(bool i_opt) {\
        i_opt ? dComIfGs_onTmpBit(0x0002) : dComIfGs_offTmpBit(0x0002);
        mSettings.mCoroTD = i_opt;
    }
    void setCursorType(u8 i_type) { mSettings.mCursorType = i_type; }
    void setDisableItemTimer(bool i_opt) { mSettings.mDisableItemTimer = i_opt; }
    void setDisableWalls(bool i_opt) { mSettings.mDisableWalls = i_opt; }
    void setDisplayMode(bool i_mode) { mSettings.mDisplayMode = i_mode; }
    void setDropShadows(bool i_dropShadows) { mSettings.mDropShadows = i_dropShadows; }
    void setElevatorEscape(bool i_opt) { mSettings.mElevatorEscape = i_opt; }
    void setEndingBlowMoonBoots(bool i_opt) { mSettings.mEndingBlowMoonBoots = i_opt; }
    void setFastBonkRecovery(bool i_opt) { mSettings.mFastBonkRecovery = i_opt; }
    void setEponaStolen(bool i_opt) { i_opt ? dComIfGs_onEventBit(0x0580) : dComIfGs_offEventBit(0x0580); }
    void setEponaTamed(bool i_opt) { i_opt ? dComIfGs_onEventBit(0x0601) : dComIfGs_offEventBit(0x0601); }
    void setFastMovement(bool i_opt) { mSettings.mFastMovement = i_opt; }
    void setFont(JUTFont* i_font) { mpFont = i_font; }
    void setGorgeVoid(bool i_opt) { mSettings.mGorgeVoid = i_opt; }
    void setInGameTimer(bool i_opt) { mSettings.mInGameTimer = i_opt; }
    void setInfiniteAir(bool i_opt) { mSettings.mInfiniteAir = i_opt; }
    void setInfiniteArrows(bool i_opt) { mSettings.mInfiniteArrows = i_opt; }
    void setInfiniteBombs(bool i_opt) { mSettings.mInfiniteBombs = i_opt; }
    void setInfiniteHearts(bool i_opt) { mSettings.mInfiniteHearts = i_opt; }
    void setInfiniteOil(bool i_opt) { mSettings.mInfiniteOil = i_opt; }
    void setInfiniteRupees(bool i_opt) { mSettings.mInfiniteRupees = i_opt; }
    void setInfiniteSlingshot(bool i_opt) { mSettings.mInfiniteSlingshot = i_opt; }
    void setInputViewer(bool i_opt) { mSettings.mInputViewer = i_opt; }
    void setInvincibleEnemies(bool i_opt) { mSettings.mInvincibleEnemies = i_opt; }
    void setInvincibleLink(bool i_opt) { mSettings.mInvincibleLink = i_opt; }
    void setLadderFreezardCancel(bool i_opt) { mSettings.mLadderFreezardCancel = i_opt; }
    void setLinkDebugInfo(bool i_opt) { mSettings.mLinkDebugInfo = i_opt; }
    void setLoadTimer(bool i_opt) { mSettings.mLoadTimer = i_opt; }
    void setMaloMartCT(bool i_opt) { i_opt ? dComIfGs_onEventBit(0x2210) : dComIfGs_offEventBit(0x2210); }
    void setMapWarping(bool i_opt) { i_opt ? dComIfGs_onEventBit(0x0604) : dComIfGs_offEventBit(0x0604); }
    void setMenuPausesGame(bool i_opt) { mSettings.mMenuPausesGame = i_opt; }
    void setBootToMenu(bool i_opt) { mSettings.mBootToMenu = i_opt; }
    void setMenuSfx(bool sfx) { mSettings.mMenuSfx = sfx; }
    void setMidnaCharge(bool i_opt) { i_opt ? dComIfGs_onEventBit(0x0501) : dComIfGs_offEventBit(0x0501); }
    void setMidnaHealthy(bool i_opt) { i_opt ? dComIfGs_onEventBit(0x1E08) : dComIfGs_offEventBit(0x1E08); }
    void setMidnaOnBack(bool i_opt) { i_opt ? dComIfGs_onTransformLV(3): dComIfGs_offTransformLV(3); }
    void setMidnaOnZ(bool i_opt)  { i_opt ? dComIfGs_onEventBit(0x0C10) : dComIfGs_offEventBit(0x0C10); }
    void setMoonJump(bool i_opt) { mSettings.mMoonJump = i_opt; }
    void setMoveLink(bool i_opt) { mSettings.mMoveLink = i_opt; }
    void setNoSinkingInSand(bool i_opt) { mSettings.mNoSinkingInSand = i_opt; }
    void setReloadType(bool i_type) { mSettings.mReloadType = i_type; }
    void setRolling(bool i_opt) { mSettings.mRolling = i_opt; }
    void setRuslTD(bool i_opt) { i_opt ? dComIfGs_onTmpBit(0x0006) : dComIfGs_offTmpBit(0x0006); }
    void setStageInfo(bool i_opt) { mSettings.mStageInfo = i_opt; }
    void setSuperClawshot(bool i_opt) { mSettings.mSuperClawshot = i_opt; }
    void setSwapEquips(bool i_swapEquips) { mSettings.mSwapEquips = i_swapEquips; }
    void setTeleport(bool i_opt) { mSettings.mTeleport = i_opt; }
    void setTextColor(u32 i_textColor) { mSettings.mTextColor = i_textColor; }
    void setTimer(bool i_opt) { mSettings.mTimer = i_opt; }
    void setTransformAnywhere(bool i_opt) { mSettings.mTransformAnywhere = i_opt; }
    void setTransformWarp(bool i_opt) { i_opt ? dComIfGs_onEventBit(0x0D04) : dComIfGs_offEventBit(0x0D04); }
    void setUniversalMapDelay(bool i_opt) { mSettings.mUniversalMapDelay = i_opt; }
    void setUnrestrictedItems(bool i_opt) { mSettings.mUnrestrictedItems = i_opt; }
    void setWolfSense(bool i_opt) { i_opt ? dComIfGs_onEventBit(0x4308) : dComIfGs_offEventBit(0x4308); }

    void seStart(u32 i_sfxID) {
        if (mSettings.mMenuSfx) mDoAud_seStart(i_sfxID, 0, 0, 0);
    }

    u8 nextCursorType() {
        switch (getCursorType()) {
        case CURSOR_CLASSIC:
            return CURSOR_TP;
        case CURSOR_TP:
            return CURSOR_BOTH;
        case CURSOR_BOTH:
            return CURSOR_CLASSIC;
        default:
            return CURSOR_CLASSIC;
        }
    }

    u8 prevCursorType() {
        switch (getCursorType()) {
        case CURSOR_CLASSIC:
            return CURSOR_BOTH;
        case CURSOR_BOTH:
            return CURSOR_TP;
        case CURSOR_TP:
            return CURSOR_CLASSIC;
        default:
            return CURSOR_CLASSIC;
        }
    }

    u32 nextTextColor() {
        static const u32 sTextColors[] = {
            COLOR_AMETHYST, COLOR_AQUAMARINE, COLOR_BANANA_MANIA, COLOR_BOLD_CRIMSON,
            COLOR_BUBBLEGUM_PINK, COLOR_CERULEAN, COLOR_COSMIC_COBALT, COLOR_ELECTRIC_BLUE,
            COLOR_FIERY_ORANGE, COLOR_FLAMINGO_FEATHER, COLOR_GOLD_DROP, COLOR_LEMON_YELLOW,
            COLOR_LIME_GREEN, COLOR_MAGENTA_MAGIC, COLOR_MIDNIGHT_BLUE, COLOR_MYSTICAL_PURPLE,
            COLOR_NEON_CARROT, COLOR_PERIWINKLE, COLOR_SAPPHIRE_SPARKLE, COLOR_SHAMROCK_GREEN,
            COLOR_SUNNY_YELLOW, COLOR_TANGERINE_TWIST, COLOR_TROPICAL_TURQUOISE, COLOR_VIVID_VIOLET,
            COLOR_WILD_STRAWBERRY, COLOR_ZESTY_CHARTREUSE
        };
        static const int sNumColors = sizeof(sTextColors) / sizeof(sTextColors[0]);

        u32 current = getTextColor();
        for (int i = 0; i < sNumColors; i++) {
            if (sTextColors[i] == current) {
                return sTextColors[(i + 1) % sNumColors];
            }
        }
        return COLOR_WHITE;
    }

    u32 prevTextColor() {
        static const u32 sTextColors[] = {
            COLOR_AMETHYST, COLOR_AQUAMARINE, COLOR_BANANA_MANIA, COLOR_BOLD_CRIMSON,
            COLOR_BUBBLEGUM_PINK, COLOR_CERULEAN, COLOR_COSMIC_COBALT, COLOR_ELECTRIC_BLUE,
            COLOR_FIERY_ORANGE, COLOR_FLAMINGO_FEATHER, COLOR_GOLD_DROP, COLOR_LEMON_YELLOW,
            COLOR_LIME_GREEN, COLOR_MAGENTA_MAGIC, COLOR_MIDNIGHT_BLUE, COLOR_MYSTICAL_PURPLE,
            COLOR_NEON_CARROT, COLOR_PERIWINKLE, COLOR_SAPPHIRE_SPARKLE, COLOR_SHAMROCK_GREEN,
            COLOR_SUNNY_YELLOW, COLOR_TANGERINE_TWIST, COLOR_TROPICAL_TURQUOISE, COLOR_VIVID_VIOLET,
            COLOR_WILD_STRAWBERRY, COLOR_ZESTY_CHARTREUSE
        };
        static const int sNumColors = sizeof(sTextColors) / sizeof(sTextColors[0]);

        u32 current = getTextColor();
        for (int i = 0; i < sNumColors; i++) {
            if (sTextColors[i] == current) {
                return sTextColors[(i - 1 + sNumColors) % sNumColors];
            }
        }
        return COLOR_WHITE;
    }

    J2DPicture* mpIcon;
    J2DPicture* mpBackground;
    gzTextBox* mpHeader;

    gzMenu_c* mpCurrentMenu;
    gzMainMenu_c* mpMainMenu;
    gzNotification_c* mpNotification;
    dSelect_cursor_c* mpTPCursor;
    gzCapture_c* mpCapture;
    gzTextBox* mpMenuDescription;
    bool mMenuOption;
    s32 mTopLine;
    s32 mVisibleLines;

    JUTFont* mpFont;
    s16 mInputWaitTimer;
    bool mDisplay;
    bool mGZInitialized;

    // Input state for menu navigation (stick + d-pad with repeat)
    u32 mStickTriggers;
    u32 mRepeatDirection;
    s16 mRepeatCounter;
    gzSettings_s mSettings;
    gzCursor mCursor;
    gzSaveLoaderMng_c mSaveLoaderMng;
    gzCheatsMng_c mCheatsMng;
    gzToolsMng_c mToolsMng;

    f32 mIconXPos;
    f32 mIconYPos;
    f32 mIconWidth;
    f32 mIconHeight;

    f32 mHeaderXPos;
    f32 mHeaderYPos;

    f32 mBackgroundXPos;
    f32 mBackgroundYPos;
    f32 mBackgroundWidth;
    f32 mBackgroundHeight;

    u8 mGzGroupID;
};

extern gzInfo_c g_gzInfo;

inline gzCursor* gzInfo_getCursor() { return g_gzInfo.getCursor(); }
inline dSelect_cursor_c* gzInfo_getTPCursor() { return g_gzInfo.getTPCursor(); }
inline gzTextBox* gzInfo_getMenuDescription() { return g_gzInfo.getMenuDescription(); }
inline bool gzInfo_isMenuOption() { return g_gzInfo.isMenuOption(); }
inline void gzInfo_setMenuOption(bool i_opt) { g_gzInfo.setMenuOption(i_opt); }
inline void gzInfo_onMenuOption() { g_gzInfo.setMenuOption(true); }
inline void gzInfo_offMenuOption() { g_gzInfo.setMenuOption(false); }
inline s32 gzInfo_getTopLine() { return g_gzInfo.getTopLine(); }
inline void gzInfo_setTopLine(s32 i_topLine) { g_gzInfo.setTopLine(i_topLine); }
inline void gzInfo_resetTopLine() { g_gzInfo.setTopLine(0); }
inline s32 gzInfo_getVisibleLines() { return g_gzInfo.getVisibleLines(); }
inline void gzInfo_setVisibleLines(s32 i_visibleLines) { g_gzInfo.setVisibleLines(i_visibleLines); }
inline u32 gzInfo_getCursorColor() { return g_gzInfo.getCursorColor(); }
inline u32 gzInfo_getTextColor() { return g_gzInfo.getTextColor(); }
inline u8 gzInfo_getCursorType() { return g_gzInfo.getCursorType(); }
inline bool gzInfo_getDisplayMode() { return g_gzInfo.getDisplayMode(); }
inline bool gzInfo_getReloadType() { return g_gzInfo.getReloadType(); }
inline u8 gzInfo_getBossFlag() { return g_gzInfo.getBossFlag(); }

inline int gzInfo_deleteSettingsMemcard() { return g_gzInfo.deleteSettingsMemcard(); }
inline int gzInfo_loadSettingsMemcard() { return g_gzInfo.loadSettingsMemcard(); }
inline u32 gzInfo_nextCursorType() { return g_gzInfo.nextCursorType(); }
inline u32 gzInfo_prevCursorType() { return g_gzInfo.prevCursorType(); }
inline u32 gzInfo_nextTextColor() { return g_gzInfo.nextTextColor(); }
inline u32 gzInfo_prevTextColor() { return g_gzInfo.prevTextColor(); }
//implementation tbd
inline u32 gzInfo_nextFont() { return 0; }
inline u32 gzInfo_prevFont() { return 0; }

inline void gzInfo_seStart(u32 i_sfxID) { g_gzInfo.seStart(i_sfxID); }
inline void gzInfo_sendNotification(const char* msg) { g_gzInfo.sendNotification(msg); }
inline void gzInfo_sendNotification(const char* msg, int i_notificationType) { g_gzInfo.sendNotification(msg, i_notificationType); }
inline int gzInfo_storeSettingsMemcard() { return g_gzInfo.storeSettingsMemcard(); }

inline bool gzInfo_isInDungeon(int i_stageNo) {
    // Check if we're in the dungeon, and return true
    switch (i_stageNo) {
    case 16:
        if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0) {
            return true;
        }
        break;
    case 17:
        if (strcmp(dComIfGp_getStartStageName(), "D_MN04") == 0) {
            return true;
        }
        break;
    case 18:
        if (strcmp(dComIfGp_getStartStageName(), "D_MN01") == 0) {
            return true;
        }
        break;
    case 19:
        if (strcmp(dComIfGp_getStartStageName(), "D_MN10") == 0) {
            return true;
        }
        break;
    case 20:
        if (strcmp(dComIfGp_getStartStageName(), "D_MN11") == 0) {
            return true;
        }
        break;
    case 21:
        if (strcmp(dComIfGp_getStartStageName(), "D_MN06") == 0) {
            return true;
        }
        break;
    case 22:
        if (strcmp(dComIfGp_getStartStageName(), "D_MN07") == 0) {
            return true;
        }
        break;
    case 23:
        if (strcmp(dComIfGp_getStartStageName(), "D_MN08") == 0) {
            return true;
        }
        break;
    case 24:
        if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
            return true;
        }
        break;
    }

    return false;
}

inline bool gzInfo_isBossDefeated(int i_stageNo) { 
    return gzInfo_isInDungeon(i_stageNo) ? dComIfGs_isStageBossEnemy() : dComIfGs_isSaveStageBossEnemy(i_stageNo);
}

inline bool gzInfo_isMiniBossDefeated(int i_stageNo) { 
    return gzInfo_isInDungeon(i_stageNo) ? dComIfGs_isStageMiddleBoss() : dComIfGs_isSaveStageMiddleBoss(i_stageNo);
}

inline bool gzInfo_isBossFlag() { return gzInfo_getBossFlag() > 0; }
inline bool gzInfo_isCoroTD() { return g_gzInfo.isCoroTD(); }
inline bool gzInfo_isCursorTypeClassic() { return g_gzInfo.isCursorTypeClassic(); }
inline bool gzInfo_isCursorTypeTP() { return g_gzInfo.isCursorTypeTP(); }
inline bool gzInfo_isDisableItemTimer() { return g_gzInfo.isDisableItemTimer(); }
inline bool gzInfo_isDisableWalls() { return g_gzInfo.isDisableWalls(); }
inline bool gzInfo_isDisplayModeInterlaced() { return g_gzInfo.getDisplayMode() == false; }
inline bool gzInfo_isDisplayModeProgressive() { return g_gzInfo.getDisplayMode() == true; }
inline bool gzInfo_isDropShadows() { return g_gzInfo.isDropShadows(); }

inline bool gzInfo_isDungeonBossKey(int i_stageNo) { 
    return gzInfo_isInDungeon(i_stageNo) ? dComIfGs_isDungeonItemBossKey() : dComIfGs_isSaveDungeonItemBossKey(i_stageNo);
}

inline bool gzInfo_isDungeonCompass(int i_stageNo) { 
    return gzInfo_isInDungeon(i_stageNo) ? dComIfGs_isDungeonItemCompass() : dComIfGs_isSaveDungeonItemCompass(i_stageNo);
}

inline bool gzInfo_isDungeonHeartContainer(int i_stageNo) { 
    return gzInfo_isInDungeon(i_stageNo) ? dComIfGs_isStageLife() : dComIfGs_isSaveStageLife(i_stageNo);
}

inline bool gzInfo_isDungeonMap(int i_stageNo) {
    return gzInfo_isInDungeon(i_stageNo) ? dComIfGs_isDungeonItemMap() : dComIfGs_isSaveDungeonItemMap(i_stageNo);
}

inline bool gzInfo_isDungeonOoccoo(int i_stageNo) { 
    return gzInfo_isInDungeon(i_stageNo) ? dComIfGs_isDungeonItemWarp() : dComIfGs_isSaveDungeonItemWarp(i_stageNo); 
}

inline bool gzInfo_isElevatorEscape() { return g_gzInfo.isElevatorEscape(); }
inline bool gzInfo_isEndingBlowMoonBoots() { return g_gzInfo.isEndingBlowMoonBoots(); }
inline bool gzInfo_isEponaStolen() { return g_gzInfo.isEponaStolen(); }
inline bool gzInfo_isEponaTamed() { return g_gzInfo.isEponaTamed(); }
inline bool gzInfo_isGorgeVoid() { return g_gzInfo.isGorgeVoid(); }
inline bool gzInfo_isInfiniteAir() { return g_gzInfo.isInfiniteAir(); }
inline bool gzInfo_isInfiniteArrows() { return g_gzInfo.isInfiniteArrows(); }
inline bool gzInfo_isInfiniteBombs() { return g_gzInfo.isInfiniteBombs(); }
inline bool gzInfo_isInfiniteHearts() { return g_gzInfo.isInfiniteHearts(); }
inline bool gzInfo_isInfiniteOil() { return g_gzInfo.isInfiniteOil(); }
inline bool gzInfo_isInfiniteRupees() { return g_gzInfo.isInfiniteRupees(); }
inline bool gzInfo_isInfiniteSlingshot() { return g_gzInfo.isInfiniteSlingshot(); }
inline bool gzInfo_isInvincibleEnemies() { return g_gzInfo.isInvincibleEnemies(); }
inline bool gzInfo_isInvincibleLink() { return g_gzInfo.isInvincibleLink(); }
inline bool gzInfo_isLadderFreezardCancel() { return g_gzInfo.isLadderFreezardCancel(); }
inline bool gzInfo_isMaloMartCT() { return g_gzInfo.isMaloMartCT(); }
inline bool gzInfo_isMainMenuVisible() { return g_gzInfo.mCursor.x == 0;}
inline bool gzInfo_isMapWarping() { return g_gzInfo.isMapWarping(); }
inline bool gzInfo_isMenuPausesGame() { return g_gzInfo.isMenuPausesGame(); }
inline bool gzInfo_isBootToMenu() { return g_gzInfo.isBootToMenu(); }
inline bool gzInfo_isMenuSfx() { return g_gzInfo.isMenuSfx(); }
inline bool gzInfo_isMidnaCharge() { return g_gzInfo.isMidnaCharge(); }
inline bool gzInfo_isMidnaHealthy() { return g_gzInfo.isMidnaHealthy(); }
inline bool gzInfo_isMidnaOnBack() { return g_gzInfo.isMidnaOnBack(); }
inline bool gzInfo_isMidnaOnZ() { return g_gzInfo.isMidnaOnZ(); }
inline bool gzInfo_isMoonJump() { return g_gzInfo.isMoonJump(); }
inline bool gzInfo_isMoveLink() { return g_gzInfo.isMoveLink(); }
inline bool gzInfo_isReloadArea() { return g_gzInfo.getReloadType() == true; }
inline bool gzInfo_isReloadFile() { return g_gzInfo.getReloadType() == false; }
inline bool gzInfo_isRolling() { return g_gzInfo.isRolling(); }
inline bool gzInfo_isRuslTD() { return g_gzInfo.isRuslTD(); }
inline bool gzInfo_isSubMenuVisible() { return g_gzInfo.mCursor.x > 0;}
inline bool gzInfo_isSuperClawshot() { return g_gzInfo.isSuperClawshot(); }
inline bool gzInfo_isSwapEquips() { return g_gzInfo.isSwapEquips(); }
inline bool gzInfo_isTransformAnywhere() { return g_gzInfo.isTransformAnywhere(); }
inline bool gzInfo_isUniversalMapDelay() { return g_gzInfo.isUniversalMapDelay(); }
inline bool gzInfo_isUnrestrictedItems() { return g_gzInfo.isUnrestrictedItems(); }
inline bool gzInfo_isAbMashRate() { return g_gzInfo.isAbMashRate(); }
inline bool gzInfo_isLinkDebugInfo() { return g_gzInfo.isLinkDebugInfo(); }
inline bool gzInfo_isInGameTimer() { return g_gzInfo.isInGameTimer(); }
inline bool gzInfo_isInputViewer() { return g_gzInfo.isInputViewer(); }
inline bool gzInfo_isLoadTimer() { return g_gzInfo.isLoadTimer(); }
inline bool gzInfo_isStageInfo() { return g_gzInfo.isStageInfo(); }
inline bool gzInfo_isTimer() { return g_gzInfo.isTimer(); }
inline bool gzInfo_isTransformWarp() { return g_gzInfo.isTransformWarp(); }
inline bool gzInfo_isWolfSense() { return g_gzInfo.isWolfSense(); }
inline bool gzInfo_isFastBonkRecovery() { return g_gzInfo.isFastBonkRecovery(); }
inline bool gzInfo_isFastMovement() { return g_gzInfo.isFastMovement(); }
inline bool gzInfo_isNoSinkingInSand() { return g_gzInfo.isNoSinkingInSand(); }
inline bool gzInfo_isTeleport() { return g_gzInfo.isTeleport(); }


inline void gzInfo_setBossFlag(u8 value) { g_gzInfo.setBossFlag(value); }
inline void gzInfo_setCursorType(u8 type) { g_gzInfo.setCursorType(type); }
inline void gzInfo_setDisplayMode(bool mode) { g_gzInfo.setDisplayMode(mode); }
inline void gzInfo_setDisplayModeInterlaced() { mDoMch_render_c::setInterlacedMode(); gzInfo_setDisplayMode(false); }
inline void gzInfo_setDisplayModeProgressive() { mDoMch_render_c::setProgressiveMode(); gzInfo_setDisplayMode(true); }
inline void gzInfo_setDropShadows(bool dropShadows) { g_gzInfo.setDropShadows(dropShadows); }
inline void gzInfo_setMenuPausesGame(bool opt) { g_gzInfo.setMenuPausesGame(opt); }
inline void gzInfo_setBootToMenu(bool opt) { g_gzInfo.setBootToMenu(opt); }
inline void gzInfo_setMenuSfx(bool sfx) { g_gzInfo.setMenuSfx(sfx); }
inline void gzInfo_setMoveLink(bool i_opt) { g_gzInfo.setMoveLink(i_opt); }
inline void gzInfo_setReloadType(bool i_type) { g_gzInfo.setReloadType(i_type); }
inline void gzInfo_setReloadArea() { gzInfo_setReloadType(true); }
inline void gzInfo_setReloadFile() { gzInfo_setReloadType(false); }
inline void gzInfo_setSwapEquips(bool swapEquips) { g_gzInfo.setSwapEquips(swapEquips); }
inline void gzInfo_setTextColor(u32 textColor) { g_gzInfo.setTextColor(textColor); }

inline void gzInfo_offAbMashRate() { g_gzInfo.setAbMashRate(false); }

inline void gzInfo_offBossDefeated(int i_stageNo) { 
    gzInfo_isInDungeon(i_stageNo) ? dComIfGs_offStageBossEnemy() : dComIfGs_offSaveStageBossEnemy(i_stageNo); 
}

inline void gzInfo_offMiniBossDefeated(int i_stageNo) {
    gzInfo_isInDungeon(i_stageNo) ? dComIfGs_offStageMiddleBoss() : dComIfGs_offSaveStageMiddleBoss(i_stageNo);
}

inline void gzInfo_offBossFlag() { g_gzInfo.setBossFlag(0); }
inline void gzInfo_offCoroTD() { g_gzInfo.setCoroTD(false); }
inline void gzInfo_offDisableItemTimer() { g_gzInfo.setDisableItemTimer(false); }
inline void gzInfo_offDisableWalls() { g_gzInfo.setDisableWalls(false); }
inline void gzInfo_offDropShadows() { g_gzInfo.setDropShadows(false); }

inline void gzInfo_offDungeonBossKey(int i_stageNo) { 
    gzInfo_isInDungeon(i_stageNo) ? dComIfGs_offDungeonItemBossKey() : dComIfGs_offSaveDungeonItemBossKey(i_stageNo); 
}

inline void gzInfo_offDungeonCompass(int i_stageNo) { 
    gzInfo_isInDungeon(i_stageNo) ? dComIfGs_offDungeonItemCompass() : dComIfGs_offSaveDungeonItemCompass(i_stageNo); 
}

inline void gzInfo_offDungeonHeartContainer(int i_stageNo) { 
    gzInfo_isInDungeon(i_stageNo) ? dComIfGs_offStageLife() : dComIfGs_offSaveStageLife(i_stageNo); 
}

inline void gzInfo_offDungeonMap(int i_stageNo) { 
    gzInfo_isInDungeon(i_stageNo) ? dComIfGs_offDungeonItemMap() : dComIfGs_offSaveDungeonItemMap(i_stageNo); 
}

inline void gzInfo_offDungeonOoccoo(int i_stageNo) { 
    gzInfo_isInDungeon(i_stageNo) ? dComIfGs_offDungeonItemWarp() : dComIfGs_offSaveDungeonItemWarp(i_stageNo);
}

inline void gzInfo_offElevatorEscape() { g_gzInfo.setElevatorEscape(false); }
inline void gzInfo_offEndingBlowMoonBoots() { g_gzInfo.setEndingBlowMoonBoots(false); }
inline void gzInfo_offEponaStolen() { g_gzInfo.setEponaStolen(false); }
inline void gzInfo_offEponaTamed() { g_gzInfo.setEponaTamed(false); }
inline void gzInfo_offFastBonkRecovery() { g_gzInfo.setFastBonkRecovery(false); }
inline void gzInfo_offFastMovement() { g_gzInfo.setFastMovement(false); }
inline void gzInfo_offGorgeVoid() { g_gzInfo.setGorgeVoid(false); }
inline void gzInfo_offInGameTimer() { g_gzInfo.setInGameTimer(false); }
inline void gzInfo_offInfiniteAir() { g_gzInfo.setInfiniteAir(false); }
inline void gzInfo_offInfiniteArrows() { g_gzInfo.setInfiniteArrows(false); }
inline void gzInfo_offInfiniteBombs() { g_gzInfo.setInfiniteBombs(false); }
inline void gzInfo_offInfiniteHearts() { g_gzInfo.setInfiniteHearts(false); }
inline void gzInfo_offInfiniteOil() { g_gzInfo.setInfiniteOil(false); }
inline void gzInfo_offInfiniteRupees() { g_gzInfo.setInfiniteRupees(false); }
inline void gzInfo_offInfiniteSlingshot() { g_gzInfo.setInfiniteSlingshot(false); }
inline void gzInfo_offInputViewer() { g_gzInfo.setInputViewer(false); }
inline void gzInfo_offInvincibleEnemies() { g_gzInfo.setInvincibleEnemies(false); }
inline void gzInfo_offInvincibleLink() { g_gzInfo.setInvincibleLink(false); }
inline void gzInfo_offLadderFreezardCancel() { g_gzInfo.setLadderFreezardCancel(false); }
inline void gzInfo_offLinkDebugInfo() { g_gzInfo.setLinkDebugInfo(false); }
inline void gzInfo_offLoadTimer() { g_gzInfo.setLoadTimer(false); }
inline void gzInfo_offMaloMartCT() { g_gzInfo.setMaloMartCT(false); }
inline void gzInfo_offMapWarping() { g_gzInfo.setMapWarping(false); }
inline void gzInfo_offMenuPausesGame() { g_gzInfo.setMenuPausesGame(false); }
inline void gzInfo_offBootToMenu() { g_gzInfo.setBootToMenu(false); }
inline void gzInfo_offMenuSfx() { g_gzInfo.setMenuSfx(false); }
inline void gzInfo_offMidnaCharge() { g_gzInfo.setMidnaCharge(false); }
inline void gzInfo_offMidnaHealthy() { g_gzInfo.setMidnaHealthy(false); }
inline void gzInfo_offMidnaOnBack() { g_gzInfo.setMidnaOnBack(false); }
inline void gzInfo_offMidnaOnZ() { g_gzInfo.setMidnaOnZ(false); }
inline void gzInfo_offMoonJump() { g_gzInfo.setMoonJump(false); }
inline void gzInfo_offMoveLink() { g_gzInfo.setMoveLink(false); }
inline void gzInfo_offNoSinkingInSand() { g_gzInfo.setNoSinkingInSand(false); }
inline void gzInfo_offRolling() { g_gzInfo.setRolling(false); }
inline void gzInfo_offRuslTD() { g_gzInfo.setRuslTD(false); }
inline void gzInfo_offStageInfo() { g_gzInfo.setStageInfo(false); }
inline void gzInfo_offSuperClawshot() { g_gzInfo.setSuperClawshot(false); }
inline void gzInfo_offSwapEquips() { g_gzInfo.setSwapEquips(false); }
inline void gzInfo_offTeleport() { g_gzInfo.setTeleport(false); }
inline void gzInfo_offTimer() { g_gzInfo.setTimer(false); }
inline void gzInfo_offTransformAnywhere() { g_gzInfo.setTransformAnywhere(false); }
inline void gzInfo_offTransformWarp() { g_gzInfo.setTransformWarp(false); }
inline void gzInfo_offUniversalMapDelay() { g_gzInfo.setUniversalMapDelay(false); }
inline void gzInfo_offUnrestrictedItems() { g_gzInfo.setUnrestrictedItems(false); }
inline void gzInfo_offWolfSense() { g_gzInfo.setWolfSense(false); }

inline void gzInfo_onAbMashRate() { g_gzInfo.setAbMashRate(true); }

inline void gzInfo_onBossDefeated(int i_stageNo) { 
    gzInfo_isInDungeon(i_stageNo) ? dComIfGs_onStageBossEnemy() : dComIfGs_onSaveStageBossEnemy(i_stageNo);
}

inline void gzInfo_onMiniBossDefeated(int i_stageNo) { 
    gzInfo_isInDungeon(i_stageNo) ? dComIfGs_onStageMiddleBoss() : dComIfGs_onSaveStageMiddleBoss(i_stageNo);
}

inline void gzInfo_onBossFlag() { g_gzInfo.setBossFlag(50); }
inline void gzInfo_onCoroTD() { g_gzInfo.setCoroTD(true); }
inline void gzInfo_onDisableItemTimer() { g_gzInfo.setDisableItemTimer(true); }
inline void gzInfo_onDisableWalls() { g_gzInfo.setDisableWalls(true); }
inline void gzInfo_onDropShadows() { g_gzInfo.setDropShadows(true); }

inline void gzInfo_onDungeonBossKey(int i_stageNo) { 
    gzInfo_isInDungeon(i_stageNo) ? execItemGet(fpcNm_ITEM_BOSS_KEY) : dComIfGs_onSaveDungeonItemBossKey(i_stageNo); 
}

inline void gzInfo_onDungeonCompass(int i_stageNo) { 
    gzInfo_isInDungeon(i_stageNo) ? execItemGet(fpcNm_ITEM_COMPUS) : dComIfGs_onSaveDungeonItemCompass(i_stageNo); 
}

inline void gzInfo_onDungeonHeartContainer(int i_stageNo) { 
    // Not using execItemGet func for this as it does more than we want in this context
    gzInfo_isInDungeon(i_stageNo) ? dComIfGs_onStageLife() : dComIfGs_onSaveStageLife(i_stageNo);
}

inline void gzInfo_onDungeonMap(int i_stageNo) { 
    gzInfo_isInDungeon(i_stageNo) ? execItemGet(fpcNm_ITEM_MAP) : dComIfGs_onSaveDungeonItemMap(i_stageNo);
}

inline void gzInfo_onDungeonOoccoo(int i_stageNo) { 
    // NOTE(Pheenoh): This won't actually give you the ooccoo, just set the flag. 
    // Maybe we should call execItemGet(fpcNm_ITEM_DUNGEON_EXIT) which will do both?
    // Or should we leave them separate and let the user set ooccoo in the inventory menu?
    gzInfo_isInDungeon(i_stageNo) ? dComIfGs_onDungeonItemWarp() : dComIfGs_onSaveDungeonItemWarp(i_stageNo);
}

inline void gzInfo_onElevatorEscape() { g_gzInfo.setElevatorEscape(true); }
inline void gzInfo_onEndingBlowMoonBoots() { g_gzInfo.setEndingBlowMoonBoots(true); }
inline void gzInfo_onEponaStolen() { g_gzInfo.setEponaStolen(true); }
inline void gzInfo_onEponaTamed() { g_gzInfo.setEponaTamed(true); }
inline void gzInfo_onFastBonkRecovery() { g_gzInfo.setFastBonkRecovery(true); }
inline void gzInfo_onFastMovement() { g_gzInfo.setFastMovement(true); }
inline void gzInfo_onGorgeVoid() { g_gzInfo.setGorgeVoid(true); }
inline void gzInfo_onInGameTimer() { g_gzInfo.setInGameTimer(true); }
inline void gzInfo_onInfiniteAir() { g_gzInfo.setInfiniteAir(true); }
inline void gzInfo_onInfiniteArrows() { g_gzInfo.setInfiniteArrows(true); }
inline void gzInfo_onInfiniteBombs() { g_gzInfo.setInfiniteBombs(true); }
inline void gzInfo_onInfiniteHearts() { g_gzInfo.setInfiniteHearts(true); }
inline void gzInfo_onInfiniteOil() { g_gzInfo.setInfiniteOil(true); }
inline void gzInfo_onInfiniteRupees() { g_gzInfo.setInfiniteRupees(true); }
inline void gzInfo_onInfiniteSlingshot() { g_gzInfo.setInfiniteSlingshot(true); }
inline void gzInfo_onInputViewer() { g_gzInfo.setInputViewer(true); }
inline void gzInfo_onInvincibleEnemies() { g_gzInfo.setInvincibleEnemies(true); }
inline void gzInfo_onInvincibleLink() { g_gzInfo.setInvincibleLink(true); }
inline void gzInfo_onLadderFreezardCancel() { g_gzInfo.setLadderFreezardCancel(true); }
inline void gzInfo_onLinkDebugInfo() { g_gzInfo.setLinkDebugInfo(true); }
inline void gzInfo_onLoadTimer() { g_gzInfo.setLoadTimer(true); }
inline void gzInfo_onMaloMartCT() { g_gzInfo.setMaloMartCT(true); }
inline void gzInfo_onMapWarping() { g_gzInfo.setMapWarping(true); }
inline void gzInfo_onMenuPausesGame() { g_gzInfo.setMenuPausesGame(true); }
inline void gzInfo_onBootToMenu() { g_gzInfo.setBootToMenu(true); }
inline void gzInfo_onMenuSfx() { g_gzInfo.setMenuSfx(true); }
inline void gzInfo_onMidnaCharge() { g_gzInfo.setMidnaCharge(true); }
inline void gzInfo_onMidnaHealthy() { g_gzInfo.setMidnaHealthy(true); }
inline void gzInfo_onMidnaOnBack() { g_gzInfo.setMidnaOnBack(true); }
inline void gzInfo_onMidnaOnZ() { g_gzInfo.setMidnaOnZ(true); }
inline void gzInfo_onMoonJump() { g_gzInfo.setMoonJump(true); }
inline void gzInfo_onMoveLink() { g_gzInfo.setMoveLink(true); }
inline void gzInfo_onNoSinkingInSand() { g_gzInfo.setNoSinkingInSand(true); }
inline void gzInfo_onRolling() { g_gzInfo.setRolling(true); }
inline void gzInfo_onRuslTD() { g_gzInfo.setRuslTD(true); }
inline void gzInfo_onStageInfo() { g_gzInfo.setStageInfo(true); }
inline void gzInfo_onSuperClawshot() { g_gzInfo.setSuperClawshot(true); }
inline void gzInfo_onSwapEquips() { g_gzInfo.setSwapEquips(true); }
inline void gzInfo_onTeleport() { g_gzInfo.setTeleport(true); }
inline void gzInfo_onTimer() { g_gzInfo.setTimer(true); }
inline void gzInfo_onTransformAnywhere() { g_gzInfo.setTransformAnywhere(true); }
inline void gzInfo_onTransformWarp() { g_gzInfo.setTransformWarp(true); }
inline void gzInfo_onUniversalMapDelay() { g_gzInfo.setUniversalMapDelay(true); }
inline void gzInfo_onUnrestrictedItems() { g_gzInfo.setUnrestrictedItems(true); }
inline void gzInfo_onWolfSense() { g_gzInfo.setWolfSense(true); }

namespace gzPad {
    inline u32 getTrig() { return mDoCPd_c::m_gzPadInfo.mPressedButtonFlags; }
    inline u32 getInputTrig() { return g_gzInfo.mStickTriggers; }  // Combined stick + d-pad with repeat
    inline u32 getTrigLockL() { return mDoCPd_c::m_gzPadInfo.mTrigLockL; }
    inline u32 getTrigLockR() { return mDoCPd_c::m_gzPadInfo.mTrigLockR; }
    inline u32 getTrigUp() { return getInputTrig() & PAD_BUTTON_UP; }
    inline u32 getTrigDown() { return getInputTrig() & PAD_BUTTON_DOWN; }
    inline u32 getTrigLeft() { return getInputTrig() & PAD_BUTTON_LEFT; }
    inline u32 getTrigRight() { return getInputTrig() & PAD_BUTTON_RIGHT; }
    inline u32 getTrigL() { return getTrig() & PAD_TRIGGER_L; }
    inline u32 getTrigR() { return getTrig() & PAD_TRIGGER_R; }
    inline u32 getTrigA() { return getTrig() & PAD_BUTTON_A; }
    inline u32 getTrigB() { return getTrig() & PAD_BUTTON_B; }
    inline u32 getTrigZ() { return getTrig() & PAD_TRIGGER_Z; }
    inline u32 getTrigY() { return getTrig() & PAD_BUTTON_Y; }
    inline u32 getTrigX() { return getTrig() & PAD_BUTTON_X; }
    inline u32 getTrigStart() { return getTrig() & PAD_BUTTON_START; }
    inline u32 getHold() { return mDoCPd_c::m_gzPadInfo.mButtonFlags; }
    inline u32 getHoldLockL() { return mDoCPd_c::m_gzPadInfo.mHoldLockL; }
    inline u32 getHoldLockR() { return mDoCPd_c::m_gzPadInfo.mHoldLockR; }
    inline u32 getHoldUp() { return getHold() & PAD_BUTTON_UP; }
    inline u32 getHoldDown() { return getHold() & PAD_BUTTON_DOWN; }
    inline u32 getHoldLeft() { return getHold() & PAD_BUTTON_LEFT; }
    inline u32 getHoldRight() { return getHold() & PAD_BUTTON_RIGHT; }
    inline u32 getHoldL() { return getHold() & PAD_TRIGGER_L; }
    inline u32 getHoldR() { return getHold() & PAD_TRIGGER_R; }
    inline u32 getHoldA() { return getHold() & PAD_BUTTON_A; }
    inline u32 getHoldB() { return getHold() & PAD_BUTTON_B; }
    inline u32 getHoldZ() { return getHold() & PAD_TRIGGER_Z; }
    inline u32 getHoldY() { return getHold() & PAD_BUTTON_Y; }
    inline u32 getHoldX() { return getHold() & PAD_BUTTON_X; }
    inline f32 getStickX() { return mDoCPd_c::m_gzPadInfo.mMainStickPosX; }
    inline f32 getStickY() { return mDoCPd_c::m_gzPadInfo.mMainStickPosY; }
    inline f32 getStickX3D() { return mDoCPd_c::m_gzPadInfo.mMainStickPosX; }
    inline f32 getStickValue() { return mDoCPd_c::m_gzPadInfo.mMainStickValue; }
    inline s16 getStickAngle() { return mDoCPd_c::m_gzPadInfo.mMainStickAngle; }
    inline s16 getStickAngle3D() { return mDoCPd_c::m_gzPadInfo.mMainStickAngle; }
    inline f32 getSubStickX3D() { return mDoCPd_c::m_gzPadInfo.mCStickPosX; }
    inline f32 getSubStickX() { return mDoCPd_c::m_gzPadInfo.mCStickPosX; }
    inline f32 getSubStickY() { return mDoCPd_c::m_gzPadInfo.mCStickPosY; }
    inline f32 getSubStickValue() { return mDoCPd_c::m_gzPadInfo.mCStickValue; }
    inline s16 getSubStickAngle() { return mDoCPd_c::m_gzPadInfo.mCStickAngle; }
    inline f32 getAnalogR() { return mDoCPd_c::m_gzPadInfo.mTriggerRight; }
    inline f32 getAnalogL() { return mDoCPd_c::m_gzPadInfo.mTriggerLeft; }
};

int gzPrint(int x, int y, u32 color, char const* string, ...);
void gzDVDLoadFile(const char* filePath, void* buffer, int length, int offset);

// Sets up 2D orthographic context for GZ overlay drawing.
// Call this before drawing J2DScreen-based elements (like haihai arrows).
void gzSetup2DContext();

// Clears all controller input (buttons, sticks, triggers)
inline void gzClearControllerInput() {
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
}

// Clears just button input (pressed and held)
inline void gzClearButtonInput() {
    mDoCPd_c::getCpadInfo(PAD_1).mPressedButtonFlags = 0;
    mDoCPd_c::getCpadInfo(PAD_1).mButtonFlags = 0;
}

// Toggles menu option state and plays appropriate sound effect
inline void gzInfo_toggleMenuOption() {
    gzInfo_setMenuOption(!gzInfo_isMenuOption());
    gzInfo_seStart(gzInfo_isMenuOption() ? Z2SE_SY_TALK_CURSOR_OK : Z2SE_SY_CURSOR_CANCEL);
}

#endif // GZ_H
