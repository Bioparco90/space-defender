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
    // Variabili salvataggio oggetti
	Object data, enemy[ENEMIES], player;
    Object rocketUp[MAX_ROCKET];
    Object rocketDown[MAX_ROCKET];
    Object bomb[MAX_BOMB];

    // Variabili di gestione gioco
	int collision = 0;
    int id;
    int i;
    int size=1;

    /* VARIBILI DEBUG COLLISIONI */
    int enemyColpito;
    int enemyNave;
    int pepega=0, pepega2=0;

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
                if (enemy[id].y >= 2 && enemy[id].y <= MAX_Y) 
                    deleteSprite(enemy[id].x,enemy[id].y);
                enemy[id] = data; // Aggiorniamo l'array dei nemici con i valori del nemico attuale
                break;

            //Caso razzo giocatore alto
            case ROCKET_UP:
                if (rocketUp[id].y >= 1 && rocketUp[id].y <= MAX_Y+1)
                    mvaddch(rocketUp[id].y, rocketUp[id].x, ' ');

                rocketUp[id] = data;

                // controllo provvisorio
                if(rocketUp[id].x >= MAX_X){
                    kill(rocketUp[id].pid, 1);
                    rocketUp[id] = resetItem();
                }
                break;

            // Caso razzo giocatore basso
            case ROCKET_DOWN:
                if (rocketDown[id].y >= 1 && rocketDown[id].y <= MAX_Y+1)
                    mvaddch(rocketDown[id].y, rocketDown[id].x, ' ');

                rocketDown[id] = data;
                
                // controllo provvisorio
                if (rocketDown[id].x >= MAX_X){
                    kill(rocketDown[id].pid, 1);
                    rocketDown[id] = resetItem();
                }
                break;

            // Caso bomba nemica
            case BOMB:
                if(bomb[id].y >=1 && bomb[id].y <= MAX_Y+1)
                    mvaddch(bomb[id].y,bomb[id].x,' ');


                bomb[id]=data;

                // Controllo ricezione bordo schermo sinistro
                if(bomb[id].x < 0 ){
                    kill(bomb[id].pid, 1);
                    bomb[id]=resetItem();
                }
                break;

        }

        switch(data.identifier){
            case PLAYER:
                printSprite(data.x, data.y, playerSprite);
                break;
            case ENEMY:
                // Debug delle collisioni con il nemico e la nave, stampa quante volte collidono i nemici
                // enemyNave=checkCollisonEnemy(enemy[id]);
                // if(enemyNave==true){
                //     pepega2++;
                //     mvprintw(0,20,"%d", pepega2);
                // }
                printSprite(data.x, data.y, enemySpriteLv1);
                break;
            case ROCKET_UP:
                // Debug delle collisioni con il razzo e il nemico, stampa quante volte collidono i razzi
                // enemyColpito=checkCollisionRocket(dataRocket[id]);
                // if(enemyColpito==true){
                //     pepega++;
                //     mvprintw(0,10,"%d",pepega);
                //     kill(dataRocket[id].pid, 10);
                //     break;
                // }
                mvaddch(rocketUp[id].y,rocketUp[id].x, ROCKET_PRINT);
                break;
            
            case ROCKET_DOWN:
                mvaddch(rocketDown[id].y,rocketDown[id].x, ROCKET_PRINT);
                break;

            case BOMB:
                mvaddch(bomb[id].y,bomb[id].x,BOMB);
                break;

        }

        mvprintw(0, 0, "Vite: %d", player.lives);
        refresh(); 
	} while (!collision);
}
