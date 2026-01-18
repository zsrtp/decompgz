#include "d/dolzel.h"  // IWYU pragma: keep

#include "gz/gz_menu_flags.h"
#include "gz/gz_menu_main.h"
#include "d/d_select_cursor.h"

// General flags
static gzBoolOption_s generalFlags[] = {
    {"boss flag", "sets the boss flag value", gzInfo_isBossFlag, gzInfo_onBossFlag,
     gzInfo_offBossFlag},
    {"coro td", "toggle temporary flag for coro text displacement", gzInfo_isCoroTD,
     gzInfo_onCoroTD, gzInfo_offCoroTD},
    {"epona stolen", "toggle flag for epona being stolen", gzInfo_isEponaStolen,
     gzInfo_onEponaStolen, gzInfo_offEponaStolen},
    {"epona tamed", "toggle flag for epona being tamed", gzInfo_isEponaTamed, gzInfo_onEponaTamed,
     gzInfo_offEponaTamed},
    {"malo mart in castle town", "toggle flag for malo mart being open in castle town",
     gzInfo_isMaloMartCT, gzInfo_onMaloMartCT, gzInfo_offMaloMartCT},
    {"map warping", "toggle flag for map warping", gzInfo_isMapWarping, gzInfo_onMapWarping,
     gzInfo_offMapWarping},
    {"midna charge", "toggle flag for midna charge", gzInfo_isMidnaCharge, gzInfo_onMidnaCharge,
     gzInfo_offMidnaCharge},
    {"midna healed", "toggle flag for midna being healed", gzInfo_isMidnaHealthy,
     gzInfo_onMidnaHealthy, gzInfo_offMidnaHealthy},
    {"midna on back", "toggle flag for midna appearing on wolf link's back", gzInfo_isMidnaOnBack,
     gzInfo_onMidnaOnBack, gzInfo_offMidnaOnBack},
    {"midna available", "toggle flag for being able to call midna", gzInfo_isMidnaOnZ,
     gzInfo_onMidnaOnZ, gzInfo_offMidnaOnZ},
    {"rusl td", "toggle temporary flag for rusl text displacement", gzInfo_isRuslTD,
     gzInfo_onRuslTD, gzInfo_offRuslTD},
    {"transform/warp", "toggle flag for transforming/warping", gzInfo_isTransformWarp,
     gzInfo_onTransformWarp, gzInfo_offTransformWarp},
    {"wolf sense", "toggle flag for wolf sense", gzInfo_isWolfSense, gzInfo_onWolfSense,
     gzInfo_offWolfSense}
};

// Dungeon bool and int flags
static gzIntOption_s dungeonFlags[] = {
    {"have boss key", "Give selected dungeon boss key", gzInfo_isDungeonBossKey,
     gzInfo_onDungeonBossKey, gzInfo_offDungeonBossKey},
    {"have compass", "Give selected dungeon compass", gzInfo_isDungeonCompass,
     gzInfo_onDungeonCompass, gzInfo_offDungeonCompass},
    {"have heart container", "Give selected dungeon heart container",
     gzInfo_isDungeonHeartContainer, gzInfo_onDungeonHeartContainer,
     gzInfo_offDungeonHeartContainer},
    {"have map", "Give selected dungeon map", gzInfo_isDungeonMap, gzInfo_onDungeonMap,
     gzInfo_offDungeonMap},
    {"have ooccoo", "Give selected dungeon ooccoo", gzInfo_isDungeonOoccoo, gzInfo_onDungeonOoccoo,
     gzInfo_offDungeonOoccoo},
    {"boss defeated", "Selected dungeon boss is defeated", gzInfo_isBossDefeated,
     gzInfo_onBossDefeated, gzInfo_offBossDefeated},
    {"miniboss defeated", "Selected dungeon miniboss is defeated", gzInfo_isMiniBossDefeated,
     gzInfo_onMiniBossDefeated, gzInfo_offMiniBossDefeated},
};

// Portal warp bool flags
static gzBoolOption_s warpFlags[] = {
    {"ordon spring", "Ordon Spring warp portal", isSpringWarp, onSpringWarp, offSpringWarp},
    {"south faron", "South Faron warp portal", isSFaronWarp, onSFaronWarp, offSFaronWarp},
    {"north faron", "North Faron warp portal", isNFaronWarp, onNFaronWarp, offNFaronWarp},
    {"sacred grove", "Sacred Grove warp portal", isGroveWarp, onGroveWarp, offGroveWarp},
    {"eldin gorge", "Eldin Gorge warp portal", isGorgeWarp, onGorgeWarp, offGorgeWarp},
    {"kak village", "Kakariko Village warp portal", isKakarikoWarp, onKakarikoWarp,
     offKakarikoWarp},
    {"death mountain", "Death Mountain warp portal", isMountainWarp, onMountainWarp,
     offMountainWarp},
    {"eldin bridge", "Bridge of Eldin warp portal", isBridgeWarp, onBridgeWarp, offBridgeWarp},
    {"castle town", "Castle Town warp portal", isTownWarp, onTownWarp, offTownWarp},
    {"lake hylia", "Lake Hylia warp portal", isLakeWarp, onLakeWarp, offLakeWarp},
    {"domain warp", "Zora's Domain warp portal", isDomainWarp, onDomainWarp, offDomainWarp},
    {"upper river", "Upper Zora's River warp portal", isUzrWarp, onUzrWarp, offUzrWarp},
    {"snowpeak", "Snowpeak warp portal", isSnowpeakWarp, onSnowpeakWarp, offSnowpeakWarp},
    {"gerudo mesa", "Gerudo Mesa warp portal", isMesaWarp, onMesaWarp, offMesaWarp},
    {"mirror chamber", "Mirror Chamber warp portal", isMirrorWarp, onMirrorWarp, offMirrorWarp}};

// Rupee bool flags
static gzBoolOption_s rupeeFlags[] = {
    {"fundraising 1", "Toggle flag for first fundraising being complete", isFundraising1, onFundraising1, offFundraising1},
    {"fundraising 2", "Toggle flag for second fundraising being complete", isFundraising2, onFundraising2, offFundraising2},
    {"rupee cutscenes", "Toggle rupee cutscenes being enabled", isRupeeCS, onRupeeCS, offRupeeCS}};

// Dungeon and region names
static const char* dungeonNames[] = {"Forest Temple",     "Goron Mines",        "Lakebed Temple",
                                     "Arbiter's Grounds", "Snowpeak Ruins",     "Temple of Time",
                                     "City in the Sky",   "Palace of Twilight", "Hyrule Castle"};

static const char* regionNames[] = {"ordon", "faron", "eldin", "lanayru", "desert", "snowpeak"};

// Static member definitions
int gzFlagsMenu_c::sSelectedDungeon = 0;
int gzFlagsMenu_c::sSelectedRegion = 0;

u32 gzFlagsMenu_c::nextDungeon() {
    sSelectedDungeon = (sSelectedDungeon + 1) % NUM_DUNGEONS;
    return sSelectedDungeon;
}

u32 gzFlagsMenu_c::prevDungeon() {
    sSelectedDungeon = (sSelectedDungeon - 1 + NUM_DUNGEONS) % NUM_DUNGEONS;
    return sSelectedDungeon;
}

u32 gzFlagsMenu_c::nextRegion() {
    sSelectedRegion = (sSelectedRegion + 1) % NUM_REGIONS;
    return sSelectedRegion;
}

u32 gzFlagsMenu_c::prevRegion() {
    sSelectedRegion = (sSelectedRegion - 1 + NUM_REGIONS) % NUM_REGIONS;
    return sSelectedRegion;
}

u32 gzFlagsMenu_c::addSmallKey() {
    int stageNo = getSelectedDungeonStageNo();
    u8 keys = getDungeonSmallKeys(stageNo);
    if (keys < 255) {
        setDungeonSmallKeys(stageNo, keys + 1);
    }
    return getDungeonSmallKeys(stageNo);
}

u32 gzFlagsMenu_c::removeSmallKey() {
    int stageNo = getSelectedDungeonStageNo();
    u8 keys = getDungeonSmallKeys(stageNo);
    if (keys > 0) {
        setDungeonSmallKeys(stageNo, keys - 1);
    }
    return getDungeonSmallKeys(stageNo);
}

gzFlagsMenu_c::gzFlagsMenu_c() {
    OSReport("creating gzFlagsMenu_c\n");
    mXPos = g_gzInfo.mBackgroundXPos + 195.0f;

    for (int i = 0; i < TAB_MAX_e; i++) {
        mpTabHeaders[i] = gzTextBox_allocate();
        mpTabHeaders[i]->setFontSize(15.0f, 15.0f);
    }
    mpTabHeaders[TAB_GENERAL]->setString("general");
    mpTabHeaders[TAB_DUNGEON]->setString("dungeon");
    mpTabHeaders[TAB_PORTAL]->setString("portal");
    mpTabHeaders[TAB_RUPEE]->setString("rupee");

    // General tab - all gzBoolOptionLine
    for (int i = 0; i < G_FLAG_MAX; i++) {
        mpLinesGeneral[i] = new gzBoolOptionLine(generalFlags[i].name, generalFlags[i].desc,
                                                  generalFlags[i].is, generalFlags[i].on, generalFlags[i].off);
    }

    // Dungeon tab
    mpLinesDungeon[D_FLAG_SELECT_DUNGEON] = new gzListOptionLine("dungeon:", "Select dungeon to modify flags", nextDungeon, prevDungeon);
    mpLinesDungeon[D_FLAG_SMALL_KEY] = new gzListOptionLine("small keys:", "Selected dungeon small keys", addSmallKey, removeSmallKey);
    for (int i = 0; i < 7; i++) {
        mpLinesDungeon[D_FLAG_BOSS_KEY + i] = new gzBoolOptionLine(dungeonFlags[i].name, dungeonFlags[i].desc);
    }
    mpLinesDungeon[D_FLAG_CLEAR_DUNGEON] = new gzLine("clear flags", "Clear all selected dungeon flags");

    // Portal tab
    mpLinesPortal[P_FLAG_SELECT_REGION] = new gzListOptionLine("region:", "Select region to view flags", nextRegion, prevRegion);
    for (int i = 0; i < 16; i++) {
        mpLinesPortal[P_FLAG_REGION + i] = new gzBoolOptionLine(warpFlags[i].name, warpFlags[i].desc,
                                                                 warpFlags[i].is, warpFlags[i].on, warpFlags[i].off);
    }

    // Rupee tab
    mpLinesRupee[R_FLAG_DONATION_AMT] = new gzListOptionLine("donation amount:", "Sets the amount of rupees donated to Charlo");
    mpLinesRupee[R_FLAG_FUNDRAISING_AMT] = new gzListOptionLine("fundraising amount:", "Sets the current fundraising amount");
    for (int i = 0; i < 3; i++) {
        mpLinesRupee[R_FLAG_FUNDRAISING_1 + i] = new gzBoolOptionLine(rupeeFlags[i].name, rupeeFlags[i].desc,
                                                                       rupeeFlags[i].is, rupeeFlags[i].on, rupeeFlags[i].off);
    }

    mpMeterHaihai = new dMeterHaihai_c(3);
    mCurrentTab = TAB_GENERAL;
}

gzFlagsMenu_c::~gzFlagsMenu_c() {
    _delete();
}

void gzFlagsMenu_c::_delete() {
    OSReport("deleting gzFlagsMenu_c\n");
    for (int i = 0; i < TAB_MAX_e; i++) {
        gzTextBox_free(mpTabHeaders[i]);
        mpTabHeaders[i] = NULL;
    }
    for (int i = 0; i < G_FLAG_MAX; i++) {
        delete mpLinesGeneral[i];
        mpLinesGeneral[i] = NULL;
    }
    for (int i = 0; i < D_FLAG_MAX; i++) {
        delete mpLinesDungeon[i];
        mpLinesDungeon[i] = NULL;
    }
    for (int i = 0; i < P_FLAG_MAX; i++) {
        delete mpLinesPortal[i];
        mpLinesPortal[i] = NULL;
    }
    for (int i = 0; i < R_FLAG_MAX; i++) {
        delete mpLinesRupee[i];
        mpLinesRupee[i] = NULL;
    }
    delete mpMeterHaihai;
    mpMeterHaihai = NULL;
}

u8 gzFlagsMenu_c::getHaihaiFlags(int idx) {
    u8 haihai_flags = ARROW_LEFT | ARROW_RIGHT;

    switch (mCurrentTab) {
    case TAB_GENERAL:
        if (generalFlags[idx].is()) {
            haihai_flags &= ~ARROW_RIGHT;
        } else {
            haihai_flags &= ~ARROW_LEFT;
        }
        break;
    case TAB_DUNGEON:
        if (idx == D_FLAG_SELECT_DUNGEON || idx == D_FLAG_SMALL_KEY) {
        } else if (idx == D_FLAG_CLEAR_DUNGEON) {
            haihai_flags = 0;
        } else if (idx >= D_FLAG_BOSS_KEY && idx <= D_FLAG_DEFEAT_MINIBOSS) {
            int bIdx = idx - D_FLAG_BOSS_KEY;
            if (dungeonFlags[bIdx].is(sSelectedDungeon + 16)) {
                haihai_flags &= ~ARROW_RIGHT;
            } else {
                haihai_flags &= ~ARROW_LEFT;
            }
        }
        break;
    case TAB_PORTAL:
        if (idx == P_FLAG_SELECT_REGION) {
        } else if (idx == P_FLAG_REGION) {
            if (getRegionFlag(sSelectedRegion + 1)) {
                haihai_flags &= ~ARROW_RIGHT;
            } else {
                haihai_flags &= ~ARROW_LEFT;
            }
        } else if (idx >= P_FLAG_SPRING_WARP && idx < P_FLAG_MAX) {
            int wIdx = idx - P_FLAG_SPRING_WARP;
            if (warpFlags[wIdx].is()) {
                haihai_flags &= ~ARROW_RIGHT;
            } else {
                haihai_flags &= ~ARROW_LEFT;
            }
        }
        break;
    case TAB_RUPEE:
        if (idx == R_FLAG_DONATION_AMT || idx == R_FLAG_FUNDRAISING_AMT) {
        } else if (idx >= R_FLAG_FUNDRAISING_1 && idx < R_FLAG_MAX) {
            int rIdx = idx - R_FLAG_FUNDRAISING_1;
            if (rupeeFlags[rIdx].is()) {
                haihai_flags &= ~ARROW_RIGHT;
            } else {
                haihai_flags &= ~ARROW_LEFT;
            }
        }
        break;
    }
    return haihai_flags;
}

int gzFlagsMenu_c::getCurrentLineNum() {
    switch (mCurrentTab) {
    case TAB_GENERAL:
        return G_FLAG_MAX;
    case TAB_DUNGEON:
        return D_FLAG_MAX;
    case TAB_PORTAL:
        return P_FLAG_MAX;
    case TAB_RUPEE:
        return R_FLAG_MAX;
    }
    return 0;
}

void gzFlagsMenu_c::updateDynamicLines() {
    gzLine** currentLines;
    int currentLineNum;
    switch (mCurrentTab) {
    case TAB_GENERAL:
        currentLines = mpLinesGeneral;
        currentLineNum = G_FLAG_MAX;
        for (int i = 0; i < G_FLAG_MAX; i++) {
            gzTextBox* opt = currentLines[i]->getOptionBox();
            if (opt) opt->setStringf("%s", generalFlags[i].is() ? "on" : "off");
        }
        break;
    case TAB_DUNGEON:
        currentLines = mpLinesDungeon;
        currentLineNum = D_FLAG_MAX;
        {
            gzTextBox* dungeonOpt = currentLines[D_FLAG_SELECT_DUNGEON]->getOptionBox();
            if (dungeonOpt) dungeonOpt->setString(dungeonNames[sSelectedDungeon]);
            gzTextBox* smallKeyOpt = currentLines[D_FLAG_SMALL_KEY]->getOptionBox();
            if (smallKeyOpt) smallKeyOpt->setStringf("%d", getDungeonSmallKeys(getSelectedDungeonStageNo()));
        }
        // TODO: these need proper option boxes when converted to gzBoolOptionLine
        for (int i = 0; i < 7; i++) {
            gzTextBox* opt = currentLines[D_FLAG_BOSS_KEY + i]->getOptionBox();
            if (opt) opt->setStringf("%s", dungeonFlags[i].is(sSelectedDungeon + 16) ? "yes" : "no");
        }
        break;
    case TAB_PORTAL:
        currentLines = mpLinesPortal;
        currentLineNum = P_FLAG_MAX;
        {
            gzTextBox* regionOpt = currentLines[P_FLAG_SELECT_REGION]->getOptionBox();
            if (regionOpt) regionOpt->setString(regionNames[sSelectedRegion]);
        }
        for (int i = 0; i < 16; i++) {
            gzTextBox* opt = currentLines[P_FLAG_REGION + i]->getOptionBox();
            if (opt) opt->setStringf("%s", warpFlags[i].is() ? "on" : "off");
        }
        break;
    case TAB_RUPEE:
        currentLines = mpLinesRupee;
        currentLineNum = R_FLAG_MAX;
        {
            gzTextBox* donationOpt = currentLines[R_FLAG_DONATION_AMT]->getOptionBox();
            if (donationOpt) donationOpt->setStringf("%d", getDonationAmt());
            gzTextBox* fundraisingOpt = currentLines[R_FLAG_FUNDRAISING_AMT]->getOptionBox();
            if (fundraisingOpt) fundraisingOpt->setStringf("%d", getFundraisingAmt());
        }
        for (int i = 0; i < 3; i++) {
            gzTextBox* opt = currentLines[R_FLAG_FUNDRAISING_1 + i]->getOptionBox();
            if (opt) opt->setStringf("%s", rupeeFlags[i].is() ? "on" : "off");
        }
        break;
    default:
        return;
    }
    J2DTextBox::TFontSize font_size;
    for (int i = 0; i < currentLineNum; i++) {
        gzTextBox* opt = currentLines[i]->getOptionBox();
        if (opt) {
            opt->getFontSize(font_size);
            font_size.mSizeX *= 0.5f;
            currentLines[i]->mText->mBounds.f.x = currentLines[i]->mText->mStringLength * font_size.mSizeX;
            opt->mBounds.f.x = opt->mStringLength * font_size.mSizeX;
        }
    }
}

void gzFlagsMenu_c::setRegionFlag(int regionBit) {
    u8 region = g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().getRegion();
    g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().setRegion(region ^=
                                                                               (1 << regionBit));
}

bool gzFlagsMenu_c::getRegionFlag(int regionBit) {
    return g_dComIfG_gameInfo.info.getPlayer().getPlayerFieldLastStayInfo().getRegion() &
           (1 << regionBit);
}

void gzFlagsMenu_c::execute() {
    if (g_gzInfo.mInputWaitTimer != 0) {
        g_gzInfo.mInputWaitTimer--;
        return;
    }

    gzCursor* l_cursor = gzInfo_getCursor();

    int maxLines = getCurrentLineNum();

    if (!gzInfo_isMenuOption()) {
        if (gzPad::getTrigRight()) {
            mCurrentTab = (mCurrentTab + 1) % TAB_MAX_e;
            l_cursor->y = 0;
            gzInfo_resetTopLine();
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }
        if (gzPad::getTrigLeft()) {
            mCurrentTab = (mCurrentTab - 1 + TAB_MAX_e) % TAB_MAX_e;
            l_cursor->y = 0;
            gzInfo_resetTopLine();
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }
        if (gzPad::getTrigDown()) {
            l_cursor->y = (l_cursor->y + 1) % maxLines;
            gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
        }
        if (gzPad::getTrigUp()) {
            l_cursor->y = (l_cursor->y == 0) ? maxLines - 1 : l_cursor->y - 1;
            gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
        }
    } else {
        if (gzPad::getTrigRight()) {
            switch (mCurrentTab) {
            case TAB_GENERAL:
                if (l_cursor->y < G_FLAG_MAX && !generalFlags[l_cursor->y].is()) {
                    generalFlags[l_cursor->y].on();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case TAB_DUNGEON:
                if (l_cursor->y == D_FLAG_SELECT_DUNGEON) {
                    nextDungeon();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y == D_FLAG_SMALL_KEY) {
                    addSmallKey();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y >= D_FLAG_BOSS_KEY && l_cursor->y <= D_FLAG_DEFEAT_MINIBOSS)
                {
                    int bIdx = l_cursor->y - D_FLAG_BOSS_KEY;
                    if (!dungeonFlags[bIdx].is(sSelectedDungeon + 16)) {
                        dungeonFlags[bIdx].on(sSelectedDungeon + 16);
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                }
                break;
            case TAB_PORTAL:
                if (l_cursor->y == P_FLAG_SELECT_REGION) {
                    nextRegion();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y == P_FLAG_REGION) {
                    if (!getRegionFlag(sSelectedRegion + 1)) {
                        setRegionFlag(sSelectedRegion + 1);
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                } else if (l_cursor->y >= P_FLAG_SPRING_WARP && l_cursor->y < P_FLAG_MAX) {
                    int wIdx = l_cursor->y - P_FLAG_SPRING_WARP;
                    if (!warpFlags[wIdx].is()) {
                        warpFlags[wIdx].on();
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                }
                break;
            case TAB_RUPEE:
                if (l_cursor->y == R_FLAG_DONATION_AMT) {
                    u16 val = getDonationAmt() + 1;
                    setDonationAmt(val);
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y == R_FLAG_FUNDRAISING_AMT) {
                    u16 val = getFundraisingAmt() + 1;
                    setFundraisingAmt(val);
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y >= R_FLAG_FUNDRAISING_1 && l_cursor->y < R_FLAG_MAX) {
                    int rIdx = l_cursor->y - R_FLAG_FUNDRAISING_1;
                    if (!rupeeFlags[rIdx].is()) {
                        rupeeFlags[rIdx].on();
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                }
                break;
            }
        }
        if (gzPad::getTrigLeft()) {
            switch (mCurrentTab) {
            case TAB_GENERAL:
                if (l_cursor->y < G_FLAG_MAX && generalFlags[l_cursor->y].is()) {
                    generalFlags[l_cursor->y].off();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case TAB_DUNGEON:
                if (l_cursor->y == D_FLAG_SELECT_DUNGEON) {
                    prevDungeon();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y == D_FLAG_SMALL_KEY) {
                    removeSmallKey();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y >= D_FLAG_BOSS_KEY && l_cursor->y <= D_FLAG_DEFEAT_MINIBOSS)
                {
                    int bIdx = l_cursor->y - D_FLAG_BOSS_KEY;
                    if (dungeonFlags[bIdx].is(sSelectedDungeon + 16)) {
                        dungeonFlags[bIdx].off(sSelectedDungeon + 16);
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                }
                break;
            case TAB_PORTAL:
                if (l_cursor->y == P_FLAG_SELECT_REGION) {
                    prevRegion();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y == P_FLAG_REGION) {
                    if (getRegionFlag(sSelectedRegion + 1)) {
                        setRegionFlag(sSelectedRegion + 1);
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                } else if (l_cursor->y >= P_FLAG_SPRING_WARP && l_cursor->y < P_FLAG_MAX) {
                    int wIdx = l_cursor->y - P_FLAG_SPRING_WARP;
                    if (warpFlags[wIdx].is()) {
                        warpFlags[wIdx].off();
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                }
                break;
            case TAB_RUPEE:
                if (l_cursor->y == R_FLAG_DONATION_AMT) {
                    u16 val = getDonationAmt() - 1;
                    if (val >= 0) {
                        setDonationAmt(val);
                    }
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y == R_FLAG_FUNDRAISING_AMT) {
                    u16 val = getFundraisingAmt() - 1;
                    if (val >= 0) {
                        setFundraisingAmt(val);
                    }
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y >= R_FLAG_FUNDRAISING_1 && l_cursor->y < R_FLAG_MAX) {
                    int rIdx = l_cursor->y - R_FLAG_FUNDRAISING_1;
                    if (rupeeFlags[rIdx].is()) {
                        rupeeFlags[rIdx].off();
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                }
                break;
            }
        }
    }
    if (gzPad::getTrigB()) {
        if (gzInfo_isMenuOption()) {
            gzInfo_offMenuOption();
            gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
        } else {
            l_cursor->x--;
            l_cursor->y = gzMainMenu_c::MENU_FLAGS;
            gzInfo_seStart(Z2SE_SY_EXP_WIN_CLOSE);
            g_gzInfo.mpMainMenu->startReverseTransition();
            return;
        }
    }
    if (gzPad::getTrigA()) {
        bool handled = false;
        if (gzInfo_isMenuOption() && mCurrentTab == TAB_DUNGEON && l_cursor->y == D_FLAG_CLEAR_DUNGEON) {
            clearDungeonFlags(sSelectedDungeon + 16);
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK);
            gzInfo_offMenuOption();
            handled = true;
        }
        if (!handled) {
            gzInfo_setMenuOption(!gzInfo_isMenuOption());
            if (gzInfo_isMenuOption()) {
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK);
            } else {
                gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
            }
        }
    }
    
    updateScrolling(maxLines);
    mpHaihai->_execute(0);
}

void gzFlagsMenu_c::draw() {
    gzCursor* l_cursor = gzInfo_getCursor();
    static const f32 Y_ALIGNMENT = 78.0f;
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
    static const int VISIBLE_LINES = 15;
    static const f32 TAB_HEADER_OFFSET = 15.0f;

    // manually set tab header text distances for now
    // need to support scrolling tabs at some point
    f32 X_POS[TAB_MAX_e];
    f32 tab_header_x_alignment = mXPos + TAB_HEADER_OFFSET;
    X_POS[TAB_GENERAL] = tab_header_x_alignment;
    X_POS[TAB_DUNGEON] = tab_header_x_alignment + 70.0f;
    X_POS[TAB_PORTAL] = tab_header_x_alignment + 150.0f;
    X_POS[TAB_RUPEE] = tab_header_x_alignment + 210.0f;

    updateDynamicLines();

    J2DTextBox::TFontSize font_size;
    gzTextBox* firstOpt = mpLinesGeneral[0]->getOptionBox();
    if (firstOpt) {
        firstOpt->getFontSize(font_size);
        mpMeterHaihai->setScale(font_size.mSizeY * HAIHAI_SCALE_FACTOR);
    }

    u32 cursor_color = gzInfo_getCursorColor();
    f32 y_header_alignment = g_gzInfo.mBackgroundYPos + 48.0f;
    f32 x_alignment_opts = mXPos + OPTIONS_X_OFFSET;
    f32 x_alignment_haihai = x_alignment_opts + HAIHAI_X_OFFSET;
    f32 y_alignment_haihai = Y_ALIGNMENT + HAIHAI_Y_OFFSET;
    f32 x_alignment_tp_cursor = mXPos + TP_CURSOR_X_OFFSET;

    gzLine** currentLines;
    int currentLineNum;
    switch (mCurrentTab) {
    case TAB_GENERAL:
        currentLines = mpLinesGeneral;
        currentLineNum = G_FLAG_MAX;
        break;
    case TAB_DUNGEON:
        currentLines = mpLinesDungeon;
        currentLineNum = D_FLAG_MAX;
        break;
    case TAB_PORTAL:
        currentLines = mpLinesPortal;
        currentLineNum = P_FLAG_MAX;
        break;
    case TAB_RUPEE:
        currentLines = mpLinesRupee;
        currentLineNum = R_FLAG_MAX;
        break;
    }

    for (int i = 0; i < TAB_MAX_e; i++) {
        mpTabHeaders[i]->draw(X_POS[i], y_header_alignment,
                              i == mCurrentTab ? cursor_color : COLOR_WHITE);
    }

    s32 topLine = gzInfo_getTopLine();
    if (l_cursor->y < topLine) {
        topLine = l_cursor->y;
    } else if (l_cursor->y >= topLine + VISIBLE_LINES) {
        topLine = l_cursor->y - VISIBLE_LINES + 1;
    }
    int maxTop = currentLineNum - VISIBLE_LINES;
    if (maxTop < 0)
        maxTop = 0;
    if (topLine > maxTop)
        topLine = maxTop;
    if (topLine < 0)
        topLine = 0;
    gzInfo_setTopLine(topLine);

    for (int screenIdx = 0; screenIdx < VISIBLE_LINES; screenIdx++) {
        int lineIdx = topLine + screenIdx;
        if (lineIdx >= currentLineNum)
            break;
        f32 y_pos = Y_ALIGNMENT + (screenIdx * LINE_SPACING);
        gzTextBox* opt = currentLines[lineIdx]->getOptionBox();
        if (l_cursor->y == lineIdx && gzInfo_isSubMenuVisible()) {
            currentLines[lineIdx]->draw(mXPos, y_pos, cursor_color);
            if (opt) {
                f32 x_size_haihai = opt->mBounds.f.x + HAIHAI_EXTRA_SPACING;
                if (gzInfo_isMenuOption()) {
                    mpMeterHaihai->drawHaihai(getHaihaiFlags(lineIdx), x_alignment_haihai,
                                              y_pos + HAIHAI_Y_OFFSET, x_size_haihai, 0.0f);
                }
                opt->draw(x_alignment_opts, y_pos, cursor_color, HBIND_CENTER);
            }
            gzInfo_getTPCursor()->setPos(x_alignment_tp_cursor, y_pos - 10.0f,
                                 (J2DPane*)currentLines[lineIdx]->mText, false);
        } else {
            currentLines[lineIdx]->draw(mXPos, y_pos, COLOR_WHITE);
            if (opt) {
                opt->draw(x_alignment_opts, y_pos, COLOR_WHITE, HBIND_CENTER);
            }
        }
    }

    if (gzInfo_isSubMenuVisible()) {
        if (currentLines[l_cursor->y] && currentLines[l_cursor->y]->m_description[0] != 0) {
            f32 description_y = g_gzInfo.mBackgroundHeight + 25.0f;
            gzInfo_getMenuDescription()->setString(currentLines[l_cursor->y]->m_description);
            gzInfo_getMenuDescription()->draw(DESCRIPTION_X, description_y, cursor_color, HBIND_CENTER);
        }
    }

    if (gzInfo_isCursorTypeTP()) {
        if (gzInfo_getTPCursor() != NULL) {
            gzInfo_getTPCursor()->draw();
        }
    }
}
