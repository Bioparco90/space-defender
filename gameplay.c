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

    player.lives = 3;
    player.pid = -1;
    for (i=0; i<ENEMIES; i++){
        enemy[i].lives = 3;
        enemy[i].pid = -1;
    }

    // Loop di gioco
	do{
        read(mainPipe, &data, sizeof(data)); // Lettura ciclica del dato dalla mainPipe
        id = data.serial;   // Assegno il serial ad una variabile d'appoggio 
        switch(data.identifier){    // Valutazione del dato
            // Caso nave giocatore
            case PLAYER:
                if (player.y >= 2 && player.y <= MAX_Y - 1)
                   deleteSprite(player);
                
                if (player.pid < 0)
                    player.pid = data.pid;

                player.x = data.x;  // Assegnamo il valore ad una variabile player
                player.y = data.y;
                break;
            
            // Caso nemico
            case ENEMY:
                if (enemy[id].y >= 2 && enemy[id].y <= MAX_Y) 
                    deleteSprite(enemy[id]);

                if (enemy[id].pid < 0)
                    enemy[id].pid = data.pid;

                enemy[id].x = data.x; // Aggiorniamo l'array dei nemici con i valori del nemico attuale
                enemy[id].y = data.y;
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
                if (checkCollision(enemyRockets[id], player)){
                    kill(enemyRockets[id].pid, 1);
                    mvaddch(enemyRockets[id].y, enemyRockets[id].x, ' ');
                    enemyRockets[id] = resetItem();
                    player.lives--;
                    score -= 500;
                }
                if (enemyRockets[id].x > -1)
                    mvaddch(enemyRockets[id].y, enemyRockets[id].x, enemyRockets[id].identifier);
                break;
        }

        mvprintw(0, 0, "Vite: %d", player.lives);
        mvprintw(0, MAX_X - 15, "Score: %d", score);
        refresh(); 
	} while (!collision && player.lives > 0);

    for (i=0; i<ENEMIES; i++)
        if (enemyRockets[i].pid > 0) kill(enemyRockets[i].pid, 1);

    for (i=0; i<ENEMIES; i++){
        if (enemy[i].pid > 0) kill(enemy[i].pid, 1);
        if (enemyRockets[i].pid > 0) kill(enemyRockets[i].pid, 1);
    }


    for (i=0; i<MAX_ROCKET; i++){
        if (rocketUp[i].pid > 0) kill(rocketUp[i].pid, 1);
        if (rocketDown[i].pid > 0) kill(rocketDown[i].pid, 1);
    }

    if (player.pid > 0) kill(player.pid, 1);

}
