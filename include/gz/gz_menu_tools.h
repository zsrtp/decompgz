#ifndef GZ_MENU_TOOLS_H
#define GZ_MENU_TOOLS_H

#include "gz/gz_menu.h"
#include "gz/gz_tab.h"

class gzToolsMenu_c : public gzMenu_c {
public:
    enum gzToolsMenuTabs_e {
        TAB_CHECKERS_e,
        TAB_DISPLAYS_e,
        TAB_LINK_e,

        TAB_MAX_e
    };

    gzToolsMenu_c();
    ~gzToolsMenu_c();
    virtual void execute();
    virtual void draw();

    gzTextBox* mpTabHeaders[TAB_MAX_e];
    int mCurrentTab;
    int mTopLine;
    gzTab_c mTabs[TAB_MAX_e];
    gzTextBox* mpDescription;
    dSelect_cursor_c* mpDrawCursor;
    dMeterHaihai_c* mpMeterHaihai;
};

#endif // GZ_MENU_TOOLS_H
