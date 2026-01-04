#ifndef GZ_TAB_H
#define GZ_TAB_H

#include "gz/gz.h"
#include "gz/gz_menu.h"
    
class gzTab_c {
public:
    void create();
    void _delete();
    int execute();
    void draw();
    void updateDynamicLines();

    static const int MAX_LINES = 15; // for tracking the max # of lines any tab can potentially have

// private:
    gzTextBox* mpLines[MAX_LINES];
    gzTextBox* mpLineOptions[MAX_LINES];
    gzBoolOption_s* mBoolOptions;
    int mLineMax; // for tracking the max # of lines a tab actually has
    bool mOptionToggle;
};

#endif //GZ_TAB_H
