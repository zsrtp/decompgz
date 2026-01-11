#ifndef GZ_MENU_PRACTICE_H
#define GZ_MENU_PRACTICE_H

#include "gz/gz_utility_keyboard.h"
#include "gz/gz_menu.h"

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

    class gzAnypSavesTab_c {
    public:
        void create();
        int execute();
        
        gzTextBox* mpLines[ANY_LINE_NUM];
    };

    class gzHundoSavesTab_c {
    public:
        void create();
        int execute();
        
        gzTextBox* mpLines[HUNDO_LINE_NUM];
    };

    class gzADSavesTab_c {
    public:
        void create();
        int execute();
        
        gzTextBox* mpLines[ALL_DUNGEONS_LINE_NUM];
    };

    class gzGlitchlessSavesTab_c {
    public:
        void create();
        int execute();
        
        gzTextBox* mpLines[GLITCHLESS_LINE_NUM];
    };

    class gzNoSQSavesTab_c {
    public:
        void create();
        int execute();
        
        gzTextBox* mpLines[NOSQ_LINE_NUM];
    };

    class gzMemfileTab_c {
    public:
        void create();
        int execute();
        void draw();

        int readMemfileNames();
        int loadMemfile(int i_no);
        int deleteMemfile(int i_slotNo);
        static int memfileNameFinishCb(gzKeyboard_c* i_keyboard, void* i_data);

        void setMemfileExists(int i_no, bool i_exists) {
            mMemfileStates[i_no] = i_exists;
        }

        bool isMemfileExist(int i_no) { return mMemfileStates[i_no] == true; }

        gzTextBox* mpLines[MEMFILE_MAX_NUM];
        gzKeyboard_c* mpKeyboard;
        bool mMemfileStates[MEMFILE_MAX_NUM];  // TODO: optimize this
    };

    gzTextBox* mpTabHeaders[TAB_MAX];
    gzAnypSavesTab_c mAnypSavesTab;
    gzNoSQSavesTab_c mNoSQSavesTab;
    gzADSavesTab_c mAllDungeonsSavesTab;
    gzHundoSavesTab_c mHundoSavesTab;
    gzGlitchlessSavesTab_c mGlitchlessSavesTab;
    gzMemfileTab_c mMemfileTab;
    gzTextBox* mpDescription;
    dMeterHaihai_c* mpMeterHaihai;
    int mTopLine;
    int mCurrentTab;
};

#endif // GZ_MENU_PRACTICE_H
