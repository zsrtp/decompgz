#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_cheats.h"
#include "gz/gz_menu_main.h"

u8 gzCheatsMenu_c::getHaihaiFlags(int idx) {
    u8 haihai_flags = ARROW_LEFT | ARROW_RIGHT;

    if (idx >= 0 && idx < LINE_NUM && mpLines[idx]->mIs != NULL) {
        if (mpLines[idx]->mIs()) {
            haihai_flags &= ~ARROW_RIGHT;
        } else {
            haihai_flags &= ~ARROW_LEFT;
        }
    }

    return haihai_flags;
}

void gzCheatsMenu_c::updateDynamicLines() {
    for (int i = 0; i < LINE_NUM; i++) {
        gzTextBox* opt = mpLines[i]->getOptionBox();
        if (mpLines[i]->mIs != NULL) {
            opt->setString(mpLines[i]->mIs() ? "on" : "off");
        }
    }
    updateLineBounds((gzLine**)mpLines, LINE_NUM);
}

gzCheatsMenu_c::gzCheatsMenu_c() {
    OSReport("creating gzCheatsMenu_c\n");
    mXPos = g_gzInfo.mBackgroundXPos + 195.0f;

    mpLines[CHEAT_DISABLE_ITEM_TIMER] = new gzBoolOptionLine("disable item timer", "disables item delete timer", gzInfo_isDisableItemTimer, gzInfo_onDisableItemTimer, gzInfo_offDisableItemTimer);
    mpLines[CHEAT_DISABLE_WALLS] = new gzBoolOptionLine("disable walls", "disables most wall collision", gzInfo_isDisableWalls, gzInfo_onDisableWalls, gzInfo_offDisableWalls);
    mpLines[CHEAT_INF_AIR] = new gzBoolOptionLine("infinite air", "gives infinite air underwater", gzInfo_isInfiniteAir, gzInfo_onInfiniteAir, gzInfo_offInfiniteAir);
    mpLines[CHEAT_INF_ARROWS] = new gzBoolOptionLine("infinite arrows", "always have 99 arrows", gzInfo_isInfiniteArrows, gzInfo_onInfiniteArrows, gzInfo_offInfiniteArrows);
    mpLines[CHEAT_INF_BOMBS] = new gzBoolOptionLine("infinite bombs", "always have 99 bombs", gzInfo_isInfiniteBombs, gzInfo_onInfiniteBombs, gzInfo_offInfiniteBombs);
    mpLines[CHEAT_INF_HEARTS] = new gzBoolOptionLine("infinite hearts", "always have full hearts", gzInfo_isInfiniteHearts, gzInfo_onInfiniteHearts, gzInfo_offInfiniteHearts);
    mpLines[CHEAT_INF_OIL] = new gzBoolOptionLine("infinite lantern oil", "gives infinite lantern oil", gzInfo_isInfiniteOil, gzInfo_onInfiniteOil, gzInfo_offInfiniteOil);
    mpLines[CHEAT_INF_RUPEES] = new gzBoolOptionLine("infinite rupees", "always have 1000 rupees", gzInfo_isInfiniteRupees, gzInfo_onInfiniteRupees, gzInfo_offInfiniteRupees);
    mpLines[CHEAT_INF_SLINGSHOT] = new gzBoolOptionLine("infinite slingshot seeds", "always have 99 slingshot pellets", gzInfo_isInfiniteSlingshot, gzInfo_onInfiniteSlingshot, gzInfo_offInfiniteSlingshot);
    mpLines[CHEAT_INVINCIBLE_LINK] = new gzBoolOptionLine("invincible link", "disables link's hurtbox", gzInfo_isInvincibleLink, gzInfo_onInvincibleLink, gzInfo_offInvincibleLink);
    mpLines[CHEAT_INVINCIBLE_ENEMIES] = new gzBoolOptionLine("invincible enemies", "make some enemies invincible", gzInfo_isInvincibleEnemies, gzInfo_onInvincibleEnemies, gzInfo_offInvincibleEnemies);
    mpLines[CHEAT_MOON_JUMP] = new gzBoolOptionLine("moon jump", "hold R+A to moon jump", gzInfo_isMoonJump, gzInfo_onMoonJump, gzInfo_offMoonJump);
    mpLines[CHEAT_SUPER_CLAWSHOT] = new gzBoolOptionLine("super clawshot", "super fast / long clawshot", gzInfo_isSuperClawshot, gzInfo_onSuperClawshot, gzInfo_offSuperClawshot);
    mpLines[CHEAT_TRANSFORM_ANYWHERE] = new gzBoolOptionLine("transform anywhere", "transform at any location", gzInfo_isTransformAnywhere, gzInfo_onTransformAnywhere, gzInfo_offTransformAnywhere);
    mpLines[CHEAT_UNRESTRICTED_ITEMS] = new gzBoolOptionLine("unrestricted items", "disable item restrictions", gzInfo_isUnrestrictedItems, gzInfo_onUnrestrictedItems, gzInfo_offUnrestrictedItems);
}

gzCheatsMenu_c::~gzCheatsMenu_c() {
    _delete();
}

void gzCheatsMenu_c::_delete() {
    OSReport("deleting gzCheatsMenu_c\n");

    for (int i = 0; i < LINE_NUM; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;
    }
}

void gzCheatsMenu_c::execute() {
    if (checkInputWait()) return;
    if (handleBackButton(gzMainMenu_c::MENU_CHEATS)) return;

    gzCursor* l_cursor = gzInfo_getCursor();
    gzBoolOptionLine* line = mpLines[l_cursor->y];

    if (gzPad::getTrigA()) {
        gzInfo_toggleMenuOption();
    }

    if (gzPad::getTrigLeft() && gzInfo_isMenuOption()) {
        if (line->mIs != NULL && line->mOff != NULL && line->mIs()) {
            line->mOff();
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }
    }

    if (gzPad::getTrigRight() && gzInfo_isMenuOption()) {
        if (line->mIs != NULL && line->mOn != NULL && !line->mIs()) {
            line->mOn();
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }
    }

    handleNavigation(LINE_NUM);
    finishExecute(LINE_NUM);
}

void gzCheatsMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();
    updateDynamicLines();
    u8 haihai_flags = getHaihaiFlags(l_cursor->y);
    drawLines((gzLine**)mpLines, LINE_NUM, haihai_flags, 0, LINE_NUM);
}
