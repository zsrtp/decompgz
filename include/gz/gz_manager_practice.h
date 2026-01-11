#ifndef GZ_MANAGER_PRACTICE_H
#define GZ_MANAGER_PRACTICE_H

#include "SSystem/SComponent/c_xyz.h"

class gzSaveLoaderMng_c {
public:
    gzSaveLoaderMng_c() {
        mLoadPhase = PHASE_WAIT_e;
        mTimer = 0;
        mSaveInjectReady = false;
    }

    enum Mode_e {
        MODE_SAVE_e,
        MODE_MEMFILE_e,
    };

    enum LoadPhase_e {
        PHASE_WAIT_e,         // "do nothing" state
        PHASE_INIT_e,         // file load process initialization
        PHASE_STAGE_INIT_e,   // processes to be run during stage initialization
        PHASE_PLAYER_INIT_e,  // processes to be run after player is initialized
    };

    enum SaveCategory_e {
        CATEGORY_ANYP_e,
        CATEGORY_NOSQ_e,
        CATEGORY_HUNDO_e,
        CATEGORY_ALLDUNGEONS_e,
        CATEGORY_GLITCHLESS_e,
    };

    enum SaveSetFlags_e {
        SETFLAG_POS_e = 1,
        SETFLAG_CAM_e = 2,
    };

    // container for any extra data to store in the memfile after the main save data
    struct memfileExData_s {
        char name[20];
        cXyz player_pos;
    };

    struct saveMetadata_s {
        char name[32];
        char desc[64];
        char filename[32];
        u8 flags;
        s16 angle;
        Vec player_pos;
        Vec camera_center;
        Vec camera_eye;
    } ATTRIBUTE_ALIGN(32);  // important that this is aligned to 32

    void execute();

    void loadSave(SaveCategory_e i_category, int i_entryNo);
    void getSaveMetadata(SaveCategory_e i_category, int i_entryNo, saveMetadata_s* o_data);
    int getSaveEntryNum(SaveCategory_e i_category);

    void setMode(Mode_e i_mode) { mMode = i_mode; }

    void start() { mLoadPhase = PHASE_INIT_e; }
    void onStageInit() { mLoadPhase = PHASE_STAGE_INIT_e; }
    void onPlayerInit() { mLoadPhase = PHASE_PLAYER_INIT_e; }
    void wait() { mLoadPhase = PHASE_WAIT_e; }

    bool isSaveInject() { return mSaveInjectReady; }
    
    void end() {
        mLoadPhase = PHASE_WAIT_e;
        mSaveInjectReady = false;
    }

    memfileExData_s mMemfileExData;
    saveMetadata_s mSaveMetadata;
    Mode_e mMode;
    int mLoadPhase;
    bool mSaveInjectReady;
    u8 mTimer;
};

#endif // GZ_MANAGER_PRACTICE_H
