#ifndef GZ_MENU_SETTINGS_H
#define GZ_MENU_SETTINGS_H

#include "gz/gz_menu_credits.h"
#include "gz/gz_utility_confirm.h"

class gzSettingsMenu_c : public gzMenu_c {
public:
    enum gzSettingsMenu_Settings_e {
        SETTING_CURSOR_TYPE,
        SETTING_DISPLAY_MODE,
        SETTING_DROP_SHADOW,
        SETTING_FONT,
        SETTING_MENU_PAUSES_GAME,
        SETTING_BOOT_TO_MENU,
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

    char* getTextColorText() {
        switch (gzInfo_getTextColor()) {
        case COLOR_AMETHYST: return "amethyst";
        case COLOR_AQUAMARINE: return "aquamarine";
        case COLOR_BANANA_MANIA: return "banana mania";
        case COLOR_BOLD_CRIMSON: return "bold crimson";
        case COLOR_BUBBLEGUM_PINK: return "bubblegum pink";
        case COLOR_CERULEAN: return "cerulean";
        case COLOR_COSMIC_COBALT: return "cosmic cobalt";
        case COLOR_ELECTRIC_BLUE: return "electric blue";
        case COLOR_FIERY_ORANGE: return "fiery orange";
        case COLOR_FLAMINGO_FEATHER: return "flamingo feather";
        case COLOR_GOLD_DROP: return "gold drop";
        case COLOR_LEMON_YELLOW: return "lemon yellow";
        case COLOR_LIME_GREEN: return "lime green";
        case COLOR_MAGENTA_MAGIC: return "magenta magic";
        case COLOR_MIDNIGHT_BLUE: return "midnight blue";
        case COLOR_MYSTICAL_PURPLE: return "mystical purple";
        case COLOR_NEON_CARROT: return "neon carrot";
        case COLOR_PERIWINKLE: return "periwinkle";
        case COLOR_SAPPHIRE_SPARKLE: return "sapphire sparkle";
        case COLOR_SHAMROCK_GREEN: return "shamrock green";
        case COLOR_SUNNY_YELLOW: return "sunny yellow";
        case COLOR_TANGERINE_TWIST: return "tangerine twist";
        case COLOR_TROPICAL_TURQUOISE: return "tropical turquoise";
        case COLOR_VIVID_VIOLET: return "vivid violet";
        case COLOR_WHITE: return "white";
        case COLOR_WILD_STRAWBERRY: return "wild strawberry";
        case COLOR_ZESTY_CHARTREUSE: return "zesty chartreuse";
        default: return "unknown";
        }
    }

    const char* getReloadTypeText() { return gzInfo_getReloadType() ? "load area" : "load file"; }
    const char* getDropShadowsText() { return gzInfo_isDropShadows() ? "enabled" : "disabled"; }
    const char* getSwapEquipsText() { return gzInfo_isSwapEquips() ? "yes" : "no"; }
    const char* getDisplayModeText() { return gzInfo_getDisplayMode() ? "progressive" : "interlaced"; }
    const char* getMenuPausesGameText() { return gzInfo_isMenuPausesGame() ? "yes" : "no"; }
    const char* getBootToMenuText() { return gzInfo_isBootToMenu() ? "yes" : "no"; }
    const char* getMenuSfxText() { return gzInfo_isMenuSfx() ? "enabled" : "disabled"; }

    static const int LINE_NUM = SETTING_MAX;

    u8 getHaihaiFlags(int i);

    static int deleteCardConfirmCb(gzConfirm_c* i_confirm, void* i_data);
    static int deleteCardReturnCb(gzConfirm_c* i_confirm, void* i_data);

    gzConfirm_c* mpConfirm;

private:
    gzListOptionLine* mpCursorType;
    gzBoolOptionLine* mpDisplayMode;
    gzBoolOptionLine* mpDropShadows;
    gzListOptionLine* mpFont;
    gzBoolOptionLine* mpMenuPausesGame;
    gzBoolOptionLine* mpBootToMenu;
    gzBoolOptionLine* mpMenuSfx;
    gzBoolOptionLine* mpReloadType;
    gzListOptionLine* mpTextColor;
    gzBoolOptionLine* mpSwapEquips;
    gzLine* mpSaveCard;
    gzLine* mpLoadCard;
    gzLine* mpDeleteCard;
    gzLine* mpCommandCombos;
    gzLine* mpMenuPositions;
    gzLine* mpCredits;
    gzLine* mpLines[LINE_NUM];
};

#endif
