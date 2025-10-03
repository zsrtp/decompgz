#ifndef GZ_H
#define GZ_H

#include "d/d_drawlist.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "f_op/f_op_scene_mng.h"
#include "m_Do/m_Do_controller_pad.h"
#include "gz/gz_menu.h"

class JKRExpHeap;
class JKRHeap;
class dDlst_2D_c;
class mDoDvdThd_mountXArchive_c;
class mDoDvdThd_mountArchive_c;
class mDoDvdThd_toMainRam_c;

class gz_c : public scene_class {
public:
    enum {
        EXEC_DVD_WAIT,
        EXEC_DRAW_MENU,
        EXEC_SCENE_CHANGE,
    };

    gz_c() {}
    ~gz_c();

    void preLoad_dyl_create();
    void preLoad_dyl_remove();
    bool preLoad_dyl();
    int create();
    int execute();
    int draw();
    void drawMenu();
    void dvdWaitDraw();
    void nextSceneChange();
    void dvdDataLoad();
    void setRenderMode();

    #if VERSION == VERSION_GCN_PAL
    u8 getPalLanguage();
    #endif

    #if DEBUG
    static void onOpeningCut() {
        mOpeningCut = true;
    }
    
    static u8 mOpeningCut;
    #endif

public:
    request_of_phase_process_class field_0x1c4;
    mDoDvdThd_toMainRam_c* sceneCommand;
    JKRExpHeap* field_0x1d0;
    JKRExpHeap* field_0x1d4;
    JKRHeap* mpHeap;
#if VERSION == VERSION_GCN_PAL
    mDoDvdThd_mountArchive_c* mpPalLogoResCommand;
#endif
    request_of_phase_process_class* m_preLoad_dylPhase;
    ResTIMG* mProgressivePro;
    ResTIMG* mProgressiveInter;
    u8 mExecCommand;
    u8 field_0x209;
    u8 field_0x20a;
    u8 field_0x20b;
    u16 mTimer;
    u16 field_0x20e;
    u16 field_0x210;
    u16 field_0x212;
    u16 field_0x214;
    u32 field_0x218;
    void* dummyGameAlloc;
    mDoDvdThd_mountXArchive_c* mpField0Command;
    mDoDvdThd_mountXArchive_c* mpAlAnmCommand;
    u8 field_0x228[4];
    mDoDvdThd_mountXArchive_c* mpFmapResCommand;
    mDoDvdThd_mountXArchive_c* mpDmapResCommand;
    mDoDvdThd_mountXArchive_c* mpCollectResCommand;
    u8 field_0x238[4];
    mDoDvdThd_mountXArchive_c* mpItemIconCommand;
    mDoDvdThd_mountXArchive_c* mpRingResCommand;
    u8 field_0x244[4];
    mDoDvdThd_mountXArchive_c* mpPlayerNameCommand;
    mDoDvdThd_mountXArchive_c* mpItemInfResCommand;
    mDoDvdThd_mountXArchive_c* mpButtonCommand;
    u8 field_0x254[4];
    mDoDvdThd_mountXArchive_c* mpCardIconCommand;
    mDoDvdThd_mountXArchive_c* mpBmgResCommand;
    mDoDvdThd_mountXArchive_c* mpMsgComCommand;
    mDoDvdThd_mountXArchive_c* mpMsgResCommand[7];
    u8 field_0x280[0x10];
    mDoDvdThd_mountXArchive_c* mpFontResCommand;
    mDoDvdThd_mountXArchive_c* mpMain2DCommand;
    mDoDvdThd_mountXArchive_c* mpRubyResCommand;
    mDoDvdThd_toMainRam_c* mParticleCommand;
    mDoDvdThd_toMainRam_c* mItemTableCommand;
    mDoDvdThd_toMainRam_c* mEnemyItemCommand;

public:
    J2DPicture* mpIcon;
    J2DTextBox* mpHeader;
    gzMainMenu_c* mpMainMenu;
    u32 mCursorColor;
    bool mDisplay;
};

static int phase_0(gz_c* logo);
static int phase_1(gz_c* logo);
static int phase_2(gz_c* logo);

typedef int (*gz_Method)(gz_c*);

class gzInfo_c {
public:
    bool isDisplay() const {
        if (m_process != NULL) {
            return m_process->mDisplay;
        }
        
        return false;
    }

    u32 getCursorColor() const {
        if (m_process != NULL) {
            return m_process->mCursorColor;
        }
        
        return 0xFFFFFFFF;
    }

    gz_c* m_process;
};

extern gzInfo_c g_gzInfo;

namespace gzPad {
    inline u32 getTrig() { return mDoCPd_c::m_gzPadInfo.mPressedButtonFlags; }
    inline u32 getTrigLockL() { return mDoCPd_c::m_gzPadInfo.mTrigLockL; }
    inline u32 getTrigLockR() { return mDoCPd_c::m_gzPadInfo.mTrigLockR; }
    inline u32 getTrigUp() { return getTrig() & PAD_BUTTON_UP; }
    inline u32 getTrigDown() { return getTrig() & PAD_BUTTON_DOWN; }
    inline u32 getTrigLeft() { return getTrig() & PAD_BUTTON_LEFT; }
    inline u32 getTrigRight() { return getTrig() & PAD_BUTTON_RIGHT; }
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

#endif // GZ_H
