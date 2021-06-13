#include "mp3_file_management.h"
/* Routines per la gestione della tastiera della PSP */

//-------------------------------------------GESTIONE TASTO X------------------------------------------
void Manage_Key_Cross() 
{
	// vediamo cos'abbiamo selezionato
	if (file_list[scroller].d_stat.st_mode < 7000 ) 
	{
		// si tratta di una directory
		// vediamo se dobbiamo salire oppure dobbiamo scendere
		if ( strcmp(NomeFiles[scroller], "..") == 0)
		{
			// dobbiamo salire di una directory
			// se siamo gia' alla root, non dobbiamo fare niente
			if (strcmp(path_attuale, "ms0:/") != 0) 
			{
				// dobbiamo trovare la posizione del penultimo / 
				// per far questo, cancelliamo l'ultimo /
				path_attuale[strlen(path_attuale)-1] = 0;
				// ora cerchiamo l'ultimo /
				char *puntatore_ultimo_slash = strrchr(path_attuale, '/');
				path_attuale[puntatore_ultimo_slash-path_attuale+1] = 0;
				// finalmente leggiamo questa directory
				Leggi_Lista_Files(path_attuale);
				Stampa_Elenco_Files();
				blitAlphaImageToImage(0,0,336,204,playlist,72,45,screenbuffer);
				printTextImage(90, 80, NomeDir, WHITE, screenbuffer);
				flipScreen();
				scroller = 0;
			}
		}
		else 
		{
			// dobbiamo scendere di una directory
			// aggiungiamo la dir selezionata
			strcat(path_attuale, NomeFiles[scroller]);
			// leggiamo questa directory
			Leggi_Lista_Files(path_attuale);
			Stampa_Elenco_Files();
			blitAlphaImageToImage(0,0,336,204,playlist,72,45,screenbuffer);
			printTextImage(90, 80, NomeDir, WHITE, screenbuffer);
			flipScreen();
			scroller = 0;
		}
	}
	else 
	{
		// vediamo se il file selezionato e' un .mp3
		if (strstr(NomeFiles[scroller], ".mp3") != NULL || strstr(NomeFiles[scroller], ".MP3") != NULL ) 
		{
			// e' un MP3
			// fermiamo un'eventuale riproduzione
			MP3_End();
			Copia_Elenco_Files_Mp3();
			sprintf(tmp,"%s%s",path_mp3,NomeFiles_Mp3[scroller_mp3]);
			DoMP3(tmp,NomeFiles_Mp3[scroller_mp3]);
		}
	}  
}

//-------------------------------------------GESTIONE TASTO O------------------------------------------
void Manage_Key_Circle()
{
	MP3_Stop();
	sprintf(songname, "%s", " ");
	song_play = 0;
	sprintf(mp3time, "%s", " ");
}

//-------------------------------------------GESTIONE TASTO [] quadrato------------------------------------------
void Manage_Key_Square() 
{
/*	while(1) {
	pspDebugScreenInit();
	printf("bwa\n");
	}*/
	MP3_Pause();
	/*if(credits_active==1) {
	credits_active = 0;
	} else {
	credits_active = 1;
	}*/
	//threeDVis = 1;
}

//-------------------------------------------GESTIONE TASTO Left------------------------------------------
void Manage_Key_Left() 
{
  if(scroller_mp3 == 0) 
    scroller_mp3 = (file_num_mp3 - 1);
  else
    scroller_mp3--;

  sprintf(tmp,"%s%s",path_mp3,NomeFiles_Mp3[scroller_mp3]);
  DoMP3(tmp,NomeFiles_Mp3[scroller_mp3]);
}

//-------------------------------------------GESTIONE TASTO Right------------------------------------------
void Manage_Key_Right() 
{
  if(scroller_mp3 == (file_num_mp3 - 1))
    scroller_mp3 = 0;
  else
    scroller_mp3++;
    
  sprintf(tmp,"%s%s",path_mp3,NomeFiles_Mp3[scroller_mp3]);
  DoMP3(tmp,NomeFiles_Mp3[scroller_mp3]);
}

