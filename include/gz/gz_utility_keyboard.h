#ifndef GZ_UTILITY_KEYBOARD_H
#define GZ_UTILITY_KEYBOARD_H

#include "gz/gz.h"

class gzKeyboard_c {
public:
    typedef int (*kbCallback)(gzKeyboard_c* i_keyboard, void* i_data);

    gzKeyboard_c() {}
    gzKeyboard_c(kbCallback finishCb, kbCallback returnCb, void* cbData);
    ~gzKeyboard_c();

    void _delete();
    int execute();
    void draw();

    void setCallbacks(kbCallback finishCb, kbCallback returnCb, void* cbData) {
        mFinishCb = finishCb;
        mReturnCb = returnCb;
        mpCbData = cbData;
    }

    static const int MAX_STRING_LEN = 20;
    static const int CHARACTER_COUNT = 91;

    gzTextBox* mpCharacters[CHARACTER_COUNT];
    gzTextBox* mpSpacebarBox;
    gzTextBox* mpStringBox;
    kbCallback mFinishCb;
    kbCallback mReturnCb;
    void* mpCbData;
    int mStringIndex;
    char mString[MAX_STRING_LEN];
};

#endif // GZ_UTILITY_KEYBOARD_H
