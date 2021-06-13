/* File Browsing routines */

//-------------------------------------------COPIA STRUTTURE SceIoDirent------------------------------------------
void Swap_Strutture_Files(SceIoDirent *prima, SceIoDirent *seconda) {
	SceIoDirent appoggio;
	
	strcpy(appoggio.d_name, prima->d_name);
	appoggio.d_stat.st_mode = prima->d_stat.st_mode;
	
	strcpy(prima->d_name, seconda->d_name);
	prima->d_stat.st_mode = seconda->d_stat.st_mode;
	
	strcpy(seconda->d_name, appoggio.d_name);
	seconda->d_stat.st_mode = appoggio.d_stat.st_mode;
}

//-------------------------------------------ORDINAMENTO FILES IN MEMORIA------------------------------------------
//------------------------------------------METTIAMO LE DIRECTORY IN TESTA-----------------------------------------
void Ordina_Files() {
	// il primo file da verificare e' il secondo, 
	// dato che l'indice parte da 0
	// perche' nel primo posto ci deve sempre stare il ..
	int primo_file_da_trattare = 1;
	// se siamo nella root...
	if (strcmp(path_attuale, "ms0:/") == 0 ) 
		primo_file_da_trattare = 0;
	// ci sono files nella lista?
	if (file_num < primo_file_da_trattare + 1)
		// non abbiamo niente da ordinare
		return;

	int i, j;
	// e' una semplice bubble-sort;
	// partiamo dall'ultima voce; se e' una directory
	// la saliamo al posto precedente, e cosi' via
	for (i = primo_file_da_trattare; i < file_num-1; i++) {
		for (j = file_num-1; j > i; j--) {
			if (file_list[j].d_stat.st_mode < 7000 ) {
				// questa e' una directory; la dobbiamo salire di una posizione
				Swap_Strutture_Files(&file_list[j], &file_list[j-1]);
			}
		}  
	}

}

//-------------------------------------------LETTURA FILES DA DISCO------------------------------------------
void Leggi_Lista_Files(char *path_da_leggere) {

  int puntatore_path;
  int file_presente;
	int i;
  
  // cancelliamo la lista dei files, prima di leggerla
	for (i = 0; i < file_num; i++) {
		file_list[file_num].d_stat.st_mode = -1;
		strcpy(file_list[file_num].d_name, "");
	}
	// azzeriamo il num di files nella FileList
  file_num = 0;

  // vediamo se dobbiamo aggiungere il .. per salire dalla directory corrente
  if (strcmp(path_da_leggere, "ms0:/") != 0 ) {
  	file_list[file_num].d_stat.st_mode = 4000; // directory
  	strcpy(file_list[file_num].d_name, "..");
  	file_num = 1;
  } // */

  puntatore_path = sceIoDopen(path_da_leggere);
  file_presente = 1; 
  while((file_presente > 0) && (file_num < MAX_NUM_FILE)) {
    file_presente = sceIoDread(puntatore_path, &file_list[file_num]);
    // vediamo se e' una directory, e se non e' gia presente lo slash finale
    if (file_list[file_num].d_stat.st_mode < 7000 && strchr(file_list[file_num].d_name, '/') == NULL)
    	// allora dobbiamo aggiungere la barra finale
    	strcat(file_list[file_num].d_name, "/");
    // non mettiamo le directory . & ..
    if (file_list[file_num].d_name[0] == '.') 
      continue; // */
    if (file_presente > 0) 
      file_num++;
  }
  sceIoDclose(puntatore_path);
  // ordiniamo i files letti, mettendo le directory in testa
  Ordina_Files();
	// azzeriamo il puntatore al file selezionato
  File_Selezionato = 0;
}

//-------------------------------------------VERIFICA DEI TASTI PREMUTI------------------------------------------
int Verifica_Tasti_Premuti(SceCtrlData pad, SceCtrlData pad_precedente) {
	// valore di ritorno, inizializzatro ad un valore di default
	int tasti_premuti = 0;
	
	// vediamo se qualche tasto e' stato premuto
	if (pad.Buttons != pad_precedente.Buttons )
		tasti_premuti = 1;
	
	// restituiamo il valore di ritorno
	return tasti_premuti;
}

