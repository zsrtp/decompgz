#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_main.h"
#include "gz/gz_menu_cheats.h"
#include "gz/gz_menu_flags.h"
#include "gz/gz_menu_framework.h"
#include "gz/gz_menu_heaps.h"
#include "gz/gz_menu_memory.h"
#include "gz/gz_menu_practice.h"
#include "gz/gz_menu_settings.h"
#include "gz/gz_menu_tools.h"
#include "f_op/f_op_scene_mng.h"
#include "SSystem/SComponent/c_counter.h"

void gzMainMenu_c::startForwardTransition() {
    mTransitioning = true;
    mTransitionForward = true;
    mTransitionStart = cCt_getFrameCount();
    mMainStartX = mMainVisibleX;
    mMainEndX = mMainHiddenX;
    mSubStartX = mSubHiddenX;
    mSubEndX = mSubVisibleX;
}

void gzMainMenu_c::startReverseTransition() {
    mTransitioning = true;
    mTransitionForward = false;
    mTransitionStart = cCt_getFrameCount();
    mMainStartX = mMainHiddenX;
    mMainEndX = mMainVisibleX;
    mSubStartX = mSubVisibleX;
    mSubEndX = mSubHiddenX;
    g_gzInfo.mInputWaitTimer = 2;
}

gzMainMenu_c::gzMainMenu_c() {
    OSReport("creating gzMainMenu_c\n");

    mpMenus[MENU_ACTORS] = NULL;
    mpMenus[MENU_CHEATS] = new gzCheatsMenu_c();
    mpMenus[MENU_FLAGS] = new gzFlagsMenu_c();
    mpMenus[MENU_FRAMEWORK] = new gzFrameworkMenu_c();
    mpMenus[MENU_HEAPS] = new gzHeapsMenu_c();
    mpMenus[MENU_INVENTORY] = NULL;
    mpMenus[MENU_MEMORY] = new gzMemoryMenu_c();
    mpMenus[MENU_PRACTICE] = new gzPracticeMenu_c();
    mpMenus[MENU_SCENE] = NULL;
    mpMenus[MENU_SETTINGS] = new gzSettingsMenu_c();
    mpMenus[MENU_TOOLS] = new gzToolsMenu_c();
    mpMenus[MENU_WARPING] = NULL;

    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = new gzTextBox();
    }

    // mpDescription = new gzTextBox();

    mpLines[MENU_ACTORS]->setStringDesc("actors", "create, read, update or delete actors in the current scene");
    mpLines[MENU_CHEATS]->setStringDesc("cheats", "toggle cheats");
    mpLines[MENU_FLAGS]->setStringDesc("flags", "toggle in-game flags");
    mpLines[MENU_FRAMEWORK]->setStringDesc("framework", "view and edit running processes");
    mpLines[MENU_HEAPS]->setStringDesc("heaps", "see how cooked we are");
    mpLines[MENU_INVENTORY]->setStringDesc("inventory", "set items and equipment");
    mpLines[MENU_MEMORY]->setStringDesc("memory", "view and edit memory");
    mpLines[MENU_PRACTICE]->setStringDesc("practice", "load practice files");
    mpLines[MENU_SCENE]->setStringDesc("scene", "adjust current scene settings");
    mpLines[MENU_SETTINGS]->setStringDesc("settings", "configure tpgz settings");
    mpLines[MENU_TOOLS]->setStringDesc("tools", "use various tools for practice and testing");
    mpLines[MENU_WARPING]->setStringDesc("warping", "warp to any area");

    // mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    // mpDrawCursor->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    // mpDrawCursor->setAlphaRate(1.0f);

    // mpMeterHaihai = new dMeterHaihai_c(3);

    mTransitioning = false;
    mTransitionForward = true;
    mTransitionStart = 0;
    mTransitionDuration = 5.0f;

    mXPos = mMainVisibleX = mSubVisibleX = g_gzInfo.mBackgroundXPos + 15.0f;
    mMainHiddenX = mXPos - 160.0f;
    mSubHiddenX = mXPos + 180.0f;
}


gzMainMenu_c::~gzMainMenu_c() {
    _delete();
}

void gzMainMenu_c::_delete() {
    OSReport("deleting gzMainMenu_c\n");

    for (int i = 0; i < LINE_NUM; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;

        delete mpMenus[i];
        mpMenus[i] = NULL;
    }
}

void gzMainMenu_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigDown()) {
        l_cursor->y = (l_cursor->y + 1) % LINE_NUM;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
        gzChangeMenu(mpMenus[l_cursor->y]);
    }

    if (gzPad::getTrigUp()) {
        l_cursor->y = (l_cursor->y - 1 + LINE_NUM) % LINE_NUM;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
        gzChangeMenu(mpMenus[l_cursor->y]);
    }

    if (gzPad::getTrigB()) {
        g_gzInfo.mDisplay = false;
        return;
    }

    if (gzPad::getTrigA()) {
        if (g_gzInfo.mpCurrentMenu != NULL) {
            startForwardTransition();
            l_cursor->x++;
            l_cursor->y = 0;
            g_gzInfo.mInputWaitTimer = 2;
            gzInfo_seStart(Z2SE_SY_EXP_WIN_OPEN);
        }

        switch (l_cursor->y) {
        case MENU_WARPING:
            scene_class* playScene = fopScnM_SearchByID(dStage_roomControl_c::getProcID());
            if (playScene != NULL)
                fopScnM_ChangeReq(playScene, PROC_MENU_SCENE, 0, 5);
            g_gzInfo.mDisplay = false;
            return;
        }
    }
}

void gzMainMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();

    static const f32 Y_ALIGNMENT = 100.0f;
    static const f32 LINE_SPACING = 22.0f;
    static const f32 DESCRIPTION_X = 0.0f;

    u32 cursor_color = gzInfo_getCursorColor();

    if (mTransitioning) {
        u32 currentFrame = cCt_getFrameCount();
        f32 age = (f32)(currentFrame - mTransitionStart);

        if (age >= mTransitionDuration) {
            mTransitioning = false;
            mXPos = mMainEndX;
            if (g_gzInfo.mpCurrentMenu != NULL) {
                g_gzInfo.mpCurrentMenu->setXPos(mSubEndX);
            }
        } else {
            mXPos = calcSlidePosition(currentFrame, mTransitionStart, mMainStartX, mMainEndX, mTransitionDuration);

            if (g_gzInfo.mpCurrentMenu != NULL)
                g_gzInfo.mpCurrentMenu->setXPos(calcSlidePosition(currentFrame, mTransitionStart, mSubStartX, mSubEndX, mTransitionDuration));
        }

        for (int i = 0; i < LINE_NUM; i++) {
            if (mpLines[i] != NULL && mXPos >= g_gzInfo.mBackgroundXPos) {
                f32 y_pos = Y_ALIGNMENT + ((i - 1) * LINE_SPACING);
                u32 color = (l_cursor->y == i && gzInfo_isMainMenuVisible()) ? gzInfo_getTextColor() : COLOR_WHITE;
                mpLines[i]->draw(mXPos, y_pos, color);
            }
        }
    } else {
        for (int i = 0; i < LINE_NUM; i++) {
            if (mpLines[i] != NULL) {
                f32 y_pos = Y_ALIGNMENT + ((i - 1) * LINE_SPACING);

                if (l_cursor->y == i && gzInfo_isMainMenuVisible()) {
                    mpLines[i]->draw(mXPos, y_pos, gzInfo_getTextColor());
                } else {
                    mpLines[i]->draw(mXPos, y_pos, COLOR_WHITE);
                }
            }
        }
    }

    // Draw description if valid and on menu
    if (gzInfo_isMainMenuVisible()) {
        if (mpLines[l_cursor->y] && *mpLines[l_cursor->y]->m_description != 0) {
            f32 description_x = DESCRIPTION_X;
            f32 description_y = g_gzInfo.mBackgroundHeight + 25.0f;

            mpDescription->setString(mpLines[l_cursor->y]->m_description);
            mpDescription->draw(DESCRIPTION_X, description_y, cursor_color, HBIND_CENTER);
        }
    }

    if (gzInfo_isCursorTypeTP()) {
        if (mpDrawCursor != NULL) {
            mpDrawCursor->draw();
        }
    }
}
