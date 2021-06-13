/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * celshading.c Simple cel shading example
 * written as xmas present for psp community
 *
 * Copyright (c) 2005 McZonk <www.teamemergencyexit.com>
 *
 */


/*#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pspctrl.h>*/

//#include <pspgu.h>
#include <pspgum.h>
#include "font.c"

//#include "../common/callbacks.h"

//PSP_MODULE_INFO("Celshading", 0, 1, 1);
//PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

static unsigned int __attribute__((aligned(16))) list[262144];
int to_quit = 0;

// Font
static int fontwidthtab[128] = {
	10, 10, 10, 10, 
	10, 10, 10, 10,
	10, 10, 10, 10, 
	10, 10, 10, 10,

	10, 10, 10, 10, 
	10, 10, 10, 10,
	10, 10, 10, 10,
	10, 10, 10, 10,

	10,  6,  8, 10, //   ! " #
	10, 10, 10,  6, // $ % & '
	10, 10, 10, 10, // ( ) * +
	 6, 10,  6, 10, // , - . /

	10, 10, 10, 10, // 0 1 2 3
	10, 10, 10, 10, // 6 5 8 7
	10, 10,  6,  6, // 10 9 : ;
	10, 10, 10, 10, // < = > ?

	16, 10, 10, 10, // @ A B C
	10, 10, 10, 10, // D E F G
	10,  6,  8, 10, // H I J K
	 8, 10, 10, 10, // L M N O

	10, 10, 10, 10, // P Q R S
	10, 10, 10, 12, // T U V W
	10, 10, 10, 10, // X Y Z [
	10, 10,  8, 10, // \ ] ^ _

	 6,  8,  8,  8, // ` a b c
	 8,  8,  6,  8, // d e f g
	 8,  6,  6,  8, // h i j k
	 6, 10,  8,  8, // l m n o

	 8,  8,  8,  8, // p q r s
	 8,  8,  8, 12, // t u v w
	 8,  8,  8, 10, // x y z {
	 8, 10,  8, 12  // | } ~  
};

extern unsigned char lightmap_start[];

typedef struct Vx_v32f_n32f {
	float normal[3];
 	float vertex[3];
} Vx_v32f_n32f;

typedef struct Vx_v32f {
	float vertex[3];
} Vx_v32f;

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)
#define PIXEL_SIZE (4) /* change this if you change to another screenmode */
#define FRAME_SIZE (BUF_WIDTH * SCR_HEIGHT * PIXEL_SIZE)
#define ZBUF_SIZE (BUF_WIDTH SCR_HEIGHT * 2) /* zbuffer seems to be 16-bit? */

#define TORUS_SLICES 48 // numc
#define TORUS_ROWS 48 // numt
#define TORUS_RADIUS 1.0f
#define TORUS_THICKNESS 0.5f

#define LIGHT_DISTANCE 3.0f

Vx_v32f_n32f   __attribute__((aligned(16))) torus_v0[TORUS_SLICES * TORUS_ROWS];
Vx_v32f        __attribute__((aligned(16))) torus_v1[TORUS_SLICES * TORUS_ROWS];
unsigned short __attribute__((aligned(16))) torus_in[TORUS_SLICES * TORUS_ROWS * 6];
	//unsigned int old = 0;
	//unsigned int flags = PSP_CTRL_CIRCLE | PSP_CTRL_CROSS | PSP_CTRL_SQUARE | PSP_CTRL_TRIANGLE | PSP_CTRL_LTRIGGER | PSP_CTRL_DOWN;
	
	float rx = 0.0f, ry = 0.0f;
	float transx = 0.0f, transy = 0.0f;
	//SceCtrlData padDonut;

void donutSimpleSpectrum()
{
	
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(0) - 1],90,47,screenbuffer);		   
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(1) - 1],140,47,screenbuffer);		   
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(2) - 1],190,47,screenbuffer);		   
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(3) - 1],240,47,screenbuffer);		   
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(4) - 1],290,47,screenbuffer);		   
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(5) - 1],340,47,screenbuffer);		   	

  int x,x1,x2;
  
    eq_delay++;

    if(eq_delay == 2)
    {
      for( x = 0; x < 32 ; x++)
      {
        if(simple_spectrum_full[258 + x] == 0)
          fillScreenRect(0xFFFFFFFF, 99 + (x * 9), 247 - 1  , 5 , 1);
        else      
          fillScreenRect(0xFFFFFFFF, 99 + (x * 9), 247 - (simple_spectrum_full[258 + x] * 2)  , 5 , (simple_spectrum_full[258 + x] * 2));
      
        simple_spectrum_bands[x] = simple_spectrum_full[258 + x];
      }
        
      eq_delay = 0;
    }
    else
    {
      for( x = 0; x < 32 ; x++)
      {
        if(simple_spectrum_bands[x] == 0)
          fillScreenRect(0xFFFFFFFF, 99 + (x * 9), 247 - 1  , 5 , 1);
        else      
          fillScreenRect(0xFFFFFFFF, 99 + (x * 9), 247 - (simple_spectrum_bands[x] * 2)  , 5 , (simple_spectrum_bands[x] * 2));
      
      }    
    }
  
/*
    for( x = 0; x < 32 ; x++)
    {
      if(simple_spectrum_bands[x] == 0)
        fillImageRect(0xFFFFFFFF, 99 + (x * 9), 247 - 1  , 5 , 1, screenbuffer);
      else      
        fillImageRect(0xFFFFFFFF, 99 + (x * 9), 247 - (simple_spectrum_bands[x])  , 5 , (simple_spectrum_bands[x]), screenbuffer);
    }
*/

}



void drawString(const char* text, int x, int y, unsigned int color, int fw) {
	int len = (int)strlen(text);
	if(!len) {
		return;
	}

	typedef struct {
		float s, t;
		unsigned int c;
		float x, y, z;
	} VERT;

	VERT* v = sceGuGetMemory(sizeof(VERT) * 2 * len);

	int i;
	for(i = 0; i < len; i++) {
		unsigned char c = (unsigned char)text[i];
		if(c < 32) {
			c = 0;
		} else if(c >= 128) {
			c = 0;
		}

		int tx = (c & 0x0F) << 4;
		int ty = (c & 0xF0);

		VERT* v0 = &v[i*2+0];
		VERT* v1 = &v[i*2+1];
		
		v0->s = (float)(tx + (fw ? ((16 - fw) >> 1) : ((16 - fontwidthtab[c]) >> 1)));
		v0->t = (float)(ty);
		v0->c = color;
		v0->x = (float)(x);
		v0->y = (float)(y);
		v0->z = 0.0f;

		v1->s = (float)(tx + 16 - (fw ? ((16 - fw) >> 1) : ((16 - fontwidthtab[c]) >> 1)));
		v1->t = (float)(ty + 16);
		v1->c = color;
		v1->x = (float)(x + (fw ? fw : fontwidthtab[c]));
		v1->y = (float)(y + 16);
		v1->z = 0.0f;

		x += (fw ? fw : fontwidthtab[c]);
	}

	sceGumDrawArray(GU_SPRITES, 
		GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_2D,
		len * 2, 0, v
	);
}



//Yep this is a torus but it looks like a donut so I call it donut :p
int SetupDonut()
{
	/* Setup Homebutton Callbacks */
	//setupCallbacks();

	/* Generate Torus */
	unsigned int i,j;
	for (j = 0; j < TORUS_SLICES; ++j) {
		for (i = 0; i < TORUS_ROWS; ++i) {
			struct Vx_v32f_n32f* v0 = &torus_v0[i + j*TORUS_ROWS];
			struct Vx_v32f*      v1 = &torus_v1[i + j*TORUS_ROWS];
			
			float s = i + 0.5f;
			float t = j;
			float cs,ct,ss,st;

			cs = cosf(s * (2*GU_PI)/TORUS_SLICES);
			ct = cosf(t * (2*GU_PI)/TORUS_ROWS);
			ss = sinf(s * (2*GU_PI)/TORUS_SLICES);
			st = sinf(t * (2*GU_PI)/TORUS_ROWS);

			v0->normal[0] = cs * ct;
			v0->normal[1] = cs * st;
			v0->normal[2] = ss;

			v0->vertex[0] = (TORUS_RADIUS + TORUS_THICKNESS * cs) * ct;
			v0->vertex[1] = (TORUS_RADIUS + TORUS_THICKNESS * cs) * st;
			v0->vertex[2] = TORUS_THICKNESS * ss;
			
			v1->vertex[0] = v0->vertex[0] + v0->normal[0] * 0.1f;
			v1->vertex[1] = v0->vertex[1] + v0->normal[1] * 0.1f;
			v1->vertex[2] = v0->vertex[2] + v0->normal[2] * 0.1f;
		}
	}
	for (j = 0; j < TORUS_SLICES; ++j) {
		for (i = 0; i < TORUS_ROWS; ++i) {
			unsigned short* in = &torus_in[(i+(j*TORUS_ROWS))*6];
			unsigned int i1 = (i+1)%TORUS_ROWS, j1 = (j+1)%TORUS_SLICES;

			*in++ = i + j  * TORUS_ROWS;
			*in++ = i1 + j * TORUS_ROWS;
			*in++ = i + j1 * TORUS_ROWS;

			*in++ = i1 + j  * TORUS_ROWS;
			*in++ = i1 + j1 * TORUS_ROWS;
			*in++ = i  + j1 * TORUS_ROWS;
		}
	}

	sceKernelDcacheWritebackAll();

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	// setup GU

	sceGuInit();

	sceGuStart(GU_DIRECT,list);
	sceGuDrawBuffer(GU_PSM_8888,(void*)0,BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,(void*)0x88000,BUF_WIDTH);
	sceGuDepthBuffer((void*)0x110000,BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
	sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
	sceGuDepthRange(0xc350,0x2710);
	sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_CLIP_PLANES);
	//sceGuEnable(GU_TEXTURE_2D);
	sceGuFinish();
	sceGuSync(0,0);
	//sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);
	
	//sceGuTerm();

	//sceKernelExitGame();
	return 0;
}

void blitImgToDonutScreen(float sx, float sy, float width, float height, Image *img,float x, float y)
{	
	// Set some variables
	if(width==0 || height==0)
		return;
	float slice = img->imageWidth/64;
	float x_width = width/slice;
	float u_width = img->imageWidth/slice;
	float cur_u = 0;
	float cur_x = 0;
	
	// Move to the desired point
	sceGumLoadIdentity();
	ScePspFVector3 move = {x, y, 0};
	sceGumTranslate(&move);
	
	// Set the texture so the u/v is right
	float u = 1.0f / ((float)img->imageWidth);
	float v = 1.0f / ((float)img->imageHeight);
	sceGuTexScale(u, v);
	
	// Set texture
	sceGuTexImage(0,img->textureWidth,img->textureHeight,img->textureWidth,(void*)img->data);
	
	// Blit stripping routine
	float ii;
	for(ii = slice; ii--;) {
		TP_Vertex_3D* vertices = (TP_Vertex_3D*)sceGuGetMemory(2 * sizeof(TP_Vertex_3D));
		
		vertices[0].u = cur_u;
		vertices[0].v = 0;
		vertices[0].x = cur_x;
		vertices[0].y = 0;
		vertices[0].z = 0;
		
		cur_u += u_width;
		cur_x += x_width;
		
		vertices[1].u = cur_u;
		vertices[1].v = img->imageHeight;
		vertices[1].x = cur_x;
		vertices[1].y = height;
		vertices[1].z = 0;
		
		sceGumDrawArray(GU_SPRITES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 2, 0, vertices);
	}
	// Reset the texture scale
	sceGuTexScale(1.0, 1.0);
}

void blitImageToDonutFullScreen(int sx, int sy, int width, int height, Image* source, int dx, int dy)
{
	//if (!initialized) return;
	Color* vram = getVramDrawBuffer();
	//sceKernelDcacheWritebackInvalidateAll();
	//guStart();
	sceGuCopyImage(GU_PSM_8888, sx, sy, width, height, source->textureWidth, source->data, dx, dy, PSP_LINE_SIZE, vram);
	sceGuFinish();
	sceGuSync(0,0);
}

int DrawDonut(int blue, char* title,Image* bg)
{
	//while(running())
	//{
		to_quit=0;
		sceGuStart(GU_DIRECT,list);

		//sceGuClearColor(0xFFFFFF); // blanc
		sceGuClearColor(0x000000); // noir
		sceGuClearDepth(0);
		sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
		//sceGuSync(0,0);
		//sceGuSwapBuffers();
		//sceGuSwapBuffers();
		sceGuEnable(GU_BLEND);
		sceGuEnable(GU_TEXTURE_2D);
		
		// Draw the background
		//sceGuStart(GU_DIRECT,list);
		//blitImgToDonutScreen(0,0,480,272,bg,0,0);
		
		// Finished drawing the background, drawing the font
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
		sceGuTexMode(GU_PSM_8888, 0, 0, 0);
		sceGuTexImage(0, 256, 128, 256, font2);
		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
		sceGuTexEnvColor(0x0);
		sceGuTexOffset(0.0f, 0.0f);
		sceGuTexScale(1.0f / 256.0f, 1.0f / 128.0f);
		sceGuTexWrap(GU_REPEAT, GU_REPEAT);
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);
		//sceGuEnable(GU_TEXTURE_2D);
		
		
		drawString(title,  0, 0, 0xFFFFFFFF, 0);
		drawString("SimpleAmp v3 - By The Shadow Phoenixes", 0, 252, 0xFFFFFFFF, 0);
		
		sceGuDisable(GU_TEXTURE_2D);
		sceGuDisable(GU_BLEND);
		
		/*sceGuFinish();
		sceGuSync(0,0);
		sceGuStart(GU_DIRECT,list);*/
		
		//sceGuDisable(GU_BLEND);
		// And finally, draw the long-waited Donut :)
		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumPerspective(75.0f,16.0/9.0f,1.0f,1000.0f);

		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();
		//transx=0.8f;
		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		ScePspFVector3 pos = { transx, transy, -3.0f };
		ScePspFVector3 rot = { ry * (GU_PI/180.0f), rx * (GU_PI/180.0f), 0.0f };
		sceGumTranslate(&pos);
		sceGumRotateXYZ(&rot);

		sceGuTexMode(GU_PSM_8888,0,0,0);
		//sceGuTexImage(0,64,64,64, lightmap_start);
		sceGuTexImage(0,64,64,64, lightmap_start);
		sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGB);
		sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		//sceGuColor(0xff9966); // bleu clair
		//sceGuColor(0x0000FF); // rouge
	
		if(!blue) {
			sceGuColor(0x0000FF);
		} else {
			sceGuColor(0xff9966);
		}
	
		ScePspFVector3 envmapMatrixColumns[2] = {
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f }
		};
		sceGuLight( 0, GU_DIRECTIONAL, GU_DIFFUSE, &envmapMatrixColumns[0] );
		sceGuLight( 1, GU_DIRECTIONAL, GU_DIFFUSE, &envmapMatrixColumns[1] );

		sceGuTexMapMode(GU_ENVIRONMENT_MAP,	0, 1);
		
		/*
			First pass: draw the torus
		*/
		//if(flags & PSP_CTRL_CROSS) {
			sceGuEnable(GU_TEXTURE_2D);
		//}
		sceGumDrawArray(GU_TRIANGLES,
			GU_NORMAL_32BITF|GU_VERTEX_32BITF|GU_INDEX_16BIT|GU_TRANSFORM_3D,
			sizeof(torus_in)/sizeof(unsigned short), torus_in, torus_v0
		);
		//if(flags & PSP_CTRL_CROSS) {
			sceGuDisable(GU_TEXTURE_2D);
		//}
		

		/*
			Seconds pass: draw the outline
		*/
		//if(flags & PSP_CTRL_CIRCLE) {
			sceGuFrontFace(GU_CCW);
			//sceGuColor(0x0); // black
			sceGuColor(0xFFFFFF); // white
			sceGumDrawArray(GU_TRIANGLES,
				GU_VERTEX_32BITF|GU_INDEX_16BIT|GU_TRANSFORM_3D,
				sizeof(torus_in)/sizeof(unsigned short), torus_in, torus_v1
			);
			sceGuFrontFace(GU_CW);
		//}
		
		sceGuFinish();
		donutSimpleSpectrum();
		sceGuSync(0,0);
		/*
		sceGuStart(GU_DIRECT,list);*/
		//blitImageToDonutFullScreen(0,0,480,272,bg,0,0);
		/*if(flags & PSP_CTRL_LTRIGGER) {
		} else {
		initGraphics();
		Manage_Key_Square();
		}
		//if(flags & PSP_CTRL_SQUARE) {
		//} else {
		if(old & PSP_CTRL_LEFT) {
			transx-=0.01f;
		}
		if(old & PSP_CTRL_RIGHT) {
			transx+=0.01f;
		}
		//if(flags & PSP_CTRL_TRIANGLE) {
		//} else {
		if(old & PSP_CTRL_UP) {
			transy+=0.01f;
			//flags = 0;
		}*/
		/*if(flags & PSP_CTRL_SELECT) {
		} else {	
			//transy-=0.01f;
			credits_active=0;
			//initGraphics();
			//return 0;
			//to_quit=1;
			//flags = 0;
		}*/
		//if(!to_quit)
		//{
		//sceCtrlReadBufferPositive(&padDonut, 1);
		
		/*float tx = (pad.Lx - 128) / 127.0f;
		float ty = (pad.Ly - 128) / 127.0f;
		if(fabs(tx) <= 0.2f) {
			tx = 0.0f;
		} else {
			if(tx > 0.0f) {
				tx -= 0.2f;
			} else {
				tx += 0.2f;
			}
		}
		if(fabs(ty) <= 0.2f) {
			ty = 0.0f;
		} else {
			if(ty > 0.0f) {
				ty -= 0.2f;
			} else {
				ty += 0.2f;
			}
		}
		ty += 0.9f;
		tx += 0.9f; */
		rx += 0.9f;
		ry += 0.9f;
		//rx = tx;
		//ry = ty;
		
		/*if(old != padDonut.Buttons) {
			flags ^= padDonut.Buttons;
		}
		old = padDonut.Buttons;
		if(credits_active == 1) {*/
		//sceDisplayWaitVblankStart();
		
		// Let's put that on the screen !
		//donutSimpleSpectrum();
		sceGuSwapBuffers();
		
		//}
		//return 0;
	//}
	
}
