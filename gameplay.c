#include "global.h"
#include <curses.h>
#include <unistd.h>


// char playerSprite[3][3]={
//         {"/\\ "},
//         {"[]>"},
//         {"\\/ "}
// };

char playerSprite[3][1] = {
    {"+"},
    {">"},
    {"+"}
};

// char enemySpriteLv1[3][3]={
//         {" S "},
//         {"<o>"},
//         {" S "}
// };

char enemySpriteLv1[3][1]={
        {"S"},
        {"<"},
        {"S"}
};


char enemySpriteLv2[3][3]={
        {"[\\ "},
        {"[@>"},
        {"[/ "}
};

void gameArea(int mainPipe){
	struct Object data, dataEnemy[ENEMIES], player, enemy, rocket;
	int collision = 0;
    int id;
    int i;

    // Loop di gioco
	do{
        read(mainPipe, &data, sizeof(data)); // Lettura ciclica del dato dalla mainPipe
        id = data.serial;   // Assegno il serial ad una variabile d'appoggio 
        switch(data.identifier){    // Valutazione del dato
            // Caso nave giocatore
            case PLAYER:
                if (player.y >= 2 && player.y <= MAX_Y - 1){
                    mvprintw(player.y, player.x, "%s", " ");
                    mvprintw(player.y+1, player.x, "%s", " ");
                    mvprintw(player.y+2, player.x, "%s", " ");
                }
                player = data;  // Assegnamo il valore ad una variabile player
                break;
            
            // Caso nemico
            case ENEMY:
                if (dataEnemy[id].y >= 2 && dataEnemy[id].y <= MAX_Y) 
                    mvaddch(dataEnemy[id].y, dataEnemy[id].x, ' ');
                dataEnemy[id] = data; // Aggiorniamo l'array dei nemici con i valori del nemico attuale
                mvaddch(data.y, data.x, data.identifier);
                break;
        }

        switch(data.identifier){
            case PLAYER:
                printSprite(data.x, data.y, playerSprite);
                break;
        }
        mvprintw(0, 0, "Vite: %d", player.lives);
        refresh(); 
	} while (!collision);
}
