#ifndef GZ_MANAGER_TOOLS_H
#define GZ_MANAGER_TOOLS_H

#include "SSystem/SComponent/c_angle.h"
#include "SSystem/Scomponent/c_sxyz.h"

struct SavedCameraState {
    cXyz center;
    cXyz eye;
    f32 fovy;
    cSAngle bank;
};

class gzToolsMng_c {
public:
    void execute();
    void executeFastBonkRecovery();
    void executeFastMovement();
    void executeMoveLink();
    void executeNoSinkSand();
    void executeTeleport();

private:
    // for "run once" reenable checks
    SavedCameraState mCamera;
    bool mFastBonkRecovery;
    bool mFastMovement;
    cXyz mLinkPos;
    csXyz mLinkAngle;
    bool mMoveLink;
};

#endif // GZ_MANAGER_TOOLS_H
