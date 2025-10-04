#ifndef GZ_H
#define GZ_H

#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_controller_pad.h"
#include "JSystem/J2DGraph/J2DPicture.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "SSystem/SComponent/c_API_controller_pad.h"

class gzMenu_c;
class gzTextBox;
class gzMainMenu_c;

class gzInfo_c {
public:
    gzInfo_c() { mGZInitialized = false; }

    int _create();
    int _delete();
    int execute();
    int draw();

    int storeSettingsMemcard();
    int loadSettingsMemcard();
    void showHeapUsage();

    bool isDisplay() const { return mDisplay; }
    u32 getCursorColor() const { return mCursorColor; }
    void setCursorColor(u32 i_cursorColor) { mCursorColor = i_cursorColor; }
    bool getDropShadows() const { return mDropShadows; }
    void setDropShadows(bool i_dropShadows) { mDropShadows = i_dropShadows; }
    bool getSwapEquips() const { return mSwapEquips; }
    void setSwapEquips(bool i_swapEquips) { mSwapEquips = i_swapEquips; }
    bool getAreaReload() const { return mAreaReload; }
    void setAreaReload(bool i_areaReload) { mAreaReload = i_areaReload; }
    bool getCursorType() const { return mCursorType; }
    void setCursorType(bool i_type) { mCursorType = i_type; }
    void setFont(JUTFont* i_font) { mpFont = i_font; }
    JUTFont* getFont() { return mpFont; }

    J2DPicture* mpIcon;
    gzTextBox* mpHeader;
    gzMenu_c* mpCurrentMenu;

    u32 mCursorColor;
    JUTFont* mpFont;
    s16 mInputWaitTimer;
    bool mDisplay;
    bool mGZInitialized;
    bool mDropShadows;
    bool mSwapEquips;
    bool mAreaReload;
    bool mCursorType;
};

extern gzInfo_c g_gzInfo;

struct gzSettings_s {
    u32 mCursorColor;  // todo: just make this an index?
    bool mDropShadows;
    bool mSwapEquips;
    bool mAreaReload;
    bool mCursorType;
};

// Static configuration options
extern bool g_progressiveMode;

namespace gzPad {
    inline u32 getTrig() { return mDoCPd_c::m_gzPadInfo.mPressedButtonFlags; }
    inline u32 getTrigLockL() { return mDoCPd_c::m_gzPadInfo.mTrigLockL; }
    inline u32 getTrigLockR() { return mDoCPd_c::m_gzPadInfo.mTrigLockR; }
    inline u32 getTrigUp() { return getTrig() & PAD_BUTTON_UP; }
    inline u32 getTrigDown() { return getTrig() & PAD_BUTTON_DOWN; }
    inline u32 getTrigLeft() { return getTrig() & PAD_BUTTON_LEFT; }
    inline u32 getTrigRight() { return getTrig() & PAD_BUTTON_RIGHT; }
    inline u32 getTrigL() { return getTrig() & PAD_TRIGGER_L; }
    inline u32 getTrigR() { return getTrig() & PAD_TRIGGER_R; }
    inline u32 getTrigA() { return getTrig() & PAD_BUTTON_A; }
    inline u32 getTrigB() { return getTrig() & PAD_BUTTON_B; }
    inline u32 getTrigZ() { return getTrig() & PAD_TRIGGER_Z; }
    inline u32 getTrigY() { return getTrig() & PAD_BUTTON_Y; }
    inline u32 getTrigX() { return getTrig() & PAD_BUTTON_X; }
    inline u32 getTrigStart() { return getTrig() & PAD_BUTTON_START; }
    inline u32 getHold() { return mDoCPd_c::m_gzPadInfo.mButtonFlags; }
    inline u32 getHoldLockL() { return mDoCPd_c::m_gzPadInfo.mHoldLockL; }
    inline u32 getHoldLockR() { return mDoCPd_c::m_gzPadInfo.mHoldLockR; }
    inline u32 getHoldUp() { return getHold() & PAD_BUTTON_UP; }
    inline u32 getHoldDown() { return getHold() & PAD_BUTTON_DOWN; }
    inline u32 getHoldLeft() { return getHold() & PAD_BUTTON_LEFT; }
    inline u32 getHoldRight() { return getHold() & PAD_BUTTON_RIGHT; }
    inline u32 getHoldL() { return getHold() & PAD_TRIGGER_L; }
    inline u32 getHoldR() { return getHold() & PAD_TRIGGER_R; }
    inline u32 getHoldA() { return getHold() & PAD_BUTTON_A; }
    inline u32 getHoldB() { return getHold() & PAD_BUTTON_B; }
    inline u32 getHoldZ() { return getHold() & PAD_TRIGGER_Z; }
    inline u32 getHoldY() { return getHold() & PAD_BUTTON_Y; }
    inline u32 getHoldX() { return getHold() & PAD_BUTTON_X; }
    inline f32 getStickX() { return mDoCPd_c::m_gzPadInfo.mMainStickPosX; }
    inline f32 getStickY() { return mDoCPd_c::m_gzPadInfo.mMainStickPosY; }
    inline f32 getStickX3D() { return mDoCPd_c::m_gzPadInfo.mMainStickPosX; }
    inline f32 getStickValue() { return mDoCPd_c::m_gzPadInfo.mMainStickValue; }
    inline s16 getStickAngle() { return mDoCPd_c::m_gzPadInfo.mMainStickAngle; }
    inline s16 getStickAngle3D() { return mDoCPd_c::m_gzPadInfo.mMainStickAngle; }
    inline f32 getSubStickX3D() { return mDoCPd_c::m_gzPadInfo.mCStickPosX; }
    inline f32 getSubStickX() { return mDoCPd_c::m_gzPadInfo.mCStickPosX; }
    inline f32 getSubStickY() { return mDoCPd_c::m_gzPadInfo.mCStickPosY; }
    inline f32 getSubStickValue() { return mDoCPd_c::m_gzPadInfo.mCStickValue; }
    inline s16 getSubStickAngle() { return mDoCPd_c::m_gzPadInfo.mCStickAngle; }
    inline f32 getAnalogR() { return mDoCPd_c::m_gzPadInfo.mTriggerRight; }
    inline f32 getAnalogL() { return mDoCPd_c::m_gzPadInfo.mTriggerLeft; }
};

int gzPrint(int x, int y, u32 color, char const* string, ...);

class gzTextBox : public J2DTextBox {
public:
    gzTextBox() : J2DTextBox() {
        setFont(g_gzInfo.mpFont);
        setFontSize(18.0f, 18.0f);
    }

    gzTextBox(const char* string, u32 color) : J2DTextBox() {
        setFont(g_gzInfo.mpFont);
        setFontSize(18.0f, 18.0f);
        setString(string);
        setCharColor(color);
        setGradColor(color);
    }

    void setFullColor(u32 color) {
        setCharColor(color);
        setGradColor(color);
    }

    void setStringf(const char* fmt, ...) {
        char buffer[256];

        va_list list;
        va_start(list, string);
        vsnprintf(buffer, sizeof(buffer), fmt, list);
        va_end(list);

        setString(buffer);
    }

    void draw(f32 x, f32 y, u32 color) {
        if (g_gzInfo.getDropShadows()) {
            setCharColor(0x00000080);
            setGradColor(0x00000080);
            J2DTextBox::draw(x + 2, y + 2, 608.0f, HBIND_LEFT);
        }
        
        setCharColor(color);
        setGradColor(color);
        J2DTextBox::draw(x, y, 608.0f, HBIND_LEFT);
    }
};

#endif
