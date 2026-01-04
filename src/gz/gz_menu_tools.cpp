#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_tools.h"
#include "gz/gz_menu_main.h"

static gzBoolOption_s checker_options[] = {
    {"coro td", NULL, gzInfo_isCoroTD, gzInfo_onCoroTD, gzInfo_offCoroTD},
    {"ending blow moon boots", NULL, gzInfo_isEndingBlowMoonBoots, gzInfo_onEndingBlowMoonBoots, gzInfo_offEndingBlowMoonBoots},
    {"elevator escape", NULL, gzInfo_isElevatorEscape, gzInfo_onElevatorEscape, gzInfo_offElevatorEscape},
    {"gorge void", NULL, gzInfo_isGorgeVoid, gzInfo_onGorgeVoid, gzInfo_offGorgeVoid},
    {"ladder freezard cancel", NULL, gzInfo_isLadderFreezardCancel, gzInfo_onLadderFreezardCancel, gzInfo_offLadderFreezardCancel},
    {"rolling", NULL, gzInfo_isRolling, gzInfo_onRolling, gzInfo_offRolling},
    {"universal map delay", NULL, gzInfo_isUniversalMapDelay, gzInfo_onUniversalMapDelay, gzInfo_offUniversalMapDelay}
};

static gzBoolOption_s display_options[] = {
    {"a/b mash rate", NULL, gzInfo_isAbMashRate, gzInfo_onAbMashRate, gzInfo_offAbMashRate},
    {"link debug info", NULL, gzInfo_isLinkDebugInfo, gzInfo_onLinkDebugInfo, gzInfo_offLinkDebugInfo},
    {"in-game timer", NULL, gzInfo_isInGameTimer, gzInfo_onInGameTimer, gzInfo_offInGameTimer},
    {"input viewer", NULL, gzInfo_isInputViewer, gzInfo_onInputViewer, gzInfo_offInputViewer},
    {"load timer", NULL, gzInfo_isLoadTimer, gzInfo_onLoadTimer, gzInfo_offLoadTimer},
    {"stage info", NULL, gzInfo_isStageInfo, gzInfo_onStageInfo, gzInfo_offStageInfo},
    {"timer", NULL, gzInfo_isTimer, gzInfo_onTimer, gzInfo_offTimer}
};

static gzBoolOption_s link_options[] = {
    {"fast bonk recovery", NULL, gzInfo_isFastBonkRecovery, gzInfo_onFastBonkRecovery, gzInfo_offFastBonkRecovery},
    {"fast movement", NULL, gzInfo_isFastMovement, gzInfo_onFastMovement, gzInfo_offFastMovement},
    {"no sinking in sand", NULL, gzInfo_isNoSinkingInSand, gzInfo_onNoSinkingInSand, gzInfo_offNoSinkingInSand},
    {"teleport", NULL, gzInfo_isTeleport, gzInfo_onTeleport, gzInfo_offTeleport},
    {"displacement", NULL, gzInfo_isDisplacement, gzInfo_onDisplacement, gzInfo_offDisplacement},
    {"move link", "move link around freely. L+R+Y to activate", gzInfo_isMoveLink, gzInfo_onMoveLink, gzInfo_offMoveLink}
};

u8 gzToolsMenu_c::getHaihaiFlags(int i) {
    u8 haihai_flags = gzMenu_c::ARROW_LEFT | gzMenu_c::ARROW_RIGHT;

    gzTab_c& curTab = mTabs[mCurrentTab];
    curTab.mBoolOptions[i].is() ? haihai_flags &= ~ARROW_RIGHT : haihai_flags &= ~ARROW_LEFT;

    return haihai_flags;
}

gzToolsMenu_c::gzToolsMenu_c() {
    OSReport("creating gzToolsMenu_c\n");

    for (int i = 0; i < TAB_MAX_e; i++) {
        mpTabHeaders[i] = new gzTextBox();
        mpTabHeaders[i]->setFontSize(15.0f,15.0f);
    }

    mpTabHeaders[TAB_CHECKERS_e]->setString("checkers");
    mpTabHeaders[TAB_DISPLAYS_e]->setString("displays");
    mpTabHeaders[TAB_LINK_e]->setString("link");

    mTabs[TAB_CHECKERS_e].mBoolOptions = checker_options;
    mTabs[TAB_CHECKERS_e].mLineMax = ARRAY_SIZE(checker_options);
    mTabs[TAB_CHECKERS_e].create();

    mTabs[TAB_DISPLAYS_e].mBoolOptions = display_options;
    mTabs[TAB_DISPLAYS_e].mLineMax = ARRAY_SIZE(display_options);
    mTabs[TAB_DISPLAYS_e].create();

    mTabs[TAB_LINK_e].mBoolOptions = link_options;
    mTabs[TAB_LINK_e].mLineMax = ARRAY_SIZE(link_options);
    mTabs[TAB_LINK_e].create();
}

gzToolsMenu_c::~gzToolsMenu_c() {
    OSReport("deleting gzToolsMenu_c\n");

    for (int i = 0; i < TAB_MAX_e; i++) {
        delete mpTabHeaders[i];
        mpTabHeaders[i] = NULL;
    }

    for (int i = 0; i < TAB_MAX_e; i++) {
        mTabs[i]._delete();
    }
}

void gzToolsMenu_c::execute() {
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return;
    }

    gzCursor* l_cursor = gzInfo_getCursor();
    gzTab_c& curTab = mTabs[mCurrentTab];
    mVisibleLines = curTab.mLineMax;

    if (!curTab.mOptionToggle) {
        if (gzPad::getTrigRight()) {
            mCurrentTab = (mCurrentTab + 1) % TAB_MAX_e;
            l_cursor->y = 0;
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }

        if (gzPad::getTrigLeft()) {
            mCurrentTab = (mCurrentTab - 1 + TAB_MAX_e) % TAB_MAX_e;
            l_cursor->y = 0;
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }
    }

    if (gzPad::getTrigB()) {
        if (curTab.mOptionToggle) {
            mOption = false;
            curTab.mOptionToggle = false;
        } else {
            l_cursor->x--;
            l_cursor->y = gzMainMenu_c::MENU_TOOLS;
            gzInfo_seStart(Z2SE_SY_EXP_WIN_CLOSE);
            g_gzInfo.mpMainMenu->startReverseTransition();
            return;
        }
    }

    if (gzPad::getTrigA()) {
        mOption = !mOption;
        curTab.mOptionToggle = !curTab.mOptionToggle;
    }

    curTab.execute();
    gzMenu_c::execute();
}

void gzToolsMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();
    gzTab_c& curTab = mTabs[mCurrentTab];

    curTab.updateDynamicLines();

    // manually set tab header text distances for now
    // need to support scrolling tabs at some point
    f32 X_POS[TAB_MAX_e];
    f32 tab_header_x_alignment = mXPos + 15.0f;
    X_POS[TAB_CHECKERS_e] = tab_header_x_alignment;
    X_POS[TAB_DISPLAYS_e] = tab_header_x_alignment + 70.0f;
    X_POS[TAB_LINK_e] = tab_header_x_alignment + 140.0f;

    u32 cursor_color = gzInfo_getCursorColor();
    f32 y_header_alignment = g_gzInfo.mBackgroundYPos + 48.0f;

    gzTextBox** currentLines = curTab.mpLines;
    gzTextBox** currentLineOptions = curTab.mpLineOptions;

    // Draw tab headers
    for (int i = 0; i < TAB_MAX_e; i++) {
        // only draw if it doesnt go past the bounds of the menu
        if (X_POS[i] <= g_gzInfo.mBackgroundWidth - 45.0f) mpTabHeaders[i]->draw(X_POS[i], y_header_alignment, i == mCurrentTab ? cursor_color : COLOR_WHITE);
    }

    u8 haihai_flags = 0;

    for (int i = 0; i < curTab.mLineMax; i++) {
        if (l_cursor->y == i) 
            haihai_flags = getHaihaiFlags(i);
    }

    drawLines(currentLines, currentLineOptions, haihai_flags, curTab.mLineMax);
}
