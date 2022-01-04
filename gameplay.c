#include "global.h"

void gameArea(int fd){
    
    struct Object datoRead;
    struct Object player;
    struct Object enemy;
    int collision;
	int firstEnemyRead =-1;
	//Il valore -1 segnala che si tratta della prima lettura 
	player.x = -1;
   

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

			// Aggiorno le coordinate relative alla nuova posizione 
			player = datoRead;
		}
		//Tutta quetsa parte è la parte dei nemici
		else
		{
    			//Controlla che sia la prima lettura, fosse così fa lo spawn dei nemici stampadoli con le coordinate iniziali assegnotale.
               if (firstEnemyRead ==-1){

				   mvaddch(datoRead.y,datoRead.x,datoRead.identifier);
				   
			   }
			   else
			   {
					
					mvaddch(enemy.y, enemy.x, ' ');  

			    }   
				
				//aggiorno le coordinate
                enemy = datoRead;

        }
        
			
		// Visualizzo il carattere dell'entità sulle nuove coordinate 
		if (enemy.x != 1 || enemy.y != 1)
			mvaddch(datoRead.y, datoRead.x, datoRead.identifier);
		
	




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