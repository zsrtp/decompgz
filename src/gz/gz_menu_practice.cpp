#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_drawlist.h"
#include "gz/gz_menu_practice.h"
#include "gz/gz_menu_main.h"
#include "d/d_select_cursor.h"
#include "m_Do/m_Do_MemCard.h"

gzPracticeMenu_c::gzPracticeMenu_c()
    : mAnypSavesTab(gzSaveLoaderMng_c::CATEGORY_ANYP_e, ANY_LINE_NUM),
      mNoSQSavesTab(gzSaveLoaderMng_c::CATEGORY_NOSQ_e, NOSQ_LINE_NUM),
      mAllDungeonsSavesTab(gzSaveLoaderMng_c::CATEGORY_ALLDUNGEONS_e, ALL_DUNGEONS_LINE_NUM),
      mHundoSavesTab(gzSaveLoaderMng_c::CATEGORY_HUNDO_e, HUNDO_LINE_NUM),
      mGlitchlessSavesTab(gzSaveLoaderMng_c::CATEGORY_GLITCHLESS_e, GLITCHLESS_LINE_NUM) {
    OSReport("creating gzPracticeMenu_c\n");
    mXPos = g_gzInfo.mBackgroundXPos + 195.0f;

    for (int i = 0; i < TAB_MAX; i++) {
        mpTabHeaders[i] = gzTextBox_allocate();
        mpTabHeaders[i]->setFontSize(15.0f,15.0f);
    }

    mpTabHeaders[TAB_ANY]->setString("any%");
    mpTabHeaders[TAB_NOSQ]->setString("no sq");
    mpTabHeaders[TAB_HUNDO]->setString("100%");
    mpTabHeaders[TAB_ALLDUNGEONS]->setString("all dungeons");
    mpTabHeaders[TAB_GLITCHLESS]->setString("glitchless");
    mpTabHeaders[TAB_MEMFILES]->setString("memfiles");

    mMemfileTab.mNamesLoaded = false;
    for (int i = 0; i < MEMFILE_MAX_NUM; i++) mMemfileTab.mpLines[i] = NULL;
    mMemfileTab.mpKeyboard = NULL;
    mMemfileTab.mpConfirm = NULL;
    mMemfileTab.mPendingDeleteSlot = -1;

    mCurrentTab = TAB_ANY;
    gzInfo_resetTopLine();
}

gzPracticeMenu_c::~gzPracticeMenu_c() {
    _delete();
}

void gzPracticeMenu_c::_delete() {
    OSReport("deleting gzPracticeMenu_c\n");

    for (int i = 0; i < TAB_MAX; i++) {
        gzTextBox_free(mpTabHeaders[i]);
        mpTabHeaders[i] = NULL;
    }
}

void gzPracticeMenu_c::execute() {
    if (checkInputWait()) return;
    if (handleBackButton(gzMainMenu_c::MENU_PRACTICE)) return;

    gzCursor* l_cursor = gzInfo_getCursor();
    int current_max_line;

    switch (mCurrentTab) {
    case TAB_ANY:
        mAnypSavesTab.execute();
        current_max_line = ANY_LINE_NUM;
        break;
    case TAB_NOSQ:
        mNoSQSavesTab.execute();
        current_max_line = NOSQ_LINE_NUM;
        break;
    case TAB_ALLDUNGEONS:
        mAllDungeonsSavesTab.execute();
        current_max_line = ALL_DUNGEONS_LINE_NUM;
        break;
    case TAB_HUNDO:
        mHundoSavesTab.execute();
        current_max_line = HUNDO_LINE_NUM;
        break;
    case TAB_GLITCHLESS:
        mGlitchlessSavesTab.execute();
        current_max_line = GLITCHLESS_LINE_NUM;
        break;
    case TAB_MEMFILES:
        if (mMemfileTab.execute() == 0)
            return;
        current_max_line = MEMFILE_MAX_NUM;
        break;
    }

    // Tab switching (uses left/right)
    if (gzPad::getTrigRight()) {
        mCurrentTab = (mCurrentTab + 1) % TAB_MAX;
        l_cursor->y = 0;
        gzInfo_resetTopLine();
        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
    }

    if (gzPad::getTrigLeft()) {
        mCurrentTab = (mCurrentTab - 1 + TAB_MAX) % TAB_MAX;
        l_cursor->y = 0;
        gzInfo_resetTopLine();
        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
    }

    if (current_max_line == 0)
        return;

    handleNavigation(current_max_line);
    finishExecute(current_max_line);
}

void gzPracticeMenu_c::draw() {
    static const f32 TAB_HEADER_OFFSET = 15.0f;

    if (mMemfileTab.mpKeyboard != NULL) {
        mMemfileTab.mpKeyboard->draw();
        return;
    }

    if (mMemfileTab.mpConfirm != NULL) {
        mMemfileTab.mpConfirm->draw();
        return;
    }

    // Set up tab header x positions
    f32 tabXPositions[TAB_MAX];
    f32 tabBaseX = mXPos + TAB_HEADER_OFFSET;
    tabXPositions[TAB_ANY] = tabBaseX;
    tabXPositions[TAB_NOSQ] = tabBaseX + 50.0f;
    tabXPositions[TAB_HUNDO] = tabBaseX + 100.0f;
    tabXPositions[TAB_ALLDUNGEONS] = tabBaseX + 150.0f;
    tabXPositions[TAB_GLITCHLESS] = tabBaseX + 250.0f;
    tabXPositions[TAB_MEMFILES] = tabBaseX + 325.0f;

    // Draw tab headers
    f32 yHeader = g_gzInfo.mBackgroundYPos + gzMenuLayout::TAB_HEADER_Y_OFFSET;
    drawTabHeaders(mpTabHeaders, tabXPositions, TAB_MAX, mCurrentTab, yHeader, gzInfo_getCursorColor());

    // Get current tab's lines and line count
    gzLine** currentLines = NULL;
    int currentLineNum = 0;
    bool isMemfileTab = false;

    switch (mCurrentTab) {
    case TAB_ANY:
        currentLines = mAnypSavesTab.mpLines;
        currentLineNum = ANY_LINE_NUM;
        break;
    case TAB_NOSQ:
        currentLines = mNoSQSavesTab.mpLines;
        currentLineNum = NOSQ_LINE_NUM;
        break;
    case TAB_ALLDUNGEONS:
        currentLines = mAllDungeonsSavesTab.mpLines;
        currentLineNum = ALL_DUNGEONS_LINE_NUM;
        break;
    case TAB_HUNDO:
        currentLines = mHundoSavesTab.mpLines;
        currentLineNum = HUNDO_LINE_NUM;
        break;
    case TAB_GLITCHLESS:
        currentLines = mGlitchlessSavesTab.mpLines;
        currentLineNum = GLITCHLESS_LINE_NUM;
        break;
    case TAB_MEMFILES:
        currentLineNum = MEMFILE_MAX_NUM;
        isMemfileTab = true;
        break;
    }

    s32 topLine = gzInfo_getTopLine();

    if (isMemfileTab) {
        // Memfile tab uses gzTextBox* instead of gzLine*, draw manually
        mMemfileTab.draw(mXPos);
    } else if (currentLines != NULL) {
        drawLines(currentLines, currentLineNum, 0, topLine, gzMenuLayout::VISIBLE_LINES);
    }
}

void gzPracticeMenu_c::gzMemfileTab_c::create() {
    memset(mMemfileStates, 0, sizeof(mMemfileStates));

    for (int i = 0; i < MEMFILE_MAX_NUM; i++) {
        mpLines[i] = gzTextBox_allocate();
        if (mpLines[i] != NULL) {
            mpLines[i]->setStringf("%d. -- empty --", i + 1);
        }
    }

    mpKeyboard = NULL;
    mpConfirm = NULL;
    mPendingDeleteSlot = -1;

    // Now read memfile names from memcard
    readMemfileNames();
}

int gzPracticeMenu_c::gzMemfileTab_c::readMemfileNames() {
    CARDFileInfo file;
    int ret;

    ret = CARDProbeEx(0, NULL, NULL);
    if (ret != CARD_RESULT_READY) {
        OSReport("readMemfileNames: card not ready\n");
        return 0;
    }

    for (int i = 0; i < MEMFILE_MAX_NUM; i++) {
        char filename_buf[9];
        sprintf(filename_buf, "tpgz_s%02d", i + 1);

        ret = CARDOpen(0, filename_buf, &file);
        if (ret == CARD_RESULT_READY) {
            // read name section from each memfile
            // TODO: can this be optimized?
            ret = CARDRead(&file, mDoMemCd_Ctrl_c::sTmpBuf, SECTOR_SIZE, 0);
            if (ret == CARD_RESULT_READY) {
                gzSaveLoaderMng_c::memfileExData_s* exdata = (gzSaveLoaderMng_c::memfileExData_s*)(mDoMemCd_Ctrl_c::sTmpBuf + sizeof(dSv_save_c));

                OSReport("read memfile name (%d. %s)\n", i + 1, exdata->name);
                mpLines[i]->setStringf("%d. %s", i + 1, exdata->name);
                setMemfileExists(i, true);
            } else {
                OSReport("readMemfileNames: read error (%d)\n", ret);
            }
            CARDClose(&file);
        }
    }

    return 1;
}

int gzPracticeMenu_c::gzMemfileTab_c::memfileNameFinishCb(gzKeyboard_c* i_keyboard, void* i_data) {
    gzPracticeMenu_c::gzMemfileTab_c* menu = (gzPracticeMenu_c::gzMemfileTab_c*)i_data;
    gzCursor* l_cursor = gzInfo_getCursor();
    int slot_no = l_cursor->y + 1;

    CARDFileInfo file;
    int ret;

    ret = CARDProbeEx(0, NULL, NULL);
    if (ret != CARD_RESULT_READY) {
        return 0;
    }

    char filename_buf[9];
    sprintf(filename_buf, "tpgz_s%02d", slot_no);

    ret = CARDCreate(0, filename_buf, SECTOR_SIZE, &file);
    if (ret == CARD_RESULT_READY || ret == CARD_RESULT_EXIST) {
        ret = CARDOpen(0, filename_buf, &file);
        if (ret == CARD_RESULT_READY) {
            // overwrite the save location to avoid the game trying to normalize it to something we dont want
            dSv_save_c* savedata = dComIfGs_getSaveData();
            savedata->getPlayer().getPlayerReturnPlace().set(dComIfGp_getStartStageName(), dComIfGp_roomControl_getStayNo(), 0);

            // write main save data
            memcpy(mDoMemCd_Ctrl_c::sTmpBuf, dComIfGs_getSaveData(), sizeof(dSv_save_c));

            // write any extra data we want
            gzSaveLoaderMng_c::memfileExData_s exdata;
            strcpy(exdata.name, i_keyboard->mString);
            exdata.player_pos.set(dComIfGp_getPlayer(0)->current.pos);

            memcpy(mDoMemCd_Ctrl_c::sTmpBuf + sizeof(dSv_save_c), &exdata, sizeof(gzSaveLoaderMng_c::memfileExData_s));

            ret = CARDWrite(&file, mDoMemCd_Ctrl_c::sTmpBuf, SECTOR_SIZE, 0);
            if (ret == CARD_RESULT_READY) {
                OSReport("saved memfile (%d. %s) to memcard!\n", slot_no, i_keyboard->mString);
                gzInfo_sendNotification("memfile saved!");
                menu->mpLines[l_cursor->y]->setStringf("%d. %s", slot_no, i_keyboard->mString);
                menu->setMemfileExists(l_cursor->y, true);
                g_gzInfo.mDisplay = false;
            }

            CARDClose(&file);
        }
    }

    return 1;
}

int gzPracticeMenu_c::gzMemfileTab_c::loadMemfile(int i_no) {
    CARDFileInfo file;
    int ret;

    ret = CARDProbeEx(0, NULL, NULL);
    if (ret != CARD_RESULT_READY) {
        return -1;
    }

    char filename_buf[9];
    sprintf(filename_buf, "tpgz_s%02d", i_no + 1);

    ret = CARDOpen(0, filename_buf, &file);
    if (ret == CARD_RESULT_READY) {
        ret = CARDRead(&file, mDoMemCd_Ctrl_c::sTmpBuf, SECTOR_SIZE, 0);
        if (ret == CARD_RESULT_READY) {
            OSReport("loadMemfile: read memfile from memcard\n");
            gzInfo_sendNotification("memfile loaded!");

            dSv_save_c* savep = (dSv_save_c*)mDoMemCd_Ctrl_c::sTmpBuf;
            dComIfGp_setNextStage(savep->getPlayer().getPlayerReturnPlace().getName(),
                                savep->getPlayer().getPlayerReturnPlace().getPlayerStatus(),
                                savep->getPlayer().getPlayerReturnPlace().getRoomNo(),
                                -1);

            OS_REPORT("attempting to load stage: [S]%s [R]%d [P]%d\n",
                     savep->getPlayer().getPlayerReturnPlace().getName(),
                     savep->getPlayer().getPlayerReturnPlace().getRoomNo(),
                     savep->getPlayer().getPlayerReturnPlace().getPlayerStatus());

            gzSaveLoaderMng_c::memfileExData_s* exdata = (gzSaveLoaderMng_c::memfileExData_s*)(mDoMemCd_Ctrl_c::sTmpBuf + sizeof(dSv_save_c));
            memcpy(&g_gzInfo.mSaveLoaderMng.mMemfileExData, exdata, sizeof(gzSaveLoaderMng_c::memfileExData_s));

            g_gzInfo.mSaveLoaderMng.setMode(gzSaveLoaderMng_c::MODE_MEMFILE_e);
            g_gzInfo.mSaveLoaderMng.start();

            g_gzInfo.mDisplay = false;
            dDlst_list_c::wipeIn(1.0f);
        }

        CARDClose(&file);
    }

    return ret;
}

int gzPracticeMenu_c::gzMemfileTab_c::deleteMemfile(int i_slotNo) {
    CARDFileInfo file;
    int ret;

    ret = CARDProbeEx(0, NULL, NULL);
    if (ret != CARD_RESULT_READY) {
        return 0;
    }

    char filename_buf[9];
    sprintf(filename_buf, "tpgz_s%02d", i_slotNo);

    ret = CARDDelete(0, filename_buf);
    if (ret == CARD_RESULT_READY) {
        OSReport("deleted memfile %d\n", i_slotNo);
        gzInfo_sendNotification("memfile deleted!");
        mpLines[i_slotNo - 1]->setStringf("%d. -- empty --", i_slotNo);
        setMemfileExists(i_slotNo - 1, false);
    }

    return ret;
}

int gzPracticeMenu_c::gzMemfileTab_c::memfileDeleteConfirmCb(gzConfirm_c* i_confirm, void* i_data) {
    gzPracticeMenu_c::gzMemfileTab_c* tab = (gzPracticeMenu_c::gzMemfileTab_c*)i_data;

    if (tab->mPendingDeleteSlot >= 0) {
        tab->deleteMemfile(tab->mPendingDeleteSlot + 1);
        tab->mPendingDeleteSlot = -1;
    }

    return 1;
}

int gzPracticeMenu_c::gzMemfileTab_c::memfileDeleteReturnCb(gzConfirm_c* i_confirm, void* i_data) {
    gzPracticeMenu_c::gzMemfileTab_c* tab = (gzPracticeMenu_c::gzMemfileTab_c*)i_data;
    tab->mPendingDeleteSlot = -1;
    return 1;
}

int gzPracticeMenu_c::gzMemfileTab_c::execute() {
    // Lazy load on first access to avoid blocking boot
    if (!mNamesLoaded) {
        create();
        mNamesLoaded = true;
    }

    gzCursor* l_cursor = gzInfo_getCursor();

    if (mpKeyboard != NULL) {
        int rt = mpKeyboard->execute();
        if (rt == 1 || rt == 2) {
            delete mpKeyboard;
            mpKeyboard = NULL;
        }
        return 0;
    }

    if (mpConfirm != NULL) {
        int rt = mpConfirm->execute();
        if (rt == 1 || rt == 2) {
            delete mpConfirm;
            mpConfirm = NULL;
        }
        return 0;
    }

    if (gzPad::getTrigA()) {
        if (isMemfileExist(l_cursor->y)) {
            loadMemfile(l_cursor->y);
        } else {
            mpKeyboard = new gzKeyboard_c(memfileNameFinishCb, NULL, this);
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
        }
    }

    if (gzPad::getTrigZ()) {
        if (isMemfileExist(l_cursor->y)) {
            mPendingDeleteSlot = l_cursor->y;
            mpConfirm = new gzConfirm_c(memfileDeleteConfirmCb, memfileDeleteReturnCb, this, "delete memfile?");
            gzInfo_seStart(Z2SE_SY_CURSOR_OK);
        }
    }

    return 1;
}

void gzPracticeMenu_c::gzMemfileTab_c::draw(f32 xPos) {
    gzCursor* l_cursor = gzInfo_getCursor();
    u32 cursorColor = gzInfo_getCursorColor();
    s32 topLine = gzInfo_getTopLine();

    f32 lineX = xPos;
    f32 lineY_start = g_gzInfo.mBackgroundYPos + gzMenuLayout::Y_ALIGNMENT;
    f32 line_spacing = gzMenuLayout::LINE_SPACING;

    s32 endLine = topLine + gzMenuLayout::VISIBLE_LINES;
    if (endLine > MEMFILE_MAX_NUM) endLine = MEMFILE_MAX_NUM;

    for (s32 i = topLine; i < endLine; i++) {
        if (mpLines[i] == NULL) continue;

        s32 screenIdx = i - topLine;
        f32 lineY = lineY_start + (screenIdx * line_spacing);
        bool isSelected = (l_cursor->y == i && gzInfo_isSubMenuVisible());
        u32 color = isSelected ? cursorColor : COLOR_WHITE;

        mpLines[i]->draw(lineX, lineY, color);

        // Draw TP cursor for selected line
        if (isSelected && gzInfo_isCursorTypeTP() && gzInfo_getTPCursor() != NULL) {
            mpLines[i]->updateBounds();
            // setPos expects center position, so calculate center of text
            f32 cursorX = lineX + (mpLines[i]->getWidth() / 2.0f) + gzMenuLayout::TP_CURSOR_X_OFFSET;
            f32 cursorY = lineY + (mpLines[i]->getHeight() / 2.0f) + gzMenuLayout::TP_CURSOR_Y_OFFSET;
            gzInfo_getTPCursor()->setPos(cursorX, cursorY, (J2DPane*)mpLines[i], false);
            gzSetup2DContext();
            gzInfo_getTPCursor()->draw();
        }
    }
}

void gzPracticeMenu_c::gzSavesTab_c::create() {
    int save_num = g_gzInfo.mSaveLoaderMng.getSaveEntryNum((gzSaveLoaderMng_c::SaveCategory_e)mCategory);

    mpLines = new gzLine*[mMaxLines];
    gzSaveLoaderMng_c::saveMetadata_s ATTRIBUTE_ALIGN(32) metadata;
    for (int i = 0; i < mMaxLines; i++) {
        if (i < save_num) {
            g_gzInfo.mSaveLoaderMng.getSaveMetadata((gzSaveLoaderMng_c::SaveCategory_e)mCategory, i, &metadata);
            mpLines[i] = new gzLine(metadata.name, metadata.desc);
        } else {
            mpLines[i] = new gzLine("", "");
        }
    }
}

int gzPracticeMenu_c::gzSavesTab_c::execute() {
    if (!mLoaded) {
        create();
        mLoaded = true;
    }

    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigA()) {
        g_gzInfo.mSaveLoaderMng.loadSave((gzSaveLoaderMng_c::SaveCategory_e)mCategory, l_cursor->y);
        gzInfo_seStart(Z2SE_SY_CURSOR_OK);
    }

    return 1;
}
