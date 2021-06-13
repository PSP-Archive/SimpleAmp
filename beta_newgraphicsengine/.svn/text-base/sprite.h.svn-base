// FlatEditPSP - FlatEditPSP is a simple text editor much like notepad, but with a cooler keyboard.
// Copyright (C) 2007  Ben Brewer
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

#ifndef SPRITE_H
#define SPRITE_H

#define SPRITE_ALPHA_COLOR 0xFFFF00FF

#include <psptypes.h>
#include "graphics.h"

typedef struct {
        Image* spriteData;
        int spriteWidth, spriteHeight;
        int spriteFrames, spriteCurFrame;
} Sprite;

extern Sprite* spriteLoad(char* inPath, int inWidth, int inHeight, int inFrames);
extern Sprite* spriteLoadAuto(char* inPath, int inWidth, int inHeight);
extern void spriteDraw(Sprite* inSprite, int inX, int inY);
extern void spriteDrawFrame(Sprite* inSprite, int inX, int inY, int inFrame);
extern void spriteDrawAnim(Sprite* inSprite, int inX, int inY);
extern void spriteFree(Sprite* inSprite);

#endif
