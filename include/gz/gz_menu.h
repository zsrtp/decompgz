#ifndef GZ_MENU_H
#define GZ_MENU_H

#include "d/d_drawlist.h"
#include "d/d_meter_haihai.h"
#include "d/d_select_cursor.h"
#include "gz/gz_textbox.h"

class gzMenu_c : public dDlst_base_c {
public:
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
    virtual void execute() {}
    virtual void draw() {}
    virtual f32 getXPos() { return mXPos; }
    virtual void setXPos(f32 x) { mXPos = x; }

    static gzTextBox* allocateTextBox();
    static void freeTextBox(gzTextBox* box);

protected:
    void handleCursorMovement(s32 maxLines);
    void updateScrolling(s32 maxLines);
    void drawTextBox(gzTextBox* box, f32 x, f32 y, u32 color, J2DTextBoxHBinding binding = HBIND_LEFT);
    void drawHaihaiArrows(u8 flags, f32 x, f32 y, f32 width, f32 height);
    void drawDescription(const char* desc, f32 x, f32 y);

    f32 mXPos; // move to private later?

private:
    // NOTE(Pheenoh): Unused for right now
    // static const u32 TEXTBOX_POOL_SIZE = 578;
    // static gzTextBox* sTextBoxPool;            // Dynamic array head
    // static u8* sTextBoxUsed;                   // Dynamic bitmap (u8 array)
    // static bool sPoolInitialized;
    // static void initPool();
    // static void shutdownPool();                // Shouldn't ever need this
    
    s32 mTopLine; // For scroll offset
    s32 mVisibleLines;          
    dMeterHaihai_c* mpHaihai;
    dSelect_cursor_c* mpCursor;
    gzTextBox* mpDescription;
};

#endif // GZ_MENU_H
