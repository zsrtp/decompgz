#ifndef DRAW_H
#define DRAW_H

#include "dolphin/gx/GXVert.h"
#include "dolphin/mtx.h"

void begin(u16, u8, GXTexObj*);
void begin(u16, u8);
void begin(u16);
void begin(u16, GXTexObj*);
void end();
void add_vertex(u32, Vec2, Vec2);
void drawQuad(u32, Vec2*, Vec2*, GXTexObj*);

#endif // DRAW_H
