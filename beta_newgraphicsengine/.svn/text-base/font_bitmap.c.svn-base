//  ****************************************
//  ****************************************
//	**********	font_bitmap.h     **********
//	**********	Ver 1.0           **********
//	**********	Author: -Aran-    **********
//	**********	Italy 31-03-2006  **********
//  ****************************************
//  ****************************************
#include "graphics.h"

char* path;
int sprite_x=0;
int sprite_y=0;
//initial position where we start to write
int posX=0;
int posY=0;
//Dimension of the psp screen
int DimScreenWidth=0;	
int DimScreenHeight=0;
Image* font;
//initiate global variables
//dim : Dimensione of the sprite
//width: width of the psp screen (or buffer)
//height: height of the psp screen (or buffer)
void SetSpriteInit(int fx,int fy,int width,int height)
{
	
	sprite_x=fx;
	sprite_y=fy;
	posX=0;
	posY=0;
	DimScreenWidth=width;
	DimScreenHeight=height;
	
}

void SetPath(char* p)
{
	path=p;
	font=imageLoadPNG(path);
}
//printing Text
Image* print_video(Image *buffer,char* msg,int x,int y,int number,int k)
{
	Image* screenbuffer=buffer;
	//Image* font=imageLoadPNG("fonts\\font.png");
	posX=x;
	posY=y;
	char* message=msg;
	char c;
	int i=0;
	int contatore=strlen(message);
	if(contatore<number)
	{
		number=contatore;
	}
	if(number==-1)
	{
		number=contatore;
	}
	for(i=0;i<number;i++)
	{
		int spostamento=1;
		c=message[i];
		if(c=='A')
		{
			if(k) imageDrawSectionAlpha(0+(0*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(0*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='B')
		{
			if(k) imageDrawSectionAlpha(0+(1*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(1*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='C')
		{
			if(k) imageDrawSectionAlpha(0+(2*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(2*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='D')
		{
			if(k) imageDrawSectionAlpha(0+(3*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(3*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='E')
		{
			if(k) imageDrawSectionAlpha(0+(4*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(4*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='F')
		{
			if(k) imageDrawSectionAlpha(0+(5*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(5*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='G')
		{
			if(k) imageDrawSectionAlpha(0+(6*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(6*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='H')
		{
			if(k) imageDrawSectionAlpha(0+(7*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(7*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='I')
		{
			if(k) imageDrawSectionAlpha(0+(8*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(8*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='J')
		{
			if(k) imageDrawSectionAlpha(0+(9*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(9*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}		
		if(c=='K')
		{
			if(k) imageDrawSectionAlpha(0+(10*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(10*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='L')
		{
			if(k) imageDrawSectionAlpha(0+(11*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(11*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='M')
		{
			if(k) imageDrawSectionAlpha(0+(12*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(12*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='N')
		{
			if(k) imageDrawSectionAlpha(0+(13*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(13*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='O')
		{
			if(k) imageDrawSectionAlpha(0+(14*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(14*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='P')
		{
			if(k) imageDrawSectionAlpha(0+(15*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(15*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='Q')
		{
			if(k) imageDrawSectionAlpha(0+(16*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(16*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='R')
		{
			if(k) imageDrawSectionAlpha(0+(17*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(17*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='S')
		{
			if(k) imageDrawSectionAlpha(0+(18*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(18*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='T')
		{
			if(k) imageDrawSectionAlpha(0+(19*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(19*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='U')
		{
			if(k) imageDrawSectionAlpha(0+(20*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(20*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='V')
		{
			if(k) imageDrawSectionAlpha(0+(21*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(21*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='W')
		{
			if(k) imageDrawSectionAlpha(0+(22*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(22*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='X')
		{
			if(k) imageDrawSectionAlpha(0+(23*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(23*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='Y')
		{
			if(k) imageDrawSectionAlpha(0+(24*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(24*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='Z')
		{
			if(k) imageDrawSectionAlpha(0+(25*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(25*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		//minuscolo
		if(c=='a')
		{
			if(k) imageDrawSectionAlpha(0+(26*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(26*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='b')
		{
			if(k) imageDrawSectionAlpha(0+(27*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(27*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='c')
		{
			if(k) imageDrawSectionAlpha(0+(28*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(28*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='d')
		{
			if(k) imageDrawSectionAlpha(0+(29*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(29*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='e')
		{
			if(k) imageDrawSectionAlpha(0+(30*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(30*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='f')
		{
			if(k) imageDrawSectionAlpha(0+(31*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(31*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='g')
		{
			if(k) imageDrawSectionAlpha(0+(32*sprite_x),0,sprite_x,sprite_y,font,posX,posY+spostamento+2); else imageDrawImageSectionAlpha(0+(32*sprite_x),0,sprite_x,sprite_y,font,posX,posY+spostamento+2,buffer);
		}
		if(c=='h')
		{
			if(k) imageDrawSectionAlpha(0+(33*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(33*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='i')
		{
			if(k) imageDrawSectionAlpha(0+(34*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(34*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='j')
		{
			if(k) imageDrawSectionAlpha(0+(35*sprite_x),0,sprite_x,sprite_y,font,posX,posY+spostamento); else imageDrawImageSectionAlpha(0+(35*sprite_x),0,sprite_x,sprite_y,font,posX,posY+spostamento,buffer);
		}
		if(c=='k')
		{
			if(k) imageDrawSectionAlpha(0+(36*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(36*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='l')
		{
			if(k) imageDrawSectionAlpha(0+(37*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(37*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='m')
		{
			if(k) imageDrawSectionAlpha(0+(38*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(38*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='n')
		{
			if(k) imageDrawSectionAlpha(0+(39*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(39*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='o')
		{
			if(k) imageDrawSectionAlpha(0+(40*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(40*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='p')
		{
			if(k) imageDrawSectionAlpha(0+(41*sprite_x),0,sprite_x,sprite_y,font,posX,posY+spostamento+1); else imageDrawImageSectionAlpha(0+(41*sprite_x),0,sprite_x,sprite_y,font,posX,posY+spostamento+1,buffer);
		}
		if(c=='q')
		{
			if(k) imageDrawSectionAlpha(0+(42*sprite_x),0,sprite_x,sprite_y,font,posX,posY+spostamento+1); else imageDrawImageSectionAlpha(0+(42*sprite_x),0,sprite_x,sprite_y,font,posX,posY+spostamento+1,buffer);
		}
		if(c=='r')
		{
			if(k) imageDrawSectionAlpha(0+(43*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(43*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='s')
		{
			if(k) imageDrawSectionAlpha(0+(44*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(44*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='t')
		{
			if(k) imageDrawSectionAlpha(0+(45*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(45*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='u')
		{
			if(k) imageDrawSectionAlpha(0+(46*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(46*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='v')
		{
			if(k) imageDrawSectionAlpha(0+(47*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(47*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='w')
		{
			if(k) imageDrawSectionAlpha(0+(48*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(48*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='x')
		{
			if(k) imageDrawSectionAlpha(0+(49*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(49*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='y')
		{
			if(k) imageDrawSectionAlpha(0+(50*sprite_x),0,sprite_x,sprite_y,font,posX,posY+spostamento); else imageDrawImageSectionAlpha(0+(50*sprite_x),0,sprite_x,sprite_y,font,posX,posY+spostamento,buffer);
		}
		if(c=='z')
		{
			if(k) imageDrawSectionAlpha(0+(51*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(51*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		//caratteri speciali
		if(c==' ')
		{
		}
		//numeri
		if(c=='1')
		{
			if(k) imageDrawSectionAlpha(0+(52*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(52*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='2')
		{
			if(k) imageDrawSectionAlpha(0+(53*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(53*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='3')
		{
			if(k) imageDrawSectionAlpha(0+(54*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(54*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='4')
		{
			if(k) imageDrawSectionAlpha(0+(55*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(55*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='5')
		{
			if(k) imageDrawSectionAlpha(0+(56*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(56*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='6')
		{
			if(k) imageDrawSectionAlpha(0+(57*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(57*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='7')
		{
			if(k) imageDrawSectionAlpha(0+(58*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(58*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='8')
		{
			if(k) imageDrawSectionAlpha(0+(59*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(59*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='9')
		{
			if(k) imageDrawSectionAlpha(0+(60*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(60*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='0')
		{
			if(k) imageDrawSectionAlpha(0+(61*sprite_x),0,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(61*sprite_x),0,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		//caratteri di punteggiatura
		if(c=='.')
		{
			if(k) imageDrawSectionAlpha(0+(0*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY+spostamento+1); else imageDrawImageSectionAlpha(0+(0*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY+spostamento+1,buffer);
		}
		if(c==',')
		{
			if(k) imageDrawSectionAlpha(0+(1*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(1*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c==';')
		{
			if(k) imageDrawSectionAlpha(0+(2*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY+spostamento+1); else imageDrawImageSectionAlpha(0+(2*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY+spostamento+1,buffer);
		}
		if(c==':')
		{
			if(k) imageDrawSectionAlpha(0+(3*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY+spostamento+1); else imageDrawImageSectionAlpha(0+(3*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY+spostamento+1,buffer);
		}
		if(c=='_')
		{
			if(k) imageDrawSectionAlpha(0+(4*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(4*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='-')
		{
			if(k) imageDrawSectionAlpha(0+(5*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(5*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='=')
		{
			if(k) imageDrawSectionAlpha(0+(6*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(6*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='+')
		{
			if(k) imageDrawSectionAlpha(0+(7*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(7*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='*')
		{
			if(k) imageDrawSectionAlpha(0+(8*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(8*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='\\')
		{
			if(k) imageDrawSectionAlpha(0+(9*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(9*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='/')
		{
			if(k) imageDrawSectionAlpha(0+(10*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(10*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='!')
		{
			if(k) imageDrawSectionAlpha(0+(11*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(11*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='?')
		{
			if(k) imageDrawSectionAlpha(0+(12*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(12*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='^')
		{
			if(k) imageDrawSectionAlpha(0+(13*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(13*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='£')
		{
			if(k) imageDrawSectionAlpha(0+(14*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(14*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='$')
		{
			if(k) imageDrawSectionAlpha(0+(15*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(15*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='%')
		{
			if(k) imageDrawSectionAlpha(0+(16*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(16*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='&')
		{
			if(k) imageDrawSectionAlpha(0+(17*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(17*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='{')
		{
			if(k) imageDrawSectionAlpha(0+(18*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(18*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='}')
		{
			if(k) imageDrawSectionAlpha(0+(19*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(19*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='[')
		{
			if(k) imageDrawSectionAlpha(0+(20*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(20*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c==']')
		{
			if(k) imageDrawSectionAlpha(0+(21*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(21*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='')
		{
			if(k) imageDrawSectionAlpha(0+(22*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(22*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='ˆ')
		{
			if(k) imageDrawSectionAlpha(0+(23*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(23*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}
		if(c=='˜')
		{
			if(k) imageDrawSectionAlpha(0+(24*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY); else imageDrawImageSectionAlpha(0+(24*sprite_x),sprite_y,sprite_x,sprite_y,font,posX,posY,buffer);
		}

		
		
		
		
		//fine punteggiatura
		
		posX+=sprite_x;
	}
	return screenbuffer;
}
