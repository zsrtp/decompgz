#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "gz/gz_menu.h"
#include "m_Do/m_Do_machine.h"
#include "printf.h"

#define CURSOR_AQUAMARINE 0x71D9E2FF
#define CURSOR_BOLD_CRIMSON 0xDC143CFF
#define CURSOR_ELECTRIC_BLUE 0x7DF9FFFF
#define CURSOR_FIERY_ORANGE 0xFF4500FF
#define CURSOR_GOLD_DROP 0xEE8000FF
#define CURSOR_HOT_PINK 0xFF69B4FF
#define CURSOR_LEMON_YELLOW 0xFFF44FFF
#define CURSOR_LIME_GREEN 0x32CD32FF
#define CURSOR_MYSTICAL_PURPLE 0x9370DBFF
#define CURSOR_TROPICAL_TURQUOISE 0x40E0D0FF
#define CURSOR_VIVID_VIOLET 0x9F00FFFF
#define CURSOR_ZESTY_CHARTREUSE 0x7FFF00FF

#define CURSOR_AMETHYST 0x9966FFFF
#define CURSOR_AQUAMARINE 0x71D9E2FF
#define CURSOR_BANANA_MANIA 0xFAE7B5FF
#define CURSOR_BOLD_CRIMSON 0xDC143CFF
#define CURSOR_BUBBLEGUM_PINK 0xFF69B4FF
#define CURSOR_CERULEAN 0x007BA7FF
#define CURSOR_COSMIC_COBALT 0x2E2D88FF
#define CURSOR_ELECTRIC_BLUE 0x7DF9FFFF
#define CURSOR_FIERY_ORANGE 0xFF4500FF
#define CURSOR_FLAMINGO_FEATHER 0xFC8EACFF
#define CURSOR_GOLD_DROP 0xEE8000FF
#define CURSOR_LEMON_YELLOW 0xFFF44FFF
#define CURSOR_LIME_GREEN 0x32CD32FF
#define CURSOR_MAGENTA_MAGIC 0xFF00FFFF
#define CURSOR_MIDNIGHT_BLUE 0x191970FF
#define CURSOR_MYSTICAL_PURPLE 0x9370DBFF
#define CURSOR_NEON_CARROT 0xFFA343FF
#define CURSOR_PERIWINKLE 0xCCCCFFFF
#define CURSOR_SAPPHIRE_SPARKLE 0x0F52BAFF
#define CURSOR_SHAMROCK_GREEN 0x009E60FF
#define CURSOR_SUNNY_YELLOW 0xFFFF00FF
#define CURSOR_TANGERINE_TWIST 0xFFA500FF
#define CURSOR_TROPICAL_TURQUOISE 0x40E0D0FF
#define CURSOR_VIVID_VIOLET 0x9F00FFFF
#define CURSOR_WILD_STRAWBERRY 0xFF43A4FF
#define CURSOR_ZESTY_CHARTREUSE 0x7FFF00FF

bool g_progressiveMode = false;

static const int CURSOR_COLOR_COUNT = 26;

static char* l_cursorColorName[] = {
    "amethyst",
    "aquamarine",
    "banana mania",
    "bold crimson",
    "bubblegum pink",
    "cerulean",
    "cosmic cobalt",
    "electric blue",
    "fiery orange",
    "flamingo feather",
    "gold drop",
    "lemon yellow",
    "lime green",
    "magenta magic",
    "midnight blue",
    "mystical purple",
    "neon carrot",
    "periwinkle",
    "sapphire sparkle",
    "shamrock green",
    "sunny yellow",
    "tangerine twist",
    "tropical turquoise",
    "vivid violet",
    "wild strawberry",
    "zesty chartreuse"
};

static u32 l_cursorColorValue[] = {
    CURSOR_AMETHYST,
    CURSOR_AQUAMARINE,
    CURSOR_BANANA_MANIA,
    CURSOR_BOLD_CRIMSON,
    CURSOR_BUBBLEGUM_PINK,
    CURSOR_CERULEAN,
    CURSOR_COSMIC_COBALT,
    CURSOR_ELECTRIC_BLUE,
    CURSOR_FIERY_ORANGE,
    CURSOR_FLAMINGO_FEATHER,
    CURSOR_GOLD_DROP,
    CURSOR_LEMON_YELLOW,
    CURSOR_LIME_GREEN,
    CURSOR_MAGENTA_MAGIC,
    CURSOR_MIDNIGHT_BLUE,
    CURSOR_MYSTICAL_PURPLE,
    CURSOR_NEON_CARROT,
    CURSOR_PERIWINKLE,
    CURSOR_SAPPHIRE_SPARKLE,
    CURSOR_SHAMROCK_GREEN,
    CURSOR_SUNNY_YELLOW,
    CURSOR_TANGERINE_TWIST,
    CURSOR_TROPICAL_TURQUOISE,
    CURSOR_VIVID_VIOLET,
    CURSOR_WILD_STRAWBERRY,
    CURSOR_ZESTY_CHARTREUSE
};

u32 nextColor() {
    // Find current color index
    int currentIndex = -1;
    for (int i = 0; i < CURSOR_COLOR_COUNT; i++) {
        if (g_gzInfo.getCursorColor() == l_cursorColorValue[i]) {
            currentIndex = i;
            break;
        }
    }
    
    // If not found, default to first color
    if (currentIndex == -1) {
        return l_cursorColorValue[0];
    }
    
    // Get next index with wrap around
    int nextIndex = (currentIndex + 1) % CURSOR_COLOR_COUNT;
    return l_cursorColorValue[nextIndex];
}

u32 previousColor() {
    // Find current color index
    int currentIndex = -1;
    for (int i = 0; i < CURSOR_COLOR_COUNT; i++) {
        if (g_gzInfo.getCursorColor() == l_cursorColorValue[i]) {
            currentIndex = i;
            break;
        }
    }
    
    // If not found, default to first color
    if (currentIndex == -1) {
        return l_cursorColorValue[0];
    }
    
    // Get previous index with wrap around
    int previousIndex = (currentIndex - 1 + CURSOR_COLOR_COUNT) % CURSOR_COLOR_COUNT;
    return l_cursorColorValue[previousIndex];
}

void gzSettingsMenu_c::updateDynamicLines() {
    mpLines[SETTING_AREA_RELOAD_BEHAVIOR]->setStringf("area reload behavior: <%s>", g_gzInfo.getAreaReload() ? "load area" : "load file");
    mpLines[SETTING_DROP_SHADOW]->setStringf("drop shadows: [%s]", g_gzInfo.getDropShadows() ? "X" : " ");
    mpLines[SETTING_SWAP_EQUIPS]->setStringf("swap equips: [%s]", g_gzInfo.getSwapEquips() ? "X" : " ");
    mpLines[SETTING_PROGRESSIVE_MODE]->setStringf("progressive mode: [%s]", g_progressiveMode ? "X" : " ");
    mpLines[SETTING_CURSOR_TYPE]->setStringf("cursor type: <%s>", g_gzInfo.getCursorType() ? "tp" : "classic");

    // Find current color name
    char* currentColorName = "unknown";
    for (int i = 0; i < CURSOR_COLOR_COUNT; i++) {
        if (g_gzInfo.getCursorColor() == l_cursorColorValue[i]) {
            currentColorName = l_cursorColorName[i];
            break;
        }
    }
    mpLines[SETTING_CURSOR_COLOR]->setStringf("cursor color: <%s>", currentColorName);
}

gzMenu_c::gzCursor gzSettingsMenu_c::mCursor = {0, 0};

gzSettingsMenu_c::gzSettingsMenu_c() {
    OSReport("creating gzSettingsMenu_c\n");

    for (int i = 0; i < LINE_NUM; i++) {
        mpLines[i] = new gzTextBox();
        mpLines[i]->mBounds.f.x = 330.0f;
        mpLines[i]->mBounds.f.y = 10.0f;
        mpLines[i]->setLineSpace(0.0f);
    }

    mpLines[SETTING_AREA_RELOAD_BEHAVIOR]->setString("area reload behavior:");
    mpLines[SETTING_CURSOR_TYPE]->setString("cursor type:");
    mpLines[SETTING_CURSOR_COLOR]->setString("cursor color:");
    mpLines[SETTING_FONT]->setString("font:");
    mpLines[SETTING_DROP_SHADOW]->setString("drop shadows:");
    mpLines[SETTING_SWAP_EQUIPS]->setString("swap equips:");
    mpLines[SETTING_PROGRESSIVE_MODE]->setString("progressive mode:");
    mpLines[SETTING_SAVE_CARD]->setString("save card");
    mpLines[SETTING_LOAD_CARD]->setString("load card");
    mpLines[SETTING_DELETE_CARD]->setString("delete card");
    mpLines[SETTING_COMMAND_COMBOS]->setString("command combos");
    mpLines[SETTING_MENU_POSITIONS]->setString("menu positions");
    mpLines[SETTING_CREDITS]->setString("credits");

    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpDrawCursor->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    mpDrawCursor->setAlphaRate(1.0f);
}

gzSettingsMenu_c::~gzSettingsMenu_c() {
    _delete();
}

void gzSettingsMenu_c::_delete() {
    OSReport("deleting gzSettingsMenu_c\n");
    for (int i = 0; i < LINE_NUM; i++) {
        delete mpLines[i];
        mpLines[i] = NULL;
    }

    delete mpDrawCursor;
}

void gzSettingsMenu_c::execute() {
    if (gzPad::getTrigDown() && mCursor.y < LINE_NUM) {
        mCursor.y++;
    }

    if (gzPad::getTrigUp() && mCursor.y >= 0) {
        mCursor.y--;
    }

    if (mCursor.y < 0) {
        mCursor.y = LINE_NUM - 1;
    } else if (mCursor.y > LINE_NUM - 1) {
        mCursor.y = 0;
    }

    if (gzPad::getTrigB()) {
        gzChangeMenu<gzMainMenu_c>();
        return;
    }

    if (gzPad::getTrigA()) {
        switch (mCursor.y) {
        case SETTING_DROP_SHADOW:
            g_gzInfo.setDropShadows(!g_gzInfo.getDropShadows());
            break;
        case SETTING_SWAP_EQUIPS:
            g_gzInfo.setSwapEquips(!g_gzInfo.getSwapEquips());
            break;
        case SETTING_PROGRESSIVE_MODE:
            g_progressiveMode ? mDoMch_render_c::setProgressiveMode() : mDoMch_render_c::setInterlacedMode();
            g_progressiveMode = !g_progressiveMode;
            break;
        case SETTING_CREDITS:
            gzChangeMenu<gzCreditsMenu_c>();
            return;
        }
    }

    if (gzPad::getTrigRight()) {
        switch (mCursor.y) {
        case SETTING_CURSOR_TYPE:
            g_gzInfo.setCursorType(!g_gzInfo.getCursorType());
            break;
        case SETTING_CURSOR_COLOR:
            g_gzInfo.setCursorColor(nextColor());
            break;
        case SETTING_AREA_RELOAD_BEHAVIOR:
            g_gzInfo.setAreaReload(!g_gzInfo.getAreaReload());
            break;
        }
    }

    if (gzPad::getTrigLeft()) {
        switch (mCursor.y) {
        case SETTING_CURSOR_TYPE:
            g_gzInfo.setCursorType(!g_gzInfo.getCursorType());
            break;
        case SETTING_CURSOR_COLOR:
            g_gzInfo.setCursorColor(previousColor());
            break;
        case SETTING_AREA_RELOAD_BEHAVIOR:
            g_gzInfo.setAreaReload(!g_gzInfo.getAreaReload());
            break;
        }
    }
}

void gzSettingsMenu_c::draw() {
    updateDynamicLines();

    for (int i = 0; i < LINE_NUM; i++) {
        if (mpLines[i] != NULL) {
            if (mCursor.y == i) {
                mpLines[i]->draw(30.0f, 90.0f + ((i - 1) * 22.0f), g_gzInfo.getCursorType() == true ? 0xFFFFFFFF : g_gzInfo.getCursorColor());
                mpDrawCursor->setPos(170.0f, 82.5f + ((i - 1) * 22.0f), (J2DPane*)mpLines[i], true);
            } else {
                mpLines[i]->draw(30.0f, 90.0f + ((i - 1) * 22.0f), 0xFFFFFFFF);
            }
        }
    }

    if (g_gzInfo.getCursorType()) mpDrawCursor->draw();
}
