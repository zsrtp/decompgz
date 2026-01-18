#ifndef GZ_MENU_CHEATS_H
#define GZ_MENU_CHEATS_H

#include "gz/gz_menu.h"
#include "gz/gz_textbox.h"

class gzCheatsMenu_c : public gzMenu_c {
public:
    enum gzCheatsMenu_e {
        CHEAT_DISABLE_ITEM_TIMER,
        CHEAT_DISABLE_WALLS,
        CHEAT_INF_AIR,
        CHEAT_INF_ARROWS,
        CHEAT_INF_BOMBS,
        CHEAT_INF_HEARTS,
        CHEAT_INF_OIL,
        CHEAT_INF_RUPEES,
        CHEAT_INF_SLINGSHOT,
        CHEAT_INVINCIBLE_LINK,
        CHEAT_INVINCIBLE_ENEMIES,
        CHEAT_MOON_JUMP,
        CHEAT_SUPER_CLAWSHOT,
        CHEAT_TRANSFORM_ANYWHERE,
        CHEAT_UNRESTRICTED_ITEMS,

        CHEAT_MAX
    };

    gzCheatsMenu_c();
    ~gzCheatsMenu_c();
    const char* getDisableItemTimerText() { return gzInfo_isDisableItemTimer() ? "on" : "off"; }
    const char* getDisableWallsText() { return gzInfo_isDisableWalls() ? "on" : "off"; }
    const char* getInfiniteAirText() { return gzInfo_isInfiniteAir() ? "on" : "off"; }
    const char* getInfiniteArrowsText() { return gzInfo_isInfiniteArrows() ? "on" : "off"; }
    const char* getInfiniteBombsText() { return gzInfo_isInfiniteBombs() ? "on" : "off"; }
    const char* getInfiniteHeartsText() { return gzInfo_isInfiniteHearts() ? "on" : "off"; }
    const char* getInfiniteOilText() { return gzInfo_isInfiniteOil() ? "on" : "off"; }
    const char* getInfiniteRupeesText() { return gzInfo_isInfiniteRupees() ? "on" : "off"; }
    const char* getInfiniteSlingshotText() { return gzInfo_isInfiniteSlingshot() ? "on" : "off"; }
    const char* getInvincibleEnemiesText() { return gzInfo_isInvincibleEnemies() ? "on" : "off"; }
    const char* getInvincibleLinkText() { return gzInfo_isInvincibleLink() ? "on" : "off"; }
    const char* getMoonJumpText() { return gzInfo_isMoonJump() ? "on" : "off"; }
    const char* getSuperClawshotText() { return gzInfo_isSuperClawshot() ? "on" : "off"; }
    const char* getTransformAnywhereText() { return gzInfo_isTransformAnywhere() ? "on" : "off"; }
    const char* getUnrestrictedItemsText() { return gzInfo_isUnrestrictedItems() ? "on" : "off"; }

    virtual void _delete();
    virtual void execute();
    virtual void draw();

    static const int LINE_NUM = CHEAT_MAX;

private:
    u8 getHaihaiFlags(int idx);
    void updateDynamicLines();

private:
    gzBoolOptionLine* mpLines[LINE_NUM];
};

#endif // GZ_MENU_CHEATS_H
