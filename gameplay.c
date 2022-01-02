#include "global.h"

void gameArea(int fd){
    
    struct Object datoRead;
    struct Object player;
    struct Object enemies[ENEMIES];
    int i, collision = 0;

	//Il valore -1 segnala che si tratta della prima lettura 
	player.x = -1;
    for(i=0; i<ENEMIES; i++){
        enemies[i].x=-1;
    }

    do
	{
		//Leggo dalla pipe 
		read(fd, &datoRead, sizeof(datoRead));

		//Si controlla se il dato letto è il player 
		if (datoRead.identifier == player.identifier)
		{
			//Si verifica che non si tratti della prima volta che lo si visualizza
			if (player.x >= 0)
			{
				//Si cancella il precedente carattere visualizzato 
				mvaddch(player.y, player.x, ' ');
			}

			/* Aggiorno le coordinate relative alla nuova posizione */
			player = datoRead;
		}
		else
		{
			
			/* Verifica se non si tratta della prima lettura */
			for(i=0;i< ENEMIES; i++){

                if (enemies[i].x >= 0)
			    {
				/* Cancello il precedente carattere visualizzato */
				mvaddch(enemies[i].y, enemies[i].x, ' ');

			    }   
                enemies[i] = datoRead;

            }
			
			
		}
		/* Visualizzo il carattere dell'entità sulle nuove coordinate */
		if (player.x != 1 || player.y != 1)
			mvaddch(datoRead.y, datoRead.x, datoRead.identifier);

		/* Ogni 100 cicli genero 3 trappole in posizione casuale */




		/* Nascondo il cursore */
		curs_set(0);

		/* Aggiorno lo schermo di output per visualizzare le modifiche */
		refresh();

		/* Qui si faranno le collisioni per i razzi e i proiettili alieni */
		
		/* Esce quando terminano le vite della nave del player (codice da fare) */
	
		/* Il ciclo si ripete finchè non finiscono le vite, ma per ora mettiamo true perché dobbiamo ancora implementarle */
	} while (true);
    
    
    // codice
}