#ifndef GZ_MENU_PRACTICE_H
#define GZ_MENU_PRACTICE_H

#include "gz/gz_utility_keyboard.h"
#include "gz/gz_utility_confirm.h"
#include "gz/gz_menu.h"
#include "gz/gz_textbox.h"

class gzPracticeMenu_c : public gzMenu_c {
public:
    gzPracticeMenu_c();
    ~gzPracticeMenu_c();

    virtual void _delete();
    virtual void execute();
    virtual void draw();

    // static const int VISIBLE_TABS = 3;

    static const int ANY_LINE_NUM = 50;  // TODO: dont hardcode these?
    static const int ALL_DUNGEONS_LINE_NUM = 43;
    static const int HUNDO_LINE_NUM = 86;
    static const int GLITCHLESS_LINE_NUM = 32;
    static const int NOSQ_LINE_NUM = 30;

    static const int MEMFILE_MAX_NUM = 20;  // TODO: should this be variable or capped?

    enum gzPracticeMenu_Tab_e {
        TAB_ANY,
        TAB_NOSQ,
        TAB_HUNDO,
        TAB_ALLDUNGEONS,
        TAB_GLITCHLESS,
        TAB_MEMFILES,

        TAB_MAX
    };

    class gzSavesTab_c {
    public:
        gzSavesTab_c(int i_category, int i_maxLines)
            : mCategory(i_category), mMaxLines(i_maxLines), mLoaded(false) {
            mpLines = NULL;
        }

        void create();
        int execute();

        gzLine** mpLines;
        int mCategory;
        int mMaxLines;
        bool mLoaded;
    };

    class gzMemfileTab_c {
    public:
        void create();
        int execute();
        void draw(f32 xPos);

        int readMemfileNames();
        int loadMemfile(int i_no);
        int deleteMemfile(int i_slotNo);
        static int memfileNameFinishCb(gzKeyboard_c* i_keyboard, void* i_data);
        static int memfileDeleteConfirmCb(gzConfirm_c* i_confirm, void* i_data);
        static int memfileDeleteReturnCb(gzConfirm_c* i_confirm, void* i_data);

        void setMemfileExists(int i_no, bool i_exists) {
            mMemfileStates[i_no] = i_exists;
        }

        bool isMemfileExist(int i_no) { return mMemfileStates[i_no] == true; }

        gzTextBox* mpLines[MEMFILE_MAX_NUM];
        gzKeyboard_c* mpKeyboard;
        gzConfirm_c* mpConfirm;
        int mPendingDeleteSlot;
        bool mMemfileStates[MEMFILE_MAX_NUM];  // TODO: optimize this
        bool mNamesLoaded;  // Defer memcard read until tab is first accessed
    };

    gzTextBox* mpTabHeaders[TAB_MAX];
    gzSavesTab_c mAnypSavesTab;
    gzSavesTab_c mNoSQSavesTab;
    gzSavesTab_c mAllDungeonsSavesTab;
    gzSavesTab_c mHundoSavesTab;
    gzSavesTab_c mGlitchlessSavesTab;
    gzMemfileTab_c mMemfileTab;
    int mCurrentTab;
};

#endif // GZ_MENU_PRACTICE_H
