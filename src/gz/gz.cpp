#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "gz/gz_menu.h"
#include "SSystem/SComponent/c_counter.h"

gzInfo_c g_gzInfo;

int gz_c::_create() {
    mCursorColor = 0xEE8000FF; // temp

    ResTIMG* icon = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "midona64.bti");
    mpIcon = new J2DPicture(icon);

    mpHeader = new J2DTextBox();
    mpHeader->setFont(mDoExt_getMesgFont());
    mpHeader->setFontSize(18.0f, 18.0f);
    mpHeader->setCharColor(mCursorColor);
    mpHeader->setGradColor(mCursorColor);
    mpHeader->setString("tpgz v1.2.0");

    mpMainMenu = new gzMainMenu_c();

    return cPhs_COMPLEATE_e;
}

int gz_c::_delete() {
    delete mpIcon;
    mpIcon = NULL;

    delete mpHeader;
    mpHeader = NULL;

    delete mpMainMenu;
    mpMainMenu = NULL;

    return 1;
}

int gz_c::execute() {
    if (gzPad::getHoldL() && gzPad::getHoldR() && gzPad::getTrigDown()) {
        mDisplay = !mDisplay;
    }

    if (mDisplay) {
        if (gzPad::getTrigB()) {
            mDisplay = false;
        }

        mpMainMenu->execute();
    }

    return 1;
}

int gz_c::draw() {
    if (mDisplay) {
        if (mpIcon != NULL) {
            mpIcon->draw(30.0f, 5.0f, 30.0f, 30.0f, false, false, false);
        }

        if (mpHeader != NULL) {
            mpHeader->draw(65.0f, 30.0f, 608.0f, HBIND_LEFT);
        }

        dComIfGd_set2DOpaTop(mpMainMenu);
    }

    return 1;
}

static int gz_Draw(gz_c* i_this) {
    return i_this->draw();
}

static int gz_Execute(gz_c* i_this) {
    return i_this->execute();
}

static int gz_IsDelete(gz_c* i_this) {
    return 1;
}

static int gz_Delete(gz_c* i_this) {
    OSReport("[%d] - run gz_Delete\n", g_Counter.mCounter0);
    g_gzInfo.m_process = NULL;
    return i_this->_delete();
}

static int gz_Create(gz_c* i_this) {
    OSReport("[%d] - run gz_Create\n", g_Counter.mCounter0);
    g_gzInfo.m_process = i_this;
    return i_this->_create();
}

// make sure these don't get stripped
#pragma push
#pragma force_active on
static leafdraw_method_class l_gzMenu_Method = {
    (process_method_func)gz_Create,
    (process_method_func)gz_Delete,
    (process_method_func)gz_Execute,
    (process_method_func)gz_IsDelete,
    (process_method_func)gz_Draw,
};

extern msg_process_profile_definition g_profile_GZMENU = {
    fpcLy_CURRENT_e,
    12,
    fpcPi_CURRENT_e,
    PROC_GZMENU,
    (process_method_class*)&g_fpcLf_Method,
    sizeof(gz_c),
    0,
    0,
    &g_fopMsg_Method,
    770,
    &l_gzMenu_Method,
};
#pragma pop
