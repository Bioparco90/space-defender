#include "global.h"
#include <curses.h>

// Funzione per l'inizializzazione, la generazione e il posizionamento iniziale delle navi nemiche.
void fleetEnlister(){
    pthread_t thEnemyShip[ENEMIES];
    Args arg[ENEMIES];
    int posX = MAX_X - 2;
    int posY = 2;
    int i;

    for (i=0; i<ENEMIES; i++){
        arg[i].x = posX;
        arg[i].y = posY;
        arg[i].serial = i;
        if (pthread_create(&thEnemyShip[i], NULL, &enemyShip, &arg[i])){
            endwin();
            exit(1);
        }
        posY += 4;
        if (posY >= MAX_Y - 1){
            posX -= 8;
            posY = 2;
        }
    }
}

// Funzione per la gestione delle azioni della singola nave nemica
void* enemyShip(void* param){
    int direction = 1;              // Direzione nave: 1 -> basso, -1 -> alto
    int flag = VERTICAL;            // Flag da sfruttare per gestire il movimento verticale
    struct timespec start, end;     // Utilizzate per scandire il tempo minimo tra uno sparo e l'altro
    struct timespec start2, end2;   // Utilizzate come countdown per il cambio di velocità del movimento nemico
    int timeTravel;                 // Tempo massimo di percorrenza dell'area di gioco
    int delay = ENEMY_DELAY;        // Ritardo movimento navi
    int id;

    pthread_t thEnemyShot;
    Args shotArg;
    Args* arg;
    arg = (Args*) param;

    id = arg->serial;
    pthread_mutex_lock(&mutex);
    enemy[id].x = arg->x;
    enemy[id].y = arg->y;
    enemy[id].serial = arg->serial;
    pthread_mutex_unlock(&mutex);

    // Rilevazioni iniziali del tempo
    clock_gettime(CLOCK_REALTIME, &start);                  // Rilevazione per il tempo degli spari
    clock_gettime(CLOCK_REALTIME, &start2);                 // Rilevazione per l'incremento della velocità delle navi
    timeTravel = timeTravelEnemyRocket(ROCKET_DELAY);       // Valutazione tempo massimo di percorrenza di uno sparo

    while(enemy[id].lives){ // Loop movimento nave nemica
        switch (flag){
            case VERTICAL:                                  // Movimento verticale
                arg->y += direction;                       // Aggiornamento coordinata Y
                if (arg->y <= 2 || arg->y > MAX_Y - 1)    // Verifica collisione bordi
                    flag = HORIZONTAL;                      // Eventuale modifica del valore flag, che ci manderà al movimento orizzontale
                break;

            case HORIZONTAL:        // Movimento orizzontale
                arg->x -= 4;   // Aggiornamento coordinata X
                direction *= -1;    // Rimbalzo sul bordo
                flag = VERTICAL;    // Settiamo la flag per tornare al movimento verticale
                break;
        }

        pthread_mutex_lock(&mutex);
        enemy[id].x = arg->x;
        enemy[id].y = arg->y;
        enemy[id].serial = arg->serial;
        pthread_mutex_unlock(&mutex);

        // Sparo nemico 
        clock_gettime(CLOCK_REALTIME, &end);                            // Rilevazione del tempo trascorso dalla precedente rilevazione (spari)
        if (end.tv_sec - start.tv_sec >= timeTravel + 1){               // Controllo sul tempo trascorso dall'ultimo sparo
            if (rand()%ENEMIES == enemy[id].serial){                        // Generazione numero casuale. Se uguale al serial della nave, può sparare
                shotArg.x = enemy[id].x;
                shotArg.y = enemy[id].y;
                shotArg.serial = enemy[id].serial;
                if (pthread_create(&thEnemyShot, NULL, &enemyShot, &shotArg)){
                    endwin();
                    exit(1);
                }
                start = end;                                            // Il valore dell'ultima rilevazione viene salvato come "rilevazione precedente"
            }
        }

        clock_gettime(CLOCK_REALTIME, &end2);       // Rilevazione del tempo trascorso dalla precedente rilevazione (movimento)
        if (end2.tv_sec - start2.tv_sec >= 7){      // Controllo sul tempo trascorso dall'ultimo incremento della velocità
            delay *= 0.8;                           // Incremento della velocità di movimento: 20%
            start2 = end2;                          // Il valore dell'ultima rilevazione viene salvato come "rilevazione precedente"
        }
        usleep(delay);                              // Ritardo movimento navi
    }
    return NULL;
}

// Funzione per la gestione del movimento del razzo nemico
void* enemyShot(void* param){
    Args* arg;
    int id;
    
    arg = (Args*) param;
    id = arg->serial;

    // Inizializzazione razzo
    enemyRocket[id].x = arg->x - 2;
    enemyRocket[id].y = arg->y + 1;
    enemyRocket[id].serial = arg->serial;

    while (enemyRocket[id].lives && enemyRocket[id].x > 0){                               // Loop movimento razzo
        enemyRocket[id].x -= 1;                            // Movimento orizzontale
        usleep(ROCKET_DELAY);                   // Ritardo movimento
    }
    return NULL;
}