#ifndef GZ_MENU_MEMORY_H
#define GZ_MENU_MEMORY_H

#include "gz/gz_menu.h"

class gzMemoryMenu_c : public gzMenu_c {
public:
    gzMemoryMenu_c();
    ~gzMemoryMenu_c();

    virtual void _delete();
    virtual void execute();
    virtual void draw();

    static const int LINE_NUM = 20;

private:
    gzTextBox* mpLines[LINE_NUM];
};

#endif // GZ_MENU_MEMORY_H
