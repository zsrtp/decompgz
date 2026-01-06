#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu.h"
// #include "JSystem/JKernel/JKRExpHeap.h"

// NOTE(Pheenoh): Unused right now
// gzTextBox* gzMenu_c::sTextBoxPool = NULL;
// u8* gzMenu_c::sTextBoxUsed = NULL;
// bool gzMenu_c::sPoolInitialized = false;

void gzMenu_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigDown() && !mOption) {
        l_cursor->y = (l_cursor->y + 1) % mVisibleLines;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigUp() && !mOption) {
        l_cursor->y = (l_cursor->y == 0) ? mVisibleLines - 1 : l_cursor->y - 1;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigB()) {
        gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
    }

    if (gzPad::getTrigA()) {
        mOption ? gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK) : gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
    }

    updateScrolling(mVisibleLines);
    mpHaihai->_execute(0);
}

void gzMenu_c::drawLines(gzTextBox** lines, gzTextBox** lineOptions, u8 haihaiFlags, s32 numLines) {
    gzCursor* l_cursor = gzInfo_getCursor();

    J2DTextBox::TFontSize font_size;
    lines[0]->getFontSize(font_size);
    mpHaihai->setScale(font_size.mSizeY * 0.04f);

    f32 lineX = mXPos;
    f32 lineY_start = g_gzInfo.mBackgroundYPos + 95.0f;
    f32 line_spacing = g_gzInfo.mBackgroundHeight / 20;

    f32 optionX = mXPos - 20.0f;
    f32 haihaiX = optionX + 305.0f;

    u32 cursorColor = gzInfo_getCursorColor();

    for (int i = 0; i < numLines; i++) {
        f32 lineY = lineY_start + ((i - 1) * line_spacing);
        f32 haihaiY = lineY - 7.0f;
        bool isSelected = (l_cursor->y == i && gzInfo_isSubMenuVisible());
        
        f32 haihaiWidth = lineOptions[i]->mBounds.f.x + 30.0f; // extra spacing so text fits inside the haihai bounds
        bool showHaihai = mOption && l_cursor->y == i;

        drawLineWithOption(lines[i], lineOptions[i], lineX, optionX, lineY, isSelected, cursorColor, showHaihai, haihaiFlags, haihaiX, haihaiY, haihaiWidth);
    }

    drawDescription(lines[l_cursor->y]->m_description);
}

void gzMenu_c::drawLines(gzLine** lines, s32 numLines, u8 haihai_flags, s32 topLine, s32 visibleLines) {
    gzCursor* l_cursor = gzInfo_getCursor();
    J2DTextBox::TFontSize font_size;

    if (numLines > 0 && lines[0] != NULL) {
        lines[0]->mText->getFontSize(font_size);
        mpHaihai->setScale(font_size.mSizeY * 0.04f);
    }

    f32 lineX = mXPos;
    f32 lineY_start = g_gzInfo.mBackgroundYPos + 95.0f;
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

        bool showHaihai = mOption && l_cursor->y == i && haihai_flags != 0;
        line->mText->draw(lineX, lineY, color);

        if (opt != NULL) {
            opt->draw(optionX, lineY, color, HBIND_CENTER);
        }

        if (showHaihai && mpHaihai != NULL) {
            mpHaihai->drawHaihai(haihai_flags, haihaiX, haihaiY, haihaiWidth, 0.0f);
        }

        if (isSelected && gzInfo_isCursorTypeTP() && mpCursor != NULL) {
            f32 cursorY = lineY - 10.0f;
            mpCursor->setPos(lineX - 20.0f, cursorY, (J2DPane*)line->mText, false);
            mpCursor->draw();
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
        mpHaihai->drawHaihai(haihaiFlags, haihaiX, haihaiY, haihaiWidth, 0.0f);
    }

    if (isSelected && gzInfo_isCursorTypeTP() && mpCursor) {
        static const f32 TP_CURSOR_Y_OFFSET = -10.0f;
        f32 cursorY = lineY + TP_CURSOR_Y_OFFSET;
        mpCursor->setPos(lineX - 20.0f, cursorY, (J2DPane*)line, false);
        mpCursor->draw();
    }
}

void gzMenu_c::updateScrolling(s32 maxLines) {
    gzCursor* cursor = gzInfo_getCursor();

    if (cursor->y < mTopLine) {
        mTopLine = cursor->y;
    } else if (cursor->y >= mTopLine + mVisibleLines) {
        mTopLine = cursor->y - mVisibleLines + 1;
    }
    
    s32 maxTop = maxLines - mVisibleLines;
    if (maxTop < 0) maxTop = 0;
    if (mTopLine > maxTop) mTopLine = maxTop;
    if (mTopLine < 0) mTopLine = 0;
}

void gzMenu_c::drawDescription(const char* desc) {
    f32 x = 0.0f;
    f32 y = g_gzInfo.mBackgroundHeight + 25.0f;

    if (gzInfo_isSubMenuVisible() && desc != NULL && *desc != NULL) {
        mpDescription->setString(desc);
        mpDescription->draw(x, y, gzInfo_getCursorColor(), HBIND_CENTER);
    }
}

// NOTE(Pheenoh): This is unused right now
// void gzMenu_c::initPool() {
//     if (sPoolInitialized) return;

//     // *Should* always be the heap set in gz/gz.cpp
//     JKRExpHeap* heap = (JKRExpHeap*)mDoExt_getCurrentHeap();

//     u32 poolBytes = TEXTBOX_POOL_SIZE * sizeof(gzTextBox);
//     u32 freeSize = heap->getFreeSize();
//     if (poolBytes + 64 > freeSize) {  // Buffer for overhead/fragmentation
//         gzInfo_sendNotification("Low mem: TextBox pool skipped!", gzNotification_c::NOTIFY_WARNING);
//         return;
//     }

//     void* poolMem = heap->alloc(poolBytes, 32);
//     if (!poolMem) {
//         gzInfo_sendNotification("TextBox pool alloc failed!", gzNotification_c::NOTIFY_ERROR);
//         return;
//     }

//     sTextBoxPool = (gzTextBox*)(poolMem);
//     for (u32 i = 0; i < TEXTBOX_POOL_SIZE; ++i) {
//         new (&sTextBoxPool[i]) gzTextBox();
//     }

//     // Alloc bitmap (~25 bytes for 200 slots)
//     u32 bitmapBytes = (TEXTBOX_POOL_SIZE / 8) + 1;
//     sTextBoxUsed = (u8*)(heap->alloc(bitmapBytes, 4));
//     if (!sTextBoxUsed) {
//         for (u32 i = 0; i < TEXTBOX_POOL_SIZE; ++i) {
//             sTextBoxPool[i].~gzTextBox();
//         }
//         heap->free(poolMem);
//         gzInfo_sendNotification("TextBox bitmap alloc failed!", gzNotification_c::NOTIFY_ERROR);
//         return;
//     }

//     memset(sTextBoxUsed, 0, bitmapBytes);
//     sPoolInitialized = true;
// }

// void gzMenu_c::shutdownPool() {
//     if (!sPoolInitialized) return;

//     JKRExpHeap* heap = (JKRExpHeap*)mDoExt_getCurrentHeap();

//     for (u32 i = 0; i < TEXTBOX_POOL_SIZE; ++i) {
//         sTextBoxPool[i].~gzTextBox();
//     }

//     heap->free(sTextBoxPool);
//     heap->free(sTextBoxUsed);
//     sTextBoxPool = NULL;
//     sTextBoxUsed = NULL;
//     sPoolInitialized = false;
// }

// gzTextBox* gzMenu_c::allocateTextBox() {
//     initPool();
//     if (!sPoolInitialized) return NULL;

//     for (u32 i = 0; i < TEXTBOX_POOL_SIZE; i++) {
//         u8 byte = (u8)(i / 8);
//         u8 bit = (u8)(i % 8);
//         if ((sTextBoxUsed[byte] & (1 << bit)) == 0) {
//             sTextBoxUsed[byte] |= (1 << bit);
//             return &sTextBoxPool[i];
//         }
//     }
//     OSReport("TextBox pool exhausted!\n");
//     return NULL;
// }

// void gzMenu_c::freeTextBox(gzTextBox* box) {
//     if (!sPoolInitialized || box < sTextBoxPool || box >= sTextBoxPool + TEXTBOX_POOL_SIZE) return;

//     u32 idx = (u32)(box - sTextBoxPool);
//     u8 byte = (u8)(idx / 8);
//     u8 bit = (u8)(idx % 8);
//     sTextBoxUsed[byte] &= ~(1 << bit);
// }

gzMenu_c::gzMenu_c() : mXPos(0.0f), mTopLine(0), mVisibleLines(15), mpHaihai(NULL), mpCursor(NULL), mpDescription(NULL) {
    mpHaihai = new dMeterHaihai_c(3);
    mpHaihai->setScale(0.04f);

    mpCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpCursor->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    mpCursor->setAlphaRate(1.0f);

    mpDescription = new gzTextBox();
}

gzMenu_c::~gzMenu_c() {
    if (mpHaihai != NULL) {
        delete mpHaihai;
        mpHaihai = NULL;
    }

    if (mpCursor != NULL) {
        delete mpCursor;
        mpCursor = NULL;
    }

    if (mpDescription != NULL) {
        delete mpDescription;
        mpDescription = NULL;
    }
}
