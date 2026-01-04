#ifndef GZ_MENU_SETTINGS_H
#define GZ_MENU_SETTINGS_H

#include "gz/gz_menu_credits.h"

class gzSettingsMenu_c : public gzMenu_c {
public:
    enum gzSettingsMenu_Settings_e {
        SETTING_CURSOR_TYPE,
        SETTING_DISPLAY_MODE,
        SETTING_DROP_SHADOW,
        SETTING_FONT,
        SETTING_MENU_PAUSES_GAME,
        SETTING_MENU_SFX,
        SETTING_RELOAD_TYPE,
        SETTING_TEXT_COLOR,
        SETTING_SWAP_EQUIPS,
        
        SETTING_SAVE_CARD,
        SETTING_LOAD_CARD,
        SETTING_DELETE_CARD,
        SETTING_COMMAND_COMBOS,
        SETTING_MENU_POSITIONS,
        SETTING_CREDITS,

        SETTING_MAX
    };

    enum gzSettingsMenu_Haihai_e {
        ARROW_RIGHT = 4,
        ARROW_LEFT = 1
    };

    gzSettingsMenu_c();
    ~gzSettingsMenu_c();
    void updateDynamicLines();

    virtual void _delete();
    virtual void execute();
    virtual void draw();

    char* getCursorTypeText() {
        switch (gzInfo_getCursorType()) {
        case g_gzInfo.CURSOR_CLASSIC:
            return "classic";
        case g_gzInfo.CURSOR_TP:
            return "tp";
        case g_gzInfo.CURSOR_BOTH:
            return "both";
        default:
            return "unknown";
        }
    }

    const char* getReloadTypeText() { return gzInfo_getReloadType() ? "load area" : "load file"; }
    const char* getDropShadowsText() { return gzInfo_isDropShadows() ? "enabled" : "disabled"; }
    const char* getSwapEquipsText() { return gzInfo_isSwapEquips() ? "yes" : "no"; }
    const char* getDisplayModeText() { return gzInfo_getDisplayMode() ? "progressive" : "interlaced"; }
    // TODO(Pheenoh): Finish writing this functionality
    const char* getMenuPausesGameText() { return "no"; }
    const char* getMenuSfxText() { return gzInfo_isMenuSfx() ? "enabled" : "disabled"; }

    static const int LINE_NUM = SETTING_MAX;

    u8 getHaihaiFlags(int i);

private:
    gzTextBox* mpLines[LINE_NUM];
    gzTextBox* mpLineOptions[LINE_NUM];
    gzCreditsMenu_c* mpCreditsMenu;
};

#endif
