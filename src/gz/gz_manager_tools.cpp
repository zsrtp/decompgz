#include "d/actor/d_a_alink.h"
#include "m_Do/m_Do_controller_pad.h"
#include "gz/gz.h"

void gzToolsMng_c::executeFastBonkRecovery() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link != NULL) {
        daAlinkHIO_frontRoll_c0::m.mCrashAnm.mStartFrame = 50.0f;
        daAlinkHIO_frontRoll_c0::m.mCrashAnm.mSpeed = 0.0f;
        mFastBonkRecovery = true;
    }
}

void gzToolsMng_c::executeFastMovement() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link != NULL) {
        daAlinkHIO_frontRoll_c0::m.mSpeedRate = 3.0f;
        daAlinkHIO_swim_c0::m.mUnderwaterMaxSpeed = 50;
        daAlinkHIO_swim_c0::m.mBackwardMaxSpeed = 50;
        daAlinkHIO_swim_c0::m.mStrafeMaxSpeed = 50;
        daAlinkHIO_swim_c0::m.mDashMaxSpeed = 50;
        daAlinkHIO_swim_c0::m.mForwardMaxSpeed = 50;
        daAlinkHIO_swim_c0::m.mUnderwaterFallMaxSpeed = 50;
        daAlinkHIO_swim_c0::m.mBootsMaxFallSpeed = -50;
        daAlinkHIO_swim_c0::m.mBootsGravity = -50;
        daAlinkHIO_wlMove_c0::m.mADashInitSpeed = 100;
        daAlinkHIO_wlMove_c0::m.mADashMaxSpeed = 100;
        daAlinkHIO_wlMove_c0::m.mADashInitSpeedSlow = 70;
        daAlinkHIO_wlMove_c0::m.mADashMaxSpeedSlow = 70;
        daAlinkHIO_wlMove_c0::m.mMaxSpeed = 50;
        // daAlinkHIO_wlMove_c0::m.mAMaxSpeedWeak = 50;
        mFastMovement = true;
    }
}

void gzToolsMng_c::executeMoveLink() {
    u32 combo = g_gzInfo.mSettings.mCommandCombos.mMoveLink;
    if (combo && ((gzPad::getHold() & combo) == combo)) {
        mMoveLink = !mMoveLink;
    }
    
    if (mMoveLink) {
        // finish
    }
}

void gzToolsMng_c::executeNoSinkSand() {
    daAlink_c* link = daAlink_getAlinkActorClass();
    if (link != NULL) {
        link->mSinkShapeOffset = 0.0f;
    }
}

void gzToolsMng_c::executeTeleport() {
    u32 saveCombo = g_gzInfo.mSettings.mCommandCombos.mTeleportSave;
    u32 loadCombo = g_gzInfo.mSettings.mCommandCombos.mTeleportLoad;
    daAlink_c* link = daAlink_getAlinkActorClass();
    dCamera_c* camera = dCam_getBody();
    
    if (saveCombo && ((gzPad::getHold() & saveCombo) == saveCombo)) {
        if (link != NULL && camera != NULL) {
            mCamera.center = camera->mCenter;
            mCamera.eye = camera->mEye;
            mCamera.fovy = camera->mFovy;
            mCamera.bank = camera->mBank;
            mLinkPos = link->current.pos;
            mLinkAngle = link->shape_angle;
        }

        gzClearButtonInput();
    }

    if (loadCombo && ((gzPad::getHold() & loadCombo) == loadCombo)) {
        if (link != NULL && camera != NULL) {
            camera->Reset(mCamera.center, mCamera.eye, mCamera.fovy, mCamera.bank.Val());
            link->current.pos = mLinkPos;
            link->shape_angle = mLinkAngle;
        }

        gzClearButtonInput();
    }
}

void gzToolsMng_c::execute() {
    if (gzInfo_isFastBonkRecovery()) {
        executeFastBonkRecovery();
    } else if (mFastBonkRecovery) {
        daAlinkHIO_frontRoll_c0::m.mCrashAnm.mStartFrame = 3.0f;
        daAlinkHIO_frontRoll_c0::m.mCrashAnm.mSpeed = 0.8f;
        mFastBonkRecovery = false;
    }

    if (gzInfo_isFastMovement()) {
        executeFastMovement();
    } else if (mFastMovement) {
        daAlinkHIO_frontRoll_c0::m.mSpeedRate = 1.3f;
        daAlinkHIO_swim_c0::m.mUnderwaterMaxSpeed = 12;
        daAlinkHIO_swim_c0::m.mForwardMaxSpeed = 8;
        daAlinkHIO_swim_c0::m.mBackwardMaxSpeed = 6;
        daAlinkHIO_swim_c0::m.mStrafeMaxSpeed = 8;
        daAlinkHIO_swim_c0::m.mDashMaxSpeed = 13;
        daAlinkHIO_swim_c0::m.mUnderwaterFallMaxSpeed = 8;
        daAlinkHIO_swim_c0::m.mBootsMaxFallSpeed = -20;
        daAlinkHIO_swim_c0::m.mBootsGravity = -0.699999988;
        daAlinkHIO_wlMove_c0::m.mADashInitSpeed = 65;
        daAlinkHIO_wlMove_c0::m.mADashMaxSpeed = 45;
        daAlinkHIO_wlMove_c0::m.mADashInitSpeedSlow = 35;
        daAlinkHIO_wlMove_c0::m.mADashMaxSpeedSlow = 33;
        daAlinkHIO_wlMove_c0::m.mMaxSpeed = 20;
        mFastMovement = false;
    }

    if (gzInfo_isMoveLink()) {
        executeMoveLink();
    } else if (mMoveLink) {
        // finish
    }

    if (gzInfo_isNoSinkingInSand()) executeNoSinkSand();
    if (gzInfo_isTeleport()) executeTeleport();
}
