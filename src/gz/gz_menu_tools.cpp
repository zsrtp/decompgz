#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_tools.h"
#include "gz/gz_menu_main.h"

// Checkers tab
static gzBoolOption_s checkerFlags[] = {
    {"coro td", "show frame info when doing coro td", gzInfo_isCoroTD, gzInfo_onCoroTD, gzInfo_offCoroTD},
    {"ebmb", "show frame info for ending blow moon boots", gzInfo_isEndingBlowMoonBoots, gzInfo_onEndingBlowMoonBoots, gzInfo_offEndingBlowMoonBoots},
    {"elevator escape", "show frame info when doing elevator escape", gzInfo_isElevatorEscape, gzInfo_onElevatorEscape, gzInfo_offElevatorEscape},
    {"gorge void", "warp to kakariko gorge", gzInfo_isGorgeVoid, gzInfo_onGorgeVoid, gzInfo_offGorgeVoid},
    {"ladder freezard cancel", "ladder freezard cancel checker", gzInfo_isLadderFreezardCancel, gzInfo_onLadderFreezardCancel, gzInfo_offLadderFreezardCancel},
    {"rolling", "frame counter for chaining rolls", gzInfo_isRolling, gzInfo_onRolling, gzInfo_offRolling},
    {"universal map delay", "practice snowpeak universal map delay timing", gzInfo_isUniversalMapDelay, gzInfo_onUniversalMapDelay, gzInfo_offUniversalMapDelay},
};

// Displays tab
static gzBoolOption_s displayFlags[] = {
    {"a/b mash rate", "display a/b button mashing speeds", gzInfo_isAbMashRate, gzInfo_onAbMashRate, gzInfo_offAbMashRate},
    {"in-game timer", "in-game time timer", gzInfo_isInGameTimer, gzInfo_onInGameTimer, gzInfo_offInGameTimer},
    {"input viewer", "show current inputs", gzInfo_isInputViewer, gzInfo_onInputViewer, gzInfo_offInputViewer},
    {"link debug info", "show link's position, angle, and speed", gzInfo_isLinkDebugInfo, gzInfo_onLinkDebugInfo, gzInfo_offLinkDebugInfo},
    {"load timer", "loading zone timer", gzInfo_isLoadTimer, gzInfo_onLoadTimer, gzInfo_offLoadTimer},
    {"stage info", "show link's current stage info", gzInfo_isStageInfo, gzInfo_onStageInfo, gzInfo_offStageInfo},
    {"timer", "frame timer", gzInfo_isTimer, gzInfo_onTimer, gzInfo_offTimer},
};

// Link tab
static gzBoolOption_s linkFlags[] = {
    {"fast bonk recovery", "reduces bonk animation significantly", gzInfo_isFastBonkRecovery, gzInfo_onFastBonkRecovery, gzInfo_offFastBonkRecovery},
    {"fast movement", "link's movement is much faster", gzInfo_isFastMovement, gzInfo_onFastMovement, gzInfo_offFastMovement},
    {"no sinking in sand", "link won't sink in sand", gzInfo_isNoSinkingInSand, gzInfo_onNoSinkingInSand, gzInfo_offNoSinkingInSand},
    {"move link", "move link around freely", gzInfo_isMoveLink, gzInfo_onMoveLink, gzInfo_offMoveLink},
    {"teleport", "store and load link's position", gzInfo_isTeleport, gzInfo_onTeleport, gzInfo_offTeleport},
};

gzToolsMenu_c::gzToolsMenu_c() {
    OSReport("creating gzToolsMenu_c\n");
    mXPos = g_gzInfo.mBackgroundXPos + 195.0f;

    for (int i = 0; i < TAB_MAX_e; i++) {
        mpTabHeaders[i] = gzTextBox_allocate();
        mpTabHeaders[i]->setFontSize(15.0f, 15.0f);
    }

    mpTabHeaders[TAB_CHECKERS_e]->setString("checkers");
    mpTabHeaders[TAB_DISPLAYS_e]->setString("displays");
    mpTabHeaders[TAB_LINK_e]->setString("link");

    // Checkers tab
    for (int i = 0; i < C_MAX; i++) {
        mpLinesCheckers[i] = new gzBoolOptionLine(checkerFlags[i].name, checkerFlags[i].desc,
                                                   checkerFlags[i].is, checkerFlags[i].on, checkerFlags[i].off);
    }

    // Displays tab
    for (int i = 0; i < D_MAX; i++) {
        mpLinesDisplays[i] = new gzBoolOptionLine(displayFlags[i].name, displayFlags[i].desc,
                                                   displayFlags[i].is, displayFlags[i].on, displayFlags[i].off);
    }

    // Link tab
    for (int i = 0; i < L_MAX; i++) {
        mpLinesLink[i] = new gzBoolOptionLine(linkFlags[i].name, linkFlags[i].desc,
                                               linkFlags[i].is, linkFlags[i].on, linkFlags[i].off);
    }

    mCurrentTab = TAB_CHECKERS_e;
    gzInfo_resetTopLine();
    gzInfo_offMenuOption();
}

gzToolsMenu_c::~gzToolsMenu_c() {
    _delete();
}

void gzToolsMenu_c::_delete() {
    OSReport("deleting gzToolsMenu_c\n");

    for (int i = 0; i < TAB_MAX_e; i++) {
        gzTextBox_free(mpTabHeaders[i]);
        mpTabHeaders[i] = NULL;
    }
    for (int i = 0; i < C_MAX; i++) {
        delete mpLinesCheckers[i];
        mpLinesCheckers[i] = NULL;
    }
    for (int i = 0; i < D_MAX; i++) {
        delete mpLinesDisplays[i];
        mpLinesDisplays[i] = NULL;
    }
    for (int i = 0; i < L_MAX; i++) {
        delete mpLinesLink[i];
        mpLinesLink[i] = NULL;
    }
}

u8 gzToolsMenu_c::getHaihaiFlags(int idx) {
    u8 haihai_flags = ARROW_LEFT | ARROW_RIGHT;
    gzBoolOption_s* flags = NULL;

    switch (mCurrentTab) {
    case TAB_CHECKERS_e:
        flags = checkerFlags;
        break;
    case TAB_DISPLAYS_e:
        flags = displayFlags;
        break;
    case TAB_LINK_e:
        flags = linkFlags;
        break;
    default:
        return 0;
    }

    if (flags[idx].is()) {
        haihai_flags &= ~ARROW_RIGHT;
    } else {
        haihai_flags &= ~ARROW_LEFT;
    }
    return haihai_flags;
}

int gzToolsMenu_c::getCurrentLineNum() {
    switch (mCurrentTab) {
    case TAB_CHECKERS_e:
        return C_MAX;
    case TAB_DISPLAYS_e:
        return D_MAX;
    case TAB_LINK_e:
        return L_MAX;
    }
    return 0;
}

void gzToolsMenu_c::updateDynamicLines() {
    gzBoolOptionLine** currentLines;
    gzBoolOption_s* flags;
    int currentLineNum;

    switch (mCurrentTab) {
    case TAB_CHECKERS_e:
        currentLines = mpLinesCheckers;
        flags = checkerFlags;
        currentLineNum = C_MAX;
        break;
    case TAB_DISPLAYS_e:
        currentLines = mpLinesDisplays;
        flags = displayFlags;
        currentLineNum = D_MAX;
        break;
    case TAB_LINK_e:
        currentLines = mpLinesLink;
        flags = linkFlags;
        currentLineNum = L_MAX;
        break;
    default:
        return;
    }

    for (int i = 0; i < currentLineNum; i++) {
        gzTextBox* opt = currentLines[i]->getOptionBox();
        if (opt) opt->setStringf("%s", flags[i].is() ? "on" : "off");
    }
    updateLineBounds((gzLine**)currentLines, currentLineNum);
}

void gzToolsMenu_c::execute() {
    if (checkInputWait()) return;
    if (handleBackButton(gzMainMenu_c::MENU_TOOLS)) return;

    gzCursor* l_cursor = gzInfo_getCursor();
    gzBoolOption_s* flags = NULL;
    int maxIdx = 0;

    switch (mCurrentTab) {
    case TAB_CHECKERS_e:
        flags = checkerFlags;
        maxIdx = C_MAX;
        break;
    case TAB_DISPLAYS_e:
        flags = displayFlags;
        maxIdx = D_MAX;
        break;
    case TAB_LINK_e:
        flags = linkFlags;
        maxIdx = L_MAX;
        break;
    }

    if (!gzInfo_isMenuOption()) {
        if (gzPad::getTrigRight()) {
            mCurrentTab = (mCurrentTab + 1) % TAB_MAX_e;
            l_cursor->y = 0;
            gzInfo_resetTopLine();
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }
        if (gzPad::getTrigLeft()) {
            mCurrentTab = (mCurrentTab - 1 + TAB_MAX_e) % TAB_MAX_e;
            l_cursor->y = 0;
            gzInfo_resetTopLine();
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }
    } else {
        if (gzPad::getTrigRight()) {
            if (l_cursor->y < maxIdx && !flags[l_cursor->y].is()) {
                flags[l_cursor->y].on();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
        }
        if (gzPad::getTrigLeft()) {
            if (l_cursor->y < maxIdx && flags[l_cursor->y].is()) {
                flags[l_cursor->y].off();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
        }
    }

    if (gzPad::getTrigA()) {
        gzInfo_toggleMenuOption();
    }

    handleNavigation(maxIdx);
    finishExecute(maxIdx);
}

void gzToolsMenu_c::draw() {
    static const f32 TAB_HEADER_OFFSET = 15.0f;

    // Set up tab header x positions
    f32 tabXPositions[TAB_MAX_e];
    f32 tabBaseX = mXPos + TAB_HEADER_OFFSET;
    tabXPositions[TAB_CHECKERS_e] = tabBaseX;
    tabXPositions[TAB_DISPLAYS_e] = tabBaseX + 80.0f;
    tabXPositions[TAB_LINK_e] = tabBaseX + 160.0f;

    updateDynamicLines();

    // Get current tab's lines
    gzLine** currentLines;
    int currentLineNum;
    switch (mCurrentTab) {
    case TAB_CHECKERS_e:
        currentLines = (gzLine**)mpLinesCheckers;
        currentLineNum = C_MAX;
        break;
    case TAB_DISPLAYS_e:
        currentLines = (gzLine**)mpLinesDisplays;
        currentLineNum = D_MAX;
        break;
    case TAB_LINK_e:
        currentLines = (gzLine**)mpLinesLink;
        currentLineNum = L_MAX;
        break;
    }

    // Draw tab headers
    f32 yHeader = g_gzInfo.mBackgroundYPos + gzMenuLayout::TAB_HEADER_Y_OFFSET;
    drawTabHeaders(mpTabHeaders, tabXPositions, TAB_MAX_e, mCurrentTab, yHeader, gzInfo_getCursorColor());

    // Draw lines with per-line haihai flags
    s32 topLine = gzInfo_getTopLine();
    drawLinesWithHaihai(currentLines, currentLineNum, topLine, gzMenuLayout::VISIBLE_LINES);
}
