#ifndef GZ_TEXTBOX_H
#define GZ_TEXTBOX_H

#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JUtility/JUTFont.h"
#include "gz/gz.h"

// TextBox pool functions (implemented in gz_textbox.cpp)
class gzTextBox;
gzTextBox* gzTextBox_allocate();
void gzTextBox_free(gzTextBox* box);
void gzTextBox_shutdownPool();

class gzTextBox : public J2DTextBox {
public:
    gzTextBox() : J2DTextBox() {
        setFont(g_gzInfo.mpFont);
        setFontSize(18.0f, 18.0f);
    }

    gzTextBox(f32 sizeX, f32 sizeY) : J2DTextBox() {
        setFont(g_gzInfo.mpFont);
        setFontSize(sizeX, sizeY);
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
        updateBounds();
    }

    // Update bounds based on actual rendered text width using font metrics
    // This is needed for TP cursor to properly size itself around the text
    void updateBounds() {
        if (mFont == NULL || mStringPtr == NULL) {
            mBounds.i.x = 0.0f;
            mBounds.i.y = 0.0f;
            mBounds.f.x = 0.0f;
            mBounds.f.y = mFontSizeY;
            return;
        }

        f32 totalWidth = 0.0f;
        f32 scale = mFontSizeX / mFont->getCellWidth();
        const char* str = mStringPtr;

        while (*str != '\0') {
            JUTFont::TWidth charWidth;
            mFont->getWidthEntry((u8)*str, &charWidth);
            totalWidth += charWidth.field_0x1 * scale;

            // Add character spacing for all but the last character
            if (*(str + 1) != '\0') {
                totalWidth += mCharSpacing;
            }
            str++;
        }

        mBounds.i.x = 0.0f;
        mBounds.i.y = 0.0f;
        mBounds.f.x = totalWidth + 6.0f;  // Add padding for TP cursor
        mBounds.f.y = mFontSizeY;
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

};

class gzLine {
public:
    gzLine(const char* i_text, const char* i_description) {
        mText = gzTextBox_allocate();
        mText->setStringf(i_text);
        strcpy(m_description, i_description);
    }

    ~gzLine() {
        gzTextBox_free(mText);
        mText = NULL;
    }

    virtual gzTextBox* getOptionBox() const { return NULL; }
    virtual void updateOptionText() {}

    void setDescription(const char* desc) { strcpy(m_description, desc); }
    void draw(f32 x, f32 y, u32 color) { mText->draw(x, y, color); }

public:
    gzTextBox* mText;
    char m_description[80];
};

class gzBoolOptionLine : public gzLine {
public:
    typedef bool (*IsFunc)();
    typedef void (*OnFunc)();
    typedef void (*OffFunc)();

    gzBoolOptionLine(const char* i_text, const char* i_description,
                    IsFunc i_isFunc, OnFunc i_onFunc,
                    OffFunc i_offFunc) : gzLine(i_text, i_description) {
        mOption = gzTextBox_allocate();
        mIs = i_isFunc;
        mOn = i_onFunc;
        mOff = i_offFunc;
    }

    // Simple constructor for when logic is handled externally
    gzBoolOptionLine(const char* i_text, const char* i_description) : gzLine(i_text, i_description) {
        mOption = gzTextBox_allocate();
        mIs = NULL;
        mOn = NULL;
        mOff = NULL;
    }

    ~gzBoolOptionLine() {
        gzTextBox_free(mOption);
        mOption = NULL;
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

    gzListOptionLine(const char* i_text, const char* i_description, NextFunc i_nextFunc, PrevFunc i_prevFunc) : gzLine(i_text, i_description) {
        mpOption = gzTextBox_allocate();
        mpNext = i_nextFunc;
        mpPrev = i_prevFunc;
    }

    // Simple constructor for when logic is handled externally
    gzListOptionLine(const char* i_text, const char* i_description) : gzLine(i_text, i_description) {
        mpOption = gzTextBox_allocate();
        mpNext = NULL;
        mpPrev = NULL;
    }

    ~gzListOptionLine() {
        gzTextBox_free(mpOption);
        mpOption = NULL;
    }

    virtual gzTextBox* getOptionBox() const { return mpOption; }
    virtual void updateOptionText() {}

public:
    gzTextBox* mpOption;
    NextFunc mpNext;
    PrevFunc mpPrev;
};

#endif // GZ_TEXTBOX_H
