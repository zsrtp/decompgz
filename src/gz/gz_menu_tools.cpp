#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_tools.h"
#include "gz/gz_menu_main.h"
#include "d/d_select_cursor.h"

// Checkers tab
static gzBoolOption_s checkerFlags[] = {
    {"link debug info", "Display link position and angle info", gzInfo_isLinkDebugInfo, gzInfo_onLinkDebugInfo, gzInfo_offLinkDebugInfo},
    {"stage info", "Display current stage information", gzInfo_isStageInfo, gzInfo_onStageInfo, gzInfo_offStageInfo},
};

// Displays tab
static gzBoolOption_s displayFlags[] = {
    {"timer", "Toggle timer display", gzInfo_isTimer, gzInfo_onTimer, gzInfo_offTimer},
    {"in-game timer", "Toggle in-game timer display", gzInfo_isInGameTimer, gzInfo_onInGameTimer, gzInfo_offInGameTimer},
    {"load timer", "Toggle load timer display", gzInfo_isLoadTimer, gzInfo_onLoadTimer, gzInfo_offLoadTimer},
    {"input viewer", "Toggle input viewer display", gzInfo_isInputViewer, gzInfo_onInputViewer, gzInfo_offInputViewer},
    {"disable item timer", "Disable item timer display", gzInfo_isDisableItemTimer, gzInfo_onDisableItemTimer, gzInfo_offDisableItemTimer},
};

// Link tab
static gzBoolOption_s linkFlags[] = {
    {"move link", "Enable free movement of Link", gzInfo_isMoveLink, gzInfo_onMoveLink, gzInfo_offMoveLink},
    {"fast movement", "Enable fast movement speed", gzInfo_isFastMovement, gzInfo_onFastMovement, gzInfo_offFastMovement},
    {"fast bonk recovery", "Enable fast bonk recovery", gzInfo_isFastBonkRecovery, gzInfo_onFastBonkRecovery, gzInfo_offFastBonkRecovery},
    {"rolling", "Enable infinite rolling", gzInfo_isRolling, gzInfo_onRolling, gzInfo_offRolling},
    {"moon jump", "Enable moon jump", gzInfo_isMoonJump, gzInfo_onMoonJump, gzInfo_offMoonJump},
    {"super clawshot", "Enable super clawshot distance", gzInfo_isSuperClawshot, gzInfo_onSuperClawshot, gzInfo_offSuperClawshot},
    {"transform anywhere", "Enable transform anywhere", gzInfo_isTransformAnywhere, gzInfo_onTransformAnywhere, gzInfo_offTransformAnywhere},
    {"no sinking in sand", "Disable sinking in sand", gzInfo_isNoSinkingInSand, gzInfo_onNoSinkingInSand, gzInfo_offNoSinkingInSand},
    {"disable walls", "Disable wall collision", gzInfo_isDisableWalls, gzInfo_onDisableWalls, gzInfo_offDisableWalls},
    {"teleport", "Enable teleport functionality", gzInfo_isTeleport, gzInfo_onTeleport, gzInfo_offTeleport},
    {"ab mash rate", "Toggle A/B mash rate display", gzInfo_isAbMashRate, gzInfo_onAbMashRate, gzInfo_offAbMashRate},
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

    mpMeterHaihai = new dMeterHaihai_c(3);
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
    delete mpMeterHaihai;
    mpMeterHaihai = NULL;
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

    J2DTextBox::TFontSize font_size;
    for (int i = 0; i < currentLineNum; i++) {
        gzTextBox* opt = currentLines[i]->getOptionBox();
        if (opt) {
            opt->getFontSize(font_size);
            font_size.mSizeX *= 0.5f;
            currentLines[i]->mText->mBounds.f.x = currentLines[i]->mText->mStringLength * font_size.mSizeX;
            opt->mBounds.f.x = opt->mStringLength * font_size.mSizeX;
        }
    }
}

void gzToolsMenu_c::execute() {
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return;
    }

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
        if (gzPad::getTrigDown()) {
            l_cursor->y = (l_cursor->y + 1) % maxIdx;
            gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
        }
        if (gzPad::getTrigUp()) {
            l_cursor->y = (l_cursor->y == 0) ? maxIdx - 1 : l_cursor->y - 1;
            gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
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

    if (gzPad::getTrigB()) {
        if (gzInfo_isMenuOption()) {
            gzInfo_offMenuOption();
            gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
        } else {
            l_cursor->x--;
            l_cursor->y = gzMainMenu_c::MENU_TOOLS;
            gzInfo_seStart(Z2SE_SY_EXP_WIN_CLOSE);
            g_gzInfo.mpMainMenu->startReverseTransition();
            return;
        }
    }

    if (gzPad::getTrigA()) {
        gzInfo_setMenuOption(!gzInfo_isMenuOption());
        if (gzInfo_isMenuOption()) {
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK);
        } else {
            gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
        }
    }

    updateScrolling(maxIdx);
    mpHaihai->_execute(0);
}

void gzToolsMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();
    static const f32 Y_ALIGNMENT = 78.0f;
    static const f32 OPTIONS_X_OFFSET = -20.0f;
    static const f32 HAIHAI_X_OFFSET = 305.0f;
    static const f32 HAIHAI_Y_OFFSET = -7.0f;
    static const f32 HAIHAI_SCALE_FACTOR = 0.04f;
    static const f32 HAIHAI_EXTRA_SPACING = 30.0f;
    static const f32 TP_CURSOR_X_OFFSET = 20.0f;
    static const f32 LINE_SPACING = 22.0f;
    static const f32 DESCRIPTION_X = 0.0f;
    static const int VISIBLE_LINES = 15;
    static const f32 TAB_HEADER_OFFSET = 15.0f;

    f32 X_POS[TAB_MAX_e];
    f32 tab_header_x_alignment = mXPos + TAB_HEADER_OFFSET;
    X_POS[TAB_CHECKERS_e] = tab_header_x_alignment;
    X_POS[TAB_DISPLAYS_e] = tab_header_x_alignment + 80.0f;
    X_POS[TAB_LINK_e] = tab_header_x_alignment + 160.0f;

    updateDynamicLines();

    J2DTextBox::TFontSize font_size;
    gzTextBox* firstOpt = mpLinesCheckers[0]->getOptionBox();
    if (firstOpt) {
        firstOpt->getFontSize(font_size);
        mpMeterHaihai->setScale(font_size.mSizeY * HAIHAI_SCALE_FACTOR);
    }

    u32 cursor_color = gzInfo_getCursorColor();
    f32 y_header_alignment = g_gzInfo.mBackgroundYPos + 48.0f;
    f32 x_alignment_opts = mXPos + OPTIONS_X_OFFSET;
    f32 x_alignment_haihai = x_alignment_opts + HAIHAI_X_OFFSET;
    f32 x_alignment_tp_cursor = mXPos + TP_CURSOR_X_OFFSET;

    gzBoolOptionLine** currentLines;
    int currentLineNum;
    switch (mCurrentTab) {
    case TAB_CHECKERS_e:
        currentLines = mpLinesCheckers;
        currentLineNum = C_MAX;
        break;
    case TAB_DISPLAYS_e:
        currentLines = mpLinesDisplays;
        currentLineNum = D_MAX;
        break;
    case TAB_LINK_e:
        currentLines = mpLinesLink;
        currentLineNum = L_MAX;
        break;
    }

    for (int i = 0; i < TAB_MAX_e; i++) {
        mpTabHeaders[i]->draw(X_POS[i], y_header_alignment,
                              i == mCurrentTab ? cursor_color : COLOR_WHITE);
    }

    s32 topLine = gzInfo_getTopLine();
    if (l_cursor->y < topLine) {
        topLine = l_cursor->y;
    } else if (l_cursor->y >= topLine + VISIBLE_LINES) {
        topLine = l_cursor->y - VISIBLE_LINES + 1;
    }
    int maxTop = currentLineNum - VISIBLE_LINES;
    if (maxTop < 0)
        maxTop = 0;
    if (topLine > maxTop)
        topLine = maxTop;
    if (topLine < 0)
        topLine = 0;
    gzInfo_setTopLine(topLine);

    for (int screenIdx = 0; screenIdx < VISIBLE_LINES; screenIdx++) {
        int lineIdx = topLine + screenIdx;
        if (lineIdx >= currentLineNum)
            break;
        f32 y_pos = Y_ALIGNMENT + (screenIdx * LINE_SPACING);
        gzTextBox* opt = currentLines[lineIdx]->getOptionBox();
        if (l_cursor->y == lineIdx && gzInfo_isSubMenuVisible()) {
            currentLines[lineIdx]->draw(mXPos, y_pos, cursor_color);
            if (opt) {
                f32 x_size_haihai = opt->mBounds.f.x + HAIHAI_EXTRA_SPACING;
                if (gzInfo_isMenuOption()) {
                    mpMeterHaihai->drawHaihai(getHaihaiFlags(lineIdx), x_alignment_haihai,
                                              y_pos + HAIHAI_Y_OFFSET, x_size_haihai, 0.0f);
                }
                opt->draw(x_alignment_opts, y_pos, cursor_color, HBIND_CENTER);
            }
            gzInfo_getTPCursor()->setPos(x_alignment_tp_cursor, y_pos - 10.0f,
                                 (J2DPane*)currentLines[lineIdx]->mText, false);
        } else {
            currentLines[lineIdx]->draw(mXPos, y_pos, COLOR_WHITE);
            if (opt) {
                opt->draw(x_alignment_opts, y_pos, COLOR_WHITE, HBIND_CENTER);
            }
        }
    }

    if (gzInfo_isSubMenuVisible()) {
        if (currentLines[l_cursor->y] && currentLines[l_cursor->y]->m_description[0] != 0) {
            f32 description_y = g_gzInfo.mBackgroundHeight + 25.0f;
            gzInfo_getMenuDescription()->setString(currentLines[l_cursor->y]->m_description);
            gzInfo_getMenuDescription()->draw(DESCRIPTION_X, description_y, cursor_color, HBIND_CENTER);
        }
    }

    if (gzInfo_isCursorTypeTP()) {
        if (gzInfo_getTPCursor() != NULL) {
            gzInfo_getTPCursor()->draw();
        }
    }
}
