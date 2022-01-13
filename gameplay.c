#include "global.h"
#include <curses.h>
#include <unistd.h>


 char playerSprite[3][3]={
         {"/\\ "},
         {"[]>"},
         {"\\/ "}
 };

// char playerSprite[3][1] = {
//     {"+"},
//     {">"},
//     {"+"}
// };

 char enemySpriteLv1[3][3]={
         {" S "},
         {"<o>"},
         {" S "}
 };

// char enemySpriteLv1[3][1]={
//         {"S"},
//         {"<"},
//         {"S"}
// };


char enemySpriteLv2[3][3]={
        {"[\\ "},
        {"[@>"},
        {"[/ "}
};

void gameArea(int mainPipe){
	Object data, dataEnemy[ENEMIES], player, enemy;
    Object* dataRocket;
	int collision = 0;
    int id;
    int i;
    int size=1;
    /* VARIBILI DEBUG COLLISIONI */
    int enemyColpito;
    int enemyNave;
    int pepega=0, pepega2=0;
    /* VARIBILI DEBUG COLLISIONI*/
    dataRocket = malloc(size * sizeof(Object));

    // Loop di gioco
	do{
        read(mainPipe, &data, sizeof(data)); // Lettura ciclica del dato dalla mainPipe
        id = data.serial;   // Assegno il serial ad una variabile d'appoggio 
        switch(data.identifier){    // Valutazione del dato
            // Caso nave giocatore
            case PLAYER:
                if (player.y >= 2 && player.y <= MAX_Y - 1){
                   deleteSprite(player.x,player.y);
                }
                player = data;  // Assegnamo il valore ad una variabile player
                break;
            
            // Caso nemico
            case ENEMY:
                if (dataEnemy[id].y >= 2 && dataEnemy[id].y <= MAX_Y) 
                    deleteSprite(dataEnemy[id].x,dataEnemy[id].y);
                dataEnemy[id] = data; // Aggiorniamo l'array dei nemici con i valori del nemico attuale
                break;

            //Caso razzo, rialloca la grandezza del vettore dinamico aggiungiendo uno spazio
            case ROCKET:
                size+=1;
                dataRocket=realloc(dataRocket, size*sizeof(Object));
                if (dataRocket[id].y >= 2 && dataRocket[id].y <= MAX_Y) 
                    mvaddch(dataRocket[id].y, dataRocket[id].x, ' ');
                dataRocket[id]=data;
                break;
        }

        switch(data.identifier){
            case PLAYER:
                printSprite(data.x, data.y, playerSprite);
                break;
            case ENEMY:
                /*Debug delle collisioni con il nemico e la nave, stampa in basso a sinistra quante volte colludono i nemici */
                enemyNave=checkCollisonEnemy(dataEnemy[id]);
                if(enemyNave==true){
                    pepega2++;
                    mvprintw(0,20,"%d", pepega2);
                }
                printSprite(data.x, data.y, enemySpriteLv1);
                break;
            case ROCKET:
                /*Debug delle collisioni con il razzo e il nemico, stampa in basso a destra quante volte colludono i razzi */
                enemyColpito=checkCollisionRocket(dataRocket[id]);
                if(enemyColpito==true){
                    pepega++;
                    mvprintw(0,10,"%d",pepega);
                    kill(dataRocket[id].pid, 10);
                    break;
                }
                mvaddch(dataRocket[id].y,dataRocket[id].x,ROCKET);
                break;
        }

        mvprintw(0, 0, "Vite: %d", player.lives);
        refresh(); 
	} while (!collision);
}
