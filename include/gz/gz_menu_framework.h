#ifndef GZ_MENU_FRAMEWORK_H
#define GZ_MENU_FRAMEWORK_H

#include "gz/gz_menu.h"

class gzFrameworkMenu_c : public gzMenu_c {
public:
    struct ProcessInfo {
        u32 layer_id;
        int node_list_index;
        base_process_class* process;
    };

    gzFrameworkMenu_c();
    ~gzFrameworkMenu_c();

    void execute();
    void draw();
    void _delete();

    static const int MAX_VISIBLE_ROWS = 10;
    static const int NUM_COLUMNS = 3;
    static const int MAX_PROCESSES = 128;  // Should be plenty?

private:
    static char* getProcessName(base_process_class* process);
    void setActiveProcesses();

    int mNumProcesses;
    int mSelectedProcess;
    int mScrollOffset;
    gzTextBox* mpTitle;
    gzTextBox* mpRowTexts[MAX_VISIBLE_ROWS * NUM_COLUMNS];
    gzTextBox* mpHeaders[NUM_COLUMNS];
    //gzConfirmMenu_c* mpConfirmMenu;
    dSelect_cursor_c* mpDrawCursor;
    dMeterHaihai_c* mpMeterHaihai;
    ProcessInfo mProcessInfos[MAX_PROCESSES];
    bool mOption;
};

#endif // GZ_MENU_FRAMEWORK_H
