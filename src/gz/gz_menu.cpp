#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu.h"
#include "JSystem/JKernel/JKRExpHeap.h"

// Init pool statics
gzTextBox* gzMenu_c::sTextBoxPool = NULL;
u8* gzMenu_c::sTextBoxUsed = NULL;
bool gzMenu_c::sPoolInitialized = false;

void gzMenu_c::drawTextBox(gzTextBox* box, f32 x, f32 y, u32 color, J2DTextBoxHBinding binding) {
    if (!box) return;

    if (gzInfo_isDropShadows()) {
        box->setCharColor(0x00000080u);  // Shadow color (ARGB)
        box->setGradColor(0x00000080u);
        box->draw(x + 2.0f, y + 2.0f, 608.0f, binding);  // Shadow offset
    }

    box->setCharColor(color);
    box->setGradColor(color);
    box->draw(x, y, 608.0f, binding);  // Main draw
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

void gzMenu_c::drawHaihaiArrows(u8 flags, f32 x, f32 y, f32 width, f32 height) {
    if (flags != 0 && gzInfo_isSubMenuVisible()) {
        mpHaihai->drawHaihai(flags, x, y, width, height);
    }
}

void gzMenu_c::drawDescription(const char* desc, f32 x, f32 y) {
    if (gzInfo_isSubMenuVisible() && desc && *desc != 0) {
        mpDescription->setString(desc);
        drawTextBox(mpDescription, x, y, gzInfo_getCursorColor(), HBIND_CENTER);
    }
}

void gzMenu_c::initPool() {
    if (sPoolInitialized) return;

    // *Should* always be the heap set in gz/gz.cpp
    JKRExpHeap* heap = (JKRExpHeap*)mDoExt_getCurrentHeap();

    u32 poolBytes = TEXTBOX_POOL_SIZE * sizeof(gzTextBox);
    u32 freeSize = heap->getFreeSize();
    if (poolBytes + 64 > freeSize) {  // Buffer for overhead/fragmentation
        gzInfo_sendNotification("Low mem: TextBox pool skipped!", gzNotification_c::NOTIFY_WARNING);
        return;
    }

    void* poolMem = heap->alloc(poolBytes, 32);
    if (!poolMem) {
        gzInfo_sendNotification("TextBox pool alloc failed!", gzNotification_c::NOTIFY_ERROR);
        return;
    }

    sTextBoxPool = (gzTextBox*)(poolMem);
    for (u32 i = 0; i < TEXTBOX_POOL_SIZE; ++i) {
        new (&sTextBoxPool[i]) gzTextBox();
    }

    // Alloc bitmap (~25 bytes for 200 slots)
    u32 bitmapBytes = (TEXTBOX_POOL_SIZE / 8) + 1;
    sTextBoxUsed = (u8*)(heap->alloc(bitmapBytes, 4));
    if (!sTextBoxUsed) {
        for (u32 i = 0; i < TEXTBOX_POOL_SIZE; ++i) {
            sTextBoxPool[i].~gzTextBox();
        }
        heap->free(poolMem);
        gzInfo_sendNotification("TextBox bitmap alloc failed!", gzNotification_c::NOTIFY_ERROR);
        return;
    }

    memset(&sTextBoxUsed, 0, sizeof(sTextBoxUsed));
    sPoolInitialized = true;
}

void gzMenu_c::shutdownPool() {
    if (!sPoolInitialized) return;

    JKRExpHeap* heap = (JKRExpHeap*)mDoExt_getCurrentHeap();

    for (u32 i = 0; i < TEXTBOX_POOL_SIZE; ++i) {
        sTextBoxPool[i].~gzTextBox();
    }

    heap->free(sTextBoxPool);
    heap->free(sTextBoxUsed);
    sTextBoxPool = NULL;
    sTextBoxUsed = NULL;
    sPoolInitialized = false;
}

gzTextBox* gzMenu_c::allocateTextBox() {
    initPool();
    if (!sPoolInitialized) return NULL;

    for (u32 i = 0; i < TEXTBOX_POOL_SIZE; i++) {
        u8 byte = (u8)(i / 8);
        u8 bit = (u8)(i % 8);
        if ((sTextBoxUsed[byte] & (1 << bit)) == 0) {
            sTextBoxUsed[byte] |= (1 << bit);
            return &sTextBoxPool[i];
        }
    }
    gzInfo_sendNotification("TextBox pool exhausted!", gzNotification_c::NOTIFY_ERROR);
    return NULL;
}

void gzMenu_c::freeTextBox(gzTextBox* box) {
    if (!sPoolInitialized || box < sTextBoxPool || box >= sTextBoxPool + TEXTBOX_POOL_SIZE) return;

    u32 idx = (u32)(box - sTextBoxPool);
    u8 byte = (u8)(idx / 8);
    u8 bit = (u8)(idx % 8);
    sTextBoxUsed[byte] &= ~(1 << bit);
}

gzMenu_c::gzMenu_c() : mXPos(0.0f), mTopLine(0), mVisibleLines(15),  // Adjust default as needed
    mpHaihai(NULL), mpCursor(NULL), mpDescription(NULL) {

    if (mpHaihai == NULL) {
        mpHaihai = new dMeterHaihai_c(3);
        mpHaihai->setScale(0.04f);
    }

    if (mpCursor == NULL) {
        mpCursor = new dSelect_cursor_c(2, 1.0f, NULL);
        mpCursor->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
        mpCursor->setAlphaRate(1.0f);
    }

    mpDescription = allocateTextBox();
    if (!mpDescription) {
        mpDescription = new gzTextBox();
    }
}

gzMenu_c::~gzMenu_c() {
    if (mpHaihai == NULL) {
        delete mpHaihai;
        mpHaihai = NULL;
    }

    if (mpCursor) {
        delete mpCursor;
        mpCursor = NULL;
    }

    if (mpDescription) {
        freeTextBox(mpDescription);
        mpDescription = NULL;
    }
}
