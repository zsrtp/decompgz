#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz_menu_cheats.h"
#include "gz/gz_menu_main.h"

u8 gzCheatsMenu_c::getHaihaiFlags(int idx) {
    u8 haihai_flags = ARROW_LEFT | ARROW_RIGHT;

    switch (idx) {
    case CHEAT_DISABLE_ITEM_TIMER:
        !gzInfo_isDisableItemTimer() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_DISABLE_WALLS:
        !gzInfo_isDisableWalls() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_INF_AIR:
        !gzInfo_isInfiniteAir() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_INF_ARROWS:
        !gzInfo_isInfiniteArrows() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_INF_BOMBS:
        !gzInfo_isInfiniteBombs() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_INF_HEARTS:
        !gzInfo_isInfiniteHearts() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_INF_OIL:
        !gzInfo_isInfiniteOil() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_INF_RUPEES:
        !gzInfo_isInfiniteRupees() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_INF_SLINGSHOT:
        !gzInfo_isInfiniteSlingshot() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_INVINCIBLE_LINK:
        !gzInfo_isInvincibleLink() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_INVINCIBLE_ENEMIES:
        !gzInfo_isInvincibleEnemies() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_MOON_JUMP:
        !gzInfo_isMoonJump() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_SUPER_CLAWSHOT:
        !gzInfo_isSuperClawshot() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_TRANSFORM_ANYWHERE:
        !gzInfo_isTransformAnywhere() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    case CHEAT_UNRESTRICTED_ITEMS:
        !gzInfo_isUnrestrictedItems() ? haihai_flags &= ~ARROW_LEFT : haihai_flags &= ~ARROW_RIGHT;
        break;
    }

    return haihai_flags;
}

void gzCheatsMenu_c::updateDynamicLines() {
    mpLines[CHEAT_DISABLE_ITEM_TIMER]->getOptionBox()->setStringf("%s", getDisableItemTimerText());
    mpLines[CHEAT_DISABLE_WALLS]->getOptionBox()->setStringf("%s", getDisableWallsText());
    mpLines[CHEAT_INF_AIR]->getOptionBox()->setStringf("%s", getInfiniteAirText());
    mpLines[CHEAT_INF_ARROWS]->getOptionBox()->setStringf("%s", getInfiniteArrowsText());
    mpLines[CHEAT_INF_BOMBS]->getOptionBox()->setStringf("%s", getInfiniteBombsText());
    mpLines[CHEAT_INF_HEARTS]->getOptionBox()->setStringf("%s", getInfiniteHeartsText());
    mpLines[CHEAT_INF_OIL]->getOptionBox()->setStringf("%s", getInfiniteOilText());
    mpLines[CHEAT_INF_RUPEES]->getOptionBox()->setStringf("%s", getInfiniteRupeesText());
    mpLines[CHEAT_INF_SLINGSHOT]->getOptionBox()->setStringf("%s", getInfiniteSlingshotText());
    mpLines[CHEAT_INVINCIBLE_LINK]->getOptionBox()->setStringf("%s", getInvincibleLinkText());
    mpLines[CHEAT_INVINCIBLE_ENEMIES]->getOptionBox()->setStringf("%s", getInvincibleEnemiesText());
    mpLines[CHEAT_MOON_JUMP]->getOptionBox()->setStringf("%s", getMoonJumpText());
    mpLines[CHEAT_SUPER_CLAWSHOT]->getOptionBox()->setStringf("%s", getSuperClawshotText());
    mpLines[CHEAT_TRANSFORM_ANYWHERE]->getOptionBox()->setStringf("%s", getTransformAnywhereText());
    mpLines[CHEAT_UNRESTRICTED_ITEMS]->getOptionBox()->setStringf("%s", getUnrestrictedItemsText());

    J2DTextBox::TFontSize font_size;

    for (int i = 0; i < LINE_NUM; i++) {
        gzTextBox* opt = mpLines[i]->getOptionBox();
        opt->getFontSize(font_size);
        font_size.mSizeX *= 0.5f;
        mpLines[i]->mText->mBounds.f.x = mpLines[i]->mText->mStringLength * font_size.mSizeX;
        opt->mBounds.f.x = opt->mStringLength * font_size.mSizeX;
    }
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

    if (gzPad::getTrigA()) {
        switch (l_cursor->y) {
        case CHEAT_DISABLE_ITEM_TIMER:
        case CHEAT_DISABLE_WALLS:
        case CHEAT_INF_AIR:
        case CHEAT_INF_ARROWS:
        case CHEAT_INF_BOMBS:
        case CHEAT_INF_HEARTS:
        case CHEAT_INF_OIL:
        case CHEAT_INF_RUPEES:
        case CHEAT_INF_SLINGSHOT:
        case CHEAT_INVINCIBLE_LINK:
        case CHEAT_INVINCIBLE_ENEMIES:
        case CHEAT_MOON_JUMP:
        case CHEAT_SUPER_CLAWSHOT:
        case CHEAT_TRANSFORM_ANYWHERE:
        case CHEAT_UNRESTRICTED_ITEMS:
            gzInfo_setMenuOption(!gzInfo_isMenuOption());
            gzInfo_isMenuOption() ? gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK) : gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
            break;
        }
    }

    if (gzPad::getTrigLeft() && gzInfo_isMenuOption()) {
        switch (l_cursor->y) {
        case CHEAT_DISABLE_ITEM_TIMER:
            if (gzInfo_isDisableItemTimer()) {
                gzInfo_offDisableItemTimer();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_DISABLE_WALLS:
            if (gzInfo_isDisableWalls()) {
                gzInfo_offDisableWalls();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_AIR:
            if (gzInfo_isInfiniteAir()) {
                gzInfo_offInfiniteAir();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_ARROWS:
            if (gzInfo_isInfiniteArrows()) {
                gzInfo_offInfiniteArrows();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_BOMBS:
            if (gzInfo_isInfiniteBombs()) {
                gzInfo_offInfiniteBombs();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_HEARTS:
            if (gzInfo_isInfiniteHearts()) {
                gzInfo_offInfiniteHearts();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_OIL:
            if (gzInfo_isInfiniteOil()) {
                gzInfo_offInfiniteOil();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_RUPEES:
            if (gzInfo_isInfiniteRupees()) {
                gzInfo_offInfiniteRupees();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_SLINGSHOT:
            if (gzInfo_isInfiniteSlingshot()) {
                gzInfo_offInfiniteSlingshot();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INVINCIBLE_LINK:
            if (gzInfo_isInvincibleLink()) {
                gzInfo_offInvincibleLink();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INVINCIBLE_ENEMIES:
            if (gzInfo_isInvincibleEnemies()) {
                gzInfo_offInvincibleEnemies();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_MOON_JUMP:
            if (gzInfo_isMoonJump()) {
                gzInfo_offMoonJump();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_SUPER_CLAWSHOT:
            if (gzInfo_isSuperClawshot()) {
                gzInfo_offSuperClawshot();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_TRANSFORM_ANYWHERE:
            if (gzInfo_isTransformAnywhere()) {
                gzInfo_offTransformAnywhere();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_UNRESTRICTED_ITEMS:
            if (gzInfo_isUnrestrictedItems()) {
                gzInfo_offUnrestrictedItems();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
        }
    }

    if (gzPad::getTrigRight() && gzInfo_isMenuOption()) {
        switch (l_cursor->y) {
        case CHEAT_DISABLE_ITEM_TIMER:
            if (!gzInfo_isDisableItemTimer()) {
                gzInfo_onDisableItemTimer();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_DISABLE_WALLS:
            if (!gzInfo_isDisableWalls()) {
                gzInfo_onDisableWalls();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_AIR:
            if (!gzInfo_isInfiniteAir()) {
                gzInfo_onInfiniteAir();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_ARROWS:
            if (!gzInfo_isInfiniteArrows()) {
                gzInfo_onInfiniteArrows();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_BOMBS:
            if (!gzInfo_isInfiniteBombs()) {
                gzInfo_onInfiniteBombs();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_HEARTS:
            if (!gzInfo_isInfiniteHearts()) {
                gzInfo_onInfiniteHearts();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_OIL:
            if (!gzInfo_isInfiniteOil()) {
                gzInfo_onInfiniteOil();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_RUPEES:
            if (!gzInfo_isInfiniteRupees()) {
                gzInfo_onInfiniteRupees();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INF_SLINGSHOT:
            if (!gzInfo_isInfiniteSlingshot()) {
                gzInfo_onInfiniteSlingshot();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INVINCIBLE_LINK:
            if (!gzInfo_isInvincibleLink()) {
                gzInfo_onInvincibleLink();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_INVINCIBLE_ENEMIES:
            if (!gzInfo_isInvincibleEnemies()) {
                gzInfo_onInvincibleEnemies();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_MOON_JUMP:
            if (!gzInfo_isMoonJump()) {
                gzInfo_onMoonJump();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_SUPER_CLAWSHOT:
            if (!gzInfo_isSuperClawshot()) {
                gzInfo_onSuperClawshot();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_TRANSFORM_ANYWHERE:
            if (!gzInfo_isTransformAnywhere()) {
                gzInfo_onTransformAnywhere();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
            break;
        case CHEAT_UNRESTRICTED_ITEMS:
            if (!gzInfo_isUnrestrictedItems()) {
                gzInfo_onUnrestrictedItems();
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
            }
        }
    }

    handleNavigation(LINE_NUM);
    finishExecute(LINE_NUM);
}

void gzCheatsMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();
    updateDynamicLines();
    u8 haihai_flags = 0;

    for (int i = 0; i < LINE_NUM; i++) {
        if (l_cursor->y == i)
            haihai_flags = getHaihaiFlags(i);
    }

    drawLines((gzLine**)mpLines, LINE_NUM, haihai_flags, 0, LINE_NUM);
}
