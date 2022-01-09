#include "global.h"
#include <unistd.h>

void gameArea(int mainPipe, int playerPipe, int enemyPipe[][2]){
	struct Object data, dataEnemy[ENEMIES], player, enemy;
	int collision = 0;
    int i;
    
    // Loop di gioco
	do{
        read(mainPipe, &data, sizeof(data)); // Lettura ciclica del dato dalla mainPipe
        switch(data.identifier){    // Valutazione del dato
            // Caso nave giocatore
            case PLAYER:    
                player = data;  // Assegnamo il valore ad una variabile player
                /* altre gestioni varie da definire? */
                write(playerPipe, &player, sizeof(player)); // Scrittura dei nuovi valori nella pipe player
                break;
            
            // Caso nemico
            case ENEMY:
                enemy = data; // Assegnamo il valore ad una variabile enemy
                /* altre gestioni varie da definire? */
                write(enemyPipe[enemy.serial][WRITE], &enemy, sizeof(enemy)); // Scrittura nuovi valori nell'apposita pipe nemico
                dataEnemy[enemy.serial] = enemy; // Aggiorniamo l'array dei nemici con i valori del nemico attuale
                break;
        }

        clear(); // Pulizia schermo delle vecchie posizioni per ristampa
        mvaddch(player.y, player.x, player.identifier); // Stampa nave nemica nella nuova posizione
        for(i=0;i<ENEMIES; i++) // Ciclo per la stampa delle navi nemiche nelle nuove posizioni
            mvaddch(dataEnemy[i].y, dataEnemy[i].x, dataEnemy[i].identifier);
        mvprintw(0, 1, "Vite: %d", player.lives); // Stampa vite rimanenti
        refresh(); 
	} while (!collision);
}
