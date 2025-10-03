#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "gz/gz_menu.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "SSystem/SComponent/c_counter.h"

gzInfo_c g_gzInfo;

int gzInfo_c::_create() {
    mCursorColor = 0xEE8000FF; // temp

    ResTIMG* icon = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "midona64.bti");
    mpIcon = new J2DPicture(icon);

    mpHeader = new gzTextBox();
    mpHeader->setFont(mDoExt_getMesgFont());
    mpHeader->setFontSize(18.0f, 18.0f);
    mpHeader->setCharColor(mCursorColor);
    mpHeader->setGradColor(mCursorColor);
    mpHeader->setString("tpgz v1.2.0");
    mpCurrentMenu = new gzMainMenu_c();

    mInputWaitTimer = 5;

    mGZInitialized = true;

    // JUTDbPrint::getManager()->changeFont(mDoExt_getMesgFont());
    return 1;
}

int gzInfo_c::_delete() {
    delete mpIcon;
    mpIcon = NULL;

    delete mpHeader;
    mpHeader = NULL;

    delete mpCurrentMenu;
    mpCurrentMenu = NULL;
    return 1;
}

int gzInfo_c::execute() {
    if (!mGZInitialized) return 0;

    if (gzPad::getHoldL() && gzPad::getHoldR() && gzPad::getTrigDown()) {
        mDisplay = !mDisplay;

        if (mDisplay)
            g_gzInfo.mInputWaitTimer = 5;
    }

    if (mDisplay) {
        if (mInputWaitTimer != 0) {
            mInputWaitTimer--;
            return 1;
        }

        mpCurrentMenu->execute();
    }

    return 1;
}

int gzInfo_c::draw() {
    if (!mGZInitialized) return 0;

    if (mDisplay) {
        if (mpIcon != NULL) {
            mpIcon->draw(30.0f, 5.0f, 30.0f, 30.0f, false, false, false);
        }

        if (mpHeader != NULL) {
            mpHeader->draw(65.0f, 30.0f, mCursorColor, true);
        }

        dComIfGd_set2DOpaTop(mpCurrentMenu);
    }

    // temp heap print
    if (zeldaHeap != NULL && gameHeap != NULL && archiveHeap != NULL) {
        u32 zeldaFree = zeldaHeap->getFreeSize();
        u32 gameFree = gameHeap->getFreeSize();
        u32 archiveFree = archiveHeap->getFreeSize();
        u32 zeldaTotal = zeldaHeap->getTotalFreeSize();
        u32 gameTotal = gameHeap->getTotalFreeSize();
        u32 archiveTotal = archiveHeap->getTotalFreeSize();

        gzPrint(200, 30, 0xFFFFFFFF, "  Zelda %5d / %5d\n", zeldaFree / 1024, zeldaTotal / 1024);
        gzPrint(200, 50, 0xFFFFFFFF, "   Game %5d / %5d\n", gameFree / 1024, gameTotal / 1024);
        gzPrint(200, 70, 0xFFFFFFFF, "Archive %5d / %5d\n", archiveFree / 1024, archiveTotal / 1024);
    }

    return 1;
}

int gzPrint(int x, int y, u32 color, char const* string, ...) {
    JUTDbPrint::getManager()->setVisible(true);
    char buffer[256];

    va_list list;
    va_start(list, string);
    vsnprintf(buffer, sizeof(buffer), string, list);
    va_end(list);

    JUTDbPrint::getManager()->flush();

    static JUtility::TColor ShadowDarkColor(0, 0, 0, 0x80);
    JUTDbPrint::getManager()->setCharColor(ShadowDarkColor);

    JUTReport(x + 2, y + 2, buffer);
    JUTDbPrint::getManager()->flush();

    JUTDbPrint::getManager()->setCharColor(color);
    JUTReport(x, y, buffer);
    JUTDbPrint::getManager()->flush();
    return 1;
}
