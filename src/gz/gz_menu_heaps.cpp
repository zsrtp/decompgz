#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_heaps.h"
#include "gz/gz_menu_main.h"
#include "SSystem/SComponent/c_counter.h"
#include "dolphin/os.h"

static int partition(u32* starts, JKRExpHeap::CMemBlock** blocks, int low, int high) {
    u32 pivot = starts[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (starts[j] < pivot) {
            i++;
            // Swap starts
            u32 temp_start = starts[i];
            starts[i] = starts[j];
            starts[j] = temp_start;
            // Swap blocks
            JKRExpHeap::CMemBlock* temp_block = blocks[i];
            blocks[i] = blocks[j];
            blocks[j] = temp_block;
        }
    }
    // Swap pivot
    u32 temp_start = starts[i + 1];
    starts[i + 1] = starts[high];
    starts[high] = temp_start;
    JKRExpHeap::CMemBlock* temp_block = blocks[i + 1];
    blocks[i + 1] = blocks[high];
    blocks[high] = temp_block;
    return i + 1;
}

static void quicksort(u32* starts, JKRExpHeap::CMemBlock** blocks, int low, int high) {
    if (low < high) {
        int pi = partition(starts, blocks, low, high);
        quicksort(starts, blocks, low, pi - 1);
        quicksort(starts, blocks, pi + 1, high);
    }
}

gzHeapsMenu_c::HeapTracker_c::HeapTracker_c(int block_max) : mpHeap(NULL),
    mpAramHeap(NULL),
    mIsAram(false),
    mpTitle(NULL),
    mpTotalBlocks(NULL),
    mpUsedBlocks(NULL),
    mpFreeBlocks(NULL),
    mpFragmentation(NULL),
    mpUsedSize(NULL),
    mpFreeSize(NULL),
    mpTotalSize(NULL),
    mpLargestFree(NULL),
    mNumBlocks(0),
    mPrevNumBlocks(-1),
    mUsedBlocks(0),
    mFreeBlocks(0),
    mBlocks(NULL),
    mAramBlocks(NULL),
    mStarts(NULL),
    mFragmentation(0.0f),
    mUsedSizeKB(0),
    mFreeSizeKB(0),
    mTotalSizeKB(0),
    mLargestFreeKB(0),
    mMaxBlocks(block_max) {

        mpTitle = gzTextBox_allocate();
        mpTitle->setFontSize(12.0f, 12.0f);

        mpTotalBlocks = gzTextBox_allocate();
        mpTotalBlocks->setFontSize(12.0f, 12.0f);

        mpUsedBlocks = gzTextBox_allocate();
        mpUsedBlocks->setFontSize(12.0f, 12.0f);

        mpFreeBlocks = gzTextBox_allocate();
        mpFreeBlocks->setFontSize(12.0f, 12.0f);

        mpFragmentation = gzTextBox_allocate();
        mpFragmentation->setFontSize(12.0f, 12.0f);

        mpUsedSize = gzTextBox_allocate();
        mpUsedSize->setFontSize(12.0f, 12.0f);

        mpFreeSize = gzTextBox_allocate();
        mpFreeSize->setFontSize(12.0f, 12.0f);

        mpTotalSize = gzTextBox_allocate();
        mpTotalSize->setFontSize(12.0f, 12.0f);

        mpLargestFree = gzTextBox_allocate();
        mpLargestFree->setFontSize(12.0f, 12.0f);

        mBlocks = new JKRExpHeap::CMemBlock*[mMaxBlocks];
        for (int i = 0; i < mMaxBlocks; ++i) {
            mBlocks[i] = NULL;
        }

        mAramBlocks = new JKRAramBlock*[mMaxBlocks];
        for (int i = 0; i < mMaxBlocks; ++i) {
            mAramBlocks[i] = NULL;
        }

        mStarts = new u32[mMaxBlocks];
}

gzHeapsMenu_c::HeapTracker_c::~HeapTracker_c() {
    delete[] mBlocks;
    mBlocks = NULL;

    delete[] mAramBlocks;
    mAramBlocks = NULL;

    delete[] mStarts;
    mStarts = NULL;

    gzTextBox_free(mpTitle);
    mpTitle = NULL;

    gzTextBox_free(mpTotalBlocks);
    mpTotalBlocks = NULL;

    gzTextBox_free(mpUsedBlocks);
    mpUsedBlocks = NULL;

    gzTextBox_free(mpFreeBlocks);
    mpFreeBlocks = NULL;

    gzTextBox_free(mpFragmentation);
    mpFragmentation = NULL;

    gzTextBox_free(mpUsedSize);
    mpUsedSize = NULL;

    gzTextBox_free(mpFreeSize);
    mpFreeSize = NULL;

    gzTextBox_free(mpTotalSize);
    mpTotalSize = NULL;

    gzTextBox_free(mpLargestFree);
    mpLargestFree = NULL;
}

void gzHeapsMenu_c::updateDynamicLines() {
    for (int i = 0; i < HEAP_TRACKER_MAX_e; i++) {
        mTrackers[i]->mpTotalBlocks->setStringf("total blocks %d", mTrackers[i]->mNumBlocks);
        mTrackers[i]->mpUsedBlocks->setStringf("used blocks %d", mTrackers[i]->mUsedBlocks);
        mTrackers[i]->mpFreeBlocks->setStringf("free blocks %d", mTrackers[i]->mFreeBlocks);
        mTrackers[i]->mpFragmentation->setStringf("frag %.2f%%", mTrackers[i]->mFragmentation);
        mTrackers[i]->mpUsedSize->setStringf("used %d kb", mTrackers[i]->mUsedSizeKB);
        mTrackers[i]->mpFreeSize->setStringf("free %d kb", mTrackers[i]->mFreeSizeKB);
        mTrackers[i]->mpTotalSize->setStringf("total %d kb", mTrackers[i]->mTotalSizeKB);
        mTrackers[i]->mpLargestFree->setStringf("largest free %d kb", mTrackers[i]->mLargestFreeKB);

        f32 used_pct = 0.0f;
        if (mTrackers[i]->mTotalSizeKB > 0) {
            used_pct = ((f32)mTrackers[i]->mUsedSizeKB / (f32)mTrackers[i]->mTotalSizeKB) * 100.0f;
        }
        f32 free_pct = 100.0f - used_pct;
        mTrackers[i]->mpUsedPercent->setStringf("used %.1f%%", used_pct);
        mTrackers[i]->mpFreePercent->setStringf("free %.1f%%", free_pct);
    }

    if (mShowOverview) {
        gzInfo_getMenuDescription()->setString("Press Y for heap details");
    } else {
        const char* next_view;
        if (mViewMode == 0) {
            next_view = "blocks";
        } else if (mViewMode == 1) {
            next_view = "percent";
        } else {
            next_view = "KB";
        }
        gzInfo_getMenuDescription()->setStringf("Press X to switch to %s view, Y for overview", next_view);
    }
}

gzHeapsMenu_c::gzHeapsMenu_c() {
    OSReport("creating gzHeapsMenu_c\n");
    mXPos = g_gzInfo.mBackgroundXPos + 195.0f;

    for (int i = 0; i < HEAP_TRACKER_MAX_e; i++) {
        mTrackers[i] = new HeapTracker_c(256);
        mTrackers[i]->mpUsedPercent = gzTextBox_allocate();
        mTrackers[i]->mpUsedPercent->setFontSize(12.0f, 12.0f);
        mTrackers[i]->mpFreePercent = gzTextBox_allocate();
        mTrackers[i]->mpFreePercent->setFontSize(12.0f, 12.0f);
    }

    mpLegendUsed = gzTextBox_allocate();
    mpLegendUsed->setFontSize(12.0f, 12.0f);
    mpLegendUsed->setString("used");

    mpLegendMenuUsed = gzTextBox_allocate();
    mpLegendMenuUsed->setFontSize(12.0f, 12.0f);
    mpLegendMenuUsed->setString("tpgz used");

    mpLegendFree = gzTextBox_allocate();
    mpLegendFree->setFontSize(12.0f, 12.0f);
    mpLegendFree->setString("free");

    mTrackers[HEAP_J2D_e]->mpHeap = mDoExt_getJ2dHeap();
    mTrackers[HEAP_J2D_e]->mpTitle->setString("j2d heap");
    
    mTrackers[HEAP_ARCHIVE_e]->mpHeap = mDoExt_getArchiveHeap();
    mTrackers[HEAP_ARCHIVE_e]->mpTitle->setString("archive heap");

    mTrackers[HEAP_GAME_e]->mpHeap = mDoExt_getGameHeap();
    mTrackers[HEAP_GAME_e]->mpTitle->setString("game heap");

    // mTrackers[HEAP_HOSTIO_e]->mpHeap = mDoExt_getHostIOHeap();
    // mTrackers[HEAP_HOSTIO_e]->mpTitle->setString("hostio heap");

    // mTrackers[HEAP_DBPRINT_e]->mpHeap = mDoExt_getDbPrintHeap();
    // mTrackers[HEAP_DBPRINT_e]->mpTitle->setString("db print heap");

    // mTrackers[HEAP_COMMAND_e]->mpHeap = mDoExt_getCommandHeap();
    // mTrackers[HEAP_COMMAND_e]->mpTitle->setString("command heap");

    // mTrackers[HEAP_SYSTEM_e]->mpHeap = (JKRExpHeap*)JKRHeap::getSystemHeap();
    // mTrackers[HEAP_SYSTEM_e]->mpTitle->setString("system heap");

    mTrackers[HEAP_ZELDA_e]->mpHeap = mDoExt_getZeldaHeap();
    mTrackers[HEAP_ZELDA_e]->mpTitle->setString("zelda heap");

    mTrackers[HEAP_ARAM_e]->mIsAram = true;
    mTrackers[HEAP_ARAM_e]->mpAramHeap = JKRAram::getAramHeap();
    mTrackers[HEAP_ARAM_e]->mpTitle->setString("aram heap");

    mViewMode = 0;  // Start with KB view
    mShowOverview = false;

    // Overview mode text boxes
    mpOverviewTitle = gzTextBox_allocate();
    mpOverviewTitle->setFontSize(14.0f, 14.0f);
    mpOverviewTitle->setString("Memory Overview");

    mpRootHeapInfo = gzTextBox_allocate();
    mpRootHeapInfo->setFontSize(12.0f, 12.0f);

    mpSystemHeapInfo = gzTextBox_allocate();
    mpSystemHeapInfo->setFontSize(12.0f, 12.0f);

    mpArenaInfo = gzTextBox_allocate();
    mpArenaInfo->setFontSize(12.0f, 12.0f);

    mpAramInfo = gzTextBox_allocate();
    mpAramInfo->setFontSize(12.0f, 12.0f);

    mpRootChildren = gzTextBox_allocate();
    mpRootChildren->setFontSize(12.0f, 12.0f);

    mpSystemChildren = gzTextBox_allocate();
    mpSystemChildren->setFontSize(12.0f, 12.0f);
}

gzHeapsMenu_c::~gzHeapsMenu_c() {
    _delete();
}

void gzHeapsMenu_c::_delete() {
    OSReport("deleting gzHeapsMenu_c\n");

    gzTextBox_free(mpLegendUsed);
    gzTextBox_free(mpLegendMenuUsed);
    gzTextBox_free(mpLegendFree);
    for (int i = 0; i < HEAP_TRACKER_MAX_e; i++) {
        gzTextBox_free(mTrackers[i]->mpUsedPercent);
        gzTextBox_free(mTrackers[i]->mpFreePercent);
    }

    gzTextBox_free(mpOverviewTitle);
    gzTextBox_free(mpRootHeapInfo);
    gzTextBox_free(mpSystemHeapInfo);
    gzTextBox_free(mpArenaInfo);
    gzTextBox_free(mpAramInfo);
    gzTextBox_free(mpRootChildren);
    gzTextBox_free(mpSystemChildren);
}

void gzHeapsMenu_c::updateHeapTracker(HeapTracker_c* tracker) {
    if (tracker->mIsAram) {
        // ARAM heap tracking
        JKRAramHeap* aramHeap = tracker->mpAramHeap;
        if (aramHeap == NULL) return;

        // Count blocks in the ARAM list
        int blockCount = 0;
        JSUListIterator<JKRAramBlock> iter;
        for (iter = JKRAramHeap::sAramList.getFirst(); iter != JKRAramHeap::sAramList.getEnd(); ++iter) {
            blockCount++;
        }

        tracker->mNumBlocks = blockCount;
        tracker->mUsedBlocks = blockCount;  // All blocks in the list are allocated
        tracker->mFreeBlocks = 0;  // Free space is tracked differently in ARAM

        // Resize arrays if needed
        if (blockCount > tracker->mMaxBlocks) {
            int newMax = blockCount * 3 / 2;
            if (newMax < 256) newMax = 256;

            delete[] tracker->mAramBlocks;
            delete[] tracker->mStarts;

            tracker->mAramBlocks = new JKRAramBlock*[newMax];
            tracker->mStarts = new u32[newMax];
            tracker->mMaxBlocks = newMax;
        }

        // Collect blocks
        int idx = 0;
        for (iter = JKRAramHeap::sAramList.getFirst(); iter != JKRAramHeap::sAramList.getEnd(); ++iter) {
            tracker->mAramBlocks[idx] = iter.getObject();
            tracker->mStarts[idx] = iter->getAddress();
            idx++;
        }

        tracker->mTotalSizeKB = aramHeap->mSize / 1024;
        tracker->mFreeSizeKB = aramHeap->getTotalFreeSize() / 1024;
        tracker->mUsedSizeKB = tracker->mTotalSizeKB - tracker->mFreeSizeKB;
        tracker->mLargestFreeKB = aramHeap->getFreeSize() / 1024;

        int total_free = aramHeap->getTotalFreeSize();
        int largest_free = aramHeap->getFreeSize();

        if (total_free > 0 && largest_free < total_free) {
            tracker->mFragmentation = (1.0f - (f32)largest_free / (f32)total_free) * 100.0f;
        } else {
            tracker->mFragmentation = 0.0f;
        }
    } else {
        // Regular JKRExpHeap tracking
        tracker->mpHeap->lock();

        int usedCount = 0;
        for (JKRExpHeap::CMemBlock* block = tracker->mpHeap->mHeadUsedList; block != NULL; block = block->getNextBlock()) {
            usedCount++;
        }

        int freeCount = 0;
        for (JKRExpHeap::CMemBlock* block = tracker->mpHeap->mHeadFreeList; block != NULL; block = block->getNextBlock()) {
            freeCount++;
        }

        int totalCount = usedCount + freeCount;

        if (totalCount > tracker->mMaxBlocks) {
            int newMax = totalCount * 3 / 2;
            if (newMax < 256) newMax = 256;

            JKRHeap* oldHeap = mDoExt_getCurrentHeap();
            mDoExt_setCurrentHeap(mDoExt_getArchiveHeap());

            JKRExpHeap* archiveHeap = (JKRExpHeap*)mDoExt_getArchiveHeap();
            u8 oldGroupId = archiveHeap->mCurrentGroupId;
            archiveHeap->mCurrentGroupId = g_gzInfo.mGzGroupID;

            JKRExpHeap::CMemBlock** newBlocks = new JKRExpHeap::CMemBlock*[newMax];
            for (int i = 0; i < newMax; ++i) {
                newBlocks[i] = NULL;
            }
            u32* newStarts = new u32[newMax];

            archiveHeap->mCurrentGroupId = oldGroupId;

            delete[] tracker->mBlocks;
            delete[] tracker->mStarts;

            tracker->mBlocks = newBlocks;
            tracker->mStarts = newStarts;
            tracker->mMaxBlocks = newMax;

            mDoExt_setCurrentHeap(oldHeap);
        }

        int idx = 0;

        for (JKRExpHeap::CMemBlock* block = tracker->mpHeap->mHeadUsedList; block != NULL; block = block->getNextBlock()) {
            tracker->mBlocks[idx++] = block;
        }

        for (JKRExpHeap::CMemBlock* block = tracker->mpHeap->mHeadFreeList; block != NULL; block = block->getNextBlock()) {
            tracker->mBlocks[idx++] = block;
        }

        tracker->mUsedBlocks = usedCount;
        tracker->mFreeBlocks = freeCount;
        tracker->mNumBlocks = totalCount;

        for (int i = 0; i < totalCount; i++) {
            JKRExpHeap::CMemBlock* b = tracker->mBlocks[i];
            u8 align = b->getAlignment();
            if (b->isTempMemBlock()) {
                tracker->mStarts[i] = (u32)((u8*)b - align);
            } else {
                tracker->mStarts[i] = (u32)b;
            }
        }

        if (totalCount != tracker->mPrevNumBlocks) {
            quicksort(tracker->mStarts, tracker->mBlocks, 0, totalCount - 1);
            tracker->mPrevNumBlocks = totalCount;
        }

        tracker->mTotalSizeKB = tracker->mpHeap->getSize() / 1024;
        tracker->mUsedSizeKB = tracker->mpHeap->getTotalUsedSize() / 1024;
        tracker->mFreeSizeKB = tracker->mpHeap->do_getTotalFreeSize() / 1024;
        tracker->mLargestFreeKB = tracker->mpHeap->do_getFreeSize() / 1024;

        int total_free = tracker->mpHeap->do_getTotalFreeSize();
        int largest_free = tracker->mpHeap->do_getFreeSize();

        if (total_free > 0 && tracker->mFreeBlocks > 1) {
            tracker->mFragmentation = (1.0f - (f32)largest_free / (f32)total_free) * 100.0f;
        } else {
            tracker->mFragmentation = 0.0f;
        }

        tracker->mpHeap->unlock();
    }
}

void gzHeapsMenu_c::execute() {
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return;
    }

    gzCursor* l_cursor = gzInfo_getCursor();

    if (cCt_getFrameCount() % 5 == 0) {
        for (int i = 0; i < HEAP_TRACKER_MAX_e; i++) {
            if (mTrackers[i] != NULL) {
                if (mTrackers[i]->mIsAram && mTrackers[i]->mpAramHeap != NULL) {
                    updateHeapTracker(mTrackers[i]);
                } else if (!mTrackers[i]->mIsAram && mTrackers[i]->mpHeap != NULL) {
                    updateHeapTracker(mTrackers[i]);
                }
            }
        }
    }

    if (gzPad::getTrigX()) {
        if (!mShowOverview) {
            mViewMode = (mViewMode + 1) % 3;
            gzInfo_seStart(Z2SE_SY_OPTION_SWITCH);
        }
    }

    if (gzPad::getTrigY()) {
        mShowOverview = !mShowOverview;
        gzInfo_seStart(Z2SE_SY_OPTION_SWITCH);
    }

    if (gzPad::getTrigB()) {
        l_cursor->x--;
        l_cursor->y = gzMainMenu_c::MENU_HEAPS;
        gzInfo_seStart(Z2SE_SY_EXP_WIN_CLOSE);
        g_gzInfo.mpMainMenu->startReverseTransition();
        return;
    }
}

void gzHeapsMenu_c::drawHeapVisualization(HeapTracker_c* tracker, f32 x, f32 y, f32 width) {
    f32 bar_h = 20.0f;

    u32 heap_start_addr;
    u32 heap_size;

    if (tracker->mIsAram) {
        heap_start_addr = tracker->mpAramHeap->mHeadAddress;
        heap_size = tracker->mpAramHeap->mSize;
    } else {
        heap_start_addr = (u32)tracker->mpHeap->getStartAddr();
        heap_size = tracker->mpHeap->getSize();
    }

    // Set up GX for 2D drawing
    Mtx44 proj;
    MTXOrtho(proj, 0, 480, 0, 640, 0, 1);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);

    // Set viewport and scissor to match ortho
    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(0, 0, 640, 480);

    Mtx model;
    MTXIdentity(model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);

    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);

    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);

    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

    GXInvalidateVtxCache();
    GXPixModeSync();

    // Draw background for entire heap
    // For ARAM: green (free space), for regular heaps: black (will be covered by blocks)
    u8 bg_r = 0, bg_g = tracker->mIsAram ? 255 : 0, bg_b = 0;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(x, y);
    GXColor4u8(bg_r, bg_g, bg_b, 255);
    GXPosition2f32(x + width, y);
    GXColor4u8(bg_r, bg_g, bg_b, 255);
    GXPosition2f32(x + width, y + bar_h);
    GXColor4u8(bg_r, bg_g, bg_b, 255);
    GXPosition2f32(x, y + bar_h);
    GXColor4u8(bg_r, bg_g, bg_b, 255);
    GXEnd();

    if (tracker->mIsAram) {
        // Draw ARAM blocks
        for (int i = 0; i < tracker->mNumBlocks; i++) {
            JKRAramBlock* b = tracker->mAramBlocks[i];
            if (b == NULL) continue;

            u32 block_start_byte = b->getAddress() - heap_start_addr;
            u32 block_byte_size = b->getSize();

            f32 pixel_x = x + ((f32)block_start_byte / (f32)heap_size) * width;
            f32 pixel_w = ((f32)block_byte_size / (f32)heap_size) * width;

            // ARAM blocks are all used (red), no group coloring for now
            u8 r = 255, g = 0, blue = 0, a = 255;

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition2f32(pixel_x, y);
            GXColor4u8(r, g, blue, a);
            GXPosition2f32(pixel_x + pixel_w, y);
            GXColor4u8(r, g, blue, a);
            GXPosition2f32(pixel_x + pixel_w, y + bar_h);
            GXColor4u8(r, g, blue, a);
            GXPosition2f32(pixel_x, y + bar_h);
            GXColor4u8(r, g, blue, a);
            GXEnd();
        }
    } else {
        // Draw regular heap blocks
        for (int i = 0; i < tracker->mNumBlocks; i++) {
            JKRExpHeap::CMemBlock* b = tracker->mBlocks[i];

            u32 block_start_byte = tracker->mStarts[i] - heap_start_addr;
            u32 header_addr = (u32)b;
            u32 block_end = header_addr + 0x10 + b->getSize();
            u32 block_byte_size = block_end - tracker->mStarts[i];

            f32 pixel_x = x + ((f32)block_start_byte / (f32)heap_size) * width;
            f32 pixel_w = ((f32)block_byte_size / (f32)heap_size) * width;

            u8 r, g, blue, a = 255;
            if (b->isValid()) {
                if (b->mGroupId == g_gzInfo.mGzGroupID) {
                    r = 0; g = 0; blue = 255;
                } else {
                    r = 255; g = 0; blue = 0;
                }
            } else {
                r = 0; g = 255; blue = 0;
            }

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition2f32(pixel_x, y);
            GXColor4u8(r, g, blue, a);
            GXPosition2f32(pixel_x + pixel_w, y);
            GXColor4u8(r, g, blue, a);
            GXPosition2f32(pixel_x + pixel_w, y + bar_h);
            GXColor4u8(r, g, blue, a);
            GXPosition2f32(pixel_x, y + bar_h);
            GXColor4u8(r, g, blue, a);
            GXEnd();
        }
    }

    GXFlush();
}

void gzHeapsMenu_c::drawSwatch(f32 x, f32 y, f32 size, u32 color) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(x, y);
    GXColor1u32(color);
    GXPosition2f32(x + size, y);
    GXColor1u32(color);
    GXPosition2f32(x + size, y + size);
    GXColor1u32(color);
    GXPosition2f32(x, y + size);
    GXColor1u32(color);
    GXEnd();
}

void gzHeapsMenu_c::drawLegend(f32 legend_x, f32 legend_y) {
    f32 swatch_size = 10.0f;
    f32 text_offset_x = swatch_size + 5.0f;
    f32 text_y = legend_y + 10.0f;

    // Item widths: swatch + gap + text width (approximate)
    // "used" ~24px, "tpgz used" ~54px, "free" ~24px
    f32 item1_width = swatch_size + 5.0f + 24.0f;   // used
    f32 item2_width = swatch_size + 5.0f + 54.0f;   // tpgz used
    f32 gap = 20.0f;  // equal gap between items

    f32 item1_x = legend_x;
    f32 item2_x = item1_x + item1_width + gap;
    f32 item3_x = item2_x + item2_width + gap;

    Mtx44 proj;
    MTXOrtho(proj, 0, 480, 0, 640, 0, 1);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);

    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    GXSetScissor(0, 0, 640, 480);

    Mtx model;
    MTXIdentity(model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);

    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);

    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);

    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

    GXInvalidateVtxCache();
    GXPixModeSync();

    // Draw swatches at calculated positions
    drawSwatch(item1_x, legend_y, swatch_size, COLOR_RED);
    drawSwatch(item2_x, legend_y, swatch_size, COLOR_BLUE);
    drawSwatch(item3_x, legend_y, swatch_size, COLOR_GREEN);

    GXFlush();

    // Draw text labels
    mpLegendUsed->draw(item1_x + text_offset_x, text_y, COLOR_WHITE);
    mpLegendMenuUsed->draw(item2_x + text_offset_x, text_y, COLOR_WHITE);
    mpLegendFree->draw(item3_x + text_offset_x, text_y, COLOR_WHITE);
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void gzHeapsMenu_c::drawOverview() {
    static const f32 LINE_SPACING = 18.0f;
    static const f32 SECTION_SPACING = 26.0f;
    f32 y_pos = 55.0f;

    // Title
    mpOverviewTitle->draw(mXPos, y_pos, COLOR_WHITE);
    y_pos += SECTION_SPACING;

    // Arena - OS level memory (root heap is carved from this)
    // After init, OSGetArenaLo/Hi are consumed, so use JKRHeap's stored values
    void* userRamStart = JKRHeap::getUserRamStart();
    void* userRamEnd = JKRHeap::getUserRamEnd();
    u32 arenaSize = (u32)userRamEnd - (u32)userRamStart;
    mpArenaInfo->setStringf("arena: 0x%08X - 0x%08X, %d kb total",
                            (u32)userRamStart, (u32)userRamEnd, arenaSize / 1024);
    mpArenaInfo->draw(mXPos, y_pos, COLOR_WHITE);
    y_pos += SECTION_SPACING;

    // Root Heap
    JKRHeap* rootHeap = JKRHeap::getRootHeap();
    if (rootHeap != NULL) {
        u32 rootTotal = rootHeap->getSize();
        u32 rootFree = rootHeap->getTotalFreeSize();
        u32 rootUsed = rootTotal - rootFree;
        mpRootHeapInfo->setStringf("root heap: %d kb total, %d kb used, %d kb free",
                                    rootTotal / 1024, rootUsed / 1024, rootFree / 1024);
        mpRootHeapInfo->draw(mXPos, y_pos, COLOR_WHITE);
        y_pos += LINE_SPACING;

        // Root heap children
        mpRootChildren->setString("  children: system, archive, game, j2d, dbprint, assert, command");
        mpRootChildren->draw(mXPos, y_pos, COLOR_WHITE);
        y_pos += SECTION_SPACING;
    }

    // System Heap (child of root, parent of zelda)
    JKRHeap* sysHeap = JKRHeap::getSystemHeap();
    if (sysHeap != NULL) {
        u32 sysTotal = sysHeap->getSize();
        u32 sysFree = sysHeap->getTotalFreeSize();
        u32 sysUsed = sysTotal - sysFree;
        mpAramInfo->setStringf("system heap: %d kb total, %d kb used, %d kb free",
                                sysTotal / 1024, sysUsed / 1024, sysFree / 1024);
        mpAramInfo->draw(mXPos, y_pos, COLOR_WHITE);
        y_pos += LINE_SPACING;

        mpOverviewTitle->setString("  children: zelda");
        mpOverviewTitle->draw(mXPos, y_pos, COLOR_WHITE);
        y_pos += SECTION_SPACING;
        mpOverviewTitle->setString("Memory Overview");
    }

    // ARAM - full memory info
    // Layout: audio region + graph region (managed by JKRAramHeap) + raw user region
    JKRAram* aramMgr = JKRAram::getManager();
    if (aramMgr != NULL) {
        u32 audioSize = aramMgr->getAudioMemSize();
        u32 graphSize = aramMgr->getGraphMemSize();  // This is what JKRAramHeap manages
        u32 rawUserSize = aramMgr->mAramMemorySize;  // Additional raw user area (may be 0)
        u32 totalAram = audioSize + graphSize + rawUserSize;

        mpRootHeapInfo->setStringf("aram: %d kb total (audio %d, graph %d)",
                                    totalAram / 1024, audioSize / 1024, graphSize / 1024);
        mpRootHeapInfo->draw(mXPos, y_pos, COLOR_WHITE);
        y_pos += LINE_SPACING;

        JKRAramHeap* aramHeap = aramMgr->mAramHeap;
        if (aramHeap != NULL) {
            u32 heapFree = aramHeap->getTotalFreeSize();
            u32 heapUsed = (heapFree <= graphSize) ? (graphSize - heapFree) : 0;
            mpRootChildren->setStringf("  graph heap: %d kb used, %d kb free",
                                        heapUsed / 1024, heapFree / 1024);
            mpRootChildren->draw(mXPos, y_pos, COLOR_WHITE);
        }
    }

    // Draw description
    if (gzInfo_isSubMenuVisible() && gzInfo_getMenuDescription() != NULL) {
        f32 description_y = g_gzInfo.mBackgroundHeight + 25.0f;
        gzInfo_getMenuDescription()->draw(0.0f, description_y, COLOR_WHITE, HBIND_CENTER);
    }
}

void gzHeapsMenu_c::draw() {
    updateDynamicLines();

    if (mShowOverview) {
        drawOverview();
        return;
    }

    gzCursor* l_cursor = gzInfo_getCursor();

    static const f32 Y_ALIGNMENT = 110.0f;
    static const f32 LINE_SPACING = 60.0f;
    static const f32 MAX_VIS_WIDTH = 550.0f;
    static const f32 MIN_PIXELS_PER_BLOCK = 1.0f;

    // Find the maximum heap size for scaling
    int max_size_kb = 0;
    for (int i = 0; i < HEAP_TRACKER_MAX_e; i++) {
        bool hasHeap = mTrackers[i]->mIsAram ? (mTrackers[i]->mpAramHeap != NULL) : (mTrackers[i]->mpHeap != NULL);
        if (hasHeap && mTrackers[i]->mTotalSizeKB > max_size_kb) {
            max_size_kb = mTrackers[i]->mTotalSizeKB;
        }
    }

    for (int i = 0; i < HEAP_TRACKER_MAX_e; i++) {
        bool hasHeap = mTrackers[i]->mIsAram ? (mTrackers[i]->mpAramHeap != NULL) : (mTrackers[i]->mpHeap != NULL);
        if (hasHeap) {
            f32 y_pos = Y_ALIGNMENT + (i * LINE_SPACING);

            mTrackers[i]->mpTitle->draw(mXPos, y_pos - 40.0f, COLOR_WHITE);

            if (mViewMode == 0) {
                mTrackers[i]->mpUsedSize->draw(mXPos, y_pos, COLOR_WHITE);
                mTrackers[i]->mpFreeSize->draw(mXPos + 110.0f, y_pos, COLOR_WHITE);
                mTrackers[i]->mpTotalSize->draw(mXPos + 210.0f, y_pos, COLOR_WHITE);
            } else if (mViewMode == 1) {
                mTrackers[i]->mpUsedBlocks->draw(mXPos, y_pos, COLOR_WHITE);
                mTrackers[i]->mpFreeBlocks->draw(mXPos + 110.0f, y_pos, COLOR_WHITE);
                mTrackers[i]->mpTotalBlocks->draw(mXPos + 210.0f, y_pos, COLOR_WHITE);
            } else {
                mTrackers[i]->mpUsedPercent->draw(mXPos, y_pos, COLOR_WHITE);
                mTrackers[i]->mpFreePercent->draw(mXPos + 110.0f, y_pos, COLOR_WHITE);
                mTrackers[i]->mpTotalSize->draw(mXPos + 210.0f, y_pos, COLOR_WHITE);
            }

            mTrackers[i]->mpFragmentation->draw(mXPos + 320.0f, y_pos, COLOR_WHITE);
            mTrackers[i]->mpLargestFree->draw(mXPos + 410.0f, y_pos, COLOR_WHITE);

            f32 size_based_width = (max_size_kb > 0) ? ((f32)mTrackers[i]->mTotalSizeKB / (f32)max_size_kb) * MAX_VIS_WIDTH : MAX_VIS_WIDTH;
            f32 block_based_min = (f32)mTrackers[i]->mNumBlocks * MIN_PIXELS_PER_BLOCK;
            f32 vis_width = MAX(size_based_width, block_based_min);
            if (vis_width > MAX_VIS_WIDTH) {
                vis_width = MAX_VIS_WIDTH;
            }

            drawHeapVisualization(mTrackers[i], mXPos, y_pos - 35.0f, vis_width);
        }
    }

    f32 legend_x = mXPos + 200.0f;
    f32 legend_y = Y_ALIGNMENT - 65.0f;
    drawLegend(legend_x, legend_y);

    if (gzInfo_isSubMenuVisible() && gzInfo_getMenuDescription() != NULL) {
        f32 description_y = g_gzInfo.mBackgroundHeight + 25.0f;
        gzInfo_getMenuDescription()->draw(0.0f, description_y, COLOR_WHITE, HBIND_CENTER);
    }
}
