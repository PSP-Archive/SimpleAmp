/* Routines per la gestione della lista dei files mp3 */

//-------------------------------COPIA L'ARRAY DEI FILES ATTUALE NELL'ARRAY DEI FILES .MP3----------------------
void Copia_Elenco_Files_Mp3() {
	// variabile di appoggio
	int i;
	// innanzitutto azzeriamo la variabile che conta quanti mp3 sono in lista
	file_num_mp3 = 0;
	// adesso ci giriamo la lista dei files attuale, e scopriamo quali sono i files mp3
	for (i = 0; i < file_num; i++) {
		// vediamo se questo e' un file .mp3
		if (strstr(NomeFiles[i], ".mp3") != NULL || strstr(NomeFiles[i], ".MP3") != NULL ) {
			// questo e' un file .mp3; lo copiamo nella lista
			strcpy(NomeFiles_Mp3[file_num_mp3], NomeFiles[i]);
			// aggiorniamo la variabile contatore
			file_num_mp3++;
		}
	}
	scroller_mp3 = scroller - 1; // la voce in prima posizione e' il [..] (directory superiore)
	strcpy(path_mp3, path_attuale);
}
