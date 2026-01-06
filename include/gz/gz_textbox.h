#ifndef GZ_TEXTBOX_H
#define GZ_TEXTBOX_H

#include "JSystem/J2DGraph/J2DTextBox.h"
#include "gz/gz.h"

class gzTextBox : public J2DTextBox {
public:
    gzTextBox() : J2DTextBox() {
        setFont(g_gzInfo.mpFont);
        setFontSize(18.0f, 18.0f);
        m_description[0] = 0;
    }

    gzTextBox(f32 sizeX, f32 sizeY) : J2DTextBox() {
        setFont(g_gzInfo.mpFont);
        setFontSize(sizeX, sizeY);
        m_description[0] = 0;
    }

    gzTextBox(const char* string, u32 color) : J2DTextBox() {
        setFont(g_gzInfo.mpFont);
        setFontSize(18.0f, 18.0f);
        setString(string);
        setCharColor(color);
        setGradColor(color);
        m_description[0] = 0;
    }

    void setStringDesc(const char* string, const char* description) {
        setString(string);
        strcpy(m_description, description);
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
        if (gzInfo_isDropShadows()) {
            setCharColor(0x00000080);
            setGradColor(0x00000080);
            J2DTextBox::draw(x + 2, y + 2, 608.0f, HBIND_LEFT);
        }
        
        setCharColor(color);
        setGradColor(color);
        J2DTextBox::draw(x, y, 608.0f, HBIND_LEFT);
    }

    void draw(f32 x, f32 y, u32 color, J2DTextBoxHBinding binding) {
        if (gzInfo_isDropShadows()) {
            setCharColor(0x00000080);
            setGradColor(0x00000080);
            J2DTextBox::draw(x + 2, y + 2, 608.0f, binding);
        }
        
        setCharColor(color);
        setGradColor(color);
        J2DTextBox::draw(x, y, 608.0f, binding);
    }

    char m_description[80]; // todo: is this the best way to handle this?
};

class gzLine {
public:
    gzLine(char* i_text, char* i_description) {
        mText = new gzTextBox();
        mText->setStringDesc(i_text, i_description);
        mText->mBounds.f.x = 430.0f;
        mText->mBounds.f.y = 10.0f;
    }

    virtual gzTextBox* getOptionBox() const { return NULL; }
    virtual void updateOptionText() {}

public:
    gzTextBox* mText;
    char m_description[80];
};

class gzBoolOptionLine : public gzLine {
public:
    typedef bool (*IsFunc)();
    typedef void (*OnFunc)();
    typedef void (*OffFunc)();

    gzBoolOptionLine(char* i_text, char* i_description, 
                    IsFunc i_isFunc, OnFunc i_onFunc, 
                    OffFunc i_offFunc) : gzLine(i_text, i_description) {
        mOption = new gzTextBox();
        mIs = i_isFunc;
        mOn = i_onFunc;
        mOff = i_offFunc;
    }

    virtual gzTextBox* getOptionBox() const { return mOption; }
    virtual void updateOptionText() {}

public:
    gzTextBox* mOption;
    IsFunc mIs;
    OnFunc mOn;
    OffFunc mOff;
};

class gzListOptionLine : public gzLine {
public:
    typedef u32 (*NextFunc)();
    typedef u32 (*PrevFunc)();

    gzListOptionLine(char* i_text, char* i_description, NextFunc i_nextFunc, PrevFunc i_prevFunc) : gzLine(i_text, i_description) {
        mpOption = new gzTextBox();
        mpNext = i_nextFunc;
        mpPrev = i_prevFunc;
    }

    virtual gzTextBox* getOptionBox() const { return mpOption; }
    virtual void updateOptionText() {}

public:
    gzTextBox* mpOption;
    NextFunc mpNext;
    PrevFunc mpPrev;
};

#endif // GZ_TEXTBOX_H
