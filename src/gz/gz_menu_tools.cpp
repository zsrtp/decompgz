#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_tools.h"
#include "gz/gz_menu_main.h"

static gzOption_s checker_options[] = {
    {"coro td", NULL, gzInfo_isCoroTD, gzInfo_onCoroTD, gzInfo_offCoroTD},
    {"ending blow moon boots", NULL, gzInfo_isEndingBlowMoonBoots, gzInfo_onEndingBlowMoonBoots, gzInfo_offEndingBlowMoonBoots},
    {"elevator escape", NULL, gzInfo_isElevatorEscape, gzInfo_onElevatorEscape, gzInfo_offElevatorEscape},
    {"gorge void", NULL, gzInfo_isGorgeVoid, gzInfo_onGorgeVoid, gzInfo_offGorgeVoid},
    {"ladder freezard cancel", NULL, gzInfo_isLadderFreezardCancel, gzInfo_onLadderFreezardCancel, gzInfo_offLadderFreezardCancel},
    {"rolling", NULL, gzInfo_isRolling, gzInfo_onRolling, gzInfo_offRolling},
    {"universal map delay", NULL, gzInfo_isUniversalMapDelay, gzInfo_onUniversalMapDelay, gzInfo_offUniversalMapDelay}
};

static gzOption_s display_options[] = {
    {"a/b mash rate", NULL, gzInfo_isAbMashRate, gzInfo_onAbMashRate, gzInfo_offAbMashRate},
    {"link debug info", NULL, gzInfo_isLinkDebugInfo, gzInfo_onLinkDebugInfo, gzInfo_offLinkDebugInfo},
    {"in-game timer", NULL, gzInfo_isInGameTimer, gzInfo_onInGameTimer, gzInfo_offInGameTimer},
    {"input viewer", NULL, gzInfo_isInputViewer, gzInfo_onInputViewer, gzInfo_offInputViewer},
    {"load timer", NULL, gzInfo_isLoadTimer, gzInfo_onLoadTimer, gzInfo_offLoadTimer},
    {"stage info", NULL, gzInfo_isStageInfo, gzInfo_onStageInfo, gzInfo_offStageInfo},
    {"timer", NULL, gzInfo_isTimer, gzInfo_onTimer, gzInfo_offTimer}
};

static gzOption_s link_options[] = {
    {"fast bonk recovery", NULL, gzInfo_isFastBonkRecovery, gzInfo_onFastBonkRecovery, gzInfo_offFastBonkRecovery},
    {"fast movement", NULL, gzInfo_isFastMovement, gzInfo_onFastMovement, gzInfo_offFastMovement},
    {"no sinking in sand", NULL, gzInfo_isNoSinkingInSand, gzInfo_onNoSinkingInSand, gzInfo_offNoSinkingInSand},
    {"teleport", NULL, gzInfo_isTeleport, gzInfo_onTeleport, gzInfo_offTeleport},
    {"displacement", NULL, gzInfo_isDisplacement, gzInfo_onDisplacement, gzInfo_offDisplacement},
    {"move link", "move link around freely. L+R+Y to activate", gzInfo_isMoveLink, gzInfo_onMoveLink, gzInfo_offMoveLink}
};

gzToolsMenu_c::gzToolsMenu_c() {
    OSReport("creating gzToolsMenu_c\n");

    for (int i = 0; i < TAB_MAX_e; i++) {
        mpTabHeaders[i] = new gzTextBox();
        mpTabHeaders[i]->setFontSize(15.0f,15.0f);
    }

    mpTabHeaders[TAB_CHECKERS_e]->setString("checkers");
    mpTabHeaders[TAB_DISPLAYS_e]->setString("displays");
    mpTabHeaders[TAB_LINK_e]->setString("link");

    mTabs[TAB_CHECKERS_e].mOptions = checker_options;
    mTabs[TAB_CHECKERS_e].mMax = sizeof(checker_options) / sizeof(gzOption_s);
    mTabs[TAB_CHECKERS_e].create();

    mTabs[TAB_DISPLAYS_e].mOptions = display_options;
    mTabs[TAB_DISPLAYS_e].mMax = sizeof(display_options) / sizeof(gzOption_s);
    mTabs[TAB_DISPLAYS_e].create();

    mTabs[TAB_LINK_e].mOptions = link_options;
    mTabs[TAB_LINK_e].mMax = sizeof(link_options) / sizeof(gzOption_s);
    mTabs[TAB_LINK_e].create();

    mpDescription = new gzTextBox();

    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpDrawCursor->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    mpDrawCursor->setAlphaRate(1.0f);

    mpMeterHaihai = new dMeterHaihai_c(3);

    mTopLine = 0;
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

    delete mpDescription;
    mpDescription = NULL;

    delete mpDrawCursor;
    mpDrawCursor = NULL;

    delete mpMeterHaihai;
    mpMeterHaihai = NULL;
}

void gzToolsMenu_c::execute() {
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return;
    }

    gzCursor* l_cursor = gzInfo_getCursor();
    gzTab_c& curTab = mTabs[mCurrentTab];
    int current_max_line = curTab.mMax;

    if (curTab.execute() == 0) return;

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
            curTab.mOptionToggle = false;
            gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
        } else {
            l_cursor->x--;
            l_cursor->y = gzMainMenu_c::MENU_TOOLS;
            gzInfo_seStart(Z2SE_SY_EXP_WIN_CLOSE);
            g_gzInfo.mpMainMenu->startReverseTransition();
            return;
        }
    }

    mpMeterHaihai->_execute(0);

    if (current_max_line == 0)
        return;

    if (!curTab.mOptionToggle) {
        if (gzPad::getTrigDown()) {
            l_cursor->y = (l_cursor->y + 1) % current_max_line;
            gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
        }

        if (gzPad::getTrigUp()) {
            l_cursor->y = (l_cursor->y - 1 + current_max_line) % current_max_line;
            gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
        }
    }
}

void gzToolsMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();
    gzTab_c& curTab = mTabs[mCurrentTab];

    static const f32 LINE_SPACING = 22.0f;
    static const f32 OPTIONS_X_OFFSET = -20.0f;
    static const f32 HAIHAI_X_OFFSET = 305.0f;
    static const f32 HAIHAI_EXTRA_SPACING = 30.0f;
    static const f32 HAIHAI_Y_OFFSET = -7.0f;
    static const f32 HAIHAI_SCALE_FACTOR = 0.04f;
    static const f32 TAB_HEADER_OFFSET = 15.0f;
    static const f32 TP_CURSOR_X_OFFSET = 20.0f;
    static const int VISIBLE_LINES = 15;

    curTab.updateDynamicLines();

    f32 X_POS[TAB_MAX_e];
    f32 tab_header_x_alignment = mXPos + TAB_HEADER_OFFSET;
    X_POS[TAB_CHECKERS_e] = tab_header_x_alignment;
    X_POS[TAB_DISPLAYS_e] = tab_header_x_alignment + 70.0f;
    X_POS[TAB_LINK_e] = tab_header_x_alignment + 140.0f;

    u32 cursor_color = gzInfo_getCursorColor();

    f32 y_header_alignment = g_gzInfo.mBackgroundYPos + 48.0f;
    f32 y_lines_alignment = y_header_alignment + 42.0f;

    int current_max_line = curTab.mMax;
    gzTextBox** currentLines = curTab.mpLines;
    gzTextBox** currentLineOptions = curTab.mpLineOptions;

    // Draw tab headers
    for (int i = 0; i < TAB_MAX_e; i++) {
        // only draw if it doesnt go past the bounds of the menu
        // TODO: fetch this magic number from gzInfo instead
        if (X_POS[i] <= 550.0f) mpTabHeaders[i]->draw(X_POS[i], y_header_alignment, i == mCurrentTab ? cursor_color : COLOR_WHITE);
    }

    if (l_cursor->y < mTopLine) {
        mTopLine = l_cursor->y;
    } else if (l_cursor->y >= mTopLine + VISIBLE_LINES) {
        mTopLine = l_cursor->y - VISIBLE_LINES + 1;
    }

    // Clamp mTopLine to valid range
    int maxTop = current_max_line - VISIBLE_LINES;
    if (maxTop < 0) maxTop = 0;
    if (mTopLine > maxTop) mTopLine = maxTop;
    if (mTopLine < 0) mTopLine = 0;

    J2DTextBox::TFontSize font_size;
    if (currentLineOptions[0] != NULL) {
        currentLineOptions[0]->getFontSize(font_size);  // assume all have same font size
        mpMeterHaihai->setScale(font_size.mSizeY * HAIHAI_SCALE_FACTOR);
    }

    f32 x_alignment_opts = mXPos + OPTIONS_X_OFFSET;
    f32 x_alignment_haihai = x_alignment_opts + HAIHAI_X_OFFSET;
    f32 x_alignment_tp_cursor = mXPos + TP_CURSOR_X_OFFSET;

    for (int screenIdx = 0; screenIdx < VISIBLE_LINES; screenIdx++) {
        int lineIdx = mTopLine + screenIdx;
        if (lineIdx >= current_max_line) break;

        if (currentLines[lineIdx] != NULL) {
            f32 y_pos = y_lines_alignment + ((screenIdx - 1) * LINE_SPACING);
            f32 y_pos_haihai = y_pos + HAIHAI_Y_OFFSET;

            if (l_cursor->y == lineIdx && gzInfo_isSubMenuVisible()) {
                currentLines[lineIdx]->draw(mXPos, y_pos, cursor_color);
                currentLineOptions[lineIdx]->draw(x_alignment_opts, y_pos, cursor_color, HBIND_CENTER);

                if (curTab.mOptionToggle && currentLineOptions[lineIdx]->mStringLength != 0) {
                    bool is_on = curTab.mOptions[lineIdx].is();
                    u8 flags = is_on ? mpMeterHaihai->DIR_LEFT_e : mpMeterHaihai->DIR_RIGHT_e;
                    f32 x_size_haihai = currentLineOptions[lineIdx]->mBounds.f.x + HAIHAI_EXTRA_SPACING;
                    mpMeterHaihai->drawHaihai(flags, x_alignment_haihai, y_pos_haihai, x_size_haihai, 0.0f);
                }

                if (gzInfo_isCursorTypeTP()) {
                    mpDrawCursor->setPos(x_alignment_tp_cursor, y_pos, (J2DPane*)currentLines[lineIdx], false);
                }
            } else {
                currentLines[lineIdx]->draw(mXPos, y_pos, COLOR_WHITE);
                currentLineOptions[lineIdx]->draw(x_alignment_opts, y_pos, COLOR_WHITE, HBIND_CENTER);
            }
        }
    }

    // Draw description if valid and on menu
    if (gzInfo_isSubMenuVisible()) {
        if (currentLines[l_cursor->y] && *currentLines[l_cursor->y]->m_description != 0) {
            f32 description_y = g_gzInfo.mBackgroundHeight + 25.0f;

            mpDescription->setString(currentLines[l_cursor->y]->m_description);
            mpDescription->draw(0.0f, description_y, cursor_color, HBIND_CENTER);
        }
    }

    if (gzInfo_isCursorTypeTP()) {
        mpDrawCursor->draw();
    }
}
