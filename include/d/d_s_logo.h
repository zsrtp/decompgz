#ifndef D_S_D_S_LOGO_H
#define D_S_D_S_LOGO_H

#include "f_op/f_op_scene.h"

class JKRExpHeap;
class JKRHeap;
struct ResTIMG;
class dDlst_2D_c;
class mDoDvdThd_mountXArchive_c;
class mDoDvdThd_mountArchive_c;
class mDoDvdThd_toMainRam_c;

class dScnLogo_c : public scene_class {
public:
    enum {
        EXEC_PROG_IN,
        EXEC_PROG_SEL,
        EXEC_PROG_OUT,
        EXEC_PROG_SET,
        EXEC_PROG_SET2,
        EXEC_PROG_CHANGE,
        EXEC_DVD_WAIT,
        EXEC_SCENE_CHANGE,
    };

    dScnLogo_c() {}
    void preLoad_dyl_create();
    void preLoad_dyl_remove();
    bool preLoad_dyl();
    void checkProgSelect();
    int draw();
    void progInDraw();
    void progSelDraw();
    void progOutDraw();
    void progSetDraw();
    void progSet2Draw();
    void progChangeDraw();
    void dvdWaitDraw();
    void nextSceneChange();
    ~dScnLogo_c();
    int create();
    void dvdDataLoad();
    void setProgressiveMode(u8);
    u8 getProgressiveMode();
    bool isProgressiveMode();
    void setRenderMode();
    void setupGameResources();

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
    dDlst_2D_c* mProgressiveChoice;
    dDlst_2D_c* mProgressiveYes;
    dDlst_2D_c* mProgressiveNo;
    dDlst_2D_c* mProgressiveSel;
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
};

static int phase_0(dScnLogo_c* logo);
static int phase_1(dScnLogo_c* logo);
static int phase_2(dScnLogo_c* logo);

typedef int (*dScnLogo_Method)(dScnLogo_c*);;

#endif /* D_S_D_S_LOGO_H */
