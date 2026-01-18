#ifndef GZ_MENU_FLAGS_H
#define GZ_MENU_FLAGS_H

#include "gz/gz_menu.h"
#include "gz/gz_textbox.h"

class gzFlagsMenu_c : public gzMenu_c {
public:
    enum gzFlagsMenu_Tabs_e {
        TAB_GENERAL,
        TAB_DUNGEON,
        TAB_PORTAL,
        TAB_RUPEE,

        TAB_MAX_e
    };

    enum gzFlagsMenu_General_e {
        G_FLAG_BOSS_FLAG,
        G_FLAG_CORO_TD,
        G_FLAG_EPONA_STOLEN,
        G_FLAG_EPONA_TAMED,
        G_FLAG_MALO_MART_CT,
        G_FLAG_MAP_WARPING,
        G_FLAG_MIDNA_CHARGE,
        G_FLAG_MIDNA_HEALTHY,
        G_FLAG_MIDNA_ON_BACK,
        G_FLAG_MIDNA_Z,
        G_FLAG_RUSL_TD,
        G_FLAG_TRANSFORM_WARP,
        G_FLAG_WOLF_SENSE,

        G_FLAG_MAX
    };

    enum gzFlagsMenu_Dungeon_e {
        D_FLAG_SELECT_DUNGEON,
        D_FLAG_SMALL_KEY,
        D_FLAG_BOSS_KEY,
        D_FLAG_COMPASS,
        D_FLAG_HC,
        D_FLAG_MAP,
        D_FLAG_OOCCOO,
        D_FLAG_DEFEAT_BOSS,
        D_FLAG_DEFEAT_MINIBOSS,
        D_FLAG_CLEAR_DUNGEON,

        D_FLAG_MAX
    };

    enum gzFlagsMenu_Portal_e {
        P_FLAG_SELECT_REGION,
        P_FLAG_REGION,
        P_FLAG_SPRING_WARP,
        P_FLAG_S_FARON_WARP,
        P_FLAG_N_FARON_WARP,
        P_FLAG_GROVE_WARP,
        P_FLAG_GORGE_WARP,
        P_FLAG_KAKARIKO_WARP,
        P_FLAG_MOUNTAIN_WARP,
        P_FLAG_BRIDGE_WARP,
        P_FLAG_TOWN_WARP,
        P_FLAG_LAKE_WARP,
        P_FLAG_DOMAIN_WARP,
        P_FLAG_UZR_WARP,
        P_FLAG_SNOWPEAK_WARP,
        P_FLAG_MESA_WARP,
        P_FLAG_MIRROR_WARP,

        P_FLAG_MAX
    };

    enum gzFlagsMenu_Rupee_e {
        R_FLAG_DONATION_AMT,
        R_FLAG_FUNDRAISING_AMT,
        R_FLAG_FUNDRAISING_1,
        R_FLAG_FUNDRAISING_2,
        R_FLAG_RUPEE_CS_FLAG,

        R_FLAG_MAX
    };

    static const int NUM_DUNGEONS = 9;
    static const int NUM_REGIONS = 6;

    gzFlagsMenu_c();
    ~gzFlagsMenu_c();

    virtual void _delete();
    virtual void execute();
    virtual void draw();
    virtual u8 getHaihaiFlags(int idx);

    static u32 nextDungeon();
    static u32 prevDungeon();
    static u32 nextRegion();
    static u32 prevRegion();
    static u32 addSmallKey();
    static u32 removeSmallKey();
    static int getSelectedDungeonStageNo() { return sSelectedDungeon + 16; }

private:
    void updateDynamicLines();
    int getCurrentLineNum();
    void setRegionFlag(int);
    bool getRegionFlag(int);

private:
    gzTextBox* mpTabHeaders[TAB_MAX_e];
    gzLine* mpLinesGeneral[G_FLAG_MAX];
    gzLine* mpLinesDungeon[D_FLAG_MAX];
    gzLine* mpLinesPortal[P_FLAG_MAX];
    gzLine* mpLinesRupee[R_FLAG_MAX];

    int mCurrentTab;

    static int sSelectedDungeon;
    static int sSelectedRegion;
};

inline u8 getDungeonSmallKeys(int i_stageNo) { return dComIfGs_getSaveData()->getSave(i_stageNo).getBit().getKeyNum(); }
inline void setDungeonSmallKeys(int i_stageNo, u8 i_numKeys) { 
    dComIfGs_setKeyNum(i_numKeys);
    dComIfGs_getSaveData()->getSave(i_stageNo).getBit().setKeyNum(i_numKeys); 
}

inline bool isSpringWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::ORDON, 52); }
inline void onSpringWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::ORDON, 52); }
inline void offSpringWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::ORDON, 52); }
inline bool isSFaronWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::FARON, 71); }
inline void onSFaronWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::FARON, 71); }
inline void offSFaronWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::FARON, 71); }
inline bool isNFaronWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::FARON, 2); }
inline void onNFaronWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::FARON, 2); }
inline void offNFaronWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::FARON, 2); }
inline bool isGroveWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::GROVE, 100); }
inline void onGroveWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::GROVE, 100); }
inline void offGroveWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::GROVE, 100); }
inline bool isGorgeWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::FIELD, 21); }
inline void onGorgeWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::FIELD, 21); }
inline void offGorgeWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::FIELD, 21); }
inline bool isKakarikoWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::ELDIN, 31); }
inline void onKakarikoWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::ELDIN, 31); }
inline void offKakarikoWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::ELDIN, 31); }
inline bool isMountainWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::ELDIN, 21); }
inline void onMountainWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::ELDIN, 21); }
inline void offMountainWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::ELDIN, 21); }
inline bool isBridgeWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::FIELD, 99); }
inline void onBridgeWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::FIELD, 99); }
inline void offBridgeWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::FIELD, 99); }
inline bool isTownWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::FIELD, 3); }
inline void onTownWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::FIELD, 3); }
inline void offTownWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::FIELD, 3); }
inline bool isLakeWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::LANAYRU, 10); }
inline void onLakeWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::LANAYRU, 10); }
inline void offLakeWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::LANAYRU, 10); }
inline bool isDomainWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::LANAYRU, 2); }
inline void onDomainWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::LANAYRU, 2); }
inline void offDomainWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::LANAYRU, 2); }
inline bool isUzrWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::LANAYRU, 21); }
inline void onUzrWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::LANAYRU, 21); }
inline void offUzrWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::LANAYRU, 21); }
inline bool isSnowpeakWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::SNOWPEAK, 21); }
inline void onSnowpeakWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::SNOWPEAK, 21); }
inline void offSnowpeakWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::SNOWPEAK, 21); }
inline bool isMesaWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::DESERT, 21); }
inline void onMesaWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::DESERT, 21); }
inline void offMesaWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::DESERT, 21); }
inline bool isMirrorWarp() { return dComIfGs_isSaveSwitch(dSv_memory_c::DESERT, 40); }
inline void onMirrorWarp() { dComIfGs_onSaveSwitch(dSv_memory_c::DESERT, 40); }
inline void offMirrorWarp() { dComIfGs_offSaveSwitch(dSv_memory_c::DESERT, 40); }

inline u16 getDonationAmt() { 
    u8 donation_high_bits = dComIfGs_getEventReg(0xf7ff);
    u8 donation_low_bits = dComIfGs_getEventReg(0xf8ff);

    return donation_high_bits << 8 | donation_low_bits; 
}
inline void setDonationAmt(u16& i_amt) {
    if (i_amt > 1000)
        i_amt = 1000;

    dComIfGs_setEventReg(0xf7ff, (i_amt >> 8) & 0xFF);
    dComIfGs_setEventReg(0xf8ff, i_amt & 0xFF);
}

inline u16 getFundraisingAmt() {
    u8 fund_high_bits = dComIfGs_getEventReg(0xf9ff);
    u8 fund_low_bits = dComIfGs_getEventReg(0xfaff);

    return fund_high_bits << 8 | fund_low_bits;
}

inline void setFundraisingAmt(u16& i_amt) {
    if (i_amt > 2000)
        i_amt = 2000;

    dComIfGs_setEventReg(0xf9ff, (i_amt >> 8) & 0xFF);
    dComIfGs_setEventReg(0xfaff, i_amt & 0xFF);
}

inline bool isFundraising1() { return dComIfGs_isEventBit(0x2e20); }
inline void onFundraising1() { dComIfGs_onEventBit(0x2e20); }
inline void offFundraising1() { dComIfGs_offEventBit(0x2e20); }
inline bool isFundraising2() { return dComIfGs_isEventBit(0x0f10); }
inline void onFundraising2() {  dComIfGs_onEventBit(0x0f10); }
inline void offFundraising2() { dComIfGs_offEventBit(0x0f10); }
inline bool isRupeeCS() { 
    for (int itemNo = fpcNm_ITEM_BLUE_RUPEE; itemNo <= fpcNm_ITEM_SILVER_RUPEE; itemNo++) {
        if (dComIfGs_isItemFirstBit(itemNo)) return false;
    }
    return true;
}
inline void onRupeeCS() { 
    for (int itemNo = fpcNm_ITEM_BLUE_RUPEE; itemNo <= fpcNm_ITEM_SILVER_RUPEE; itemNo++) {
        dComIfGs_offItemFirstBit(itemNo);
    }
}
inline void offRupeeCS() { 
    for (int itemNo = fpcNm_ITEM_BLUE_RUPEE; itemNo <= fpcNm_ITEM_SILVER_RUPEE; itemNo++) {
        dComIfGs_onItemFirstBit(itemNo);
    }
}

inline void clearDungeonFlags(int i_stageNo) {
    memset((void*)&dComIfGs_getSaveData()->getSave(i_stageNo).getBit(), 0, sizeof(dSv_memBit_c));
}

#endif // GZ_MENU_FLAGS_H
