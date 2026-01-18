#include "gz/gz.h"
#include "d/actor/d_a_alink.h"

void gzCheatsMng_c::executeDisableItemTimer() {
    daItemBase_c::m_data.mWaitTime = 0x7FFF;
    mDisableItemTimer = true;
}

void gzCheatsMng_c::executeDisableWalls() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link != NULL) {
        link->mLinkAcch.SetWallNone();
        link->mLinkAcch.OnLineCheckNone();
        mDisableWalls = true;
    }
}

void gzCheatsMng_c::executeEnableItemTimer() {
    daItemBase_c::m_data.mWaitTime = 240;
    mDisableItemTimer = false;
}


void gzCheatsMng_c::executeEnableWalls() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link != NULL) {
        link->mLinkAcch.ClrWallNone();
        link->mLinkAcch.OffLineCheckNone();
        mDisableWalls = false;
    }
}

void gzCheatsMng_c::executeInfiniteAir() {
    dComIfGp_setOxygen(600);
    dComIfGp_setNowOxygen(600);
    dComIfGp_setMaxOxygen(600);
}

void gzCheatsMng_c::executeInfiniteArrows() {
    dComIfGs_setArrowNum(100);
}

void gzCheatsMng_c::executeInfiniteBombs() {
    // setting to 60 works on all bags regardless type / upgrade
    dComIfGs_setBombNum(0, 60);
    dComIfGs_setBombNum(1, 60);
    dComIfGs_setBombNum(2, 60);
}

void gzCheatsMng_c::executeInfiniteHearts() {
    // needs to be maxLifeGauge for great spin to still work without modification
    dComIfGs_setLife(dComIfGs_getMaxLifeGauge());
}

void gzCheatsMng_c::executeInfiniteOil() {
    dComIfGs_setOil(21600);
}

void gzCheatsMng_c::executeInfiniteRupees() {
    dComIfGs_setRupee(1000);
}

void gzCheatsMng_c::executeInfiniteSlingshot() {
    dComIfGs_setPachinkoNum(dComIfGs_getPachinkoMax());
}

void gzCheatsMng_c::executeInvincibleLink() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link != NULL) {
        for (int i = 0; i < 3; i++) {
            link->mTgCyls[i].GetGObjInf()->OffTgSetBit();
            link->mTgCyls[i].GetGObjInf()->ClrTgHit();
        }

        if (link->checkWolf()) {
            link->mAtSph.GetGObjInf()->OffTgSetBit();
            link->mAtSph.GetGObjInf()->ClrTgHit();
        }
    }
}

void gzCheatsMng_c::executeMoonJump() {
    u32 combo = g_gzInfo.mSettings.mCommandCombos.mMoonJump;
    if (combo && ((gzPad::getHold() & combo) == combo)) {
        daAlink_c* link = daAlink_getAlinkActorClass();
        if (link) {
            fopAcM_GetSpeed_p(link)->y = 56.0f;
        }
    }
}

void gzCheatsMng_c::executeSuperClawshot() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link != NULL) {
        daAlinkHIO_hookshot_c0::m.mShootSpeed= 2870.0f;
        daAlinkHIO_hookshot_c0::m.mMaxLength = 69420.0f;
        daAlinkHIO_hookshot_c0::m.mReturnSpeed = 2870.0f;
        daAlinkHIO_hookshot_c0::m.mStickReturnSpeed = 500.0f;
        mSuperClawshot = true;
    }
}

void gzCheatsMng_c::execute() {
    if (gzInfo_isDisableItemTimer()) {
        executeDisableItemTimer();
    } else if (mDisableItemTimer) {
        executeEnableItemTimer();
    }

    if (gzInfo_isDisableWalls()) {
        executeDisableWalls();
    } else if (mDisableWalls) {
        executeEnableWalls();
    }

    if (gzInfo_isInfiniteAir()) executeInfiniteAir();
    if (gzInfo_isInfiniteArrows()) executeInfiniteArrows();
    if (gzInfo_isInfiniteBombs()) executeInfiniteBombs();
    if (gzInfo_isInfiniteHearts()) executeInfiniteHearts();
    if (gzInfo_isInvincibleLink()) executeInvincibleLink();
    if (gzInfo_isMoonJump()) executeMoonJump();
    if (gzInfo_isInfiniteOil()) executeInfiniteOil();
    if (gzInfo_isInfiniteRupees()) executeInfiniteRupees();
    if (gzInfo_isInfiniteSlingshot()) executeInfiniteSlingshot();

    if (gzInfo_isSuperClawshot()) {
        executeSuperClawshot();
    } else if (mSuperClawshot) {
        daAlinkHIO_hookshot_c0::m.mShootSpeed = 100.0f;
        daAlinkHIO_hookshot_c0::m.mMaxLength = 2000.0f;
        daAlinkHIO_hookshot_c0::m.mReturnSpeed = 150.0f;
        daAlinkHIO_hookshot_c0::m.mStickReturnSpeed = 60.0f;
        mSuperClawshot = false;
}
}
