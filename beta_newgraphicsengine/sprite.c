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

#include <psptypes.h>
#include <stdio.h>
#include <malloc.h>
#include "graphics.h"
#include "error.h"
#include "sprite.h"


Sprite* spriteLoad(char* inPath, int inWidth, int inHeight, int inFrames) {
      Sprite* tempSprite = (Sprite*) malloc(sizeof(Sprite));
      if (!tempSprite) {
           char tempError[256];
           sprintf(tempError, "Couldn't load sprite (%s).\nOut of memory.", inPath);
           errorThrow(tempError);
      }
      
      tempSprite->spriteData = imageLoad(inPath);
      tempSprite->spriteWidth = inWidth;
      tempSprite->spriteHeight = inHeight;
      tempSprite->spriteFrames = inFrames;
      tempSprite->spriteCurFrame = 0;
      imageReplaceColor(SPRITE_ALPHA_COLOR, 0x00000000, tempSprite->spriteData);
      
      return(tempSprite);
}

Sprite* spriteLoadAuto(char* inPath, int inWidth, int inHeight) {
      Sprite* tempSprite = (Sprite*) malloc(sizeof(Sprite));
      if (!tempSprite) {
           char tempError[256];
           sprintf(tempError, "Couldn't load sprite (%s).\nOut of memory.", inPath);
           errorThrow(tempError);
      }
      
      tempSprite->spriteData = imageLoad(inPath);
      tempSprite->spriteWidth = inWidth;
      tempSprite->spriteHeight = inHeight;
      tempSprite->spriteFrames = (tempSprite->spriteData->imageWidth / inWidth);
      tempSprite->spriteFrames *= (tempSprite->spriteData->imageHeight / inHeight);
      tempSprite->spriteCurFrame = 0;
      imageReplaceColor(SPRITE_ALPHA_COLOR, 0x00000000, tempSprite->spriteData);
      
      return(tempSprite);
}


void spriteDraw(Sprite* inSprite, int inX, int inY) {
     int sy = 0;
     int sx = inSprite->spriteCurFrame * inSprite->spriteWidth;
     while(sx >= inSprite->spriteData->imageWidth) {
              sx -= inSprite->spriteData->imageWidth;
              sy += inSprite->spriteHeight;
     }
     imageDrawSectionAlpha(sx, sy, inSprite->spriteWidth, inSprite->spriteHeight, inX, inY, inSprite->spriteData);
}

void spriteDrawFrame(Sprite* inSprite, int inX, int inY, int inFrame) {
     int sy = 0;
     int sx = inFrame * inSprite->spriteWidth;
     while(sx >= inSprite->spriteData->imageWidth) {
              sx -= inSprite->spriteData->imageWidth;
              sy += inSprite->spriteHeight;
     }
     imageDrawSectionAlpha(sx, sy, inSprite->spriteWidth, inSprite->spriteHeight, inX, inY, inSprite->spriteData);
}

void spriteDrawAnim(Sprite* inSprite, int inX, int inY) {
     int sy = 0;
     int sx = inSprite->spriteCurFrame * inSprite->spriteWidth;
     while(sx >= inSprite->spriteData->imageWidth) {
              sx -= inSprite->spriteData->imageWidth;
              sy += inSprite->spriteHeight;
     }
     imageDrawSectionAlpha(sx, sy, inSprite->spriteWidth, inSprite->spriteHeight, inX, inY, inSprite->spriteData);
     inSprite->spriteCurFrame++;
     if(inSprite->spriteCurFrame >= inSprite->spriteFrames)
          inSprite->spriteCurFrame = 0;
}

void spriteFree(Sprite* inSprite) {
     //imageFree(inSprite->spriteData);
     free(inSprite);
}
