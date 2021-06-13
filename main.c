/* SimpleAmp by Team Shadow, changes from Reborn to v3 by the Shadow Phoenixes */
/* TODO :
   - Add skin file checking - Semi-Done : Errors if a file doesn't exsist.
   - USBHostFS support
   - Donut BG without flickering
   - Etc ...
*/

#include "main.h"
//#include "usbhostfs.h"

//PSP_MODULE_INFO("SimpleAmp v3", 0x1000,1,1);
//PSP_MAIN_THREAD_ATTR (0);
PSP_MODULE_INFO("SimpleAmp v3", 0,1,1);

#define RGB(r, g, b) ((b << 16) | (g << 8) | r)
#define BLACK RGB(0, 0, 0)
#define WHITE RGB(255,255,255)
#define WHITEY RGB(255,255,254)
#define LIGHTBLUE RGB(0,132,255)
#define LIGHTBLUE RGB(0,132,255)
#define GREY RGB(180,180,180)
#define delay sceKernelDelayThread
#define START_PATH "ms0:/psp/music/"
//#define START_PATH "host0:/"
//#define MAXPATH_SIZE 0x200 // 200 in esadecimale = 512 caratteri
#define MAXPATH_SIZE 0x400 // 400 in hex = 1024 caracters
#define MAX_NUM_FILE 1024 // max 1024 mp3
#define SPLASH_ENABLE 1 //Enable splash ?
#define USBHOST_ENABLE 1 //Enable USBHostFS ?

static enum UsbState usbhost_state = USB_NOT_STARTED;

const char *bootfile;
char bootpath[1024];

int i;
int scroller = 0;
int scroller_mp3 = 0;
int  File_Selezionato;

// Here are all the skins picture specs
int Skin_Buttons_Height = 24;
int Skin_Buttons_Width = 62;
int Skin_Buttons_FromLeft = 0;
int Skin_Button_1_Y = 39;
int Skin_Button_1_Step = 3;
//int Skin_Button_2_FromLeft = 0;
int Skin_Button_2_Y = 150;
int Skin_Button_2_Step = 3;
/*int Skin_Button_Both_X = 24;
int Skin_Button_Both_Y = 62;
int Skin_Button_Equalizer_X = 24;
int Skin_Button_Equalizer_Y = 62;
int Skin_Button_Normal_X = 24;
int Skin_Button_Normal_Y = 62;
int Skin_Button_Oscilloscope_X = 24;
int Skin_Button_Oscilloscope_Y = 62;
int Skin_Button_Repeat_X = 24;
int Skin_Button_Repeat_Y = 62;
int Skin_Button_Shuffle_X = 24;
int Skin_Button_Shuffle_Y = 62;*/
int Skin_Filelist_Height = 196;
int Skin_Filelist_Width = 408;
int Skin_Hour_X = 60;
int Skin_Hour_Y = 80;
int Skin_UpMask_Height = 23;
int Song_Playing_Time_X = 405;
int Song_Playing_Time_Y = 97;
Skin_Object Skin_Battery;
Skin_Object Skin_ScrollerBar;
Skin_Object Skin_ScrollerCursor;

int playlist_changed  = 1;
int anim_up_count     = 0;
int anim_down_count   = 0;
int play_count        = 0;
int x;
int y;
int count_title       = 0;
int slide_text        = 0;
int Menu_Mp3          = 0;
int credits_active    = 0;
int thidDonut         = 0;
	
int eq_anim           = 0;

int font_loaded		  = 0;

char mp3time[200];
char buff[150];
char NomeDir[100];
char NomeFiles[1024][512];
char NomeFiles_Mp3[1024][512];
char tmp[1024];
char songname[512];
char skinName[100];
char tmpLoadPath[1024];

char path_attuale[MAXPATH_SIZE];
char path_mp3[MAXPATH_SIZE];
SceIoDirent    file_list[MAX_NUM_FILE];
int file_num;
int file_num_mp3;
Image* playlist;
Image* screenbuffer;
//Image* bg_vis;
int song_play = 0;
int threeDVis = 0;

Image*  splash;

void Stampa_Elenco_Files();

unsigned char simple_spectrum_bands[32];
unsigned char simple_spectrum_full[290];
unsigned char simple_spectrum_full_sign[290];
int           eq_delay = 0;

int           spectrum_active = 0;
int           spectrum_active_last = 0;

unsigned int percentagePlayed = 0;
char percentagePlayed_string[20];

int           play_mode = 0; // 0 = NORMAL (next song in dir) , 1 = SHUFFLE (random play) , 2 = REPEAT (repeat song)
/*
void parse_sceargs(SceSize args, void *argp)
{
	int  loc = 0;
	char *ptr = argp;
	int argc = 0;
	char *argv[MAX_ARGS];

	while(loc < args)
	{
		argv[argc] = &ptr[loc];
		loc += strlen(&ptr[loc]) + 1;
		argc++;
		if(argc == (MAX_ARGS-1))
		{
			break;
		}
	}

	argv[argc] = NULL;

	if(argc > 0)
	{
		char *lastdir;

		bootfile = argv[0];
		lastdir = strrchr(argv[0], '/');
		if(lastdir != NULL)
		{
			memcpy(bootpath, argv[0], lastdir - argv[0] + 1);
		}
	}

}*/

/*int start_usbhost(const char *bootpath)
{
	int retVal;
	char prx_path[1024];
	do
	{
		if(usbhost_state == USB_ON)
		{
			retVal = 0;
			break;
		}
		retVal=-1;
		//if(usbhost_state == USB_NOT_STARTED)
		//{
			strcpy(prx_path, bootpath);
			strcat(prx_path, "usbhostfs.prx");
			load_start_module(prx_path, 0, NULL);
			//retVal = load_start_module("ms0:/usbhostfs.prx", 0, NULL);
		//}
		if(retVal >= 0)
		{
		clearScreen(BLACK);
		flipScreen();
		printTextImage(276,200, "USB ON - 1 :D", BLACK, splash);
		}
		retVal = sceUsbActivate(HOSTFSDRIVER_PID);
		retVal = sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
		if (retVal != 0) {
			break;
		}
		retVal = sceUsbStart(HOSTFSDRIVER_NAME, 0, 0);
		if (retVal != 0) {
			break;
		}

		if(retVal == 0)
		{
			usbhost_state = USB_ON;
		}
	}
	while(0);

	return retVal;
}*/

void errorDisplay(char* ToDisplay1,char* ToDisplay2)
{
	pspDebugScreenInit();
	pspDebugScreenPrintf("I have the regret to inform you that SimpleAmp has crashed.\nError details :\n\n");
	pspDebugScreenPrintf(ToDisplay1);
	pspDebugScreenPrintf(ToDisplay2);
	sceKernelDelayThread(12000000);
	sceKernelExitGame();
}

int CrossProduct(int Multiplicater1,int Multiplicater2,int Divider)
{
	int x;
	x = ((Multiplicater1 * Multiplicater2) / Divider);
	return x;
}

void DoMP3(char *path,char *fname)
{
    //char returnname[512];
	MP3_End();
	/*ID3_GetID3v2Info("path");
	if(ID3_Info.Title=="")
		ID3_GetID3v1Info("path"); // ID3v1 fallback !
	*/
	ID3_GetID3Info(path);
	MP3_Init(1);
	MP3_Load(path);
	MP3_Play();
	song_play = 1;
	count_title=0;
	if(ID3_Info.Title=="")
	{
			sprintf(songname,"%s",fname);
	} else {
			//return ID3_Info.Title;
			sprintf(songname,"%s",ID3_Info.Title);
	}
}

//-------------------------------------------Reading From Memory Stick------------------------------------------
#include "file_browsing.h"
#include "keyboard_management.h"
#include "donut.h"

//---------------------------------------Copying filenames in a Char-----------------------------------

void Stampa_Elenco_Files(void) 
{
	sprintf(NomeDir,"%s", path_attuale);
	for (i = 0; i < file_num; i++) 	
		sprintf(NomeFiles[i],"%s", file_list[i].d_name);
}

//------------------------------------------------Time function-----------------------------------------

void drawtime()     
{
        struct tm *tsys;
        time_t cur_time;
        sceKernelLibcTime (&cur_time);
        tsys = localtime (&cur_time);
        sprintf (buff, "%02d%c%02d%s",(tsys->tm_hour > 12 ? tsys->tm_hour - 12 : tsys->tm_hour),
        (tsys->tm_sec & 1 ? ':' : ' '), tsys->tm_min,tsys->tm_hour > 12 ? "PM" : "AM");
}

void initTimezone ()      
{
	int tzOffset = 0;
	sceUtilityGetSystemParamInt (PSP_SYSTEMPARAM_ID_INT_TIMEZONE, &tzOffset);
	int tzOffsetAbs = tzOffset < 0 ? -tzOffset : tzOffset;
	int hours = tzOffsetAbs / 60;
	int minutes = tzOffsetAbs - hours * 60;
	static char tz[10];
	sprintf (tz, "GMT%s%02i:%02i", tzOffset < 0 ? "+" : "-", hours, minutes);
	setenv ("TZ", tz, 1);
	tzset ();
}

Image* BatteryStatus(Image* scrbuf,Image* battery0,Image* battery1,Image* battery2,Image* battery3,int BatteryPercent)
{
	Image* screenbuffer=scrbuf;
	
	//No need for that anymore, see upper for ints of the skin
	/*int xpos,ypos;
	
	xpos = 20;
	ypos = 25;
	*/
	
	if(BatteryPercent < 10) 
	{
		blitAlphaImageToScreen(0,0,Skin_Battery.Width,Skin_Battery.Height,battery0,Skin_Battery.x,Skin_Battery.y);
	}
	if(BatteryPercent >= 10 && BatteryPercent < 33) 
	{
		blitAlphaImageToScreen(0,0,Skin_Battery.Width,Skin_Battery.Height,battery1,Skin_Battery.x,Skin_Battery.y);
	}
	if(BatteryPercent >= 33 && BatteryPercent < 66) 
	{
		blitAlphaImageToScreen(0,0,Skin_Battery.Width,Skin_Battery.Height,battery2,Skin_Battery.x,Skin_Battery.y);
	}
	if(BatteryPercent >= 66 && BatteryPercent <= 100) 
	{
		blitAlphaImageToScreen(0,0,Skin_Battery.Width,Skin_Battery.Height,battery3,Skin_Battery.x,Skin_Battery.y);
	}
	return screenbuffer;
}

//-------------------------------------------Playlist drawing function-----------------------------------------
//Image* Img_tmp;
Image* Create_PlayList(Image* img_pl,Image* playlist,int changed)	
//Image* Create_PlayList(Image* playlist,int changed)	
{
	Image* Img_tmp=img_pl;
	//freeImage(Img_tmp);
	//Img_tmp=createImage(Skin_Filelist_Width,Skin_Filelist_Height);
	if(changed==1)
	{
		//blitAlphaImageToImage(0,0,Skin_Filelist_Width,Skin_Filelist_Height,playlist,0,0,Img_tmp);
		imageDrawImageSectionAlpha(0,0,Skin_Filelist_Width,Skin_Filelist_Height,playlist,0,0,Img_tmp);
		//blitAlphaImageToScreen(0,0,Skin_Filelist_Width,Skin_Filelist_Height,playlist,72,37);
	}
	for (i = 0; i < 14; i++) 
	{
		if (i+scroller < file_num)
			Img_tmp=print_video(Img_tmp,NomeFiles[i+scroller],32,36+((i*10)+1),43,0);	  
	}
	//blitAlphaImageToScreen(0,0,Skin_Filelist_Width,Skin_Filelist_Height,playlist,72,37);
	Img_tmp=print_video(Img_tmp,NomeDir,30,12,42,0);
	return Img_tmp;
}
//-------------------------------------------Simple spectrum function-----------------------------------------
Image* SimpleSpectrum(Image* scrbuf,Image** pun_bar)
{
	Image* screenbuffer=scrbuf;
	
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(0) - 1],90,47,screenbuffer);		   
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(1) - 1],140,47,screenbuffer);		   
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(2) - 1],190,47,screenbuffer);		   
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(3) - 1],240,47,screenbuffer);		   
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(4) - 1],290,47,screenbuffer);		   
	//blitAlphaImageToImage(0,0,46,220,pun_bar[MP3_SimpleSpectrum(5) - 1],340,47,screenbuffer);		   	

  int x,x1,x2;
  
  if((spectrum_active == 1) || (spectrum_active == 3))
  {
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

  if(( spectrum_active == 2 ) || ( spectrum_active == 3))
  {
    for(x = 0; x < 289; x++ )
    {
      if(simple_spectrum_full_sign[x])
        x1 = simple_spectrum_full[x];
      else
        x1 = -1 * simple_spectrum_full[x];
        
      if(simple_spectrum_full_sign[x+1])
        x2 = simple_spectrum_full[x+1];
      else
        x2 = -1 * simple_spectrum_full[x+1];
  
      drawLineScreen((96 + (x )), (142 + x1) , (97 + (x )), (142 + x2),0xFFFFFFFF);
  
    }
  }


	return screenbuffer;
}

void displayDonut(Image* bg)
{
	//int DonutCounter;
	//initGraphics();
	while(1) {
		if(credits_active) {
			scePowerSetClockFrequency(266, 266, 133);
			
			while(credits_active)
				{
		//for(DonutCounter=0;DonutCounter < 500;DonutCounter++)
		//{
				DrawDonut(0,songname,bg);
		//printf(" %03d",DonutCounter);
				}
		//}
			scePowerSetClockFrequency(222, 222, 111);
		}
	}
}              

void launchDonutThread()
{
	/*thidDonut = sceKernelCreateThread("Super Donut Thread", displayDonut, 0x05, 0xFA0, 0, 0);
	if(thidDonut >= 0) {
		sceKernelStartThread(thidDonut, 0, 0);
    }*/
}	

//------------------------------------------------Main Function-------------------------------------------------


//int main(SceSize args,void *argp)
//int main(int args,char **argp) 
int main()
{
	//scePowerSetClockFrequency(333, 333, 166);
	//scePowerSetClockFrequency(266, 266, 133);
	//scePowerSetClockFrequency(222, 222, 111);
	
	sprintf(skinName,"v3skin");
	
	pspDebugScreenInit();
	pspAudioInit();
	SetupCallbacks();
	int BatteryPercent = scePowerGetBatteryLifePercent();
	initTimezone();
	//parse_sceargs(args, argp);
	SetSpriteInit(8,10,480,272);
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\static\\font.png");
	SetPath(tmpLoadPath);
	strcpy(path_attuale, START_PATH);
	Leggi_Lista_Files(path_attuale);
	//SetupDonut();
	//launchDonutThread();
	initGraphics();
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\font.ttf");
	Font* mainFont = Font_Load(tmpLoadPath);
    sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\static\\splashscreen.png");
	splash = loadImage(tmpLoadPath);
	clearScreen(BLACK);
	printTextImage(276,255, "v3 BETA - Not for release", BLACK, splash);
	blitAlphaImageToScreen(0,0,480,272,splash,0,0);
	flipScreen();
	Init_ID3();
	scePowerSetClockFrequency(333, 333, 166);
	/*if(load_font("font.ttf"))
	{
		font_loaded = 1;
		set_font_size(30);
		set_font_angle(0.0);
		set_font_color(0xff0000ff);
	}*/
	//FontTexture* myFontTexture = Font_CreateTexture(250, 100);
	Font_SetSize(mainFont, 16);
	//Img_tmp=createImage(Skin_Filelist_Width,Skin_Filelist_Height);
	
	// SEEKING
	int     prev_analog_x  = 128;
	int     prev_analog_y  = 128;
	
    int w,xx,yy;
  	
	Image* sfondo;
	//start_usbhost(bootpath);
	//hostfs_init();
	//if(usbhost_state == USB_ON)
	//  printTextImage(276,230, "USB ON :D", BLACK, splash);
	
	// SKIN LOADING , Manual for the momentss
	
	Skin_Battery.x = 417;
	Skin_Battery.y = 0;
	Skin_Battery.Width = 38;
	Skin_Battery.Height = 16;
	
	Skin_ScrollerBar.x = 73;
	Skin_ScrollerBar.y = 88;
	Skin_ScrollerBar.Width = 392;
	Skin_ScrollerBar.Height = 2;
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\static\\trackbar.png");
	Skin_ScrollerBar.ImageData = loadImage(tmpLoadPath);
	
	//Skin_ScrollerCursor->x = Skin_ScrollerBar->x;
	Skin_ScrollerCursor.y = 81;
	Skin_ScrollerCursor.Width = 4;
	Skin_ScrollerCursor.Height = 14;
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\static\\trackcursor.png");
	Skin_ScrollerCursor.ImageData = loadImage(tmpLoadPath);
	
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\static\\battery0.png");
	Image* battery0 = loadImage(tmpLoadPath);
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\static\\battery1.png");
	Image* battery1 = loadImage(tmpLoadPath);
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\static\\battery2.png");
	Image* battery2 = loadImage(tmpLoadPath);
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\static\\battery3.png");
	Image* battery3 = loadImage(tmpLoadPath);
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\static\\upmask.png");
	Image* upmask =   loadImage(tmpLoadPath);
	//sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\static\\credits.png");
	//Image* credits =  loadImage(tmpLoadPath); // Disabled because of da Donut
	Image* img_playlist = createImage(Skin_Filelist_Width,Skin_Filelist_Height);
	//bg_vis = createImage(480,272)
	screenbuffer = createImage(480,272);	  
	
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\oscilloscope1.png");
	Image* osc_off   = loadImage(tmpLoadPath);
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\oscilloscope2.png");
	Image* osc_on    = loadImage(tmpLoadPath);
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\equalizer1.png");
	Image* eq_off    = loadImage(tmpLoadPath);
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\equalizer2.png");
	Image* eq_on     = loadImage(tmpLoadPath);
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\both1.png");
	Image* both_off  = loadImage(tmpLoadPath);
	sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\both2.png");
	Image* both_on   = loadImage(tmpLoadPath);
  
  sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\normal1.png");
  Image* norm_off  = loadImage(tmpLoadPath);
  sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\normal2.png");
  Image* norm_on   = loadImage(tmpLoadPath);
  sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\shuffle1.png");
  Image* shuff_off = loadImage(tmpLoadPath);
  sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\shuffle2.png");
  Image* shuff_on  = loadImage(tmpLoadPath);
  sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\repeat1.png");
  Image* rep_off   = loadImage(tmpLoadPath);
  sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\buttons\\repeat2.png");
  Image* rep_on    = loadImage(tmpLoadPath);

  sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\dynamic\\equalizer_base.png");
  Image* eq_base   = loadImage(tmpLoadPath);
//  Image* eq_base_a = loadImage("v3skin\\data\\dynamic\\equalizer_base2.png");

  sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\dynamic\\oscilloscope_base.png");
  Image* osc_base   = loadImage(tmpLoadPath);
  sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\dynamic\\oscilloscope_part1.png");
  Image* osc_left   = loadImage(tmpLoadPath);
  sprintf(tmpLoadPath,"%s%s",skinName,"\\data\\dynamic\\oscilloscope_part2.png");
  Image* osc_right  = loadImage(tmpLoadPath);
  
  
  void showLeftButtons(int mode)
     {
		switch(mode)
              {
                case 0: // NORMAL

                  blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,norm_on  ,Skin_Buttons_FromLeft,Skin_Button_1_Y);
                  blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,shuff_off ,Skin_Buttons_FromLeft,(Skin_Button_1_Y+Skin_Buttons_Height+Skin_Button_1_Step));
                  blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,rep_off    ,Skin_Buttons_FromLeft,(Skin_Button_1_Y+(2*(Skin_Buttons_Height+Skin_Button_1_Step))));
                  
                  break;

                case  1: // SHUFFLE

                  blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,norm_off  ,Skin_Buttons_FromLeft,Skin_Button_1_Y);
                  blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,shuff_on ,Skin_Buttons_FromLeft,(Skin_Button_1_Y+Skin_Buttons_Height+Skin_Button_2_Step));
                  blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,rep_off    ,Skin_Buttons_FromLeft,(Skin_Button_1_Y+(2*(Skin_Buttons_Height+Skin_Button_1_Step))));
                  
                  break;

                case 2: // REPEAT

                  blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,norm_off  ,Skin_Buttons_FromLeft,Skin_Button_1_Y);
                  blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,shuff_off ,Skin_Buttons_FromLeft,(Skin_Button_1_Y+Skin_Buttons_Height+Skin_Button_2_Step));
                  blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,rep_on    ,Skin_Buttons_FromLeft,(Skin_Button_1_Y+(2*(Skin_Buttons_Height+Skin_Button_1_Step))));
                  
                  break;
                  
              }
     }
   void showSpectrum1(int spectrumActive)
     {
            switch(spectrumActive)
          {
            case 0:
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y); 
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step)); 
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));
              break;
              
            case 1:
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_on   ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));
              break;
              
            case 2:
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_on  ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));
              break;
              
            case 3: 
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_on ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));
              break;
          }
      }     
     
//  Image* osc_base_a = loadImage("v3skin\\data\\dynamic\\oscilloscope_base2.png");
  
  // APPLY ALPHA TO EQ/OSC BASE CUT
/*  
  // EQ
  for(xx = 0 ; xx < 297; xx++)
    for(yy = 0 ; yy < 48; yy++)
      //if(((eq_base_a->data[eq_base_a->textureWidth * yy + xx] & 0xFF000000) >> 24 ) != 0x00)
        eq_base_a->data[eq_base_a->textureWidth * yy + xx] = (eq_base_a->data[eq_base_a->textureWidth * yy + xx] & 0x00FFFFFF) + (0xAA << 24);

  // OSC
  for(xx = 0 ; xx < 307; xx++)
    for(yy = 0 ; yy < 74; yy++)
      //if(((osc_base_a->data[osc_base_a->textureWidth * yy + xx] & 0xFF000000) >> 24 ) != 0x00)
        osc_base_a->data[osc_base_a->textureWidth * yy + xx] = (osc_base_a->data[osc_base_a->textureWidth * yy + xx] & 0x00FFFFFF) + (0xAA << 24);
*/  
 
  
	
	Image*  bar[10];
	Image** pun_bar=bar;
	char    nFile[256];
	int     k;
  
  int     time_xpos,time_ypos;
  
  time_xpos = 412;
  time_ypos = 237;
  
	/*for(k = 0; k < 10; k++)
	{
		sprintf(nFile,"data\\spectrum\\bar%d.png",(k+1));
		//bar[k] = loadImage(nFile);
	}*/

	Stampa_Elenco_Files();
	sfondo = loadImage("v3skin\\data\\static\\sfondo.png");
	playlist = loadImage("v3skin\\data\\dynamic\\filelist.png");

	sceDisplayWaitVblankStart();
	SceCtrlData pad;
	SceCtrlData pad_precedente;
	
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	// inizializziamo le variabili
	sceCtrlReadBufferPositive(&pad, 1);


  // SPLASH SCREEN COMES OUT

 if (SPLASH_ENABLE)
  {sceCtrlReadBufferPositive(&pad_precedente, 1);


  // WAIT FOR A KEY PRESS
/*	do
	{
	  sceCtrlReadBufferPositive(&pad, 1);
  }
  while(Verifica_Tasti_Premuti(pad, pad_precedente) != 1);
  
  pad_precedente.Buttons = pad.Buttons = 0;
  */
  
  
  delay(1000000);
  
  
  /*for(i = 0; i < 300;i++)
  {
  sceWaitForvBlankStart();
  }*/
  // FADING TO BLACK
  for(w = 0xFF; w > 0x00 ; w -= 0x18) 
  {
    clearScreen(BLACK);
    //fillScreenRect(WHITE,0,0,480,272);
    for(xx = 0 ; xx < 480; xx++)
      for(yy = 0 ; yy < 272; yy++)
        splash->data[splash->textureWidth * yy + xx] = (splash->data[splash->textureWidth * yy + xx] & 0x00FFFFFF) + (w << 24);
	  blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);
	  blitAlphaImageToScreen(0,0,480,272,splash,0,0);
 	  sceDisplayWaitVblankStart();
    flipScreen(); 
  }

  freeImage(splash);
  }
    
 /*   for(w = 0; w < Skin_Buttons_Width+Skin_Buttons_FromLeft; w+=2)
  {
    clearScreen(BLACK);
    blitAlphaImageToScreen(0,0,480,272,sfondo,0,0); */
  
  
  // TOP BAR ANIMATION - By Dream_Team
   
  //int Skin_UpMask_Height = 23;
  for(w = 0; w < Skin_UpMask_Height; w++)
  {
     clearScreen(BLACK);
	 blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);
	 blitAlphaImageToScreen(0,0,480,w,upmask,0,(w-Skin_UpMask_Height));
	 flipScreen();
  }
  // BUTTONS IN ANIMATION

  for(w = 0; w < Skin_Buttons_Width+Skin_Buttons_FromLeft; w+=2)
  {
    clearScreen(BLACK);
	blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);
	blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask  ,0,0);
    if(w < Skin_Buttons_Width)
    {
      blitAlphaImageToScreen((Skin_Buttons_Width-w),0,w,Skin_Buttons_Height,norm_off  ,0,Skin_Button_1_Y);
      blitAlphaImageToScreen((Skin_Buttons_Width-w),0,w,Skin_Buttons_Height,eq_off  ,0,Skin_Button_2_Y);      
    }
    else
    {
      blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,norm_off  ,w-Skin_Buttons_Width,Skin_Button_1_Y); 
      blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,w-Skin_Buttons_Width,Skin_Button_2_Y);      
    } 
 	  //sceDisplayWaitVblankStart();
    flipScreen(); 
  }

  for(w = 0; w < Skin_Buttons_Width+Skin_Buttons_FromLeft; w+=2)
  {
    clearScreen(BLACK);
	blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);
	blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask  ,0,0);
    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,norm_off  ,Skin_Buttons_FromLeft,Skin_Button_1_Y); 
    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y); 
    if(w < Skin_Buttons_Width)
    {
      blitAlphaImageToScreen((Skin_Buttons_Width-w),0,w,Skin_Buttons_Height,shuff_off  ,0,(Skin_Button_1_Y+Skin_Buttons_Height+Skin_Button_1_Step));
      blitAlphaImageToScreen((Skin_Buttons_Width-w),0,w,Skin_Buttons_Height,osc_off  ,0,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));      
    }
    else
    {
      blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,shuff_off  ,w-Skin_Buttons_Width,(Skin_Button_1_Y+Skin_Buttons_Height+Skin_Button_1_Step)); 
      blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off  ,w-Skin_Buttons_Width,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));      
    } 

 	  //sceDisplayWaitVblankStart();
    flipScreen(); 
  }

  for(w = 0; w < Skin_Buttons_Width+Skin_Buttons_FromLeft; w+=2)
  {
    clearScreen(BLACK);
	blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);
	blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask  ,0,0);

    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,norm_off  ,Skin_Buttons_FromLeft,Skin_Button_1_Y); 
    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,shuff_off ,Skin_Buttons_FromLeft,(Skin_Button_1_Y+Skin_Buttons_Height+Skin_Button_1_Step));
   
    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y); 
    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off  ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));

    if(w < Skin_Buttons_Width)
    {
      blitAlphaImageToScreen((Skin_Buttons_Width-w),0,w,Skin_Buttons_Height,rep_off  ,0,(Skin_Button_1_Y+(2*(Skin_Buttons_Height+Skin_Button_1_Step))));
      blitAlphaImageToScreen((Skin_Buttons_Width-w),0,w,Skin_Buttons_Height,both_off  ,0,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));      
    }
    else
    {
      blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,rep_off  ,w-Skin_Buttons_Width,(Skin_Button_1_Y+(2*(Skin_Buttons_Height+Skin_Button_1_Step)))); 
      blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off  ,w-Skin_Buttons_Width,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));      
    } 

 	  //sceDisplayWaitVblankStart();
    flipScreen(); 
  }  
  
  // INIT SEEKING
  prev_analog_x = pad.Lx;
  prev_analog_y = pad.Ly;
  
  ///////////////////////////////////////////////////////////////
  // MAIN LOOP   


	while(1) 
	{

		sceCtrlReadBufferPositive(&pad, 1);


		if(credits_active==0)
		{
          drawtime();
          clearScreen(BLACK);          
		  // CHECK FOR EQ./OSC ANIMATION REQUEST
		  if(eq_anim)
		  {
		   int eq_y;
		   
		   if((spectrum_active == 1) || (spectrum_active == 3))
		   {		    
		     int hub = 1;

         if(spectrum_active == 1)
         {
    		   // OSC ANIMATION
  
    		   for(eq_y = 148; eq_y > 0; eq_y -= (hub/2))
    		   {
    		      hub++;
    		    
              blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);
   
               // STATUS
			  showLeftButtons(play_mode);
             
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_on  ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off  ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
              blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));

              blitAlphaImageToScreen(0,0,297,48,eq_base,91,200);

              //blitAlphaImageToScreen(0,0,307,74,osc_base ,86,110);
              //blitAlphaImageToImage(0,0,307,74,osc_base_a ,86,110,screenbuffer);
              blitAlphaImageToScreen((148 - eq_y),0,eq_y+1,109,osc_left ,0,88);
              blitAlphaImageToScreen(0,0,eq_y+1,109,osc_right,(480 - (eq_y + 1)),88);            
        		blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask,0,0);
        		screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
        		//blitAlphaImageToScreen(0,0,480,272,screenbuffer,0,0);
        		printTextScreen(time_xpos+1,time_ypos+1, buff, BLACK);
        		printTextScreen(time_xpos  ,time_ypos  , buff, WHITE);
			if(Menu_Mp3==0)
			{
			printTextScreen(Song_Playing_Time_X+1,Song_Playing_Time_Y+1, mp3time, BLACK);
			printTextScreen(Song_Playing_Time_X,Song_Playing_Time_Y, mp3time, WHITE);
			}
    
              flipScreen();	
              	     
    		   }
            
         }

         if(spectrum_active == 3)
         {
              hub = 1;
      		   // EQ ANIMATION
      		   for(eq_y = 0; eq_y < 48; eq_y += (hub/2))
      		   {
      		      hub++;
      		      
                blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);
                
                switch(spectrum_active)
                {
                    
                  case 1: // EQ
                    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_on   ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
                    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
                    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));
    
                    blitAlphaImageToScreen(0,0,297,eq_y+1,eq_base,91,(200 + (48 - eq_y)));
                    //blitAlphaImageToImage(0,0,297,eq_y+1,eq_base_a,91,(200 + (48 - eq_y)),screenbuffer);
                    break;
                    
                    
                  case 3: // BOTH
                    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
                    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
                    blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_on ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));
                    blitAlphaImageToScreen(0,0,297,eq_y+1,eq_base,91,(200 + (48 - eq_y)));
                    //blitAlphaImageToImage(0,0,297,eq_y+1,eq_base_a,91,(200 + (48 - eq_y)),screenbuffer);
                    blitAlphaImageToScreen(0,0,307,74,osc_base ,86,110);
                    //blitAlphaImageToImage(0,0,307,74,osc_base_a ,86,110,screenbuffer);
                    blitAlphaImageToScreen(0,0,149,109,osc_left ,0,88);
                    blitAlphaImageToScreen(0,0,149,109,osc_right,(480 - 149),88);
                    break;
                }
                
                 // STATUS
                showLeftButtons(play_mode);
                        
          		blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask,0,0);
          		screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
          		//blitAlphaImageToScreen(0,0,480,272,screenbuffer,0,0);
          		printTextScreen(time_xpos+1,time_ypos+1, buff, BLACK);
          		printTextScreen(time_xpos  ,time_ypos  , buff, WHITE);
				if(Menu_Mp3==0)
				{
				printTextScreen(Song_Playing_Time_X+1,Song_Playing_Time_Y+1, mp3time, BLACK);
				printTextScreen(Song_Playing_Time_X,Song_Playing_Time_Y, mp3time, WHITE);
				}
      
      
                flipScreen();
                		     
      		   }
    		   
		      }
        }
      
        if((spectrum_active == 2))
        {
  		   int hub = 1;

 		      // EQ ANIMATION
  		   for(eq_y = 47; eq_y > 0; eq_y -= (hub/2))
  		   {
  		      hub++;
            blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);
            

            blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_on   ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
            blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
            blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));

            blitAlphaImageToScreen(0,0,297,eq_y+1,eq_base,91,(200 + (48 - eq_y)));
            //blitAlphaImageToImage(0,0,297,eq_y+1,eq_base_a,91,(200 + (48 - eq_y)),screenbuffer);

            
             // STATUS
            showLeftButtons(play_mode);
                    
      		blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask,0,0);
      		screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
      		//blitAlphaImageToScreen(0,0,480,272,screenbuffer,0,0);
      		printTextScreen(time_xpos+1,time_ypos+1, buff, BLACK);
      		printTextScreen(time_xpos  ,time_ypos  , buff, WHITE);
			if(Menu_Mp3==0)
			{
			printTextScreen(Song_Playing_Time_X+1,Song_Playing_Time_Y+1, mp3time, BLACK);
			printTextScreen(Song_Playing_Time_X,Song_Playing_Time_Y, mp3time, WHITE);
			}
  
  
            flipScreen();
            		     
  		   }          
          
          
  		   // OSC ANIMATION

  		   for(eq_y = 0; eq_y < 149; eq_y += (hub/2))
  		   {
  		      hub++;
  		    
            blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);
 
             // STATUS
            showLeftButtons(play_mode);

            
            switch(spectrum_active)
            {
              case 0: // OFF
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));
                break;
                
              case 1: // EQ
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_on   ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));

                blitAlphaImageToScreen(0,0,297,48,eq_base,91,200);
                //blitAlphaImageToImage(0,0,297,48,eq_base_a,91,200,screenbuffer);
                break;
                
              case 2: // OSC
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_on  ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));

                //blitAlphaImageToScreen(0,0,307,74,osc_base ,86,110);
                //blitAlphaImageToImage(0,0,307,74,osc_base_a ,86,110,screenbuffer);
                blitAlphaImageToScreen((148 - eq_y),0,eq_y+1,109,osc_left ,0,88);
                blitAlphaImageToScreen(0,0,eq_y+1,109,osc_right,(480 - (eq_y + 1)),88);
                break;
                
              case 3: // BOTH
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
                blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_on ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));

                //blitAlphaImageToScreen(0,0,297,48,eq_base,91,200);
                //blitAlphaImageToImage(0,0,297,48,eq_base_a,91,200,screenbuffer);

                blitAlphaImageToScreen(0,0,307,74,osc_base ,86,110);
                //blitAlphaImageToImage(0,0,307,74,osc_base_a ,86,110,screenbuffer);
                blitAlphaImageToScreen((148 - eq_y),0,eq_y+1,109,osc_left ,0,88);
                blitAlphaImageToScreen(0,0,eq_y+1,109,osc_right,(480 - (eq_y + 1)),88);
                break;
            }
            
				
      			blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask,0,0);
      			screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
      			//blitAlphaImageToScreen(0,0,480,272,screenbuffer,0,0);
      			printTextScreen(time_xpos+1,time_ypos+1, buff, BLACK);
      			printTextScreen(time_xpos  ,time_ypos  , buff, WHITE);
				if(Menu_Mp3==0)
				{
				printTextScreen(Song_Playing_Time_X+1,Song_Playing_Time_Y+1, mp3time, BLACK);
				printTextScreen(Song_Playing_Time_X,Song_Playing_Time_Y, mp3time, WHITE);
				}
  
            flipScreen();	
            	     
  		   }
		             
		     blitAlphaImageToScreen(0,0,307,74,osc_base ,86,110);       
        }
      		 
      		 
      	// FADE OUT EQ AND OSC
      		   
		   
		    eq_anim = 0;
		  }
		  
		  
			blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);

      // BUTTONS
      
      // SPECTRUM
      switch(spectrum_active)
      {
        case 0: // OFF
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));
          break;
          
        case 1: // EQ
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_on   ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));
          blitAlphaImageToScreen(0,0,297,48,eq_base,91,200);
          //blitAlphaImageToImage(0,0,297,48,eq_base_a,91,200,screenbuffer);
          break;
          
        case 2: // OSC
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_on  ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_off,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));

          blitAlphaImageToScreen(0,0,307,74,osc_base ,86,110);
          //blitAlphaImageToImage(0,0,307,74,osc_base_a ,86,110,screenbuffer);
          blitAlphaImageToScreen(0,0,149,109,osc_left ,0,88);
          blitAlphaImageToScreen(0,0,149,109,osc_right,(480 - 149),88);
          break;
          
        case 3: // BOTH
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,eq_off  ,Skin_Buttons_FromLeft,Skin_Button_2_Y);
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,osc_off ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+Skin_Buttons_Height+Skin_Button_2_Step));
          blitAlphaImageToScreen(0,0,Skin_Buttons_Width,Skin_Buttons_Height,both_on ,Skin_Buttons_FromLeft,(Skin_Button_2_Y+(2*(Skin_Buttons_Height+Skin_Button_2_Step))));

          blitAlphaImageToScreen(0,0,297,48,eq_base,91,200);
          //blitAlphaImageToImage(0,0,297,48,eq_base_a,91,200,screenbuffer);

          blitAlphaImageToScreen(0,0,307,74,osc_base ,86,110);
          //blitAlphaImageToImage(0,0,307,74,osc_base_a ,86,110,screenbuffer);
          blitAlphaImageToScreen(0,0,149,109,osc_left ,0,88);
          blitAlphaImageToScreen(0,0,149,109,osc_right,(480 - 149),88);
          break;
      }
      
       // STATUS
      showLeftButtons(play_mode);
      
 
     

			if(Menu_Mp3==1)
			{
				//free(img_playlist);
				img_playlist = Create_PlayList(img_playlist,playlist,playlist_changed);
				blitAlphaImageToScreen(0,0,Skin_Filelist_Width,Skin_Filelist_Height,img_playlist,72,37);
				playlist_changed=0;
			}
			if( (spectrum_active != 0 )&&(song_play == 1) )
			{
				screenbuffer = SimpleSpectrum(screenbuffer,pun_bar);
			}
			//screenbuffer=print_video(screenbuffer,songname,350-(count_title),21,-1,1);
			//blitAlphaImageToScreen(0,0,480,44,upmask,0,0);
//			screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
			//blitAlphaImageToScreen(0,0,480,272,screenbuffer,0,0);
//			printTextScreen(time_xpos+1,time_ypos+1, buff, BLACK);
//			printTextScreen(time_xpos  ,time_ypos  , buff, WHITE);
			if(Menu_Mp3==0)
			{
			printTextScreen(Song_Playing_Time_X+1,Song_Playing_Time_Y+1, mp3time, BLACK);
			printTextScreen(Song_Playing_Time_X,Song_Playing_Time_Y, mp3time, WHITE);
			}
      // SONG SEEKING
			{			  
			  int step;
			  
			  step = (pad.Lx - prev_analog_x);
			  if(((step > 0) && (step > 30))  || (pad.Lx == 255)) MP3_Seek(2048);
			  else
			  if(((step < 0) && (step < -30)) || (pad.Lx == 0))   MP3_Seek(-(2048 * 2));
			  
			  prev_analog_x = pad.Lx;
		  }
								
	 	  

			//flipScreen();
			
		}
		
		if(credits_active==1)
		{
			//blitAlphaImageToScreen(0,0,480,272,credits,0,0);
			//if(threeDVis) {
			//displayDonut(0);
								
			/*if((pad.Buttons & PSP_CTRL_SELECT)) 
			{ 
				credits_active=0;
				initGraphics();
			}*/
			//}
			/*flipScreen();
			initGraphics();
			clearScreen(WHITE);
			blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);*/
			//blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask,0,0);
			//showLeftButtons(play_mode);
			//BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
			//flipScreen();
			//flipScreen();
			//SetupDonut();
			DrawDonut(0,songname,sfondo);
			//sceGuTerm();
			//blitAlphaImageToScreen(0,0,480,272,screenbuffer,0,0);
			//flipScreen();
			
		}
//----------------------------------------------Slide SongTitle----------------------------------------------
		if(song_play==1)
		{
			/*if(count_title==( (slide_text*10)+260))
			{
				count_title=0;
			}	
			else*/
			//{
			// Char : 11
				//if(credits_active==0) screenbuffer=print_video(screenbuffer,songname,350-(count_title),21,-1,1);
				//if (credits_active==0)
				 
				//printf("%04d\n",(465-(11*strlen(songname))));
			//	count_title++;
			//}
			if(credits_active==0)
			{
				if(Menu_Mp3==0)
				{
					/*printTextScreen((470-(8*strlen(songname))),48,songname,BLACK);
					printTextScreen((469-(8*strlen(songname))),47,songname,WHITE);
					printTextScreen((470-(8*strlen(ID3_Info.Artist))),65,ID3_Info.Artist,BLACK);
					printTextScreen((469-(8*strlen(ID3_Info.Artist))),64,ID3_Info.Artist,WHITE);
					printTextScreen((470-(8*strlen(ID3_Info.Album))),81,ID3_Info.Album,BLACK);
					printTextScreen((469-(8*strlen(ID3_Info.Album))),80,ID3_Info.Album,WHITE);*/
					Font_PrintScreen(mainFont, 65, 130, songname, GU_RGBA(255, 255, 255, 255), getVramDrawBuffer());
					Font_PrintScreen(mainFont, 65, 160, ID3_Info.Artist, GU_RGBA(255, 255, 255, 255), getVramDrawBuffer());
					Font_PrintScreen(mainFont, 65, 190, ID3_Info.Album, GU_RGBA(255, 255, 255, 255), getVramDrawBuffer());
					blitAlphaImageToScreen(0,0,Skin_ScrollerBar.Width,Skin_ScrollerBar.Height,Skin_ScrollerBar.ImageData,Skin_ScrollerBar.x,Skin_ScrollerBar.y);
					Skin_ScrollerCursor.x=(Skin_ScrollerBar.x+CrossProduct(Skin_ScrollerBar.Width,MP3_GetBytesPlayed(),MP3_GetFileSize()));
					blitAlphaImageToScreen(0,0,Skin_ScrollerCursor.Width,Skin_ScrollerCursor.Height,Skin_ScrollerCursor.ImageData,Skin_ScrollerCursor.x,Skin_ScrollerCursor.y);
					//sprintf(percentagePlayed_string,"%03.1d%",percentagePlayed);
					//Font_PrintScreen(mainFont, 65, 220, percentagePlayed_string, GU_RGBA(255, 255, 255, 255), getVramDrawBuffer());
				}
				blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask,0,0);
				screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
				//blitAlphaImageToScreen(0,0,480,272,screenbuffer,0,0);
  			printTextScreen(time_xpos+1,time_ypos+1, buff, BLACK);
  			printTextScreen(time_xpos  ,time_ypos  , buff, WHITE);
//  			printTextScreen(Song_Playing_Time_X+1,Song_Playing_Time_Y+1, mp3time, BLACK);
//  			printTextScreen(Song_Playing_Time_X,Song_Playing_Time_Y, mp3time, WHITE);
				//flipScreen();
				
			}
		}
		else
		{
			if(credits_active==0)
			{
				blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask,0,0);
				screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
				//blitAlphaImageToScreen(0,0,480,272,screenbuffer,0,0);
  			printTextScreen(time_xpos+1,time_ypos+1, buff, BLACK);
  			printTextScreen(time_xpos  ,time_ypos  , buff, WHITE);
//  			printTextScreen(Song_Playing_Time_X+1,Song_Playing_Time_Y+1, mp3time, BLACK);
//  			printTextScreen(Song_Playing_Time_X,Song_Playing_Time_Y, mp3time, WHITE);
      }		  
		}
		
		if (MP3_EndOfStream() == 1) 
		{	
		  switch(play_mode)
		  {
		    case 0 : // NORMAL
		      {
      			if(scroller_mp3<file_num_mp3) 
      				scroller_mp3++;
      			if(scroller_mp3 == file_num_mp3) 
      				scroller_mp3 = 0;
      						
      			sprintf(tmp,"%s%s",path_mp3,NomeFiles_Mp3[scroller_mp3]);
				DoMP3(tmp,NomeFiles_Mp3[scroller_mp3]);
		        
		      }
		      break;
		      
		    case 1 : // SHUFFLE
		      {
            int prevSong = scroller_mp3;
            
            double r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
          
            r *= file_num_mp3;
          
            scroller_mp3 = (int)r;
            
            if(prevSong == scroller_mp3)
            {
              if((scroller_mp3 + 1) == file_num_mp3)
                scroller_mp3 = 0;
              else
                scroller_mp3 += 1;
            }
            
       			sprintf(tmp,"%s%s",path_mp3,NomeFiles_Mp3[scroller_mp3]);
				DoMP3(tmp,NomeFiles_Mp3[scroller_mp3]);
			}
		      break;
		    
		    case 2 : // REPEAT
		      {
      			sprintf(tmp,"%s%s",path_mp3,NomeFiles_Mp3[scroller_mp3]);
				DoMP3(tmp,NomeFiles_Mp3[scroller_mp3]);

		      }
		      break; 
		  }	  
		}

		if (song_play == 1) 
		{
			MP3_GetTimeString(mp3time);
			percentagePlayed = MP3_GetPercentagePlayed();
		}

//--------------------------------------------------------------------------------------------------
//---------------------------------------------------START-----------------------------------------
//--------------------------------------------------------------------------------------------------
		//if(!threeDVis) {
		if( (pad.Buttons & PSP_CTRL_START)&&(credits_active==0) ) 
		{ 
			if(Menu_Mp3==0)
			{
				play_count = 1;
				anim_up_count = 1; 
				Menu_Mp3=1;
				//if(spectrum_active == 0) spectrum_active = 1;
				spectrum_active_last = spectrum_active;
				spectrum_active = 0;
			}
			else
			{
				if(Menu_Mp3==1)
				{
					Menu_Mp3 = 0;
					spectrum_active = spectrum_active_last;
				}
			}			
			
			int hub;
			
			if(Menu_Mp3==1)  //Se Menu_mp3  attivo, parte l'animazione per la visualizzazione della filelist
			{
			  hub = 1;
				for (x = 480; x>=72;x -= (hub/2)) 
				{
				  hub++;
					blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);
					printTextScreen(time_xpos+1,time_ypos+1, buff, BLACK);
    			    printTextScreen(time_xpos  ,time_ypos  , buff, WHITE);

      showSpectrum1(spectrum_active);

           // STATUS
          showLeftButtons(play_mode);
     
//          screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
					blitAlphaImageToScreen(0,0,Skin_Filelist_Width,Skin_Filelist_Height,playlist,x,37);
					if (song_play == 1) 
					{
						MP3_GetTimeString(mp3time);
						percentagePlayed = MP3_GetPercentagePlayed();
						//screenbuffer=print_video(screenbuffer,songname,350-(count_title),21,-1,1);
						//blitAlphaImageToScreen(0,0,480,44,upmask,0,0);
					}
					blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask,0,0);
					screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
					//blitAlphaImageToScreen(0,0,480,272,screenbuffer,0,0);

 	        sceDisplayWaitVblankStart();
					flipScreen();
					
				}			
				anim_up_count = 0;
				img_playlist = Create_PlayList(img_playlist,playlist,playlist_changed);
				playlist_changed=0;

				blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask,0,0);
				screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
				printTextScreen(time_xpos+1,time_ypos+1, buff, BLACK);
				printTextScreen(time_xpos  ,time_ypos  , buff, WHITE);
				blitAlphaImageToScreen(0,0,Skin_Filelist_Width,Skin_Filelist_Height,img_playlist,72,37);
				//blitAlphaImageToScreen(0,0,480,272,screenbuffer,0,0);
				//flipScreen();
			
			}
			if (Menu_Mp3 == 0) 
			{
			  hub = 1;
				for (x = 72; x<=480;x += (hub/2)) 
				{
				  hub++;
				  
					blitAlphaImageToScreen(0,0,480,272,sfondo,0,0);
					printTextScreen(time_xpos+1,time_ypos+1, buff, BLACK);
					printTextScreen(time_xpos  ,time_ypos  , buff, WHITE);
					showSpectrum1(spectrum_active);

           // STATUS
          showLeftButtons(play_mode);
    

					//screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
					blitAlphaImageToScreen(0,0,Skin_Filelist_Width,Skin_Filelist_Height,playlist,x,37);
					if (song_play == 1) 
					{
						MP3_GetTimeString(mp3time);
						percentagePlayed = MP3_GetPercentagePlayed();
						//screenbuffer=print_video(screenbuffer,songname,350-(count_title),21,-1,1);
					}
					blitAlphaImageToScreen(0,0,480,Skin_UpMask_Height,upmask,0,0);
					screenbuffer=BatteryStatus(screenbuffer,battery0,battery1,battery2,battery3,BatteryPercent);
					//blitAlphaImageToScreen(0,0,480,272,screenbuffer,0,0);

					sceDisplayWaitVblankStart();
					flipScreen();
					
				}
				anim_down_count = 0;
			}
		}

//------------------------------------------Buttons with active filelist--------------------------------------

	    // verifichiamo se sono stati premuti dei tasti
		if ( Verifica_Tasti_Premuti(pad, pad_precedente) == 1) 
		{
			pad_precedente.Buttons = pad.Buttons;
			// tasto croce
			if (pad.Buttons & PSP_CTRL_CROSS) 
			{
			  if(Menu_Mp3 == 1)
  			  {				
  			    Manage_Key_Cross();
    				slide_text=strlen(songname);
    				count_title=0;
    				playlist_changed=1;
			  }
			} 
			// tasto cerchio
			if (pad.Buttons & PSP_CTRL_CIRCLE) 
				Manage_Key_Circle();
			// tasto quadrato
			if (pad.Buttons & PSP_CTRL_SQUARE) 
				Manage_Key_Square();
			
			// tasto left (dorsale sinistro)
			if (pad.Buttons & PSP_CTRL_LTRIGGER) 
			{
				Manage_Key_Left();
				count_title=0;
			}
			// tasto right (dorsale destro)
			if (pad.Buttons & PSP_CTRL_RTRIGGER)
			{
				Manage_Key_Right();
				count_title=0;
			}
			if (pad.Buttons & PSP_CTRL_DOWN) 
			{
			  if(Menu_Mp3 == 1)
			  {
  				playlist_changed=1;
  				if(scroller<file_num-1)
  				scroller++;
			  }
			  else
			    MP3_SetGain(MP3_GetGain() - 0.5);
			}        
			if (pad.Buttons & PSP_CTRL_UP) 
			{
			  if(Menu_Mp3 == 1)
			  {
  				playlist_changed=1;
  				if(scroller>0)
  				scroller--;
			  }
			  else
			  MP3_SetGain(MP3_GetGain() + 0.5);
			}
			

			if(pad.Buttons & PSP_CTRL_RIGHT)
			{
				/*if(spectrum_active == 0)
				{
					spectrum_active = 1;
					Menu_Mp3        = 0;
				}	
				else
				{
					if(spectrum_active != 0)
					{*/
						spectrum_active++;
						if(spectrum_active == 4) spectrum_active = 0;
						Menu_Mp3=0;
				//	}
				//}
				
				eq_anim = 1;	
			}
					

			if(pad.Buttons & PSP_CTRL_LEFT)
			{
        play_mode++;
        if(play_mode == 3) play_mode = 0;
			}



			if((pad.Buttons & PSP_CTRL_SELECT)) 
			{ 
				if(credits_active==0)
				{
					//SetupDonut();
					SetupDonut();
					credits_active=1;
					//displayDonut();
				}
				else
				{
					if(credits_active==1)
					{
						credits_active=0;
						sceGuTerm();
						initGraphics();
						//sceGuTerm();
						//initGraphics();
					}	
				}
			}
				
			
		}
		
    // GAIN DEBUG
    if(credits_active==0)
    {
      char deb_str[100];
      if(MP3_GetGain() >= 0)
      {
        if(MP3_GetGain() > 9.5)
          sprintf(deb_str," %03.1fdB",MP3_GetGain());    
        else
          sprintf(deb_str," %03.1f dB",MP3_GetGain());    
      }
      else
      {
        if(MP3_GetGain() < -9.5)
          sprintf(deb_str,"%03.1fdB",MP3_GetGain());    
        else
          sprintf(deb_str,"%03.1f dB",MP3_GetGain());    
      }
                
      printTextScreen(307  , 259  , deb_str, BLACK);
      printTextScreen(306  , 258  , deb_str, WHITE);

    
		
		
		//sceDisplayWaitVblankStart();
		flipScreen();
	}	
	}
	sceKernelSleepThread();
	return 0;

}
