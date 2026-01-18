#ifndef GZ_MENU_TOOLS_H
#define GZ_MENU_TOOLS_H

#include "gz/gz_menu.h"
#include "gz/gz_textbox.h"

class gzToolsMenu_c : public gzMenu_c {
public:
    enum gzToolsMenuTabs_e {
        TAB_CHECKERS_e,
        TAB_DISPLAYS_e,
        TAB_LINK_e,

        TAB_MAX_e
    };

    enum gzToolsMenu_Checkers_e {
        C_LINK_DEBUG_INFO,
        C_STAGE_INFO,

        C_MAX
    };

    enum gzToolsMenu_Displays_e {
        D_TIMER,
        D_IN_GAME_TIMER,
        D_LOAD_TIMER,
        D_INPUT_VIEWER,
        D_DISABLE_ITEM_TIMER,

        D_MAX
    };

    enum gzToolsMenu_Link_e {
        L_MOVE_LINK,
        L_FAST_MOVEMENT,
        L_FAST_BONK_RECOVERY,
        L_ROLLING,
        L_MOON_JUMP,
        L_SUPER_CLAWSHOT,
        L_TRANSFORM_ANYWHERE,
        L_NO_SINKING_IN_SAND,
        L_DISABLE_WALLS,
        L_TELEPORT,
        L_AB_MASH_RATE,

        L_MAX
    };

    gzToolsMenu_c();
    ~gzToolsMenu_c();

    virtual void _delete();
    virtual void execute();
    virtual void draw();

private:
    void updateDynamicLines();
    u8 getHaihaiFlags(int idx);
    int getCurrentLineNum();

private:
    gzTextBox* mpTabHeaders[TAB_MAX_e];
    gzBoolOptionLine* mpLinesCheckers[C_MAX];
    gzBoolOptionLine* mpLinesDisplays[D_MAX];
    gzBoolOptionLine* mpLinesLink[L_MAX];

    dMeterHaihai_c* mpMeterHaihai;
    int mCurrentTab;
};

#endif // GZ_MENU_TOOLS_H
