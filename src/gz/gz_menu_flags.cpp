#include "d/dolzel.h"  // IWYU pragma: keep

#include "gz/gz_menu_flags.h"
#include "gz/gz_menu_main.h"

// General flags
static const BoolFlagInfo generalFlags[15] = {
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

// Dungeon bool flags
static const DungeonBoolFlagInfo dungeonFlags[7] = {
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

// Portal warp flags
static const WarpBoolFlagInfo warpFlags[15] = {
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
static const RupeeBoolFlagInfo rupeeFlags[3] = {
    {"fundraising 1", "Toggle flag for first fundraising being complete", isFundraising1, onFundraising1, offFundraising1},
    {"fundraising 2", "Toggle flag for second fundraising being complete", isFundraising2, onFundraising2, offFundraising2},
    {"rupee cutscenes", "Toggle rupee cutscenes being enabled", isRupeeCS, onRupeeCS, offRupeeCS}};

// Dungeon and region names
static const char* dungeonNames[] = {"Forest Temple",     "Goron Mines",        "Lakebed Temple",
                                     "Arbiter's Grounds", "Snowpeak Ruins",     "Temple of Time",
                                     "City in the Sky",   "Palace of Twilight", "Hyrule Castle"};
#define NUM_DUNGEONS ARRAY_SIZE(dungeonNames)

static const char* regionNames[] = {"ordon", "faron", "eldin", "lanayru", "desert", "snowpeak"};
#define NUM_REGIONS ARRAY_SIZE(regionNames)

gzFlagsMenu_c::gzFlagsMenu_c() {
    OSReport("creating gzFlagsMenu_c\n");
    for (int i = 0; i < TAB_MAX; i++) {
        mpTabHeaders[i] = allocateTextBox();
        mpTabHeaders[i]->setFontSize(15.0f, 15.0f);
    }
    mpTabHeaders[TAB_GENERAL]->setString("general");
    mpTabHeaders[TAB_DUNGEON]->setString("dungeon");
    mpTabHeaders[TAB_PORTAL]->setString("portal");
    mpTabHeaders[TAB_RUPEE]->setString("rupee");

    for (int i = 0; i < G_FLAG_MAX; i++) {
        mpLinesGeneral[i] = allocateTextBox();
        mpLinesGeneral[i]->mBounds.f.x = 430.0f;
        mpLinesGeneral[i]->mBounds.f.y = 10.0f;
        mpLinesGeneral[i]->setStringDesc(generalFlags[i].mName, generalFlags[i].mDesc);
        mpLineOptionsGeneral[i] = allocateTextBox();
        mpLineOptionsGeneral[i]->mBounds.f.y = 10.0f;
    }

    for (int i = 0; i < D_FLAG_MAX; i++) {
        mpLinesDungeon[i] = allocateTextBox();
        mpLinesDungeon[i]->mBounds.f.x = 430.0f;
        mpLinesDungeon[i]->mBounds.f.y = 10.0f;
        mpLineOptionsDungeon[i] = allocateTextBox();
        mpLineOptionsDungeon[i]->mBounds.f.y = 10.0f;
    }

    // Set strings for dungeon tab
    mpLinesDungeon[D_FLAG_SELECT_DUNGEON]->setStringDesc("dungeon:", "Selected dungeon flags");
    mpLinesDungeon[D_FLAG_SMALL_KEY]->setStringDesc("small keys", "Selected dungeon small keys");

    for (int i = 0; i < 7; i++) {
        mpLinesDungeon[D_FLAG_BOSS_KEY + i]->setStringDesc(dungeonFlags[i].mName,
                                                           dungeonFlags[i].mDesc);
    }

    mpLinesDungeon[D_FLAG_CLEAR_DUNGEON]->setStringDesc("clear flags",
                                                        "Clear all selected dungeon flags");

    for (int i = 0; i < P_FLAG_MAX; i++) {
        mpLinesPortal[i] = allocateTextBox();
        mpLinesPortal[i]->mBounds.f.x = 430.0f;
        mpLinesPortal[i]->mBounds.f.y = 10.0f;
        mpLineOptionsPortal[i] = allocateTextBox();
        mpLineOptionsPortal[i]->mBounds.f.y = 10.0f;
    }

    // Set strings for portal tab
    mpLinesPortal[P_FLAG_SELECT_REGION]->setStringDesc("region:", "Select region flag");
    mpLinesPortal[P_FLAG_REGION]->setStringDesc("region unlocked", "Unlock selected map region");

    for (int i = 0; i < 15; i++) {
        mpLinesPortal[P_FLAG_SPRING_WARP + i]->setStringDesc(warpFlags[i].mName,
                                                             warpFlags[i].mDesc);
    }

    for (int i = 0; i < R_FLAG_MAX; i++) {
        mpLinesRupee[i] = allocateTextBox();
        mpLinesRupee[i]->mBounds.f.x = 430.0f;
        mpLinesRupee[i]->mBounds.f.y = 10.0f;
        mpLineOptionsRupee[i] = allocateTextBox();
        mpLineOptionsRupee[i]->mBounds.f.y = 10.0f;
    }
    // Set strings for rupee tab
    mpLinesRupee[R_FLAG_DONATION_AMT]->setStringDesc("donation amount:",
                                                     "Sets the amount of rupees donated to Charlo");
    mpLinesRupee[R_FLAG_FUNDRAISING_AMT]->setStringDesc("fundraising amount:",
                                                        "Sets the current fundraising amount");

    for (int i = 0; i < 3; i++) {
        mpLinesRupee[R_FLAG_FUNDRAISING_1 + i]->setStringDesc(rupeeFlags[i].mName,
                                                              rupeeFlags[i].mDesc);
    }

    mpDescription = allocateTextBox();
    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpDrawCursor->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    mpDrawCursor->setAlphaRate(1.0f);
    mpMeterHaihai = new dMeterHaihai_c(3);
    mCurrentTab = TAB_GENERAL;
    mTopLine = 0;
    mSelectedDungeon = 0;
    mSelectedRegion = 0;
    mOption = false;
}

gzFlagsMenu_c::~gzFlagsMenu_c() {
    _delete();
}

void gzFlagsMenu_c::_delete() {
    OSReport("deleting gzFlagsMenu_c\n");
    for (int i = 0; i < TAB_MAX; i++) {
        delete mpTabHeaders[i];
        mpTabHeaders[i] = NULL;
    }
    for (int i = 0; i < G_FLAG_MAX; i++) {
        delete mpLinesGeneral[i];
        mpLinesGeneral[i] = NULL;
        delete mpLineOptionsGeneral[i];
        mpLineOptionsGeneral[i] = NULL;
    }
    for (int i = 0; i < D_FLAG_MAX; i++) {
        delete mpLinesDungeon[i];
        mpLinesDungeon[i] = NULL;
        delete mpLineOptionsDungeon[i];
        mpLineOptionsDungeon[i] = NULL;
    }
    for (int i = 0; i < P_FLAG_MAX; i++) {
        delete mpLinesPortal[i];
        mpLinesPortal[i] = NULL;
        delete mpLineOptionsPortal[i];
        mpLineOptionsPortal[i] = NULL;
    }
    for (int i = 0; i < R_FLAG_MAX; i++) {
        delete mpLinesRupee[i];
        mpLinesRupee[i] = NULL;
        delete mpLineOptionsRupee[i];
        mpLineOptionsRupee[i] = NULL;
    }
    delete mpDescription;
    mpDescription = NULL;
    delete mpDrawCursor;
    mpDrawCursor = NULL;
    delete mpMeterHaihai;
    mpMeterHaihai = NULL;
}

u8 gzFlagsMenu_c::getHaihaiFlags(int idx) {
    u8 haihai_flags = ARROW_LEFT | ARROW_RIGHT;
    switch (mCurrentTab) {
    case TAB_GENERAL:
        if (generalFlags[idx].mIsOn()) {
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
            if (dungeonFlags[bIdx].mIsOn(mSelectedDungeon + 16)) {
                haihai_flags &= ~ARROW_RIGHT;
            } else {
                haihai_flags &= ~ARROW_LEFT;
            }
        }
        break;
    case TAB_PORTAL:
        if (idx == P_FLAG_SELECT_REGION) {
        } else if (idx == P_FLAG_REGION) {
            if (getRegionFlag(mSelectedRegion + 1)) {
                haihai_flags &= ~ARROW_RIGHT;
            } else {
                haihai_flags &= ~ARROW_LEFT;
            }
        } else if (idx >= P_FLAG_SPRING_WARP && idx < P_FLAG_MAX) {
            int wIdx = idx - P_FLAG_SPRING_WARP;
            if (warpFlags[wIdx].mIsOn()) {
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
            if (rupeeFlags[rIdx].mIsOn()) {
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
    gzTextBox** currentLines;
    gzTextBox** currentLineOptions;
    int currentLineNum;
    switch (mCurrentTab) {
    case TAB_GENERAL:
        currentLines = mpLinesGeneral;
        currentLineOptions = mpLineOptionsGeneral;
        currentLineNum = G_FLAG_MAX;
        for (int i = 0; i < G_FLAG_MAX; i++) {
            currentLineOptions[i]->setStringf("%s", generalFlags[i].mIsOn() ? "on" : "off");
        }
        break;
    case TAB_DUNGEON:
        currentLines = mpLinesDungeon;
        currentLineOptions = mpLineOptionsDungeon;
        currentLineNum = D_FLAG_MAX;
        currentLineOptions[D_FLAG_SELECT_DUNGEON]->setStringf("%s", dungeonNames[mSelectedDungeon]);
        currentLineOptions[D_FLAG_SMALL_KEY]->setStringf("%d", getDungeonSmallKeys(mSelectedDungeon + 16));
        for (int i = 0; i < 7; i++) {
            currentLineOptions[D_FLAG_BOSS_KEY + i]->setStringf(
                "%s", dungeonFlags[i].mIsOn(mSelectedDungeon + 16) ? "yes" : "no");
        }
        currentLineOptions[D_FLAG_CLEAR_DUNGEON]->setString("");
        break;
    case TAB_PORTAL:
        currentLines = mpLinesPortal;
        currentLineOptions = mpLineOptionsPortal;
        currentLineNum = P_FLAG_MAX;
        currentLineOptions[P_FLAG_SELECT_REGION]->setStringf("%s", regionNames[mSelectedRegion]);
        currentLineOptions[P_FLAG_REGION]->setStringf("%s", getRegionFlag(mSelectedRegion + 1) ? "on" : "off");
        OSReport("mSelectedRegion: %d\n", mSelectedRegion);
        for (int i = 0; i < 15; i++) {
            currentLineOptions[P_FLAG_SPRING_WARP + i]->setStringf(
                "%s", warpFlags[i].mIsOn() ? "on" : "off");
        }
        break;
    case TAB_RUPEE:
        currentLines = mpLinesRupee;
        currentLineOptions = mpLineOptionsRupee;
        currentLineNum = R_FLAG_MAX;
        currentLineOptions[R_FLAG_DONATION_AMT]->setStringf("%d", getDonationAmt());
        currentLineOptions[R_FLAG_FUNDRAISING_AMT]->setStringf("%d", getFundraisingAmt());
        for (int i = 0; i < 3; i++) {
            currentLineOptions[R_FLAG_FUNDRAISING_1 + i]->setStringf(
                "%s", rupeeFlags[i].mIsOn() ? "on" : "off");
        }
        break;
    default:
        return;
    }
    J2DTextBox::TFontSize font_size;
    for (int i = 0; i < currentLineNum; i++) {
        currentLineOptions[i]->getFontSize(font_size);
        font_size.mSizeX *= 0.5f;
        currentLines[i]->mBounds.f.x = currentLines[i]->mStringLength * font_size.mSizeX;
        currentLineOptions[i]->mBounds.f.x =
            currentLineOptions[i]->mStringLength * font_size.mSizeX;
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
    if (!mOption) {
        if (gzPad::getTrigDown()) {
            l_cursor->y = (l_cursor->y + 1) % getCurrentLineNum();
            gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
        }
        if (gzPad::getTrigUp()) {
            l_cursor->y = (l_cursor->y - 1 + getCurrentLineNum()) % getCurrentLineNum();
            gzInfo_seStart(Z2SE_SY_NAME_CURSOR);
        }
        if (gzPad::getTrigRight()) {
            mCurrentTab = (mCurrentTab + 1) % TAB_MAX;
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }
        if (gzPad::getTrigLeft()) {
            mCurrentTab = (mCurrentTab - 1 + TAB_MAX) % TAB_MAX;
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
        }
    } else {
        if (gzPad::getTrigRight()) {
            switch (mCurrentTab) {
            case TAB_GENERAL:
                if (l_cursor->y < G_FLAG_MAX && !generalFlags[l_cursor->y].mIsOn()) {
                    generalFlags[l_cursor->y].mOn();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case TAB_DUNGEON:
                if (l_cursor->y == D_FLAG_SELECT_DUNGEON) {
                    mSelectedDungeon = (mSelectedDungeon + 1) % NUM_DUNGEONS;
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y == D_FLAG_SMALL_KEY) {
                    int val = getDungeonSmallKeys(mSelectedDungeon + 16) + 1;
                    setDungeonSmallKeys(mSelectedDungeon + 16, val);
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y >= D_FLAG_BOSS_KEY && l_cursor->y <= D_FLAG_DEFEAT_MINIBOSS)
                {
                    int bIdx = l_cursor->y - D_FLAG_BOSS_KEY;
                    if (!dungeonFlags[bIdx].mIsOn(mSelectedDungeon + 16)) {
                        dungeonFlags[bIdx].mOn(mSelectedDungeon + 16);
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                }
                break;
            case TAB_PORTAL:
                if (l_cursor->y == P_FLAG_SELECT_REGION) {
                    mSelectedRegion = (mSelectedRegion + 1) % NUM_REGIONS;
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y == P_FLAG_REGION) {
                    if (!getRegionFlag(mSelectedRegion + 1)) {
                        setRegionFlag(mSelectedRegion + 1);
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                } else if (l_cursor->y >= P_FLAG_SPRING_WARP && l_cursor->y < P_FLAG_MAX) {
                    int wIdx = l_cursor->y - P_FLAG_SPRING_WARP;
                    if (!warpFlags[wIdx].mIsOn()) {
                        warpFlags[wIdx].mOn();
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
                    if (!rupeeFlags[rIdx].mIsOn()) {
                        rupeeFlags[rIdx].mOn();
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                }
                break;
            }
        }
        if (gzPad::getTrigLeft()) {
            switch (mCurrentTab) {
            case TAB_GENERAL:
                if (l_cursor->y < G_FLAG_MAX && generalFlags[l_cursor->y].mIsOn()) {
                    generalFlags[l_cursor->y].mOff();
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                }
                break;
            case TAB_DUNGEON:
                if (l_cursor->y == D_FLAG_SELECT_DUNGEON) {
                    mSelectedDungeon = (mSelectedDungeon - 1 + NUM_DUNGEONS) % NUM_DUNGEONS;
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y == D_FLAG_SMALL_KEY) {
                    int val = getDungeonSmallKeys(mSelectedDungeon + 16) - 1;
                    if (val >= 0) {
                        setDungeonSmallKeys(mSelectedDungeon + 16, val);
                    }
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y >= D_FLAG_BOSS_KEY && l_cursor->y <= D_FLAG_DEFEAT_MINIBOSS)
                {
                    int bIdx = l_cursor->y - D_FLAG_BOSS_KEY;
                    if (dungeonFlags[bIdx].mIsOn(mSelectedDungeon + 16)) {
                        dungeonFlags[bIdx].mOff(mSelectedDungeon + 16);
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                }
                break;
            case TAB_PORTAL:
                if (l_cursor->y == P_FLAG_SELECT_REGION) {
                    mSelectedRegion = (mSelectedRegion - 1 + NUM_REGIONS) % NUM_REGIONS;
                    gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                } else if (l_cursor->y == P_FLAG_REGION) {
                    if (getRegionFlag(mSelectedRegion + 1)) {
                        setRegionFlag(mSelectedRegion + 1);
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                } else if (l_cursor->y >= P_FLAG_SPRING_WARP && l_cursor->y < P_FLAG_MAX) {
                    int wIdx = l_cursor->y - P_FLAG_SPRING_WARP;
                    if (warpFlags[wIdx].mIsOn()) {
                        warpFlags[wIdx].mOff();
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
                    if (rupeeFlags[rIdx].mIsOn()) {
                        rupeeFlags[rIdx].mOff();
                        gzInfo_seStart(Z2SE_SY_TALK_CURSOR);
                    }
                }
                break;
            }
        }
    }
    if (gzPad::getTrigB()) {
        if (mOption) {
            mOption = false;
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
        if (mOption && mCurrentTab == TAB_DUNGEON && l_cursor->y == D_FLAG_CLEAR_DUNGEON) {
            clearDungeonFlags(mSelectedDungeon + 16);
            gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK);
            mOption = false;
            handled = true;
        }
        if (!handled) {
            mOption = !mOption;
            if (mOption) {
                gzInfo_seStart(Z2SE_SY_TALK_CURSOR_OK);
            } else {
                gzInfo_seStart(Z2SE_SY_CURSOR_CANCEL);
            }
        }
    }
    mpMeterHaihai->_execute(0);
}

void gzFlagsMenu_c::draw() {
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
    static const int VISIBLE_LINES = 15;
    static const f32 TAB_HEADER_OFFSET = 15.0f;

    // manually set tab header text distances for now
    // need to support scrolling tabs at some point
    f32 X_POS[TAB_MAX];
    f32 tab_header_x_alignment = mXPos + TAB_HEADER_OFFSET;
    X_POS[TAB_GENERAL] = tab_header_x_alignment;
    X_POS[TAB_DUNGEON] = tab_header_x_alignment + 70.0f;
    X_POS[TAB_PORTAL] = tab_header_x_alignment + 150.0f;
    X_POS[TAB_RUPEE] = tab_header_x_alignment + 210.0f;

    updateDynamicLines();

    J2DTextBox::TFontSize font_size;
    mpLineOptionsGeneral[0]->getFontSize(font_size);
    mpMeterHaihai->setScale(font_size.mSizeY * HAIHAI_SCALE_FACTOR);

    u32 cursor_color = gzInfo_getCursorColor();
    f32 y_header_alignment = g_gzInfo.mBackgroundYPos + 48.0f;
    f32 x_alignment_opts = mXPos + OPTIONS_X_OFFSET;
    f32 x_alignment_haihai = x_alignment_opts + HAIHAI_X_OFFSET;
    f32 y_alignment_haihai = Y_ALIGNMENT + HAIHAI_Y_OFFSET;
    f32 x_alignment_tp_cursor = mXPos + TP_CURSOR_X_OFFSET;

    gzTextBox** currentLines;
    gzTextBox** currentLineOptions;
    int currentLineNum;
    switch (mCurrentTab) {
    case TAB_GENERAL:
        currentLines = mpLinesGeneral;
        currentLineOptions = mpLineOptionsGeneral;
        currentLineNum = G_FLAG_MAX;
        break;
    case TAB_DUNGEON:
        currentLines = mpLinesDungeon;
        currentLineOptions = mpLineOptionsDungeon;
        currentLineNum = D_FLAG_MAX;
        break;
    case TAB_PORTAL:
        currentLines = mpLinesPortal;
        currentLineOptions = mpLineOptionsPortal;
        currentLineNum = P_FLAG_MAX;
        break;
    case TAB_RUPEE:
        currentLines = mpLinesRupee;
        currentLineOptions = mpLineOptionsRupee;
        currentLineNum = R_FLAG_MAX;
        break;
    }

    for (int i = 0; i < TAB_MAX; i++) {
        mpTabHeaders[i]->draw(X_POS[i], y_header_alignment,
                              i == mCurrentTab ? cursor_color : COLOR_WHITE);
    }

    if (l_cursor->y < mTopLine) {
        mTopLine = l_cursor->y;
    } else if (l_cursor->y >= mTopLine + VISIBLE_LINES) {
        mTopLine = l_cursor->y - VISIBLE_LINES + 1;
    }
    int maxTop = currentLineNum - VISIBLE_LINES;
    if (maxTop < 0)
        maxTop = 0;
    if (mTopLine > maxTop)
        mTopLine = maxTop;
    if (mTopLine < 0)
        mTopLine = 0;

    for (int screenIdx = 0; screenIdx < VISIBLE_LINES; screenIdx++) {
        int lineIdx = mTopLine + screenIdx;
        if (lineIdx >= currentLineNum)
            break;
        f32 y_pos = Y_ALIGNMENT + (screenIdx * LINE_SPACING);
        if (l_cursor->y == lineIdx && gzInfo_isSubMenuVisible()) {
            currentLines[lineIdx]->draw(mXPos, y_pos, cursor_color);
            f32 x_size_haihai = currentLineOptions[lineIdx]->mBounds.f.x + HAIHAI_EXTRA_SPACING;
            if (mOption) {
                mpMeterHaihai->drawHaihai(getHaihaiFlags(lineIdx), x_alignment_haihai,
                                          y_pos + HAIHAI_Y_OFFSET, x_size_haihai, 0.0f);
            }
            mpDrawCursor->setPos(x_alignment_tp_cursor, y_pos - 10.0f,
                                 (J2DPane*)currentLines[lineIdx], false);
            currentLineOptions[lineIdx]->draw(x_alignment_opts, y_pos, cursor_color, HBIND_CENTER);
        } else {
            currentLines[lineIdx]->draw(mXPos, y_pos, COLOR_WHITE);
            currentLineOptions[lineIdx]->draw(x_alignment_opts, y_pos, COLOR_WHITE, HBIND_CENTER);
        }
    }

    if (gzInfo_isSubMenuVisible()) {
        if (currentLines[l_cursor->y] && *currentLines[l_cursor->y]->m_description != 0) {
            f32 description_y = g_gzInfo.mBackgroundHeight + 25.0f;
            mpDescription->setString(currentLines[l_cursor->y]->m_description);
            mpDescription->draw(DESCRIPTION_X, description_y, cursor_color, HBIND_CENTER);
        }
    }

    if (gzInfo_isCursorTypeTP()) {
        if (mpDrawCursor != NULL) {
            mpDrawCursor->draw();
        }
    }
}
