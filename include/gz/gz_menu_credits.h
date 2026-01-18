#ifndef GZ_MENU_CREDITS_H
#define GZ_MENU_CREDITS_H

#include "gz/gz_menu.h"

class gzCreditsMenu_c : public gzMenu_c {
public:
    gzCreditsMenu_c();
    ~gzCreditsMenu_c();

    virtual void _delete();
    virtual void execute();
    virtual void draw();

    static const int LINE_NUM = 50;

private:
    gzTextBox* mpLines[LINE_NUM];
    dMeterHaihai_c* mpMeterHaihai;
};

#endif // GZ_MENU_CREDITS_H
