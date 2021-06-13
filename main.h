#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <callback.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <png.h>
#include "graphics.h"
#include <pspiofilemgr.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <psppower.h>
#include <psputils.h>
#include <psputility.h>
#include <time.h>
//#include <pspusb.h>
//#include <pspusbstor.h>
#include "mp3player.h"
#include <pspaudio.h>
#include <pspaudiolib.h>
//#include "utils.h"
#include "font_bitmap.c"
#include "id3.h"
#include "fontloader.h"

#define MAX_ARGS 16

enum UsbState
{
	USB_NOT_STARTED = 0, // Drivers not loaded
	USB_ON = 1, // Running :D
	USB_OFF = 2, // Can be disabled but with drivers loaded
};

typedef struct
{
	int x;
	int y;
	int Width; // Largeur
	int Height; // Hauteur
	Image* ImageData;
} Skin_Object;

void errorDisplay(char* ToDisplay1,char* ToDisplay2);
