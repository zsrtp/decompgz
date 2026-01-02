#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_practice.h"
#include "gz/gz_menu_main.h"
#include "m_Do/m_Do_MemCard.h"

gzPracticeMenu_c::gzPracticeMenu_c() {
    OSReport("creating gzPracticeMenu_c\n");

    for (int i = 0; i < TAB_MAX; i++) {
        mpTabHeaders[i] = new gzTextBox;
        mpTabHeaders[i]->setFontSize(15.0f,15.0f);
    }

    mpTabHeaders[TAB_ANY]->setString("any%");
    mpTabHeaders[TAB_NOSQ]->setString("no sq");
    mpTabHeaders[TAB_HUNDO]->setString("100%");
    mpTabHeaders[TAB_ALLDUNGEONS]->setString("all dungeons");
    mpTabHeaders[TAB_GLITCHLESS]->setString("glitchless");
    mpTabHeaders[TAB_MEMFILES]->setString("memfiles");

    mAnypSavesTab.create();
    mNoSQSavesTab.create();
    mAllDungeonsSavesTab.create();
    mHundoSavesTab.create();
    mGlitchlessSavesTab.create();
    mMemfileTab.create();

    mpDescription = new gzTextBox();

    mTopLine = 0;

    mpMeterHaihai = new dMeterHaihai_c(3);
    mpMeterHaihai->setScale(0.04f);
}

gzPracticeMenu_c::~gzPracticeMenu_c() {
    _delete();
}

void gzPracticeMenu_c::_delete() {
    OSReport("deleting gzPracticeMenu_c\n");

    for (int i = 0; i < TAB_MAX; i++) {
        delete mpTabHeaders[i];
        mpTabHeaders[i] = NULL;
    }
}

void gzPracticeMenu_c::execute() {
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return;
    }

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

    if (gzPad::getTrigRight()) {
        mCurrentTab = (mCurrentTab + 1) % TAB_MAX;
        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
    }

    if (gzPad::getTrigLeft())  {
        mCurrentTab = (mCurrentTab - 1 + TAB_MAX) % TAB_MAX;
        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
    }

    if (gzPad::getTrigB()) {
        l_cursor->x--;
        l_cursor->y = gzMainMenu_c::MENU_PRACTICE;
        gzInfo_seStart(Z2SE_SY_EXP_WIN_CLOSE);
        g_gzInfo.mpMainMenu->startReverseTransition();
        return;
    }

    mpMeterHaihai->_execute(0);

    if (current_max_line == 0)
        return;

    if (gzPad::getTrigDown()) {
        l_cursor->y = (l_cursor->y + 1) % current_max_line;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigUp()) {
        l_cursor->y = (l_cursor->y - 1 + current_max_line) % current_max_line;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }
}

void gzPracticeMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();

    static const f32 LINE_SPACING = 22.0f;
    static const f32 HAIHAI_X_OFFSET = 225.0f;
    static const f32 HAIHAI_X_SIZE = 460.0f;
    static const f32 TAB_HEADER_OFFSET = 15.0f;
    static const f32 DESCRIPTION_X = 0.0f;
    static const int VISIBLE_LINES = 15;

    // manually set tab header text distances for now
    // need to support scrolling tabs at some point
    f32 X_POS[TAB_MAX];
    f32 tab_header_x_alignment = mXPos + TAB_HEADER_OFFSET;
    X_POS[TAB_ANY] = tab_header_x_alignment;
    X_POS[TAB_NOSQ] = tab_header_x_alignment + 50.0f;
    X_POS[TAB_HUNDO] = tab_header_x_alignment + 100.0f;
    X_POS[TAB_ALLDUNGEONS] = tab_header_x_alignment + 150.0f;
    X_POS[TAB_GLITCHLESS] = tab_header_x_alignment + 250.0f;
    X_POS[TAB_MEMFILES] = tab_header_x_alignment + 325.0f;

    u32 cursor_color = gzInfo_getCursorColor();

    f32 y_header_alignment = g_gzInfo.mBackgroundYPos + 48.0f;
    f32 y_lines_alignment = y_header_alignment + 42.0f;
    f32 x_alignment_haihai = mXPos + HAIHAI_X_OFFSET;
    f32 y_alignment_haihai = y_header_alignment - 5.0f;

    int current_max_line;
    gzTextBox** currentLines;

    // TODO: extract tab drawing to separate functions
    if (mMemfileTab.mpKeyboard != NULL) {
        mMemfileTab.mpKeyboard->draw();
        return;
    }

    // Draw tab headers
    for (int i = 0; i < TAB_MAX; i++) {
        // only draw if it doesnt go past the bounds of the menu
        // TODO: fetch this magic number from gzInfo instead
        if (X_POS[i] <= 550.0f) mpTabHeaders[i]->draw(X_POS[i], y_header_alignment, i == mCurrentTab ? cursor_color : COLOR_WHITE);
    }

    switch (mCurrentTab) {
    case TAB_ANY:
        current_max_line = ANY_LINE_NUM;
        currentLines = mAnypSavesTab.mpLines;
        break;
    case TAB_NOSQ:
        current_max_line = NOSQ_LINE_NUM;
        currentLines = mNoSQSavesTab.mpLines;
        break;
    case TAB_ALLDUNGEONS:
        current_max_line = ALL_DUNGEONS_LINE_NUM;
        currentLines = mAllDungeonsSavesTab.mpLines;
        break;
    case TAB_HUNDO:
        current_max_line = HUNDO_LINE_NUM;
        currentLines = mHundoSavesTab.mpLines;
        break;
    case TAB_GLITCHLESS:
        current_max_line = GLITCHLESS_LINE_NUM;
        currentLines = mGlitchlessSavesTab.mpLines;
        break;
    case TAB_MEMFILES:
        current_max_line = MEMFILE_MAX_NUM;
        currentLines = mMemfileTab.mpLines;
        break;
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

    for (int screenIdx = 0; screenIdx < VISIBLE_LINES; screenIdx++) {
        int lineIdx = mTopLine + screenIdx;
        if (lineIdx >= current_max_line) break;

        if (currentLines[lineIdx] != NULL) {
            f32 y_pos = y_lines_alignment + ((screenIdx - 1) * LINE_SPACING);

            if (l_cursor->y == lineIdx && gzInfo_isSubMenuVisible()) {
                currentLines[lineIdx]->draw(mXPos, y_pos, cursor_color);
            } else {
                currentLines[lineIdx]->draw(mXPos, y_pos, COLOR_WHITE);
            }
        }
    }

    // Draw description if valid and on menu
    if (gzInfo_isSubMenuVisible()) {
        if (currentLines[l_cursor->y] && *currentLines[l_cursor->y]->m_description != 0) {
            f32 description_x = DESCRIPTION_X;
            f32 description_y = g_gzInfo.mBackgroundHeight + 25.0f;

            mpDescription->setString(currentLines[l_cursor->y]->m_description);
            mpDescription->draw(0.0f, description_y, cursor_color, HBIND_CENTER);
        }
    }

    if (mpMeterHaihai != NULL && gzInfo_isSubMenuVisible()) {
        mpMeterHaihai->drawHaihai(ARROW_LEFT | ARROW_RIGHT, x_alignment_haihai, y_alignment_haihai, HAIHAI_X_SIZE, 0.0f);
    }
}

void gzPracticeMenu_c::gzMemfileTab_c::create() {
    for (int i = 0; i < MEMFILE_MAX_NUM; i++) {
        mpLines[i] = new gzTextBox;
        mpLines[i]->setStringf("%d. -- empty --", i + 1);
    }

    readMemfileNames();

    mpKeyboard = NULL;
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
        }

        CARDClose(&file);
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

int gzPracticeMenu_c::gzMemfileTab_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();

    if (mpKeyboard != NULL) {
        int rt = mpKeyboard->execute();
        if (rt == 1 || rt == 2) {
            delete mpKeyboard;
            mpKeyboard = NULL;
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
            deleteMemfile(l_cursor->y + 1);
            gzInfo_seStart(Z2SE_SY_FILE_DELETE_OK);
        }
    }

    return 1;
}

void gzPracticeMenu_c::gzAnypSavesTab_c::create() {
    for (int i = 0; i < ANY_LINE_NUM; i++) {
        mpLines[i] = new gzTextBox;
    }

    int save_num = g_gzInfo.mSaveLoaderMng.getSaveEntryNum(gzSaveLoaderMng_c::CATEGORY_ANYP_e);

    gzSaveLoaderMng_c::saveMetadata_s metadata;
    for (int i = 0; i < save_num; i++) {
        g_gzInfo.mSaveLoaderMng.getSaveMetadata(gzSaveLoaderMng_c::CATEGORY_ANYP_e, i, &metadata);
        mpLines[i]->setStringDesc(metadata.name, metadata.desc);
    }
}

int gzPracticeMenu_c::gzAnypSavesTab_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigA()) {
        g_gzInfo.mSaveLoaderMng.loadSave(gzSaveLoaderMng_c::CATEGORY_ANYP_e, l_cursor->y);
        gzInfo_seStart(Z2SE_SY_CURSOR_OK);
    }

    return 1;
}

void gzPracticeMenu_c::gzHundoSavesTab_c::create() {
    for (int i = 0; i < HUNDO_LINE_NUM; i++) {
        mpLines[i] = new gzTextBox;
    }

    int save_num = g_gzInfo.mSaveLoaderMng.getSaveEntryNum(gzSaveLoaderMng_c::CATEGORY_HUNDO_e);

    gzSaveLoaderMng_c::saveMetadata_s metadata;
    for (int i = 0; i < save_num; i++) {
        g_gzInfo.mSaveLoaderMng.getSaveMetadata(gzSaveLoaderMng_c::CATEGORY_HUNDO_e, i, &metadata);
        mpLines[i]->setStringDesc(metadata.name, metadata.desc);
    }
}

int gzPracticeMenu_c::gzHundoSavesTab_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigA()) {
        g_gzInfo.mSaveLoaderMng.loadSave(gzSaveLoaderMng_c::CATEGORY_HUNDO_e, l_cursor->y);
        gzInfo_seStart(Z2SE_SY_CURSOR_OK);
    }

    return 1;
}

void gzPracticeMenu_c::gzADSavesTab_c::create() {
    for (int i = 0; i < ALL_DUNGEONS_LINE_NUM; i++) {
        mpLines[i] = new gzTextBox;
    }

    int save_num = g_gzInfo.mSaveLoaderMng.getSaveEntryNum(gzSaveLoaderMng_c::CATEGORY_ALLDUNGEONS_e);

    gzSaveLoaderMng_c::saveMetadata_s metadata;
    for (int i = 0; i < save_num; i++) {
        g_gzInfo.mSaveLoaderMng.getSaveMetadata(gzSaveLoaderMng_c::CATEGORY_ALLDUNGEONS_e, i, &metadata);
        mpLines[i]->setStringDesc(metadata.name, metadata.desc);
    }
}

int gzPracticeMenu_c::gzADSavesTab_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigA()) {
        g_gzInfo.mSaveLoaderMng.loadSave(gzSaveLoaderMng_c::CATEGORY_ALLDUNGEONS_e, l_cursor->y);
        gzInfo_seStart(Z2SE_SY_CURSOR_OK);
    }

    return 1;
}

void gzPracticeMenu_c::gzGlitchlessSavesTab_c::create() {
    for (int i = 0; i < GLITCHLESS_LINE_NUM; i++) {
        mpLines[i] = new gzTextBox;
    }

    int save_num = g_gzInfo.mSaveLoaderMng.getSaveEntryNum(gzSaveLoaderMng_c::CATEGORY_GLITCHLESS_e);

    gzSaveLoaderMng_c::saveMetadata_s metadata;
    for (int i = 0; i < save_num; i++) {
        g_gzInfo.mSaveLoaderMng.getSaveMetadata(gzSaveLoaderMng_c::CATEGORY_GLITCHLESS_e, i, &metadata);
        mpLines[i]->setStringDesc(metadata.name, metadata.desc);
    }
}

int gzPracticeMenu_c::gzGlitchlessSavesTab_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigA()) {
        g_gzInfo.mSaveLoaderMng.loadSave(gzSaveLoaderMng_c::CATEGORY_GLITCHLESS_e, l_cursor->y);
        gzInfo_seStart(Z2SE_SY_CURSOR_OK);
    }

    return 1;
}

void gzPracticeMenu_c::gzNoSQSavesTab_c::create() {
    for (int i = 0; i < NOSQ_LINE_NUM; i++) {
        mpLines[i] = new gzTextBox;
    }

    int save_num = g_gzInfo.mSaveLoaderMng.getSaveEntryNum(gzSaveLoaderMng_c::CATEGORY_NOSQ_e);

    gzSaveLoaderMng_c::saveMetadata_s metadata;
    for (int i = 0; i < save_num; i++) {
        g_gzInfo.mSaveLoaderMng.getSaveMetadata(gzSaveLoaderMng_c::CATEGORY_NOSQ_e, i, &metadata);
        mpLines[i]->setStringDesc(metadata.name, metadata.desc);
    }
}

int gzPracticeMenu_c::gzNoSQSavesTab_c::execute() {
    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigA()) {
        g_gzInfo.mSaveLoaderMng.loadSave(gzSaveLoaderMng_c::CATEGORY_NOSQ_e, l_cursor->y);
        gzInfo_seStart(Z2SE_SY_CURSOR_OK);
    }

    return 1;
}
