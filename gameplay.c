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
    int playerLives=3;
    int allEnemies=ENEMIES;
    int score=0;
    int id;
    int i;
    

    // Loop di gioco
	do{
        if(allEnemies==0){
            collision=1;
        }
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

            // Caso bombe nemiche
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
                if(checkCollision(enemy[id],player)){
                        collision=2;               
                }   
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
                        score+=100;
                        allEnemies--;
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
                        score+=100;
                        allEnemies--;
                    }
                }
                if (rocketDown[id].y > -1)
                    mvaddch(rocketDown[id].y,rocketDown[id].x, ROCKET);
                break;

            case BOMB:
                if(checkCollision(bomb[id], player)){            
                    playerLives--;
                    kill(bomb[id].pid,1);
                    bomb[id]=resetItem();
                    if(playerLives==0){
                        kill(player.pid,1);
                        deleteSprite(player);
                        player=resetItem();
                        collision=2;
                    }
                }
                if (bomb[id].x >  -1)
                    mvaddch(bomb[id].y,bomb[id].x,BOMB);
                break;

        }

        mvprintw(0, 0, "Vite: %d   Score: %d", playerLives, score);
        refresh(); 
	} while (!collision);

    clear();
    gameOver(score,collision);
    getch();
}

void gameOver(int score, int collision){
    if(collision==1){
        mvprintw(MAX_Y/2,MAX_X/2-10,"V I T T O R I A");
        mvprintw(MAX_Y/2+1,MAX_X/2-18,"Ecco il tuo punteggio finale: %d", score);
    }
    else if(collision==2){
        mvprintw(MAX_Y/2,MAX_X/2-10,"S E I  M O R T O");
        mvprintw(MAX_Y/2+1,MAX_X/2-8,"Punteggio: %d",score);
    }
}   