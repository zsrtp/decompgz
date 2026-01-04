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

    u8 getHaihaiFlags(int i);

private:
    gzTextBox* mpTabHeaders[TAB_MAX_e];
    gzTab_c mTabs[TAB_MAX_e];
    int mCurrentTab;
};

#endif // GZ_MENU_TOOLS_H
