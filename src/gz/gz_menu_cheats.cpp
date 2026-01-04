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
    mpLineOptions[CHEAT_DISABLE_ITEM_TIMER]->setStringf("%s", getDisableItemTimerText());
    mpLineOptions[CHEAT_DISABLE_WALLS]->setStringf("%s", getDisableWallsText());
    mpLineOptions[CHEAT_INF_AIR]->setStringf("%s", getInfiniteAirText());
    mpLineOptions[CHEAT_INF_ARROWS]->setStringf("%s", getInfiniteArrowsText());
    mpLineOptions[CHEAT_INF_BOMBS]->setStringf("%s", getInfiniteBombsText());
    mpLineOptions[CHEAT_INF_HEARTS]->setStringf("%s", getInfiniteHeartsText());
    mpLineOptions[CHEAT_INF_OIL]->setStringf("%s", getInfiniteOilText());
    mpLineOptions[CHEAT_INF_RUPEES]->setStringf("%s", getInfiniteRupeesText());
    mpLineOptions[CHEAT_INF_SLINGSHOT]->setStringf("%s", getInfiniteSlingshotText());
    mpLineOptions[CHEAT_INVINCIBLE_LINK]->setStringf("%s", getInvincibleLinkText());
    mpLineOptions[CHEAT_INVINCIBLE_ENEMIES]->setStringf("%s", getInvincibleEnemiesText());
    mpLineOptions[CHEAT_MOON_JUMP]->setStringf("%s", getMoonJumpText());
    mpLineOptions[CHEAT_SUPER_CLAWSHOT]->setStringf("%s", getSuperClawshotText());
    mpLineOptions[CHEAT_TRANSFORM_ANYWHERE]->setStringf("%s", getTransformAnywhereText());
    mpLineOptions[CHEAT_UNRESTRICTED_ITEMS]->setStringf("%s", getUnrestrictedItemsText());

    J2DTextBox::TFontSize font_size;

    for (int i = 0; i < LINE_NUM; i++) {
        mpLineOptions[i]->getFontSize(font_size);
        font_size.mSizeX *= 0.5f;
        mpLines[i]->mBounds.f.x = mpLines[i]->mStringLength * font_size.mSizeX;
        mpLineOptions[i]->mBounds.f.x = mpLineOptions[i]->mStringLength * font_size.mSizeX;
    }
}

gzCheatsMenu_c::gzCheatsMenu_c() {
    OSReport("creating gzCheatsMenu_c\n");

    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = new gzTextBox();
        mpLines[i]->mBounds.f.x = 430.0f;
        mpLines[i]->mBounds.f.y = 10.0f;

        mpLineOptions[i] = new gzTextBox();
        mpLineOptions[i]->mBounds.f.y = 10.0f;
    }

    mpLines[CHEAT_DISABLE_ITEM_TIMER]->setStringDesc("disable item timer", "disables item delete timer");
    mpLines[CHEAT_DISABLE_WALLS]->setStringDesc("disable walls", "disables most wall collision");
    mpLines[CHEAT_INF_AIR]->setStringDesc("infinite air", "gives infinite air underwater");
    mpLines[CHEAT_INF_ARROWS]->setStringDesc("infinite arrows", "always have 99 arrows");
    mpLines[CHEAT_INF_BOMBS]->setStringDesc("infinite bombs", "always have 99 bombs");
    mpLines[CHEAT_INF_HEARTS]->setStringDesc("infinite hearts", "always have full hearts");
    mpLines[CHEAT_INF_OIL]->setStringDesc("infinite lantern oil", "gives infinite lantern oil");
    mpLines[CHEAT_INF_RUPEES]->setStringDesc("infinite rupees", "always have 1000 rupees");
    mpLines[CHEAT_INF_SLINGSHOT]->setStringDesc("infinite slingshot seeds", "always have 99 slingshot pellets");
    mpLines[CHEAT_INVINCIBLE_LINK]->setStringDesc("invincible link", "disables link's hurtbox");
    mpLines[CHEAT_INVINCIBLE_ENEMIES]->setStringDesc("invincible enemies", "make some enemies invincible");
    mpLines[CHEAT_MOON_JUMP]->setStringDesc("moon jump", "hold R+A to moon jump"); // todo: platform agnostic description
    mpLines[CHEAT_SUPER_CLAWSHOT]->setStringDesc("super clawshot", "super fast / long clawshot");
    mpLines[CHEAT_TRANSFORM_ANYWHERE]->setStringDesc("transform anywhere", "transform at any location");
    mpLines[CHEAT_UNRESTRICTED_ITEMS]->setStringDesc("unrestricted items", "disable item restrictions");
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
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return;
    }

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
            mOption = !mOption;
            mOption ? gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK) : gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
            break;
        }
    }

    if (gzPad::getTrigB()) {
        if (mOption) {
            mOption = false;
            gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
        } else {
            l_cursor->x--;
            l_cursor->y = gzMainMenu_c::MENU_CHEATS;
            gzInfo_seStart(Z2SE_SY_EXP_WIN_CLOSE);
            g_gzInfo.mpMainMenu->startReverseTransition();
            return;
        }
    }

    if (gzPad::getTrigLeft() && mOption) {
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

    if (gzPad::getTrigRight() && mOption) {
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
    
    gzMenu_c::execute();
}

void gzCheatsMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();
    updateDynamicLines();
    u8 haihai_flags = 0;

    for (int i = 0; i < LINE_NUM; i++) {
        if (l_cursor->y == i)
            haihai_flags = getHaihaiFlags(i);
    }

    drawLines(mpLines, mpLineOptions, haihai_flags, LINE_NUM);
}
