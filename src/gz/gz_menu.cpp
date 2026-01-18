#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu.h"
#include "gz/gz_menu_main.h"
#include "d/d_select_cursor.h"

void gzMenu_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();
    s32 visibleLines = gzInfo_getVisibleLines();

    if (gzPad::getTrigDown() && !gzInfo_isMenuOption()) {
        l_cursor->y = (l_cursor->y + 1) % visibleLines;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigUp() && !gzInfo_isMenuOption()) {
        l_cursor->y = (l_cursor->y == 0) ? visibleLines - 1 : l_cursor->y - 1;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigB()) {
        gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
    }

    if (gzPad::getTrigA()) {
        gzInfo_isMenuOption() ? gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK) : gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
    }

    updateScrolling(visibleLines);
    mpHaihai->_execute(0);
}

void gzMenu_c::drawLines(gzTextBox** lines, gzTextBox** lineOptions, u8 haihaiFlags, s32 numLines) {
    gzCursor* l_cursor = gzInfo_getCursor();

    J2DTextBox::TFontSize font_size;
    lines[0]->getFontSize(font_size);
    mpHaihai->setScale(font_size.mSizeY * 0.04f);

    f32 lineX = mXPos;
    f32 lineY_start = g_gzInfo.mBackgroundYPos + 78.0f;
    f32 line_spacing = g_gzInfo.mBackgroundHeight / 20;

    f32 optionX = mXPos - 20.0f;
    f32 haihaiX = optionX + 305.0f;

    u32 cursorColor = gzInfo_getCursorColor();

    for (int i = 0; i < numLines; i++) {
        f32 lineY = lineY_start + ((i - 1) * line_spacing);
        f32 haihaiY = lineY - 7.0f;
        bool isSelected = (l_cursor->y == i && gzInfo_isSubMenuVisible());
        
        f32 haihaiWidth = lineOptions[i]->mBounds.f.x + 30.0f;
        bool showHaihai = gzInfo_isMenuOption() && l_cursor->y == i;

        drawLineWithOption(lines[i], lineOptions[i], lineX, optionX, lineY, isSelected, cursorColor, showHaihai, haihaiFlags, haihaiX, haihaiY, haihaiWidth);
    }
}

void gzMenu_c::drawLines(gzLine** lines, s32 numLines, u8 haihai_flags, s32 topLine, s32 visibleLines) {
    gzCursor* l_cursor = gzInfo_getCursor();
    J2DTextBox::TFontSize font_size;

    if (numLines > 0 && lines[0] != NULL) {
        lines[0]->mText->getFontSize(font_size);
        mpHaihai->setScale(font_size.mSizeY * 0.04f);
    }

    f32 lineX = mXPos;
    f32 lineY_start = g_gzInfo.mBackgroundYPos + 78.0f;
    f32 line_spacing = g_gzInfo.mBackgroundHeight / 20;
    f32 optionX = mXPos - 20.0f;
    f32 haihaiX = optionX + 305.0f;
    u32 cursorColor = gzInfo_getCursorColor();
    s32 endLine = topLine + visibleLines;

    if (endLine > numLines) endLine = numLines;
    for (s32 i = topLine; i < endLine; i++) {
        gzLine* line = lines[i];
        if (line == NULL) continue;
        
        f32 lineY = lineY_start + ((i - topLine) * line_spacing);
        f32 haihaiY = lineY - 7.0f;
        bool isSelected = (l_cursor->y == i && gzInfo_isSubMenuVisible());
        u32 color = isSelected ? cursorColor : COLOR_WHITE;
        f32 haihaiWidth = 0.0f;
        gzTextBox* opt = line->getOptionBox();

        if (opt != NULL && opt->mStringLength != 0) {
            haihaiWidth = opt->mBounds.f.x + 30.0f;
        }

        bool showHaihai = gzInfo_isMenuOption() && l_cursor->y == i && haihai_flags != 0;
        line->mText->draw(lineX, lineY, color);

        if (opt != NULL) {
            opt->draw(optionX, lineY, color, HBIND_CENTER);
        }

        if (showHaihai && mpHaihai != NULL) {
            gzSetup2DContext();
            mpHaihai->drawHaihai(haihai_flags, haihaiX, haihaiY, haihaiWidth, 0.0f);
        }

        if (isSelected && gzInfo_isCursorTypeTP() && gzInfo_getTPCursor() != NULL) {
            line->mText->updateBounds();
            f32 totalWidth;
            f32 leftEdge = lineX;

            // If there's an option box, calculate combined bounds
            f32 optionXAdjust = 0.0f;
            if (opt != NULL && opt->mStringLength != 0) {
                opt->updateBounds();
                // Option is drawn near the haihai area, use haihaiX as reference
                // Account for haihai arrows extending beyond the option text
                f32 rightEdge = haihaiX + (opt->getWidth() / 2.0f) + 40.0f;  // Extra for right arrow
                totalWidth = rightEdge - leftEdge + 30.0f;
                optionXAdjust = -15.0f;  // Shift left to center with arrows
            } else {
                totalWidth = line->mText->getWidth();
            }

            // setPos expects center position
            f32 cursorX = leftEdge + (totalWidth / 2.0f) + gzMenuLayout::TP_CURSOR_X_OFFSET + optionXAdjust;
            f32 cursorY = lineY + (line->mText->getHeight() / 2.0f) + gzMenuLayout::TP_CURSOR_Y_OFFSET;

            // Temporarily set bounds for the combined width
            line->mText->mBounds.f.x = totalWidth;
            gzInfo_getTPCursor()->setPos(cursorX, cursorY, (J2DPane*)line->mText, false);
            gzSetup2DContext();
            gzInfo_getTPCursor()->draw();
        }
    }

    drawDescription((l_cursor->y < numLines && lines[l_cursor->y] != NULL) ? lines[l_cursor->y]->m_description : NULL);
}

// Draws the line, line option, haihai arrows (for option boxes), and TP cursor if enabled
void gzMenu_c::drawLineWithOption(gzTextBox* line, gzTextBox* option, f32 lineX, f32 optionX, f32 lineY, bool isSelected, u32 selectedColor, bool showHaihai, u8 haihaiFlags, f32 haihaiX, f32 haihaiY, f32 haihaiWidth) {
    u32 color = isSelected ? selectedColor : COLOR_WHITE;

    if (line) line->draw(lineX, lineY, color);

    if (option) option->draw(optionX, lineY, color, HBIND_CENTER);

    if (showHaihai && option && option->mStringLength != 0 && haihaiFlags != 0) {
        gzSetup2DContext();
        mpHaihai->drawHaihai(haihaiFlags, haihaiX, haihaiY, haihaiWidth, 0.0f);
    }

    if (isSelected && gzInfo_isCursorTypeTP() && gzInfo_getTPCursor()) {
        line->updateBounds();

        // setPos expects center position, so calculate center of text
        f32 cursorX = lineX + (line->getWidth() / 2.0f) + gzMenuLayout::TP_CURSOR_X_OFFSET;
        f32 cursorY = lineY + (line->getHeight() / 2.0f) + gzMenuLayout::TP_CURSOR_Y_OFFSET;
        gzInfo_getTPCursor()->setPos(cursorX, cursorY, (J2DPane*)line, false);
        gzSetup2DContext();
        gzInfo_getTPCursor()->draw();
    }
}

void gzMenu_c::updateScrolling(s32 maxLines) {
    gzCursor* cursor = gzInfo_getCursor();
    s32 topLine = gzInfo_getTopLine();
    s32 visibleLines = gzInfo_getVisibleLines();

    if (cursor->y < topLine) {
        topLine = cursor->y;
    } else if (cursor->y >= topLine + visibleLines) {
        topLine = cursor->y - visibleLines + 1;
    }

    s32 maxTop = maxLines - visibleLines;
    if (maxTop < 0) maxTop = 0;
    if (topLine > maxTop) topLine = maxTop;
    if (topLine < 0) topLine = 0;

    gzInfo_setTopLine(topLine);
}

void gzMenu_c::drawDescription(const char* desc) {
    f32 x = 0.0f;
    f32 y = g_gzInfo.mBackgroundHeight + 25.0f;

    if (gzInfo_isSubMenuVisible() && desc != NULL && *desc != '\0' && gzInfo_getMenuDescription() != NULL) {
        gzInfo_getMenuDescription()->setString(desc);
        gzInfo_getMenuDescription()->draw(x, y, gzInfo_getCursorColor(), HBIND_CENTER);
    }
}

void gzMenu_c::drawTPCursor() {
    if (gzInfo_isCursorTypeTP() && gzInfo_getTPCursor() != NULL) {
        gzSetup2DContext();
        gzInfo_getTPCursor()->draw();
    }
}

void gzMenu_c::drawTPCursorForBox(gzTextBox* box, f32 x, f32 y) {
    if (!gzInfo_isCursorTypeTP() || gzInfo_getTPCursor() == NULL || box == NULL) {
        return;
    }
    box->updateBounds();
    f32 cursorX = x + (box->getWidth() / 2.0f) + gzMenuLayout::TP_CURSOR_X_OFFSET;
    f32 cursorY = y + (box->getHeight() / 2.0f) + gzMenuLayout::TP_CURSOR_Y_OFFSET;
    gzInfo_getTPCursor()->setPos(cursorX, cursorY, (J2DPane*)box, false);
    gzSetup2DContext();
    gzInfo_getTPCursor()->draw();
}

void gzMenu_c::updateLineBounds(gzLine** lines, int numLines) {
    J2DTextBox::TFontSize font_size;
    for (int i = 0; i < numLines; i++) {
        if (lines[i] == NULL) continue;
        lines[i]->mText->getFontSize(font_size);
        font_size.mSizeX *= 0.5f;
        lines[i]->mText->mBounds.f.x = lines[i]->mText->mStringLength * font_size.mSizeX;
        gzTextBox* opt = lines[i]->getOptionBox();
        if (opt != NULL) {
            opt->getFontSize(font_size);
            font_size.mSizeX *= 0.5f;
            opt->mBounds.f.x = opt->mStringLength * font_size.mSizeX;
        }
    }
}

void gzMenu_c::drawTabHeaders(gzTextBox** headers, const f32* xPositions, int numTabs,
                               int currentTab, f32 yPosition, u32 activeColor) {
    for (int i = 0; i < numTabs; i++) {
        if (headers[i] != NULL) {
            u32 color = (i == currentTab) ? activeColor : COLOR_WHITE;
            headers[i]->draw(xPositions[i], yPosition, color);
        }
    }
}

void gzMenu_c::drawLinesWithHaihai(gzLine** lines, s32 numLines, s32 topLine, s32 visibleLines) {
    gzCursor* l_cursor = gzInfo_getCursor();
    J2DTextBox::TFontSize font_size;

    if (numLines > 0 && lines[0] != NULL) {
        lines[0]->mText->getFontSize(font_size);
        mpHaihai->setScale(font_size.mSizeY * gzMenuLayout::HAIHAI_SCALE_FACTOR);
    }

    f32 lineX = mXPos;
    f32 lineY_start = g_gzInfo.mBackgroundYPos + gzMenuLayout::Y_ALIGNMENT;
    f32 line_spacing = gzMenuLayout::LINE_SPACING;
    f32 optionX = mXPos + gzMenuLayout::OPTIONS_X_OFFSET;
    f32 haihaiX = optionX + gzMenuLayout::HAIHAI_X_OFFSET;
    u32 cursorColor = gzInfo_getCursorColor();
    s32 endLine = topLine + visibleLines;

    if (endLine > numLines) endLine = numLines;

    for (s32 i = topLine; i < endLine; i++) {
        gzLine* line = lines[i];
        if (line == NULL) continue;

        s32 screenIdx = i - topLine;
        f32 lineY = lineY_start + (screenIdx * line_spacing);
        f32 haihaiY = lineY + gzMenuLayout::HAIHAI_Y_OFFSET;
        bool isSelected = (l_cursor->y == i && gzInfo_isSubMenuVisible());
        u32 color = isSelected ? cursorColor : COLOR_WHITE;

        line->mText->draw(lineX, lineY, color);

        gzTextBox* opt = line->getOptionBox();
        if (opt != NULL) {
            opt->draw(optionX, lineY, color, HBIND_CENTER);

            // Get per-line haihai flags via virtual method
            if (isSelected && gzInfo_isMenuOption()) {
                u8 flags = getHaihaiFlags(i);
                if (flags != 0) {
                    f32 haihaiWidth = opt->mBounds.f.x + gzMenuLayout::HAIHAI_EXTRA_SPACING;
                    gzSetup2DContext();
                    mpHaihai->drawHaihai(flags, haihaiX, haihaiY, haihaiWidth, 0.0f);
                }
            }
        }

        if (isSelected && gzInfo_isCursorTypeTP() && gzInfo_getTPCursor() != NULL) {
            line->mText->updateBounds();
            f32 totalWidth;
            f32 leftEdge = lineX;

            // If there's an option box, calculate combined bounds
            f32 optionXAdjust = 0.0f;
            if (opt != NULL && opt->mStringLength != 0) {
                opt->updateBounds();
                // Option is drawn near the haihai area, use haihaiX as reference
                // Account for haihai arrows extending beyond the option text
                f32 rightEdge = haihaiX + (opt->getWidth() / 2.0f) + 40.0f;  // Extra for right arrow
                totalWidth = rightEdge - leftEdge + 30.0f;
                optionXAdjust = -15.0f;  // Shift left to center with arrows
            } else {
                totalWidth = line->mText->getWidth();
            }

            // setPos expects center position
            f32 cursorX = leftEdge + (totalWidth / 2.0f) + gzMenuLayout::TP_CURSOR_X_OFFSET + optionXAdjust;
            f32 cursorY = lineY + (line->mText->getHeight() / 2.0f) + gzMenuLayout::TP_CURSOR_Y_OFFSET;

            // Temporarily set bounds for the combined width
            line->mText->mBounds.f.x = totalWidth;
            gzInfo_getTPCursor()->setPos(cursorX, cursorY, (J2DPane*)line->mText, false);
            gzSetup2DContext();
            gzInfo_getTPCursor()->draw();
        }
    }

    // Draw description for selected line
    if (l_cursor->y < numLines && lines[l_cursor->y] != NULL) {
        drawDescription(lines[l_cursor->y]->m_description);
    }
}

bool gzMenu_c::checkInputWait() {
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return true;
    }
    return false;
}

bool gzMenu_c::handleBackButton(int mainMenuIndex) {
    if (gzPad::getTrigB()) {
        if (gzInfo_isMenuOption()) {
            gzInfo_offMenuOption();
            gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
        } else {
            gzCursor* l_cursor = gzInfo_getCursor();
            l_cursor->x--;
            l_cursor->y = mainMenuIndex;
            gzInfo_seStart(Z2SE_SY_EXP_WIN_CLOSE);
            g_gzInfo.mpMainMenu->startReverseTransition();
            return true;
        }
    }
    return false;
}

void gzMenu_c::handleNavigation(int maxLines) {
    if (gzInfo_isMenuOption()) {
        return;
    }

    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigDown()) {
        l_cursor->y = (l_cursor->y + 1) % maxLines;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigUp()) {
        l_cursor->y = (l_cursor->y == 0) ? maxLines - 1 : l_cursor->y - 1;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }
}

void gzMenu_c::handleTabSwitch(int& currentTab, int maxTabs) {
    if (gzInfo_isMenuOption()) {
        return;
    }

    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigLeft()) {
        currentTab = (currentTab == 0) ? maxTabs - 1 : currentTab - 1;
        l_cursor->y = 0;
        gzInfo_setTopLine(0);
        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
    }

    if (gzPad::getTrigRight()) {
        currentTab = (currentTab + 1) % maxTabs;
        l_cursor->y = 0;
        gzInfo_setTopLine(0);
        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
    }
}

void gzMenu_c::finishExecute(int maxLines) {
    updateScrolling(maxLines);
    mpHaihai->_execute(0);
}

gzMenu_c::gzMenu_c() : mXPos(0.0f), mpHaihai(NULL) {
    mpHaihai = new dMeterHaihai_c(3);
    mpHaihai->setScale(0.04f);
}

gzMenu_c::~gzMenu_c() {
    if (mpHaihai != NULL) {
        delete mpHaihai;
        mpHaihai = NULL;
    }
}
