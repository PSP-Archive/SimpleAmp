
struct {
  int Type;
  int Present;
  int Revision;
  long TagSize;
  char *FileName;
  char *Album;
  char *Artist;
  char *Comment;
  char *Genre;
  char *Title;
  char *Track;
  char *Year;
  unsigned short *Picture;
} ID3_Info;

//ID3_INFO * ID3_Info;

int Init_ID3();
int ID3_GetID3v1Info(char *szFileName);
int ID3_SetID3v1Info(char *szFileName);
int ID3_GetID3v2Info(char *szFileName);

int ID3_GetID3Info(char *szFileName);

//#define USE_WIN32 1

#ifdef USE_WIN32
#include <windows.h>
#define MSG(str) MessageBox(GetDesktopWindow(), str, "", MB_OK)
#else
#define MSG(str) printf(str)
#endif
