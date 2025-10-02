#ifndef GZ_MENU_H
#define GZ_MENU_H

#include "d/d_drawlist.h"
#include "JSystem/J2DGraph/J2DTextBox.h"

class gzMenu_c : public dDlst_base_c {
public:
    struct gzCursor {
        int x;
        int y;
    };

    virtual void _delete() {}
    virtual void execute() {}
};

class gzMainMenu_c : public gzMenu_c {
public:
    gzMainMenu_c();
    ~gzMainMenu_c();

    virtual void _delete();
    virtual void execute();
    virtual void draw();

    static const int LINE_NUM = 9;
    static gzCursor mCursor;

    J2DTextBox* mpLines[LINE_NUM];
};

#endif
