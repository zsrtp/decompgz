#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_framework.h"
#include "gz/gz_menu_main.h"

char* gzFrameworkMenu_c::getProcessName(base_process_class* process) {
    switch (process->name) {
    case PROC_OPENING_SCENE:
        return "opening scene";
    case PROC_PLAY_SCENE:
        return "play scene";
    case PROC_ROOM_SCENE:
        return "room scene";
    case PROC_NAME_SCENE:
        return "name scene";
    case PROC_LOGO_SCENE:
        return "logo scene";
    case PROC_MENU_SCENE:
        return "menu scene";
    default:
        return "unk";
    }
}

static void deleteProcess(void* proc) {
    base_process_class* i_proc = (base_process_class*)proc;
    if (fpcBs_IsDelete(i_proc)) fpcBs_Delete(i_proc);
}

static void returnToFramework() {
    // gzChangeMenu<gzFrameworkMenu_c>();
}

gzFrameworkMenu_c::gzFrameworkMenu_c() {
    OSReport("creating gzFrameworkMenu_c\n");

    mNumProcesses = 0;
    mSelectedProcess = 0;
    mScrollOffset = 0;

    for (int i = 0; i < MAX_VISIBLE_ROWS * NUM_COLUMNS; i++) {
        mpRowTexts[i] = new gzTextBox();
    }

    for (int i = 0; i < NUM_COLUMNS; i++) {
        mpHeaders[i] = new gzTextBox();
    }

    mpHeaders[0]->setString("Name");
    mpHeaders[1]->setString("PID");
    mpHeaders[2]->setString("Status");

    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpDrawCursor->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    mpDrawCursor->setAlphaRate(1.0f);

    mpMeterHaihai = new dMeterHaihai_c(3);
    mpTitle = new gzTextBox();
}

gzFrameworkMenu_c::~gzFrameworkMenu_c() {
    _delete();
}

void gzFrameworkMenu_c::_delete() {
    OSReport("deleting gzFrameworkMenu_c\n");

    for (int i = 0; i < MAX_VISIBLE_ROWS * NUM_COLUMNS; i++) {
        delete mpRowTexts[i];
        mpRowTexts[i] = NULL;
    }

    for (int i = 0; i < NUM_COLUMNS; i++) {
        delete mpHeaders[i];
        mpHeaders[i] = NULL;
    }

    delete mpDrawCursor;
    mpDrawCursor = NULL;

    delete mpMeterHaihai;
    mpMeterHaihai = NULL;

    delete mpTitle;
    mpTitle = NULL;
}

void gzFrameworkMenu_c::setActiveProcesses() {
    // Collect all active processes
    mNumProcesses = 0;
    layer_class* layer = fpcLy_RootLayer();
    while (layer != NULL && mNumProcesses < MAX_PROCESSES) {
        for (int nl_idx = 0; nl_idx < layer->node_tree.mNumLists; nl_idx++) {
            node_list_class* list = &layer->node_tree.mpLists[nl_idx];
            if (list->mSize > 0) {
                node_class* node = list->mpHead;
                while (node != NULL && mNumProcesses < MAX_PROCESSES) {
                    create_tag_class* tag = (create_tag_class*)node;
                    base_process_class* process = (base_process_class*)tag->mpTagData;
                    if (process != NULL) {
                        mProcessInfos[mNumProcesses].layer_id = layer->layer_id;
                        mProcessInfos[mNumProcesses].node_list_index = nl_idx;
                        mProcessInfos[mNumProcesses].process = process;
                        mNumProcesses++;
                    }
                    node = node->mpNextNode;
                }
            }
        }
        layer = (layer_class*)layer->node.mpNextNode;
    }

    // Clamp selected process
    if (mNumProcesses > 0) {
        if (mSelectedProcess >= mNumProcesses) {
            mSelectedProcess = mNumProcesses - 1;
        }
    } else {
        mSelectedProcess = 0;
    }

    // Clamp scroll offset
    if (mScrollOffset > mNumProcesses - MAX_VISIBLE_ROWS) {
        mScrollOffset = mNumProcesses - MAX_VISIBLE_ROWS;
    }
    if (mScrollOffset < 0) {
        mScrollOffset = 0;
    }
}

void gzFrameworkMenu_c::execute() {
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return;
    }
    
    gzCursor* l_cursor = gzInfo_getCursor();

    if (gzPad::getTrigDown()) {
        if (mSelectedProcess < mNumProcesses - 1) {
            mSelectedProcess++;
            if (mSelectedProcess >= mScrollOffset + MAX_VISIBLE_ROWS) {
                mScrollOffset = mSelectedProcess - MAX_VISIBLE_ROWS + 1;
            }
        } else if (mNumProcesses > 0) {
            mSelectedProcess = 0;
            mScrollOffset = 0;
        }

        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigUp()) {
        if (mSelectedProcess > 0) {
            mSelectedProcess--;
            if (mSelectedProcess < mScrollOffset) {
                mScrollOffset = mSelectedProcess;
            }
        } else if (mNumProcesses > 0) {
            mSelectedProcess = mNumProcesses - 1;
            if (mNumProcesses > MAX_VISIBLE_ROWS) {
                mScrollOffset = mNumProcesses - MAX_VISIBLE_ROWS;
            } else {
                mScrollOffset = 0;
            }
        }

        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigB()) {
        l_cursor->x--;
        l_cursor->y = gzMainMenu_c::MENU_FRAMEWORK;
        gzInfo_seStart(Z2SE_SY_EXP_WIN_CLOSE);
        g_gzInfo.mpMainMenu->startReverseTransition();
        return;
    }

    if (gzPad::getTrigA()) {
        mProcessInfos[mSelectedProcess].process->pause_flag = !mProcessInfos[mSelectedProcess].process->pause_flag;
        gzInfo_seStart(Z2SE_SY_OPTION_SWITCH);
    }

    if (gzPad::getTrigZ()) {
        // char* buf;
        // sprintf(buf, "delete %s?", getProcessName(mProcessInfos[mSelectedProcess].process));
        gzInfo_seStart(Z2SE_SY_FILE_DELETE_OK);
        // gzChangeMenu<gzConfirmMenu_c>(deleteProcess, mProcessInfos[mSelectedProcess].process, returnToFramework, buf);
        // return;
    }

    mpMeterHaihai->_execute(0);
}

void gzFrameworkMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();

    static const f32 LINE_SPACING = 22.0f;
    static const f32 CURSOR_X = 20.0f;
    static const f32 CURSOR_Y_OFFSET = -5.0f;
    static const f32 HAIHAI_Y_SIZE = MAX_VISIBLE_ROWS * 26.0f;
    static const f32 HAIHAI_SCALE_FACTOR = 0.04f;

    f32 X_POS[NUM_COLUMNS] = {mXPos, mXPos+140.0f, mXPos+210.0f};
    f32 X_TITLE = mXPos+50.0f;
    f32 HAIHAI_X = mXPos;
    f32 Y_HEADER = g_gzInfo.mBackgroundYPos + 78.0f;
    f32 Y_TITLE = Y_HEADER - 25.0f;
    f32 Y_START = Y_HEADER + 22.0f;
    f32 HAIHAI_Y = Y_START + 80.0f;

    u32 cursor_color = gzInfo_getCursorColor();

    J2DTextBox::TFontSize font_size;
    mpHeaders[0]->getFontSize(font_size); // assume all have the same font size
    mpMeterHaihai->setScale(font_size.mSizeY * HAIHAI_SCALE_FACTOR);

    // Draw title
    mpTitle->setStringf("(%d processes)", mNumProcesses);
    mpTitle->draw(X_TITLE, Y_TITLE, COLOR_WHITE);

    // Draw headers
    for (int c = 0; c < NUM_COLUMNS; c++) {
        mpHeaders[c]->draw(X_POS[c], Y_HEADER, COLOR_WHITE);
    }

    // Set active processes
    setActiveProcesses();

    // Draw visible rows
    for (int i = 0; i < MAX_VISIBLE_ROWS; i++) {
        int proc_idx = mScrollOffset + i;
        if (proc_idx >= mNumProcesses) {
            break;
        }

        ProcessInfo& info = mProcessInfos[proc_idx];
        mpRowTexts[i * NUM_COLUMNS + 0]->setString(getProcessName(info.process));
        mpRowTexts[i * NUM_COLUMNS + 1]->setStringf("%d", info.process->name);
        mpRowTexts[i * NUM_COLUMNS + 2]->setString(info.process->pause_flag ? "paused" : "active");
        
        u32 color = (proc_idx == mSelectedProcess && gzInfo_isSubMenuVisible()) ? cursor_color : COLOR_WHITE;

        for (int c = 0; c < NUM_COLUMNS; c++) {
            mpRowTexts[i * NUM_COLUMNS + c]->draw(X_POS[c], Y_START + (i * LINE_SPACING), color);
        }
    }

    // Draw cursor if applicable
    if (gzInfo_isCursorTypeTP() && mNumProcesses > 0) {
        int sel_row = mSelectedProcess - mScrollOffset;
        f32 cursor_y = Y_START + (sel_row * LINE_SPACING) + CURSOR_Y_OFFSET;
        mpDrawCursor->setPos(CURSOR_X, cursor_y, (J2DPane*)mpRowTexts[sel_row * NUM_COLUMNS], true);
        mpDrawCursor->draw();
    }

    // Draw haihai arrows
    u8 arrows = 0;
    if (mNumProcesses > MAX_VISIBLE_ROWS) {
        if (mScrollOffset > 0) {
            arrows |= ARROW_UP;
        }
        if (mScrollOffset < mNumProcesses - MAX_VISIBLE_ROWS) {
            arrows |= ARROW_DOWN;
        }
    }

    if (arrows != 0 && gzInfo_isSubMenuVisible()) {
        mpMeterHaihai->drawHaihai(arrows, HAIHAI_X, HAIHAI_Y, 0.0f, HAIHAI_Y_SIZE);
    }
}
