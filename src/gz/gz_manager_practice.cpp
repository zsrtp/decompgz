#include "d/dolzel.h" // IWYU pragma: keep

#include "gz/gz.h"
#include "m_Do/m_Do_MemCard.h"
#include "SSystem/SComponent/c_lib.h"

void gzSaveLoaderMng_c::execute() {
    switch (mLoadPhase) {
    case PHASE_WAIT_e:
        return;
    case PHASE_INIT_e:
        mTimer = 10;
        mSaveInjectReady = true;
        wait();
        break;
    case PHASE_STAGE_INIT_e:
        memcpy(&g_dComIfG_gameInfo.info.mSavedata, mDoMemCd_Ctrl_c::sTmpBuf, sizeof(dSv_save_c));
        dComIfGs_getSave(dComIfGs_getSaveInfo()->getDan().mStageNo);
        mLoadPhase = PHASE_PLAYER_INIT_e;
        break;
    case PHASE_PLAYER_INIT_e:
        // waits for player to not be null before running any processes here
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        if (player != NULL) {
            if (mMode == MODE_MEMFILE_e) {
                player->current.pos = mMemfileExData.player_pos;
            } else {
                if (mSaveMetadata.flags & SETFLAG_POS_e) {
                    OS_REPORT("attempting to set position (%f, %f, %f) %d\n", mSaveMetadata.player_pos.x, mSaveMetadata.player_pos.y, mSaveMetadata.player_pos.z, mSaveMetadata.angle);
                    player->current.pos = mSaveMetadata.player_pos;
                    player->current.angle.y = mSaveMetadata.angle;
                }

                if (mSaveMetadata.flags & SETFLAG_CAM_e) {
                    // TODO
                }
            }

            // TODO: would prefer not to do this, but until we can safely modify d_a_alink processes this is the simplest solution
            if (cLib_calcTimer(&mTimer) == 0) {
                end();
            }
        }
        break;
    }
}

void gzSaveLoaderMng_c::loadSave(SaveCategory_e i_category, int i_entryNo) {
    const int SAVE_READ_SIZE = OSRoundUp32B(sizeof(dSv_save_c));
    mMode = MODE_SAVE_e;

    char* category_str = NULL;
    switch (i_category) {
    case CATEGORY_ANYP_e:
    default:
        category_str = "any_saves";
        break;
    case CATEGORY_NOSQ_e:
        category_str = "nosq_saves";
        break;
    case CATEGORY_HUNDO_e:
        category_str = "hundo_saves";
        break;
    case CATEGORY_ALLDUNGEONS_e:
        category_str = "ad_saves";
        break;
    case CATEGORY_GLITCHLESS_e:
        category_str = "glitchless_saves";
        break;
    }

    getSaveMetadata(i_category, i_entryNo, &mSaveMetadata);

    char pathbuf[64];
    sprintf(pathbuf, "/gz/%s/%s.bin", category_str, mSaveMetadata.filename);
    gzDVDLoadFile(pathbuf, mDoMemCd_Ctrl_c::sTmpBuf, SAVE_READ_SIZE, 0);

    dSv_save_c* savep = (dSv_save_c*)mDoMemCd_Ctrl_c::sTmpBuf;
    dComIfGp_setNextStage(savep->getPlayer().getPlayerReturnPlace().getName(),
                          savep->getPlayer().getPlayerReturnPlace().getPlayerStatus(),
                          savep->getPlayer().getPlayerReturnPlace().getRoomNo(),
                          -1);

    start();
}

void gzSaveLoaderMng_c::getSaveMetadata(SaveCategory_e i_category, int i_entryNo, saveMetadata_s* o_data) {
    const int METADATA_OFFSET = 32;

    char* category_str = NULL;
    switch (i_category) {
    case CATEGORY_ANYP_e:
    default:
        category_str = "any_saves/any";
        break;
    case CATEGORY_NOSQ_e:
        category_str = "nosq_saves/nosq";
        break;
    case CATEGORY_HUNDO_e:
        category_str = "hundo_saves/hundo";
        break;
    case CATEGORY_ALLDUNGEONS_e:
        category_str = "ad_saves/ad";
        break;
    case CATEGORY_GLITCHLESS_e:
        category_str = "glitchless_saves/glitchless";
        break;
    }

    char pathbuf[64];
    sprintf(pathbuf, "/gz/%s.bin", category_str);
    gzDVDLoadFile(pathbuf, o_data, sizeof(saveMetadata_s), METADATA_OFFSET + (i_entryNo * sizeof(saveMetadata_s)));
}

int gzSaveLoaderMng_c::getSaveEntryNum(SaveCategory_e i_category) {
    const int MIN_READ_SIZE = 32;

    char* category_str = NULL;
    switch (i_category) {
    case CATEGORY_ANYP_e:
    default:
        category_str = "any_saves/any";
        break;
    case CATEGORY_NOSQ_e:
        category_str = "nosq_saves/nosq";
        break;
    case CATEGORY_HUNDO_e:
        category_str = "hundo_saves/hundo";
        break;
    case CATEGORY_ALLDUNGEONS_e:
        category_str = "ad_saves/ad";
        break;
    case CATEGORY_GLITCHLESS_e:
        category_str = "glitchless_saves/glitchless";
        break;
    }

    u8 ATTRIBUTE_ALIGN(32) read_buffer[MIN_READ_SIZE];

    char pathbuf[64];
    sprintf(pathbuf, "/gz/%s.bin", category_str);
    gzDVDLoadFile(pathbuf, read_buffer, MIN_READ_SIZE, 0);

    int entry_num = *(int*)read_buffer;
    return entry_num;
}
