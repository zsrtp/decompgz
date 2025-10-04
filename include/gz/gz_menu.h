#ifndef GZ_MENU_H
#define GZ_MENU_H

#include "d/d_drawlist.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "gz/gz.h"


class gzMenu_c : public dDlst_base_c {
public:
    struct gzCursor {
        int x;
        int y;
    };

    virtual void _delete() {}
    virtual void execute() {}
};

class gzMainMenu_c : public gzMenu_c {
public:

    enum gzMainMenu_Menus_e {
        MENU_CHEATS,
        MENU_FLAGS,
        MENU_INVENTORY,
        MENU_MEMORY,
        MENU_PRACTICE,
        MENU_SCENE,
        MENU_SETTINGS,
        MENU_TOOLS,
        MENU_WARPING,
    };

    gzMainMenu_c();
    ~gzMainMenu_c();

    virtual void _delete();
    virtual void execute();
    virtual void draw();

    static const int LINE_NUM = 9;
    static gzCursor mCursor;

    gzTextBox* mpLines[LINE_NUM];
};

class gzSettingsMenu_c : public gzMenu_c {
public:
    enum gzSettingsMenu_Settings_e {
        SETTING_AREA_RELOAD_BEHAVIOR,
        SETTING_CURSOR_COLOR,
        SETTING_FONT,
        SETTING_DROP_SHADOW,
        SETTING_SWAP_EQUIPS,
        SETTING_PROGRESSIVE_MODE,
        SETTING_SAVE_CARD,
        SETTING_LOAD_CARD,
        SETTING_DELETE_CARD,
        SETTING_COMMAND_COMBOS,
        SETTING_MENU_POSITIONS,
        SETTING_CREDITS,

        SETTING_MAX
    };

    gzSettingsMenu_c();
    ~gzSettingsMenu_c();
    void updateDynamicLines();

    virtual void _delete();
    virtual void execute();
    virtual void draw();

    static const int LINE_NUM = SETTING_MAX;
    static gzCursor mCursor;

public:
    gzTextBox* mpLines[LINE_NUM];
};

class gzCreditsMenu_c : public gzMenu_c {
public:
    gzCreditsMenu_c();
    ~gzCreditsMenu_c();

    virtual void _delete();
    virtual void execute();
    virtual void draw();

public:
    // gzTextBox* mpLines[LINE_NUM];
};

template <typename T>
inline void gzChangeMenu() {
    gzMenu_c* next = new T();
    g_gzInfo.mpCurrentMenu->_delete();
    delete g_gzInfo.mpCurrentMenu;
    g_gzInfo.mpCurrentMenu = next;

    g_gzInfo.mInputWaitTimer = 5;
}

#endif
