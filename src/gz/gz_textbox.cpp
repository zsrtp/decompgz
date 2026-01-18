#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_textbox.h"
#include "gz/gz_utility_notification.h"
#include "JSystem/JKernel/JKRExpHeap.h"

static const u32 TEXTBOX_POOL_SIZE = 700;
static gzTextBox* sTextBoxPool = NULL;
static u8* sTextBoxUsed = NULL;
static bool sPoolInitialized = false;

static void initPool() {
    if (sPoolInitialized) return;

    JKRExpHeap* heap = (JKRExpHeap*)mDoExt_getCurrentHeap();

    u32 poolBytes = TEXTBOX_POOL_SIZE * sizeof(gzTextBox);
    u32 freeSize = heap->getFreeSize();
    if (poolBytes + 64 > freeSize) {
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

    memset(sTextBoxUsed, 0, bitmapBytes);
    sPoolInitialized = true;
}

void gzTextBox_shutdownPool() {
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

gzTextBox* gzTextBox_allocate() {
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
    OSReport("TextBox pool exhausted!\n");
    return NULL;
}

void gzTextBox_free(gzTextBox* box) {
    if (!sPoolInitialized || box < sTextBoxPool || box >= sTextBoxPool + TEXTBOX_POOL_SIZE) return;

    u32 idx = (u32)(box - sTextBoxPool);
    u8 byte = (u8)(idx / 8);
    u8 bit = (u8)(idx % 8);
    sTextBoxUsed[byte] &= ~(1 << bit);
}
