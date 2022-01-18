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

    // Variabili di gestione gioco
	int collision = 0;
    int score = 0;
    int id;
    int i;

    // variabili test sparo nemico
    Object enemyRockets[ENEMIES];

    // Loop di gioco
	do{
        read(mainPipe, &data, sizeof(data)); // Lettura ciclica del dato dalla mainPipe
        id = data.serial;   // Assegno il serial ad una variabile d'appoggio 
        switch(data.identifier){    // Valutazione del dato
            // Caso nave giocatore
            case PLAYER:
                if (player.y >= 2 && player.y <= MAX_Y - 1){
                   deleteSprite(player);
                }
                player = data;  // Assegnamo il valore ad una variabile player
                break;
            
            // Caso nemico
            case ENEMY:
                if (enemy[id].y >= 2 && enemy[id].y <= MAX_Y) 
                    deleteSprite(enemy[id]);
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

            // Caso sparo nemico
            case ENEMY_ROCKET:
                if (enemyRockets[id].x > -1)
                    mvaddch(enemyRockets[id].y, enemyRockets[id].x, ' ');
                
                enemyRockets[id] = data;

                if (enemyRockets[id].x <= 0){
                    kill(enemyRockets[id].pid, 1);
                    enemyRockets[id] = resetItem();
                }
                break;
        }

        switch(data.identifier){
            case PLAYER:
                printSprite(data.x, data.y, playerSprite);
                break;

            case ENEMY:
                printSprite(data.x, data.y, enemySpriteLv1);
                break;

            case ROCKET_UP:
                for (i=0; i<ENEMIES; i++){
                    if (checkCollision(rocketUp[id], enemy[i])){
                        kill(rocketUp[id].pid, 1);
                        kill(enemy[i].pid, 1);
                        deleteSprite(enemy[i]);
                        mvaddch(rocketUp[id].y, rocketUp[id].x, ' ');
                        rocketUp[id] = resetItem();
                        enemy[i] = resetItem();
                        score += 100;
                    }
                }
                if (rocketUp[id].y > -1)
                    mvaddch(rocketUp[id].y,rocketUp[id].x, ROCKET);
                break;
            
            case ROCKET_DOWN:
                for (i=0; i<ENEMIES; i++){
                    if (checkCollision(rocketDown[id], enemy[i])){
                        kill(rocketDown[id].pid, 1);
                        kill(enemy[i].pid, 1);
                        deleteSprite(enemy[i]);
                        mvaddch(rocketDown[id].y, rocketDown[id].x, ' ');
                        rocketDown[id] = resetItem();
                        enemy[i] = resetItem();
                        score += 100;
                    }
                }
                if (rocketDown[id].y > -1)
                    mvaddch(rocketDown[id].y,rocketDown[id].x, ROCKET);
                break;

            case ENEMY_ROCKET:
                if (enemyRockets[id].x > -1)
                    mvaddch(enemyRockets[id].y, enemyRockets[id].x, enemyRockets[id].identifier);
        }

        mvprintw(0, 0, "Vite: %d", player.lives);
        mvprintw(0, MAX_X - 15, "Score: %d", score);
        refresh(); 
	} while (!collision);
}
