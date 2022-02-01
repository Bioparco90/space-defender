#include "global.h"

// Funzione per l'inizializzazione e la gestione delle azioni della nave del giocatore
void* playerShip(){
    struct timespec time, checker;
    // Object ship;
    int serial;

    clock_gettime(CLOCK_REALTIME, &time);   // Rilevazione iniziale del tempo
    
    // Inizializzazione nave giocatore
    player.x = 2;
    player.y = MAX_Y / 2;
    player.identifier = PLAYER;
    player.serial = 777;
    
    serial = 0;                         // Inizializzazione serial razzi giocatore

    // Ciclo movimento e sparo
    while(player.lives && enemyCounter){
        int c = getch();

        switch (c){
            case KEY_UP:                // Movimento verso l'alto
                if(player.y > 2)          // Verifica collisione bordo
                    player.y -= 1;
                break;
            
            case KEY_DOWN:              // Movimento verso il basso
                if (player.y < MAX_Y-1)   // Verifica collisione bordo
                    player.y += 1;
                break;

            case ' ':                                                   // Sparo alla pressione del tasto "spazio"
                clock_gettime(CLOCK_REALTIME, &checker);                // Rilevazione del tempo trascorso dalla precedente rilevazione
                if (serial == 0){                                       // Primo sparo della partita
                    system("aplay sounds/laser.wav 2> /dev/null &");    // Riproduce suono sparo
                    playerShotInit(player.x, player.y, serial);     // Generazione sparo
                    serial++;                                           // Incremento del serial (verrà utilizzato come indice nella funzione gestore di gioco)
                    time = checker;                                     // Il valore dell'ultima rilevazione viene salvato come "rilevazione precedente"
                }
                if (checker.tv_sec - time.tv_sec >= 1){                 // Spari successivi al primo. Valuta se sia passato ALMENO un secondo dallo sparo precedente
                    system("aplay sounds/laser.wav 2> /dev/null &");    // Riproduce suono sparo
                    playerShotInit(player.x, player.y, serial);     // Generazione sparo
                    serial++;                                           // Incremento del serial (verrà utilizzato come indice nel gameLoop)
                    time = checker;                                     // Il valore dell'ultima rilevazione viene salvato come "rilevazione precedente"
                }
                break;
        }
        if(serial >= MAX_ROCKET)            // Verifica se sia stato raggiunto il limite di spari esistenti in uno stesso momento
            serial = 0;                     // L'indicizzazione riparte da 0
    }
}

// Funzione per l'inizializzazione degli spari diagonali
void playerShotInit(int x, int y, int serial){
    pthread_t thRocketUp[MAX_ROCKET], thRocketDown[MAX_ROCKET];

    ShotArgs argsUp = {x, y, DIR_UP, serial};
    ShotArgs argsDown = {x, y, DIR_DOWN, serial};

    if (pthread_create(&thRocketUp[serial], NULL, &shotUp, &argsUp)){
        endwin();
        exit;
    }

    if (pthread_create(&thRocketDown[serial], NULL, &shotDown, &argsDown)){
        endwin();
        exit;
    }
}

// Funzione per la gestione del movimento del singolo razzo
void* shotUp(void* param){
    ShotArgs* args;
    int id;
    int direction;
    
    args = (ShotArgs*) param;

    id = args->serial;
    direction = args->dir;

    rocketUp[id].x = args->x + 2;
   	rocketUp[id].y = args->y + 1 + args->dir;
    rocketUp[id].lives = 1;
    rocketUp[id].serial = args->serial;

    // Ciclo movimento razzo
    while(rocketUp[id].lives && rocketUp[id].x < MAX_X){
        if((rocketUp[id].y < 2 || rocketUp[id].y > MAX_Y)){     // Verifica collisione bordi
            direction *= -1;                                    // Cambio direzione
        }
        rocketUp[id].x += 1;                                    // Spostamento orizzontale
        rocketUp[id].y += direction;                            // Spostamento verticale, direzione dipendente dal valore di "direction"

		usleep(ROCKET_DELAY);                                   // Delay movimento razzo
    }
}

void* shotDown(void* param){
    ShotArgs* args;
    int id;
    int direction;
    
    args = (ShotArgs*) param;

    id = args->serial;
    direction = args->dir;

    rocketDown[id].x = args->x + 2;
   	rocketDown[id].y = args->y + 1 + args->dir;
    rocketDown[id].lives = 1;
    rocketDown[id].serial = args->serial;

    // Ciclo movimento razzo
    while(rocketDown[id].lives && rocketDown[id].x < MAX_X){
        if((rocketDown[id].y < 2 || rocketDown[id].y > MAX_Y)){     // Verifica collisione bordi
            direction *= -1;                                        // Cambio direzione
        }
        rocketDown[id].x += 1;                                      // Spostamento orizzontale
        rocketDown[id].y += direction;                              // Spostamento verticale, direzione dipendente dal valore di "direction"

		usleep(ROCKET_DELAY);                                       // Delay movimento razzo
    }
}
