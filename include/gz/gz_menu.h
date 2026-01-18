#ifndef GZ_MENU_H
#define GZ_MENU_H

#include "d/d_drawlist.h"
#include "d/d_meter_haihai.h"
#include "gz/gz_textbox.h"

struct gzBoolOption_s {
    const char* name;
    const char* desc;
    bool (*is)();
    void (*on)();
    void (*off)();
};

struct gzIntOption_s {
    const char* name;
    const char* desc;
    bool (*is)(int);
    void (*on)(int);
    void (*off)(int);
};

class gzMenu_c : public dDlst_base_c {
public:
    typedef u8 (*haihaiCallback)(int idx);

    enum gzMenu_Haihai_e {
        ARROW_LEFT = 1,
        ARROW_DOWN = 2,
        ARROW_RIGHT = 4,
        ARROW_UP = 8
    };

    gzMenu_c();
    virtual ~gzMenu_c();

    virtual void create() {}
    virtual void _delete() {}
    virtual void execute();
    virtual void draw() {}
    virtual f32 getXPos() { return mXPos; }
    virtual void setXPos(f32 x) { mXPos = x; }

protected:
    void updateScrolling(s32 maxLines);
    void drawHaihaiArrows(u8 flags, f32 x, f32 y, f32 width, f32 height);
    void drawDescription(const char* desc);
    void drawLineWithOption(gzTextBox* line, gzTextBox* option, f32 lineX, f32 optionX, f32 lineY, bool isSelected, u32 selectedColor, bool showHaihai, u8 haihaiFlags, f32 haihaiX, f32 haihaiY, f32 haihaiWidth);
    void drawLines(gzTextBox** lines, gzTextBox** lineOptions, u8 haihaiFlags, s32 numLines);
    void drawLines(gzLine** lines, s32 numLines, u8 haihai_flags, s32 topLine, s32 visibleLines);

    f32 mXPos; // move to private later?
    dMeterHaihai_c* mpHaihai;
};

#endif // GZ_MENU_H
