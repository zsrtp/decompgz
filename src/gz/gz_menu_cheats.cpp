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

    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpDrawCursor->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    mpDrawCursor->setAlphaRate(1.0f);

    mpMeterHaihai = new dMeterHaihai_c(3);

    mpDescription = new gzTextBox();
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

    delete mpMeterHaihai;
    mpMeterHaihai = NULL;
}

void gzCheatsMenu_c::execute() {
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return;
    }

    gzCursor* l_cursor = gzInfo_getCursor();
    if (gzPad::getTrigDown() && !mOption) {
        l_cursor->y = (l_cursor->y + 1) % LINE_NUM;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

    if (gzPad::getTrigUp() && !mOption) {
        l_cursor->y = (l_cursor->y - 1 + LINE_NUM) % LINE_NUM;
        gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
    }

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
    mpMeterHaihai->_execute(0);
}

void gzCheatsMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();

    static const f32 Y_ALIGNMENT = 100.0f;
    static const f32 OPTIONS_X_OFFSET = -20.0f;
    static const f32 HAIHAI_X_OFFSET = 305.0f;
    static const f32 HAIHAI_Y_OFFSET = -7.0f;
    static const f32 HAIHAI_SCALE_FACTOR = 0.04f;
    static const f32 HAIHAI_EXTRA_SPACING = 30.0f;
    static const f32 TP_CURSOR_X_OFFSET = 20.0f;
    static const f32 CURSOR_Y_BASE = 90.0f;
    static const f32 LINE_SPACING = 22.0f;
    static const f32 DESCRIPTION_X = 0.0f;
    static const f32 DESCRIPTION_Y = 420.0f;

    updateDynamicLines();

    J2DTextBox::TFontSize font_size;
    mpLineOptions[0]->getFontSize(font_size);  // assume that all lines have the same font size
    mpMeterHaihai->setScale(font_size.mSizeY * HAIHAI_SCALE_FACTOR);

    u32 cursor_color = gzInfo_getCursorColor();


    f32 x_alignment_opts = mXPos + OPTIONS_X_OFFSET;
    f32 x_alignment_haihai = x_alignment_opts + HAIHAI_X_OFFSET;
    f32 y_alignment_haihai = Y_ALIGNMENT + HAIHAI_Y_OFFSET;
    f32 x_alignment_tp_cursor = mXPos + TP_CURSOR_X_OFFSET;

   for (int i = 0; i < LINE_NUM; i++) {
        f32 y_pos = Y_ALIGNMENT + ((i - 1) * LINE_SPACING);
        f32 y_pos_haihai = y_alignment_haihai + ((i - 1) * LINE_SPACING);
        f32 y_pos_cursor = CURSOR_Y_BASE + ((i - 1) * LINE_SPACING);

        if (l_cursor->y == i && gzInfo_isSubMenuVisible()) {
            // Spacing between arrows determined by text box bound size
            f32 x_size_haihai = mpLineOptions[i]->mBounds.f.x + HAIHAI_EXTRA_SPACING;
            if (mpLineOptions[i]->mStringLength != 0 && mOption) {
                mpMeterHaihai->drawHaihai(getHaihaiFlags(i), x_alignment_haihai, y_pos_haihai, x_size_haihai, 0.0f);
            }

            mpLines[i]->draw(mXPos, y_pos, cursor_color);
            mpDrawCursor->setPos(x_alignment_tp_cursor, y_pos_cursor, (J2DPane*)mpLines[i], false);
            mpLineOptions[i]->draw(x_alignment_opts, y_pos, cursor_color, HBIND_CENTER);
        } else {
            mpLines[i]->draw(mXPos, y_pos, COLOR_WHITE);
            mpLineOptions[i]->draw(x_alignment_opts, y_pos, COLOR_WHITE, HBIND_CENTER);
        }
    }

    // Draw description if valid and on menu
    if (gzInfo_isSubMenuVisible()) {
        if (mpLines[l_cursor->y] && *mpLines[l_cursor->y]->m_description != 0) {
            f32 description_x = DESCRIPTION_X;
            f32 description_y = g_gzInfo.mBackgroundHeight + 25.0f;

            mpDescription->setString(mpLines[l_cursor->y]->m_description);
            mpDescription->draw(DESCRIPTION_X, description_y, cursor_color, HBIND_CENTER);
        }
    }

    if (gzInfo_isCursorTypeTP()) {
        if (mpDrawCursor != NULL) {
            mpDrawCursor->draw();
        }
    }
}
