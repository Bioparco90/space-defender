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

    // Inizializzazione vite e pid del giocatore e delle navi nemiche
    // player.lives = 3;
    // player.pid = -1;
    // for (i=0; i<ENEMIES; i++){
    //     enemy[i].lives = 3;
    //     enemy[i].pid = -1;
    // }
        
    // system("aplay sounds/gameplay.wav 2> /dev/null &");     // Prima riproduzione sottofondo musicale
    // clock_gettime(CLOCK_REALTIME, &start);                  // Rilevazione iniziale tempo (sottofondo)

    do{
        
        // Area giocatore
        if (tmpPlayer.y != player.y){
            pthread_mutex_lock(&mutex);
            deleteSprite(tmpPlayer);
            pthread_mutex_unlock(&mutex);

            tmpPlayer = player;

            pthread_mutex_lock(&mutex);
            printSprite(tmpPlayer.x, tmpPlayer.y, 3, 3, playerSprite);
            pthread_mutex_unlock(&mutex);
        }

        // Area razzi giocatore
        for (i=0; i<MAX_ROCKET; i++){
            if (tmpRocketUp[i].x != rocketUp[i].x || tmpRocketUp[i].y != rocketUp[i].y){
                pthread_mutex_lock(&mutex);
                mvaddch(tmpRocketUp[i].y, tmpRocketUp[i].x, ' ');
                pthread_mutex_unlock(&mutex);

                tmpRocketUp[i] = rocketUp[i];

                pthread_mutex_lock(&mutex);
                rocketAnimation(tmpRocketUp[i].x, tmpRocketUp[i].y);
                pthread_mutex_unlock(&mutex);
            }
        }

        for (i=0; i<MAX_ROCKET; i++){
            if (tmpRocketDown[i].x != rocketDown[i].x || tmpRocketDown[i].y != rocketDown[i].y){
                pthread_mutex_lock(&mutex);
                mvaddch(tmpRocketDown[i].y, tmpRocketDown[i].x, ' ');
                pthread_mutex_unlock(&mutex);

                tmpRocketDown[i] = rocketDown[i];

                pthread_mutex_lock(&mutex);
                rocketAnimation(tmpRocketDown[i].x, tmpRocketDown[i].y);
                pthread_mutex_unlock(&mutex);
            }
        }
        
        // Area nemici
        for (i=0; i<ENEMIES; i++){
            if (enemy[i].lives && (tmpEnemy[i].y != enemy[i].y || tmpEnemy[i].x != enemy[i].x)){
                pthread_mutex_lock(&mutex);
                deleteSprite(tmpEnemy[i]);
                pthread_mutex_unlock(&mutex);

                tmpEnemy[i] = enemy[i];

                pthread_mutex_lock(&mutex);
                printSprite(tmpEnemy[i].x, tmpEnemy[i].y, 3, 3, enemySpriteLv1);
                pthread_mutex_unlock(&mutex);
            }
        }

        // Area razzi nemici
        for (i=0; i<ENEMIES; i++){
            if (tmpEnemyRocket[i].x != enemyRocket[i].x){
                pthread_mutex_lock(&mutex);
                mvaddch(tmpEnemyRocket[i].y, tmpEnemyRocket[i].x, ' ');
                pthread_mutex_unlock(&mutex);

                tmpEnemyRocket[i] = enemyRocket[i];

                pthread_mutex_lock(&mutex);
                mvaddch(tmpEnemyRocket[i].y, tmpEnemyRocket[i].x, ROCKET);
                pthread_mutex_unlock(&mutex);
            }
        }

        printLives(player.lives);
        mvprintw(0, MAX_X - 15, "Score: %d", score);
        refresh(); 

        // Rilevazione ciclica del tempo trascorso. 
        // Se sono passati almeno 23 secondi, il sottofondo musicale viene riprodotto dall'inizio
        // clock_gettime(CLOCK_REALTIME, &end);
        // if (end.tv_sec - start.tv_sec >= 23){
        //     system("aplay sounds/gameplay.wav 2> /dev/null &");
        //     start = end;
        // }

        // Se i nemici sono stati tutti eliminati, il giocatore ha vinto
        if (!enemyCounter)
            gameResult = WIN;
    } while (!gameResult && player.lives);
    // Loop di gioco
	// do{
    //     read(mainPipe, &data, sizeof(data));    // Lettura ciclica del dato dalla mainPipe
    //     id = data.serial;                       // Assegno il serial ad una variabile d'appoggio

    //     switch(data.identifier){                                            // Valutazione del dato estratto dalla pipe
    //         case PLAYER:                                                    // Caso nave giocatore
    //             if (player.y >= 2 && player.y <= MAX_Y - 1)                 // Controllo posizione
    //                deleteSprite(player);                                    // Eliminazione sprite dallo schermo
                
    //             if (player.pid < 0)                                         // Controllo primo assegnamento pid
    //                 player.pid = data.pid;                                  // Assegnamento pid

    //             player.x = data.x;                                          // Assegnamento coordinata asse X del giocatore
    //             player.y = data.y;                                          // Assegnamento coordinata asse Y del giocatore
    //             break;
            
    //         case ENEMY:                                                     // Caso nave nemica
    //             if (enemy[id].y >= 2 && enemy[id].y <= MAX_Y)               // Controllo posizione
    //                 deleteSprite(enemy[id]);                                // Eliminazione sprite dallo schermo

    //             if (enemy[id].pid < 0)                                      // Controllo primo assegnamento pid
    //                 enemy[id].pid = data.pid;                               // Assegnamento pid

    //             enemy[id].x = data.x;                                       // Aggiornamento dell'array dei nemici con i valori del nemico attuale (coordinate X)
    //             enemy[id].y = data.y;                                       // Aggiornamento dell'array dei nemici con i valori del nemico attuale (coordinate Y)

    //             if (enemy[id].x <= 2)                                       // Controllo raggiungimento colonna giocatore
    //                 player.lives = 0;                                       // Sconfitta del giocatore
    //             break;

    //         // Area razzi. I controlli sono quasi uguali per tutti i casi
    //         case ROCKET_UP:                                                 // Caso razzo giocatore superiore
    //             if (rocketUp[id].y >= 1 && rocketUp[id].y <= MAX_Y+1)       // Controllo posizione
    //                 mvaddch(rocketUp[id].y, rocketUp[id].x, ' ');           // Cancellazione carattere dallo schermo

    //             rocketUp[id] = data;                                        // Aggiornamento array razzi diagonali superiori

    //             if(rocketUp[id].x >= MAX_X){                                // Verifica che il razzo non sia uscito dall'area di gioco 
    //                 kill(rocketUp[id].pid, 1);                              // Terminazione del processo del razzo
    //                 rocketUp[id] = resetItem();                             // Reset dei valori dell'array alla posizione del razzo appena eliminato
    //             }
    //             break;

    //         case ROCKET_DOWN:                                               // Caso razzo giocatore inferiore
    //             if (rocketDown[id].y >= 1 && rocketDown[id].y <= MAX_Y+1)   
    //                 mvaddch(rocketDown[id].y, rocketDown[id].x, ' ');       

    //             rocketDown[id] = data;                                      // Aggiornamento array razzi diagonali inferiori
                
    //             if (rocketDown[id].x >= MAX_X){                              
    //                 kill(rocketDown[id].pid, 1);                            
    //                 rocketDown[id] = resetItem();                           
    //             }
    //             break;

    //         case ENEMY_ROCKET:                                              // Caso razzo nemico
    //             if (enemyRockets[id].x > -1)                                
    //                 mvaddch(enemyRockets[id].y, enemyRockets[id].x, ' ');   
                
    //             enemyRockets[id] = data;                                    // Aggiornamento array razzi nemici

    //             if (enemyRockets[id].x <= 0){                               
    //                 kill(enemyRockets[id].pid, 1);                          
    //                 enemyRockets[id] = resetItem();                         // Reset dei valori dell'array alla posizione del razzo appena eliminato
    //             }
    //             break;
    //     }

    //     // Stampe e controlli di gioco
    //     // Giocatore e nemico hanno diversi colori, ma mentre quello del giocatore è fisso, 
    //     // il nemico cambia colore e aspetto a seconda delle vite a disposizione
    //     // attron/off: gestione della coppia di colori corrispondente al numero passato alla color_pair (rif. righe 45->49)
    //     // printSprite: stampa la sprite corrispondente al tipo di nave e/o alle vite rimanenti
    //     // deleteSprite: elimina la sprite 
    //     switch(data.identifier){

    //         // Area navi 
    //         case PLAYER:
    //             attron(COLOR_PAIR(3));
    //             printSprite(data.x, data.y, 3,3,  playerSprite);
    //             attroff(COLOR_PAIR(3));
    //             break;

    //         case ENEMY:
    //             if (enemy[id].lives == 3){
    //                 attron(COLOR_PAIR(1));
    //                 printSprite(data.x, data.y, 3, 3, enemySpriteLv1);
    //                 attroff(COLOR_PAIR(1));
    //             }
    //             else if(enemy[id].lives == 2){
    //                 attron(COLOR_PAIR(4));
    //                 printSprite(data.x, data.y, 3 ,3, enemySpriteLv2);
    //                 attroff(COLOR_PAIR(4));
    //             }
    //             else {
    //                 attron(COLOR_PAIR(4));
    //                 printSprite(data.x, data.y, 3,3 , enemySpriteLv2Damaged);
    //                 attroff(COLOR_PAIR(4));
    //             }    
    //             break;

    //         // Area razzi giocatore. I controlli dei razzi giocatore sono pressocchè uguali. 
    //         // Cambia l'array di riferimento a seconda che si tratti del razzo superiore o inferiore
    //         case ROCKET_UP:
    //             for (i=0; i<ENEMIES; i++){                          // Il ciclo compara il razzo ad ogni nemico presente nell'array
    //                 if (checkCollision(rocketUp[id], enemy[i])){    // Controllo collisione del razzo con la nave nemica corrente 
    //                     kill(rocketUp[id].pid, 1);                  // Terminazione processo razzo
    //                     rocketUp[id] = resetItem();                 // Reset dei valori dell'array alla posizione del razzo appena eliminato
    //                     enemy[i].lives--;                           // Il nemico colpito perde una vita

    //                     // Controllo delle vite della nave nemica
    //                     switch (enemy[i].lives){                
    //                         case 0:                                             // La nave nemica ha finito le vite
    //                             if (enemyRockets[i].pid > 0)                    // Verifica che la nave nemica non abbia processi figli in vita 
    //                                     waitpid(enemyRockets[i].pid, NULL, 0);  // Attende la terminazione degli eventuali processi figli
    //                             kill(enemy[i].pid, 1);                          // Terminazione processo nave nemica
    //                             deleteSprite(enemy[i]);                         // Eliminazione della sprite della nave dall'area di gioco
    //                             mvaddch(rocketUp[id].y, rocketUp[id].x, ' ');   // Eliminazione del razzo dall'area di gioco
    //                             enemy[i] = resetItem();                         // Reset dei valori dell'array alla posizione del nemico appena eliminato
    //                             score += 300;                                   // Incremento del punteggio del giocatore
    //                             enemyCounter--;                                 // Decremento del contatore nemici in vita
    //                             break;

    //                         case 1: 
    //                             deleteSprite(enemy[i]);
    //                             attron(COLOR_PAIR(4));
    //                             printSprite(enemy[i].x, enemy[i].y, 3, 3, enemySpriteLv2Damaged);
    //                             attroff(COLOR_PAIR(4));
    //                             score += 100;
    //                             break;
                            
    //                         case 2:
    //                             deleteSprite(enemy[i]);
    //                             attron(COLOR_PAIR(4));
    //                             printSprite(enemy[i].x, enemy[i].y, 3, 3, enemySpriteLv2);
    //                             attroff(COLOR_PAIR(4));
    //                             score += 100;
    //                             break;
    //                     }
    //                 }
    //             }
    //             if (rocketUp[id].y > -1)
    //                 rocketAnimation(rocketUp[id].x,rocketUp[id].y); // Gestore dell'animazione del razzo giocatore
    //             break;
            
    //         case ROCKET_DOWN:
    //             for (i=0; i<ENEMIES; i++){
    //                 if (checkCollision(rocketDown[id], enemy[i])){
    //                     kill(rocketDown[id].pid, 1);
    //                     rocketDown[id] = resetItem();
    //                     enemy[i].lives--;
                        
    //                     switch (enemy[i].lives){
    //                         case 0:
    //                             if (enemyRockets[i].pid > 0)
    //                                     waitpid(enemyRockets[i].pid, NULL, 0);
    //                             kill(enemy[i].pid, 1);
    //                             deleteSprite(enemy[i]);
    //                             mvaddch(rocketDown[id].y, rocketDown[id].x, ' ');
    //                             enemy[i] = resetItem();
    //                             score += 300;
    //                             enemyCounter--;
    //                             break;

    //                         case 1: 
    //                             deleteSprite(enemy[i]);
    //                             attron(COLOR_PAIR(4));
    //                             printSprite(enemy[i].x, enemy[i].y, 3, 3, enemySpriteLv2Damaged);
    //                             attroff(COLOR_PAIR(4));
    //                             score += 100;
    //                             break;
                            
    //                         case 2:
    //                             deleteSprite(enemy[i]);
    //                             attron(COLOR_PAIR(4));
    //                             printSprite(enemy[i].x, enemy[i].y, 3, 3, enemySpriteLv2);
    //                             attroff(COLOR_PAIR(4));
    //                             score += 100;
    //                             break;
    //                     }                        
    //                 }
    //             }
    //             if (rocketDown[id].y > -1)
    //                 rocketAnimation(rocketDown[id].x,rocketDown[id].y);
    //             break;

    //         // Razzi nemici
    //         case ENEMY_ROCKET:  
    //             if (checkCollision(enemyRockets[id], player)){              // Verifica collisione razzo nemico - nave giocatore
    //                 kill(enemyRockets[id].pid, 1);                          // Terminazione processo razzo
    //                 mvaddch(enemyRockets[id].y, enemyRockets[id].x, ' ');   // Cancellazione carattere razzo
    //                 enemyRockets[id] = resetItem();                         // Reset dei valori dell'array alla posizione del razzo appena eliminato
    //                 player.lives--;                                         // Decremento delle vite del giocatore
    //                 score -= 500;                                           // Decremento del punteggio del giocatore
    //             }
    //             if (enemyRockets[id].x > -1)
    //                 mvaddch(enemyRockets[id].y, enemyRockets[id].x, enemyRockets[id].identifier);   // Rappresentazione del razzo nell'area di gioco
    //             break;
    //     }

    //     // Stampa delle vite del giocatore e del punteggio nella parte alta dello schermo 
    //     printLives(player.lives);
    //     mvprintw(0, MAX_X - 15, "Score: %d", score);
    //     refresh(); 

    //     // Rilevazione ciclica del tempo trascorso. 
    //     // Se sono passati almeno 23 secondi, il sottofondo musicale viene riprodotto dall'inizio
    //     clock_gettime(CLOCK_REALTIME, &end);
    //     if (end.tv_sec - start.tv_sec >= 23){
    //         system("aplay sounds/gameplay.wav 2> /dev/null &");
    //         start = end;
    //     }

    //     // Se i nemici sono stati tutti eliminati, il giocatore ha vinto
    //     if (!enemyCounter)
    //         gameResult = WIN;
	// } while (!gameResult && player.lives);

    // Terminazione processi. Se ci sono altri processi relativi a navi giocatore, nemiche o razzi, essi vengono terminati.  
    // for (i=0; i<ENEMIES; i++){
    //     if (enemyRockets[i].pid > 0) kill(enemyRockets[i].pid, 1);  // Terminazione processi razzi nemici
    //     if (enemy[i].pid > 0) kill(enemy[i].pid, 1);                // Terminazione processi navi nemiche (avviene dopo i razzi in modo da non lasciare i processi orfani)
    // }

    // // Terminazione processi razzi giocatore
    // for (i=0; i<MAX_ROCKET; i++){
    //     if (rocketUp[i].pid > 0) kill(rocketUp[i].pid, 1);
    //     if (rocketDown[i].pid > 0) kill(rocketDown[i].pid, 1);
    // }

    // if (player.pid > 0) kill(player.pid, 1);    // Terminazione processo nave giocatore (avviene dopo i razzi in modo da non lascia processi orfani)

    // system("killall aplay");        // Terminazione della riproduzione del sottofondo musicale
    gameOver(gameResult, score);    // Chiamata a gestore della schermata di fine gioco
}
