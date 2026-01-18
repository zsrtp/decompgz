#ifndef GZ_MENU_HEAPS_H
#define GZ_MENU_HEAPS_H

#include "gz/gz_menu.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRAram.h"

class gzHeapsMenu_c : public gzMenu_c {
public:

    enum gzHeaps_HeapInfoList_e {
        HEAP_ROOT_e,   
        HEAP_SYSTEM_e,

        HEAP_INFO_MAX_e
    };

    enum gzHeaps_HeapTrackerList_e {
        HEAP_ARCHIVE_e,
        HEAP_GAME_e,
        HEAP_J2D_e,
        HEAP_ZELDA_e,
        HEAP_ARAM_e,

        HEAP_TRACKER_MAX_e
    };

    gzHeapsMenu_c();
    ~gzHeapsMenu_c();

    virtual void _delete();
    virtual void execute();
    virtual void draw();
    void drawMagic(s16 i_max, s16 i_magic, f32 i_posX, f32 i_posY);
    void setAlphaMagicChange(bool i_forceSet);
    void drawHeapText();
    void drawFragmentationOverlay(s16 currentMagic);

    class HeapTracker_c {
    public:
        HeapTracker_c(int);
        ~HeapTracker_c();

    public:
        JKRExpHeap* mpHeap;
        JKRAramHeap* mpAramHeap;
        bool mIsAram;
        gzTextBox* mpTitle;
        gzTextBox* mpTotalBlocks;
        gzTextBox* mpUsedBlocks;
        gzTextBox* mpFreeBlocks;
        int mNumBlocks;
        int mPrevNumBlocks;
        int mUsedBlocks;
        int mFreeBlocks;
        int mMaxBlocks;
        JKRExpHeap::CMemBlock** mBlocks;
        JKRAramBlock** mAramBlocks;
        u32* mStarts;
        f32 mFragmentation;
        int mUsedSizeKB;
        int mFreeSizeKB;
        int mTotalSizeKB;
        int mLargestFreeKB;
        gzTextBox* mpFragmentation;
        gzTextBox* mpUsedSize;
        gzTextBox* mpFreeSize;
        gzTextBox* mpTotalSize;
        gzTextBox* mpLargestFree;
        gzTextBox* mpUsedPercent;
        gzTextBox* mpFreePercent;
    };

private:
    void updateDynamicLines();
    void updateHeapTracker(HeapTracker_c* tracker);
    void drawHeapVisualization(HeapTracker_c* tracker, f32 x, f32 y, f32 width);
    void drawLegend(f32 legend_x, f32 legend_y);
    void drawSwatch(f32 x, f32 y, f32 size, u32 color);

private:
    void drawOverview();

private:
    bool mShowKB;
    int mViewMode;
    bool mShowOverview;
    HeapTracker_c* mTrackers[HEAP_TRACKER_MAX_e];
    gzTextBox* mpLegendUsed;
    gzTextBox* mpLegendMenuUsed;
    gzTextBox* mpLegendFree;

    // Overview mode text boxes
    gzTextBox* mpOverviewTitle;
    gzTextBox* mpRootHeapInfo;
    gzTextBox* mpSystemHeapInfo;
    gzTextBox* mpArenaInfo;
    gzTextBox* mpAramInfo;
    gzTextBox* mpRootChildren;
    gzTextBox* mpSystemChildren;
};

#endif // GZ_MENU_HEAPS_H
