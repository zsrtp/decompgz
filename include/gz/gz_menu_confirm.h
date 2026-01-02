#ifndef GZ_MENU_CONFIRM_H
#define GZ_MENU_CONFIRM_H

#include "gz/gz_menu.h"

typedef void (*confirmCallback)(void*);
typedef void (*returnCallback)();
class gzConfirmMenu_c : public gzMenu_c {
public:
    enum gzConfirmMenu_Confirm_e {
        CONFIRM_NO,
        CONFIRM_YES,

        CONFIRM_MAX
    };

    gzConfirmMenu_c();
    gzConfirmMenu_c(confirmCallback, void*, returnCallback);
    gzConfirmMenu_c(confirmCallback, void*, returnCallback, const char* msg);
    ~gzConfirmMenu_c();

    virtual void _delete();
    virtual void execute();
    virtual void draw();

    static const int LINE_NUM = CONFIRM_MAX;

private:
    gzTextBox* mpLineConfirmPrompt;
    gzTextBox* mpLines[LINE_NUM];
    confirmCallback mpConfirmCallback;
    void* mCallbackData;
    returnCallback mpReturnCallback;
};

#endif // GZ_MENU_CONFIRM_H
