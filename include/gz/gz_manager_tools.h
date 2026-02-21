#ifndef GZ_MANAGER_TOOLS_H
#define GZ_MANAGER_TOOLS_H

#include "SSystem/SComponent/c_angle.h"
#include "SSystem/SComponent/c_xyz.h"
#include "SSystem/SComponent/c_sxyz.h"

class gzTextBox;

struct SavedCameraState {
    cXyz center;
    cXyz eye;
    f32 fovy;
    cSAngle bank;
};

struct TeleportState {
    SavedCameraState camera;
    cXyz linkPos;
    csXyz linkAngle;
    bool saveComboHeld;
    bool loadComboHeld;
};

struct MoveLinkState {
    bool active;
    bool prevActive;
    bool comboHeld;
    f32 camAngle;
};

struct FreeCamState {
    bool active;
    bool initialized;
    bool comboHeld;
    f32 pitch;
    f32 yaw;
    SavedCameraState savedState;
};

struct CoroTDState {
    static const u8 GOAL_FRAME = 10;
    static const u8 WINDOW_FRAMES = 20;
    bool timerStarted;
    bool goalHit;
    u8 frameCount;
};

struct EBMBState {
    static const s32 PERFECT_FRAME = 4;
    static const s32 LATE_LIMIT = 10;
    u16 prevAction;
    s32 frameDelta;
    bool ibOnLastFrame;
};

struct RollCheckerState {
    static const u8 RESULT_DISPLAY_FRAMES = 60;
    u16 prevAction;
    u8 frameDelta;
    u8 endFrame;
    u8 earlyFrame;
    u8 lateFrame;
    u8 resultTimer;
    u32 resultColor;
    char resultText[20];
    cXyz worldPos;
    gzTextBox* pText;
};

struct GorgeVoidState {
    static const s32 WARP_CS_FRAMES = 160;
    bool comboHeld;
    bool timerStarted;
    bool gotIt;
    s32 previousFrame;
    s32 counterDifference;
    s32 afterCsVal;
};

struct ElevatorEscapeState {
    static const u8 METAMORPHOSE_ANM_LENGTH = 56;
    u16 prevAction;
    s32 metamorphoseStartFrame;
    s32 lateRollFrame;
    s32 targetFrame;
};

struct AbMashRateState {
    static const u8 WINDOW_FRAMES = 30;
    bool initialized;
    u8 cursor;
    u8 aPresses;
    u8 bPresses;
    u8 aHistory[WINDOW_FRAMES];
    u8 bHistory[WINDOW_FRAMES];
    gzTextBox* pText;
};

class J2DPicture;
struct gzInputViewer_s {
    void drawButton(J2DPicture* pic, u32 button, u32 color, f32 x, f32 y, f32 sx, f32 sy);
    void cleanup();

    bool isInitialized;
    J2DPicture* pAbtn;
    J2DPicture* pBbtn;
    J2DPicture* pXbtn;
    J2DPicture* pYbtn;
    J2DPicture* pZbtn;
    J2DPicture* pSbtn;
    J2DPicture* pStick;
    J2DPicture* pSubstick;
    J2DPicture* pDPad[4];
    J2DPicture* pTrigL;
    J2DPicture* pTrigR;
    gzTextBox* pStickValueText;
    gzTextBox* pSubstickValueText;
    void* pStickBuf;
    void* pDPadBuf;
    void* pTrigBuf;
};

struct gzToolLinkDebugInfo_s {
    void create();
    void delete_();

    bool isInitialized;
    gzTextBox* pTimeTbox;
    gzTextBox* pActionTbox;
    gzTextBox* pPosText[3];
    gzTextBox* pAngleText[2];
    gzTextBox* pSpeedText;
};

class gzToolsMng_c {
public:
    void execute();
    void draw();
    void executeElevatorEscape();
    void executeFreeCam();
    void executeGorgeVoid();
    void executeMoveLink();
    void executeCoroTD();
    void executeEBMB();
    void executeRollChecker();
    void executeTeleport();
    void executeAbMashRate();

    void drawRollChecker();
    void drawLinkInfo();
    void drawInputViewer();
    void drawAbMashRate();

    bool isMoveLinkActive() const { return mMoveLink.active; }

private:
    ElevatorEscapeState mElevatorEscape;
    GorgeVoidState mGorgeVoid;
    TeleportState mTeleport;
    MoveLinkState mMoveLink;
    FreeCamState mFreeCam;
    CoroTDState mCoroTD;
    EBMBState mEBMB;
    RollCheckerState mRollChecker;
    gzInputViewer_s mInputViewer;
    gzToolLinkDebugInfo_s mLinkInfo;
    AbMashRateState mAbMashRate;
};

#endif // GZ_MANAGER_TOOLS_H
