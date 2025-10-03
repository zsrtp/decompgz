/**
 * @file gz.cpp
 * 
 * @details Scene process implementation code to execute the bare minimum code to get the game to boot to the title screen
 *  Mostly a stripped down version of d_s_logo.cpp
 * 
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "JSystem/JKernel/JKRAram.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "c/c_dylink.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_map_path_dmap.h"
#include "m_Do/m_Do_Reset.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_machine.h"

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

gzInfo_c g_gzInfo;
static bool runonce;

typedef void (gz_c::*execFunc)();
static execFunc l_execFunc[3] = {
    &gz_c::dvdWaitDraw,
    &gz_c::drawMenu,
    &gz_c::nextSceneChange,
};

void gz_c::preLoad_dyl_create() {
    m_preLoad_dylPhase = new request_of_phase_process_class[14];
    memset(m_preLoad_dylPhase, 0, sizeof(request_of_phase_process_class) * 14);
}

void gz_c::preLoad_dyl_remove() {
    delete[] m_preLoad_dylPhase;
}

static s16 const l_preLoad_dylKeyTbl[14] = {
    0x02DC, 0x02CE, 0x0221, 0x00F2, 0x021B, 0x02F4, 0x0139,
    0x015A, 0x02E4, 0x00FE, 0x0308, 0x030F, 0x00FF, 0x013F,
};

bool gz_c::preLoad_dyl() {
    bool ret = true;

    for (int i = 0; i < 14; i++) {
        int phase_state = cDylPhs::Link(&m_preLoad_dylPhase[i], l_preLoad_dylKeyTbl[i]);

        if (phase_state != cPhs_COMPLEATE_e) {
            ret = false;
        }
    }

    return ret;
}

int gz_c::draw() {
    cLib_calcTimer<u16>(&mTimer);
    (this->*l_execFunc[mExecCommand])();
    return 1;
}

void gz_c::dvdWaitDraw() {
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
            mExecCommand = EXEC_DRAW_MENU;
        }
    }
}

void gz_c::drawMenu() {
    if (!runonce) {
        // create the gz menu
        mCursorColor = 0xEE8000FF; // temp
        // ResTIMG* icon = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "midona64.bti");
        // mpIcon = new J2DPicture(icon);
        mpHeader = new J2DTextBox();
        mpHeader->setFont(mDoExt_getMesgFont());
        mpHeader->setFontSize(18.0f, 18.0f);
        mpHeader->setCharColor(mCursorColor);
        mpHeader->setGradColor(mCursorColor);
        mpHeader->setString("tpgz v1.2.0");

        mpMainMenu = new gzMainMenu_c();
        runonce = true;
        // mDisplay = true;
    }

    OSReport("executing!\n");
    mpMainMenu->execute();

    OSReport("drawing menu!\n");
    if (mpIcon != NULL) {
        mpIcon->draw(30.0f, 5.0f, 30.0f, 30.0f, false, false, false);
    }

    if (mpHeader != NULL) {
        mpHeader->draw(65.0f, 30.0f, 608.0f, HBIND_LEFT);
    }

    dComIfGd_set2DOpaTop(mpMainMenu);

    if (gzPad::getTrigB()) {
        mDisplay = false;
        mExecCommand = gz_c::EXEC_SCENE_CHANGE;
    }
}

void gz_c::nextSceneChange() {
    if (!mDoRst::isReset()) {
        dComIfG_changeOpeningScene(this, PROC_OPENING_SCENE);
    }
}

static int phase_0(gz_c* i_this) {
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

static int phase_1(gz_c* i_this) {
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

static int phase_2(gz_c* i_this) {
    if (dComIfG_syncAllObjectRes()) {
        return cPhs_INIT_e;
    } else {
        return cPhs_COMPLEATE_e;
    }
}

static int resLoad(request_of_phase_process_class* i_phase, gz_c* i_this) {
    static int (*l_method[3])(void*) = {(int (*)(void*))phase_0, (int (*)(void*))phase_1,
                                        (int (*)(void*))phase_2};

    return dComLbG_PhaseHandler(i_phase, l_method, i_this);
}

int gz_c::create() {
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
    field_0x20a = 0;
    mDoGph_gInf_c::startFadeIn(30);
    JUTGamePad::clearResetOccurred();
    JUTGamePad::setResetCallback(mDoRst_resetCallBack, NULL);
    mDoRst::offReset();
    mDoRst::offResetPrepare();

    return phase_state;
}

void gz_c::dvdDataLoad() {
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

static int gz_create(scene_class* i_this) {
    return (new (i_this) gz_c())->create();
}

static int gz_Execute(gz_c* i_this) {
    if (mDoRst::isReset()) {
        fopScnM_ChangeReq(i_this, PROC_GZMENU, 0, 5);
    }

    // if (gzPad::getHoldL() && gzPad::getHoldR() && gzPad::getTrigDown()) {
    //     i_this->mDisplay = !i_this->mDisplay;
    // }

    if (i_this != NULL) {
        
    }

    return 1;
}

static int gz_Draw(gz_c* i_this) {
    i_this->draw();
    return 1;
}

static int gz_Delete(gz_c* i_this) {
    i_this->~gz_c();
    return 1;
}

static int gz_IsDelete(gz_c* i_this) {
    return 1;
}

#if VERSION == VERSION_GCN_PAL
u8 gz_c::getPalLanguage() {
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

gz_c::~gz_c() {
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

    JKRAramHeap* aram_heap = JKRAram::getAramHeap();
    u32 free_size = aram_heap->getTotalFreeSize();
    mDoExt_getMesgFont();
    mDoExt_getSubFont();
    mDoExt_getRubyFont();
    mDoExt_setAraCacheSize(free_size - aram_heap->getTotalFreeSize());

    // gz
    delete mpIcon;
    mpIcon = NULL;

    delete mpHeader;
    mpHeader = NULL;

    delete mpMainMenu;
    mpMainMenu = NULL;

#if VERSION == VERSION_GCN_JPN
    if (dComIfGp_getFontArchive() != NULL) {
        dComIfGp_getFontArchive()->unmount();
        dComIfGp_setFontArchive(NULL);
    }
#endif

    dComIfGp_setItemTable(mItemTableCommand->getMemAddress());
    mItemTableCommand->destroy();

    dEnemyItem_c::setItemData((u8*)mEnemyItemCommand->getMemAddress());
    mEnemyItemCommand->destroy();

    dDlst_shadowControl_c::setSimpleTex((ResTIMG*)dComIfG_getObjectRes("Always", 0x4A));
    dTres_c::createWork();
    dMpath_c::createWork();
}

#pragma push
#pragma force_active on
static gz_Method l_gz_Method[5] = {
    (gz_Method)gz_create,
    gz_Delete,
    gz_Execute,
    gz_IsDelete,
    gz_Draw,
};

scene_process_profile_definition g_profile_GZ_MENU= {
    fpcLy_ROOT_e,
    1,
    fpcPi_CURRENT_e,
    PROC_GZMENU,
    &g_fpcNd_Method.base,
    sizeof(gz_c),
    0,
    0,
    &g_fopScn_Method.base,
    (process_method_class*)&l_gz_Method,
    NULL,
};
#pragma pop
