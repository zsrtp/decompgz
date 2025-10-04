/**
 * d_s_logo.cpp
 * Game Boot Logo's Display
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_s_logo.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "c/c_dylink.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_map_path_dmap.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_machine.h"

#include "gz/gz.h"

#if VERSION == VERSION_GCN_JPN
#define LOGO_ARC  "Logo"
#define MSG_PATH  "/res/Msgjp/bmgres.arc"
#elif VERSION == VERSION_GCN_PAL
#define LOGO_ARC "LogoPal"
#define MSG_PATH  "/res/Msgus/bmgres.arc"
#else
#define LOGO_ARC "LogoUs"
#define MSG_PATH  "/res/Msgus/bmgres.arc"
#endif

#if VERSION == VERSION_GCN_PAL
#define PROGRESSIVE_MODE_OFF OS_EURGB60_OFF
#define PROGRESSIVE_MODE_ON  OS_EURGB60_ON
#else
#define PROGRESSIVE_MODE_OFF OS_PROGRESSIVE_MODE_OFF
#define PROGRESSIVE_MODE_ON  OS_PROGRESSIVE_MODE_ON
#endif

typedef void (dScnLogo_c::*execFunc)();
static execFunc l_execFunc[2] = {
    &dScnLogo_c::dvdWaitDraw,
    &dScnLogo_c::nextSceneChange,
};

/* 802560B4-802560F8 2509F4 0044+00 1/1 0/0 0/0 .text            preLoad_dyl_create__10dScnLogo_cFv
 */
void dScnLogo_c::preLoad_dyl_create() {
    m_preLoad_dylPhase = new request_of_phase_process_class[14];
    memset(m_preLoad_dylPhase, 0, sizeof(request_of_phase_process_class) * 14);
}

/* 802560F8-8025611C 250A38 0024+00 1/1 0/0 0/0 .text            preLoad_dyl_remove__10dScnLogo_cFv
 */
void dScnLogo_c::preLoad_dyl_remove() {
    delete[] m_preLoad_dylPhase;
}

/* 80399FE0-80399FFC 026640 001C+00 1/1 0/0 0/0 .rodata          l_preLoad_dylKeyTbl */
static s16 const l_preLoad_dylKeyTbl[14] = {
    0x02DC, 0x02CE, 0x0221, 0x00F2, 0x021B, 0x02F4, 0x0139,
    0x015A, 0x02E4, 0x00FE, 0x0308, 0x030F, 0x00FF, 0x013F,
};

/* 8025611C-80256198 250A5C 007C+00 2/2 0/0 0/0 .text            preLoad_dyl__10dScnLogo_cFv */
bool dScnLogo_c::preLoad_dyl() {
    bool ret = true;

    for (int i = 0; i < 14; i++) {
        int phase_state = cDylPhs::Link(&m_preLoad_dylPhase[i], l_preLoad_dylKeyTbl[i]);

        if (phase_state != cPhs_COMPLEATE_e) {
            ret = false;
        }
    }

    return ret;
}

/* 80256210-80256264 250B50 0054+00 1/1 0/0 0/0 .text            draw__10dScnLogo_cFv */
int dScnLogo_c::draw() {
    (this->*l_execFunc[mExecCommand])();
    return 1;
}

/* 80257070-80257284 2519B0 0214+00 1/0 0/0 0/0 .text            dvdWaitDraw__10dScnLogo_cFv */
void dScnLogo_c::dvdWaitDraw() {
    if (!dComIfG_syncAllObjectRes()) {
        if (mpField0Command->sync() && mpAlAnmCommand->sync() && mpFmapResCommand->sync() &&
            mpDmapResCommand->sync() && mpCollectResCommand->sync() && mpItemIconCommand->sync() &&
            mpRingResCommand->sync() && mpPlayerNameCommand->sync() &&
            mpItemInfResCommand->sync() && mpButtonCommand->sync() && mpCardIconCommand->sync() &&
            mpBmgResCommand->sync() && mpMsgComCommand->sync() && mpMsgResCommand[0]->sync() &&
            mpMsgResCommand[1]->sync() && mpMsgResCommand[2]->sync() &&
            mpMsgResCommand[3]->sync() && mpMsgResCommand[4]->sync() &&
            mpMsgResCommand[5]->sync() && mpMsgResCommand[6]->sync() && mpFontResCommand->sync() &&
            mpMain2DCommand->sync() && mpRubyResCommand->sync() && mParticleCommand->sync() &&
            mItemTableCommand->sync() && mEnemyItemCommand->sync() && preLoad_dyl())
        {
            mDoRst::setLogoScnFlag(0);
            mDoRst::setProgChgFlag(0);
            mExecCommand = EXEC_SCENE_CHANGE;
        }
    }
}

void dScnLogo_c::setupGameResources() {
    dComIfGp_particle_createCommon(mParticleCommand->getMemAddress());
    dComIfGp_setFieldMapArchive2(mpField0Command->getArchive());
    dComIfGp_setAnmArchive(mpAlAnmCommand->getArchive());
    dComIfGp_setFmapResArchive(mpFmapResCommand->getArchive());
    dComIfGp_setDmapResArchive(mpDmapResCommand->getArchive());
    dComIfGp_setCollectResArchive(mpCollectResCommand->getArchive());
    dComIfGp_setItemIconArchive(mpItemIconCommand->getArchive());
    dComIfGp_setAllMapArchive(NULL);
    dComIfGp_setRingResArchive(mpRingResCommand->getArchive());
    dComIfGp_setNameResArchive(mpPlayerNameCommand->getArchive());
    dComIfGp_setDemoMsgArchive(mpItemInfResCommand->getArchive());
    dComIfGp_setMeterButtonArchive(mpButtonCommand->getArchive());
    dComIfGp_setErrorResArchive(NULL);
    dComIfGp_setCardIconResArchive(mpCardIconCommand->getArchive());
    dComIfGp_setMsgDtArchive(0, mpBmgResCommand->getArchive());
    dComIfGp_setMsgCommonArchive(mpMsgComCommand->getArchive());
    
    for (int i = 0; i < 7; i++) {
        dComIfGp_setMsgArchive(i, mpMsgResCommand[i]->getArchive());
    }

    dComIfGp_setFontArchive(mpFontResCommand->getArchive());
    dComIfGp_setRubyArchive(mpRubyResCommand->getArchive());
    dComIfGp_setMain2DArchive(mpMain2DCommand->getArchive());
    JKRAramHeap* aram_heap = JKRAram::getAramHeap();
    u32 free_size = aram_heap->getTotalFreeSize();

    mDoExt_getMesgFont();
    mDoExt_getSubFont();
    mDoExt_getRubyFont();
    mDoExt_setAraCacheSize(free_size - aram_heap->getTotalFreeSize());

    #if VERSION == VERSION_GCN_JPN
    if (dComIfGp_getFontArchive() != NULL) {
        dComIfGp_getFontArchive()->unmount();
        dComIfGp_setFontArchive(NULL);
    }
    #endif

    dComIfGp_setItemTable(mItemTableCommand->getMemAddress());
    dEnemyItem_c::setItemData((u8*)mEnemyItemCommand->getMemAddress());
    dDlst_shadowControl_c::setSimpleTex((ResTIMG*)dComIfG_getObjectRes("Always", 0x4A));
    dTres_c::createWork();
    dMpath_c::createWork();
}

/* 80257284-802572B8 251BC4 0034+00 1/0 0/0 0/0 .text            nextSceneChange__10dScnLogo_cFv */
void dScnLogo_c::nextSceneChange() {
    if (!mDoRst::isReset()) {
        dComIfG_changeOpeningScene(this, PROC_OPENING_SCENE);
    }
}

/* 802572B8-80257910 251BF8 0658+00 1/1 0/0 0/0 .text            __dt__10dScnLogo_cFv */
dScnLogo_c::~dScnLogo_c() {
    if (mDoRst::isReset()) {
        if (mDoAud_zelAudio_c::isInitFlag()) {
            Z2AudioMgr::getInterface()->resetProcess(5, true);
        }
        mDoRst_reset(0, 0x80000000, 0);
    }

#if VERSION == VERSION_GCN_PAL
    mpPalLogoResCommand->getArchive()->removeResourceAll();
    mpPalLogoResCommand->getArchive()->unmount();
    mpPalLogoResCommand->destroy();
#endif

    preLoad_dyl_remove();
    dComIfG_deleteObjectResMain(LOGO_ARC);

    field_0x1d4->destroy();
    field_0x1d0->destroy();
    JKRFree(dummyGameAlloc);

    mpField0Command->destroy();
    mpAlAnmCommand->destroy();
    mpFmapResCommand->destroy();
    mpDmapResCommand->destroy();
    mpCollectResCommand->destroy();
    mpItemIconCommand->destroy();
    mpRingResCommand->destroy();
    mpPlayerNameCommand->destroy();
    mpItemInfResCommand->destroy();
    mpButtonCommand->destroy();
    mpCardIconCommand->destroy();
    mpBmgResCommand->destroy();
    mpMsgComCommand->destroy();

    for (int i = 0; i < 7; i++) {
        mpMsgResCommand[i]->destroy();
    }

    mpFontResCommand->destroy();
    mpMain2DCommand->destroy();
    mpRubyResCommand->destroy();
    mParticleCommand->destroy();
    mItemTableCommand->destroy();
    mEnemyItemCommand->destroy();
}

/* 80257910-802579BC 252250 00AC+00 1/0 0/0 0/0 .text            phase_0__FP10dScnLogo_c */
static int phase_0(dScnLogo_c* i_this) {
    mDoGph_gInf_c::setFadeColor(*(JUtility::TColor*)&g_blackColor);
    dComIfGp_particle_create();

    i_this->dummyGameAlloc = mDoExt_getGameHeap()->alloc(0x340000, -0x10);
    JUT_ASSERT(1523, i_this->dummyGameAlloc != NULL);
    i_this->field_0x1d0 = JKRExpHeap::create(i_this->dummyGameAlloc, 0x340000, NULL, false);
    i_this->field_0x1d4 = JKRExpHeap::create(0x130000, i_this->field_0x1d0, false);

    #if VERSION == VERSION_GCN_PAL
    switch (i_this->getPalLanguage()) {
    case 1:
        i_this->mpPalLogoResCommand = mDoDvdThd_mountArchive_c::create("/res/Layout/LogoPalGm.arc", 0, NULL);
        break;
    case 2:
        i_this->mpPalLogoResCommand = mDoDvdThd_mountArchive_c::create("/res/Layout/LogoPalFr.arc", 0, NULL);
        break;
    case 3:
        i_this->mpPalLogoResCommand = mDoDvdThd_mountArchive_c::create("/res/Layout/LogoPalSp.arc", 0, NULL);
        break;
    case 4:
        i_this->mpPalLogoResCommand = mDoDvdThd_mountArchive_c::create("/res/Layout/LogoPalIt.arc", 0, NULL);
        break;
    case 0:
    default:
        i_this->mpPalLogoResCommand = mDoDvdThd_mountArchive_c::create("/res/Layout/LogoPalUk.arc", 0, NULL);
        break;
    }
    #endif

    return cPhs_NEXT_e;
}

/* 802579BC-80257A70 2522FC 00B4+00 1/0 0/0 0/0 .text            phase_1__FP10dScnLogo_c */
static int phase_1(dScnLogo_c* i_this) {
    if (!cDyl_InitAsyncIsDone()) {
        return cPhs_INIT_e;
    }

    if (!mDoAud_zelAudio_c::isInitFlag() || Z2AudioMgr::getInterface()->checkFirstWaves()) {
        return cPhs_INIT_e;
    }

#if VERSION == VERSION_GCN_PAL
    if (!SyncWidthSound) {
        return cPhs_INIT_e;
    }

    if (!i_this->mpPalLogoResCommand->sync()) {
        return cPhs_INIT_e;
    }
#endif

    dComIfG_setObjectRes(LOGO_ARC, (u8)0, i_this->field_0x1d0);
    mDoRst::setLogoScnFlag(1);
    archiveHeap->dump_sort();
    return cPhs_NEXT_e;
}

/* 80257A70-80257AB4 2523B0 0044+00 1/0 0/0 0/0 .text            phase_2__FP10dScnLogo_c */
static int phase_2(dScnLogo_c* i_this) {
    if (dComIfG_syncAllObjectRes()) {
        return cPhs_INIT_e;
    } else {
        return cPhs_COMPLEATE_e;
    }
}

/* 80257AB4-80257AE0 2523F4 002C+00 1/1 0/0 0/0 .text
 * resLoad__FP30request_of_phase_process_classP10dScnLogo_c     */
static int resLoad(request_of_phase_process_class* i_phase, dScnLogo_c* i_this) {
    static int (*l_method[3])(void*) = {(int (*)(void*))phase_0, (int (*)(void*))phase_1,
                                        (int (*)(void*))phase_2};

    return dComLbG_PhaseHandler(i_phase, l_method, i_this);
}

/* 80257AE0-80257C64 252420 0184+00 1/1 0/0 0/0 .text            create__10dScnLogo_cFv */
int dScnLogo_c::create() {
    int phase_state = resLoad(&field_0x1c4, this);
    if (phase_state != cPhs_COMPLEATE_e) {
        return phase_state;
    }

    mpHeap = mDoExt_setCurrentHeap(field_0x1d4);
    mpHeap->becomeCurrentHeap();

    dvdDataLoad();
    Z2AudioMgr::getInterface()->loadStaticWaves();
    mDoGph_gInf_c::setTickRate((OS_BUS_CLOCK / 4) / 60);
    mDoGph_gInf_c::waitBlanking(0);
    mDoGph_gInf_c::startFadeIn(30);

    // Apply static progressive mode configuration
    setProgressiveMode(g_progressiveMode ? PROGRESSIVE_MODE_ON : PROGRESSIVE_MODE_OFF);
    if (g_progressiveMode) {
        setRenderMode();
    }

    // Always go directly to resource loading
    mExecCommand = EXEC_DVD_WAIT;
    mDoRst::setProgSeqFlag(1);

    JUTGamePad::clearResetOccurred();
    JUTGamePad::setResetCallback(mDoRst_resetCallBack, NULL);
    mDoRst::offReset();
    mDoRst::offResetPrepare();

    return phase_state;
}

/* 80257FEC-80258420 25292C 0434+00 1/1 0/0 0/0 .text            dvdDataLoad__10dScnLogo_cFv */
void dScnLogo_c::dvdDataLoad() {
    dComIfG_setObjectRes("Always", (u8)0, NULL);
    archiveHeap->dump_sort();

    dComIfG_setObjectRes("Alink", (u8)0, NULL);

    mpField0Command = mDoDvdThd_mountXArchive_c::create(
        "/res/FieldMap/Field0.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpAlAnmCommand =
        mDoDvdThd_mountXArchive_c::create("/res/Object/AlAnm.arc", 0, JKRArchive::MOUNT_ARAM, NULL);
    mpFmapResCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/fmapres.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpDmapResCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/dmapres.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpCollectResCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/clctres.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpItemIconCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/itemicon.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpRingResCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/ringres.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpPlayerNameCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/playerName.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpItemInfResCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/itmInfRes.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpButtonCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/button.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpCardIconCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/CardIcon/cardicon.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());

    #if VERSION == VERSION_GCN_PAL
    switch (getPalLanguage()) {
    case 1:
        mpBmgResCommand = mDoDvdThd_mountXArchive_c::create("/res/Msgde/bmgres.arc", 0, JKRArchive::MOUNT_MEM, NULL);
        break;
    case 2:
        mpBmgResCommand = mDoDvdThd_mountXArchive_c::create("/res/Msgfr/bmgres.arc", 0, JKRArchive::MOUNT_MEM, NULL);
        break;
    case 3:
        mpBmgResCommand = mDoDvdThd_mountXArchive_c::create("/res/Msgsp/bmgres.arc", 0, JKRArchive::MOUNT_MEM, NULL);
        break;
    case 4:
        mpBmgResCommand = mDoDvdThd_mountXArchive_c::create("/res/Msgit/bmgres.arc", 0, JKRArchive::MOUNT_MEM, NULL);
        break;
    case 0:
    default:
        mpBmgResCommand = mDoDvdThd_mountXArchive_c::create("/res/Msguk/bmgres.arc", 0, JKRArchive::MOUNT_MEM, NULL);
        break;
    }
    #else
    mpBmgResCommand = mDoDvdThd_mountXArchive_c::create(MSG_PATH, 0, JKRArchive::MOUNT_MEM, NULL);
    #endif

    mpMsgComCommand = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgcom.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMsgResCommand[0] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres00.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMsgResCommand[1] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres01.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMsgResCommand[2] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres02.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMsgResCommand[3] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres03.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
#if VERSION == VERSION_GCN_JPN
    mpMsgResCommand[4] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres04.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
#else
    mpMsgResCommand[4] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres04F.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
#endif
    mpMsgResCommand[5] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres05.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMsgResCommand[6] = mDoDvdThd_mountXArchive_c::create(
        "/res/Layout/msgres06.arc", 0, JKRArchive::MOUNT_ARAM, mDoExt_getJ2dHeap());
    mpMain2DCommand =
        mDoDvdThd_mountXArchive_c::create("/res/Layout/main2D.arc", 0, JKRArchive::MOUNT_MEM, NULL);

#if VERSION == VERSION_GCN_JPN
    mpFontResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fontjp/fontres.arc", 1,
                                                         JKRArchive::MOUNT_MEM, NULL);
    mpRubyResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fontjp/rubyres.arc", 0,
                                                         JKRArchive::MOUNT_MEM, NULL);
#elif VERSION == VERSION_GCN_PAL
    mpFontResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fonteu/fontres.arc", 0,
                                                         JKRArchive::MOUNT_MEM, NULL);
    mpRubyResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fonteu/rubyres.arc", 0,
                                                         JKRArchive::MOUNT_MEM, NULL);
#else
    mpFontResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fontus/fontres.arc", 0,
                                                         JKRArchive::MOUNT_MEM, NULL);
    mpRubyResCommand = mDoDvdThd_mountXArchive_c::create("/res/Fontus/rubyres.arc", 0,
                                                         JKRArchive::MOUNT_MEM, NULL);
#endif
    mParticleCommand = mDoDvdThd_toMainRam_c::create("/res/Particle/common.jpc", 0,
                                                     dComIfGp_particle_getResHeap());
    mItemTableCommand = mDoDvdThd_toMainRam_c::create("/res/ItemTable/item_table.bin", 0, NULL);
    mEnemyItemCommand = mDoDvdThd_toMainRam_c::create("/res/ItemTable/enemy_table.bin", 0, NULL);

    preLoad_dyl_create();
    preLoad_dyl();
}

/* 80258420-80258444 252D60 0024+00 1/0 0/0 0/0 .text            dScnLogo_Create__FP11scene_class */
static int dScnLogo_Create(scene_class* i_this) {
    return (new (i_this) dScnLogo_c())->create();
}

/* 80258444-80258484 252D84 0040+00 1/0 0/0 0/0 .text            dScnLogo_Execute__FP10dScnLogo_c */
static int dScnLogo_Execute(dScnLogo_c* i_this) {
    if (mDoRst::isReset()) {
        fopScnM_ChangeReq(i_this, PROC_LOGO_SCENE, 0, 5);
    }
    return 1;
}

/* 80258484-802584A8 252DC4 0024+00 1/0 0/0 0/0 .text            dScnLogo_Draw__FP10dScnLogo_c */
static int dScnLogo_Draw(dScnLogo_c* i_this) {
    i_this->draw();
    return 1;
}

/* 802584A8-802584D0 252DE8 0028+00 1/0 0/0 0/0 .text            dScnLogo_Delete__FP10dScnLogo_c */
static int dScnLogo_Delete(dScnLogo_c* i_this) {
    i_this->setupGameResources();
    // initialize GZ after most game resources are setup
    g_gzInfo._create();
    i_this->~dScnLogo_c();
    return 1;
}

/* 802584D0-802584D8 252E10 0008+00 1/0 0/0 0/0 .text            dScnLogo_IsDelete__FP10dScnLogo_c
 */
static int dScnLogo_IsDelete(dScnLogo_c* i_this) {
    return 1;
}

#if VERSION == VERSION_GCN_PAL
u8 dScnLogo_c::getPalLanguage() {
    u8 language;
    switch (OSGetLanguage()) {
    case OS_LANGUAGE_ENGLISH:
        language = 0;
        break;
    case OS_LANGUAGE_GERMAN:
        language = 1;
        break;
    case OS_LANGUAGE_FRENCH:
        language = 2;
        break;
    case OS_LANGUAGE_SPANISH:
        language = 3;
        break;
    case OS_LANGUAGE_ITALIAN:
        language = 4;
        break;
    case OS_LANGUAGE_DUTCH:
        language = 5;
        break;
    }

    return language;
}
#endif

/* 802584D8-802584FC 252E18 0024+00 1/1 0/0 0/0 .text            setProgressiveMode__10dScnLogo_cFUc
 */
void dScnLogo_c::setProgressiveMode(u8 mode) {
    #if VERSION == VERSION_GCN_PAL
    OSSetEuRgb60Mode(mode);
    #else
    OSSetProgressiveMode(mode);
    #endif
}

/* 802584FC-80258520 252E3C 0024+00 3/3 0/0 0/0 .text            getProgressiveMode__10dScnLogo_cFv
 */
u8 dScnLogo_c::getProgressiveMode() {
    #if VERSION == VERSION_GCN_PAL
    return OSGetEuRgb60Mode();
    #else
    return OSGetProgressiveMode();
    #endif
}

/* 80258520-8025854C 252E60 002C+00 1/1 0/0 0/0 .text            isProgressiveMode__10dScnLogo_cFv
 */
bool dScnLogo_c::isProgressiveMode() {
    #if VERSION == VERSION_GCN_PAL
    return OSGetEuRgb60Mode() == OS_EURGB60_ON;
    #else
    return OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_ON;
    #endif
}

/* 8025854C-8025855C 252E8C 0010+00 1/1 0/0 0/0 .text            setRenderMode__10dScnLogo_cFv */
void dScnLogo_c::setRenderMode() {
    mDoMch_render_c::setProgressiveMode();
}

/* 803C2FD0-803C2FE4 -00001 0014+00 1/0 0/0 0/0 .data            l_dScnLogo_Method */
static dScnLogo_Method l_dScnLogo_Method[5] = {
    (dScnLogo_Method)dScnLogo_Create,
    dScnLogo_Delete,
    dScnLogo_Execute,
    dScnLogo_IsDelete,
    dScnLogo_Draw,
};

/* 803C2FE4-803C300C -00001 0028+00 0/0 0/0 1/0 .data            g_profile_LOGO_SCENE */
extern scene_process_profile_definition g_profile_LOGO_SCENE = {
    fpcLy_ROOT_e,
    1,
    fpcPi_CURRENT_e,
    PROC_LOGO_SCENE,
    &g_fpcNd_Method.base,
    sizeof(dScnLogo_c),
    0,
    0,
    &g_fopScn_Method.base,
    (process_method_class*)&l_dScnLogo_Method,
    NULL,
};
