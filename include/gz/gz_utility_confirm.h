#ifndef GZ_UTILITY_CONFIRM_H
#define GZ_UTILITY_CONFIRM_H

#include "gz/gz.h"

class gzConfirm_c {
public:
    typedef int (*confirmCallback)(gzConfirm_c* i_confirm, void* i_data);

    gzConfirm_c() {}
    gzConfirm_c(confirmCallback confirmCb, confirmCallback returnCb, void* cbData);
    gzConfirm_c(confirmCallback confirmCb, confirmCallback returnCb, void* cbData, const char* msg);
    ~gzConfirm_c();

    void _delete();
    int execute();
    void draw();

    void setCallbacks(confirmCallback confirmCb, confirmCallback returnCb, void* cbData) {
        mConfirmCb = confirmCb;
        mReturnCb = returnCb;
        mpCbData = cbData;
    }

    void setPrompt(const char* msg);

    gzTextBox* mpPromptBox;
    gzTextBox* mpNoBox;
    gzTextBox* mpYesBox;
    confirmCallback mConfirmCb;
    confirmCallback mReturnCb;
    void* mpCbData;
};

#endif // GZ_UTILITY_CONFIRM_H
