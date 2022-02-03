#include "global.h"
#include <pthread.h>

char playerSprite[3][3]={
         {"/\\ "},
         {"[]>"},
         {"\\/ "}
 };

char enemySpriteLv1[3][3]={
         {" / "},
         {"(o)"},
         {" \\ "}
 };

char enemySpriteLv2[3][3]={
        {"{|="},
        {"(o)"},
        {"{|="}
};

char enemySpriteLv2Damaged[3][3]={
    {" |="},
    {"(o)"},
    {" |="}
};

// Funzione gestore di gioco
void gameArea(){
    // Variabili d'appoggio salvataggio oggetti
    Object data;
    Object tmpPlayer;
    Object tmpEnemy[ENEMIES];
    Object tmpRocketUp[MAX_ROCKET];
    Object tmpRocketDown[MAX_ROCKET];
    Object tmpEnemyRocket[ENEMIES];

    // Variabili di gestione gioco
    enemyCounter = ENEMIES;
	int gameResult = DEFEAT;
    int score = 0;
    int id;
    int i;

    // Variabili gestione sottofondo musicale
    struct timespec start, end;

    // Inizializzazione colori
    init_pair(1,COLOR_RED,COLOR_BLACK);         // Navi primo livello e sconfitta
    init_pair(2,COLOR_GREEN,COLOR_BLACK);       // Vite
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);      // Nave giocatore e vittoria e stelle
    init_pair(4,COLOR_MAGENTA,COLOR_BLACK);     // Nemici secondo livello
    init_pair(5,COLOR_BLACK,COLOR_WHITE);       // Menù iniziale
        
    system("aplay sounds/gameplay.wav 2> /dev/null &");     // Prima riproduzione sottofondo musicale
    clock_gettime(CLOCK_REALTIME, &start);                  // Rilevazione iniziale tempo (sottofondo)

//-------------------------------------------------------------------------
    // do{
    //     data = extract();
    //     id = data.serial;

    //     switch(data.identifier){
    //         case PLAYER:
    //             if (tmpPlayer.y >= 2 && tmpPlayer.y <= MAX_Y - 1)
    //                 deleteSprite(tmpPlayer);
                
    //             tmpPlayer.x = data.x;
    //             tmpPlayer.y = data.y;
    //             break;

    //         case ENEMY:
    //             if (tmpEnemy[id].y >= 2 && tmpEnemy[id].y <= MAX_Y)
    //                 deleteSprite(tmpEnemy[id]);

    //             tmpEnemy[id].x = data.x;
    //             tmpEnemy[id].y = data.y;

    //             if (tmpEnemy[id].x <= 2)
    //                 player.lives = 0;
    //             break;

    //         case ROCKET_UP:
    //             if (tmpRocketUp[id].y >= 1 && tmpRocketUp[id].y <= MAX_Y+1)
    //                 mvaddch(tmpRocketUp[id].y, tmpRocketUp[id].x, ' ');

    //             tmpRocketUp[id] = data;

    //             if (tmpRocketUp[id].x >= MAX_X)
    //                 rocketUp[id].lives--;

    //             break;

    //         case ROCKET_DOWN:
    //             if (tmpRocketDown[id].y >= 1 && tmpRocketDown[id].y <= MAX_Y+1)
    //                 mvaddch(tmpRocketDown[id].y, tmpRocketDown[id].x, ' ');

    //             tmpRocketDown[id] = data;

    //             if (tmpRocketDown[id].x >= MAX_X)
    //                 rocketDown[id].lives--;
                    
    //             break;

    //         case ENEMY_ROCKET:
    //             if (tmpEnemyRocket[id].x > -1)
    //                 mvaddch(tmpEnemyRocket[id].y, tmpEnemyRocket[id].x, ' ');

    //             tmpEnemyRocket[id] = data;

    //             if (tmpEnemyRocket[id].x <= 0)
    //                 enemyRocket[id].lives--;
    //             break;
    //     }

    //     switch (data.identifier){
    //         case PLAYER:
    //             printSprite(data.x, data.y, 3, 3, playerSprite);
    //             break;

    //         case ENEMY:
    //             printSprite(data.x, data.y, 3, 3, enemySpriteLv1);
    //             break;

    //         case ROCKET_UP:
    //             for (i=0; i<ENEMIES; i++){
    //                 // -------------------
    //             }
    //             if (tmpRocketUp[id].y > -1)
    //                 rocketAnimation(tmpRocketUp[id].x, tmpRocketUp[id].y);
    //             break;

    //         case ROCKET_DOWN:
    //             if (tmpRocketDown[id].y > -1)
    //                 rocketAnimation(tmpRocketDown[id].x, tmpRocketDown[id].y);
    //             break;

    //         case ENEMY_ROCKET:
    //             if (tmpEnemyRocket[id].x > -1)
    //                 mvaddch(tmpEnemyRocket[id].y, tmpEnemyRocket[id].x, ENEMY_ROCKET);
    //             break;

    //     }
        


    //     printLives(player.lives);
    //     mvprintw(0, MAX_X - 15, "Score: %d", score);
    //     refresh(); 

    //     // Rilevazione ciclica del tempo trascorso. 
    //     // Se sono passati almeno 23 secondi, il sottofondo musicale viene riprodotto dall'inizio
    //     // clock_gettime(CLOCK_REALTIME, &end);
    //     // if (end.tv_sec - start.tv_sec >= 23){
    //     //     system("aplay sounds/gameplay.wav 2> /dev/null &");
    //     //     start = end;
    //     // }

    //     // Se i nemici sono stati tutti eliminati, il giocatore ha vinto
    //     if (!enemyCounter)
    //         gameResult = WIN;
    // } while (!gameResult && player.lives);
//-----------------------------------------------------------------------------------

    //Loop di gioco
	do{
        data = extract();    // Lettura ciclica del dato dalla mainPipe
        id = data.serial;                       // Assegno il serial ad una variabile d'appoggio

        switch(data.identifier){                                            // Valutazione del dato estratto dalla pipe
            case PLAYER:                                                    // Caso nave giocatore
                if (tmpPlayer.y >= 2 && tmpPlayer.y <= MAX_Y - 1)                 // Controllo posizione
                   deleteSprite(tmpPlayer);                                    // Eliminazione sprite dallo schermo
                
                tmpPlayer.x = data.x;                                          // Assegnamento coordinata asse X del giocatore
                tmpPlayer.y = data.y;                                          // Assegnamento coordinata asse Y del giocatore
                break;
            
            case ENEMY:                                                     // Caso nave nemica
                if (tmpEnemy[id].y >= 2 && tmpEnemy[id].y <= MAX_Y)               // Controllo posizione
                    deleteSprite(tmpEnemy[id]);                                // Eliminazione sprite dallo schermo

                tmpEnemy[id].x = data.x;                                       // Aggiornamento dell'array dei nemici con i valori del nemico attuale (coordinate X)
                tmpEnemy[id].y = data.y;                                       // Aggiornamento dell'array dei nemici con i valori del nemico attuale (coordinate Y)

                if (tmpEnemy[id].x <= 2)                                       // Controllo raggiungimento colonna giocatore
                    player.lives = 0;                                       // Sconfitta del giocatore
                break;

            // Area razzi. I controlli sono quasi uguali per tutti i casi
            case ROCKET_UP:                                                 // Caso razzo giocatore superiore
                if (tmpRocketUp[id].y >= 1 && tmpRocketUp[id].y <= MAX_Y+1)       // Controllo posizione
                    mvaddch(tmpRocketUp[id].y, tmpRocketUp[id].x, ' ');           // Cancellazione carattere dallo schermo

                tmpRocketUp[id] = data;                                        // Aggiornamento array razzi diagonali superiori

                if(tmpRocketUp[id].x >= MAX_X)                              // Verifica che il razzo non sia uscito dall'area di gioco 
                    rocketUp[id].lives--;
                break;

            case ROCKET_DOWN:                                               // Caso razzo giocatore inferiore
                if (tmpRocketDown[id].y >= 1 && tmpRocketDown[id].y <= MAX_Y+1)   
                    mvaddch(tmpRocketDown[id].y, tmpRocketDown[id].x, ' ');       

                tmpRocketDown[id] = data;                                      // Aggiornamento array razzi diagonali inferiori
                
                if (rocketDown[id].x >= MAX_X)                              
                    rocketDown[id].lives--;                         
                
                break;

            case ENEMY_ROCKET:                                              // Caso razzo nemico
                if (tmpEnemyRocket[id].x > -1)                                
                    mvaddch(tmpEnemyRocket[id].y, tmpEnemyRocket[id].x, ' ');   
                
                tmpEnemyRocket[id] = data;                                    // Aggiornamento array razzi nemici

                if (tmpEnemyRocket[id].x <= 0)                               
                    enemyRocket[id].lives--;                         // Reset dei valori dell'array alla posizione del razzo appena eliminato
                break;
        }

        // Stampe e controlli di gioco
        // Giocatore e nemico hanno diversi colori, ma mentre quello del giocatore è fisso, 
        // il nemico cambia colore e aspetto a seconda delle vite a disposizione
        // attron/off: gestione della coppia di colori corrispondente al numero passato alla color_pair (rif. righe 45->49)
        // printSprite: stampa la sprite corrispondente al tipo di nave e/o alle vite rimanenti
        // deleteSprite: elimina la sprite 
        switch(data.identifier){

            // Area navi 
            case PLAYER:
                attron(COLOR_PAIR(3));
                printSprite(data.x, data.y, 3,3,  playerSprite);
                attroff(COLOR_PAIR(3));
                break;

            case ENEMY:
                if (enemy[id].lives == 3){
                    attron(COLOR_PAIR(1));
                    printSprite(data.x, data.y, 3, 3, enemySpriteLv1);
                    attroff(COLOR_PAIR(1));
                }
                else if(enemy[id].lives == 2){
                    attron(COLOR_PAIR(4));
                    printSprite(data.x, data.y, 3 ,3, enemySpriteLv2);
                    attroff(COLOR_PAIR(4));
                }
                else if (enemy[id].lives == 1){
                    attron(COLOR_PAIR(4));
                    printSprite(data.x, data.y, 3,3 , enemySpriteLv2Damaged);
                    attroff(COLOR_PAIR(4));
                }    
                break;

            // Area razzi giocatore. I controlli dei razzi giocatore sono pressocchè uguali. 
            // Cambia l'array di riferimento a seconda che si tratti del razzo superiore o inferiore
            case ROCKET_UP:
                for (i=0; i<ENEMIES; i++){                          // Il ciclo compara il razzo ad ogni nemico presente nell'array
                    if (checkCollision(tmpRocketUp[id], tmpEnemy[i])){    // Controllo collisione del razzo con la nave nemica corrente 
                        mvaddch(tmpRocketUp[id].y, tmpRocketUp[id].x, ' ');
                        rocketUp[id].lives--;
                        enemy[i].lives--;                           // Il nemico colpito perde una vita

                        // Controllo delle vite della nave nemica
                        switch (enemy[i].lives){                
                            case 0:                                             // La nave nemica ha finito le vite
                                deleteSprite(tmpEnemy[i]);                         // Eliminazione della sprite della nave dall'area di gioco
                                // mvaddch(tmpRocketUp[id].y, tmpRocketUp[id].x, ' ');   // Eliminazione del razzo dall'area di gioco
                                score += 300;                                   // Incremento del punteggio del giocatore
                                enemyCounter--;                                 // Decremento del contatore nemici in vita
                                break;

                            case 1: 
                                deleteSprite(tmpEnemy[i]);
                                attron(COLOR_PAIR(4));
                                printSprite(tmpEnemy[i].x, tmpEnemy[i].y, 3, 3, enemySpriteLv2Damaged);
                                attroff(COLOR_PAIR(4));
                                score += 100;
                                break;
                            
                            case 2:
                                deleteSprite(tmpEnemy[i]);
                                attron(COLOR_PAIR(4));
                                printSprite(tmpEnemy[i].x, tmpEnemy[i].y, 3, 3, enemySpriteLv2);
                                attroff(COLOR_PAIR(4));
                                score += 100;
                                break;
                        }
                    }
                }
                if (tmpRocketUp[id].y > -1)
                    rocketAnimation(tmpRocketUp[id].x, tmpRocketUp[id].y); // Gestore dell'animazione del razzo giocatore
                break;
            
            case ROCKET_DOWN:
                for (i=0; i<ENEMIES; i++){
                    if (checkCollision(tmpRocketDown[id], tmpEnemy[i])){
                        mvaddch(tmpRocketDown[id].y, tmpRocketDown[id].x, ' ');
                        rocketDown[id].lives--;
                        enemy[i].lives--;
                        
                        switch (enemy[i].lives){
                            case 0:
                                deleteSprite(tmpEnemy[i]);
                                // mvaddch(tmpRocketDown[id].y, tmpRocketDown[id].x, ' ');
                                score += 300;
                                enemyCounter--;
                                break;

                            case 1: 
                                deleteSprite(tmpEnemy[i]);
                                attron(COLOR_PAIR(4));
                                printSprite(tmpEnemy[i].x, tmpEnemy[i].y, 3, 3, enemySpriteLv2Damaged);
                                attroff(COLOR_PAIR(4));
                                score += 100;
                                break;
                            
                            case 2:
                                deleteSprite(tmpEnemy[i]);
                                attron(COLOR_PAIR(4));
                                printSprite(tmpEnemy[i].x, tmpEnemy[i].y, 3, 3, enemySpriteLv2);
                                attroff(COLOR_PAIR(4));
                                score += 100;
                                break;
                        }                        
                    }
                }
                if (tmpRocketDown[id].y > -1)
                    rocketAnimation(tmpRocketDown[id].x, tmpRocketDown[id].y);
                break;

            // Razzi nemici
            case ENEMY_ROCKET:  
                if (checkCollision(tmpEnemyRocket[id], tmpPlayer)){              // Verifica collisione razzo nemico - nave giocatore
                    mvaddch(tmpEnemyRocket[id].y, tmpEnemyRocket[id].x, ' ');   // Cancellazione carattere razzo
                    tmpEnemyRocket[id].lives--;                         // Reset dei valori dell'array alla posizione del razzo appena eliminato
                    player.lives--;                                         // Decremento delle vite del giocatore
                    score -= 500;                                           // Decremento del punteggio del giocatore
                }
                if (tmpEnemyRocket[id].x > 0)
                    mvaddch(tmpEnemyRocket[id].y, tmpEnemyRocket[id].x, ENEMY_ROCKET);   // Rappresentazione del razzo nell'area di gioco
                break;
        }

        // Stampa delle vite del giocatore e del punteggio nella parte alta dello schermo 
        printLives(player.lives);
        mvprintw(0, MAX_X - 15, "Score: %d", score);
        refresh(); 

        // Rilevazione ciclica del tempo trascorso. 
        // Se sono passati almeno 23 secondi, il sottofondo musicale viene riprodotto dall'inizio
        clock_gettime(CLOCK_REALTIME, &end);
        if (end.tv_sec - start.tv_sec >= 23){
            system("aplay sounds/gameplay.wav 2> /dev/null &");
            start = end;
        }

        // Se i nemici sono stati tutti eliminati, il giocatore ha vinto
        if (!enemyCounter)
            gameResult = WIN;
	} while (!gameResult && player.lives);

    system("killall aplay");        // Terminazione della riproduzione del sottofondo musicale
    gameOver(gameResult, score);    // Chiamata a gestore della schermata di fine gioco
}
