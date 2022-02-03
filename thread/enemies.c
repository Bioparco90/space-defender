#include "global.h"
#include <curses.h>

// Funzione per l'inizializzazione, la generazione e il posizionamento iniziale delle navi nemiche.
void fleetEnlister(){
    pthread_t thEnemyShip[ENEMIES];
    Args arg;
    int posX = MAX_X - 2;
    int posY = 2;
    int i;

    for (i=0; i<ENEMIES; i++){
        arg.x = posX;
        arg.y = posY;
        arg.serial = i;
        if (pthread_create(&thEnemyShip[i], NULL, &enemyShip, &arg)){
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
    Object ship;
    Args shotArg;
    Args* arg;
    arg = (Args*) param;

    id = arg->serial;
    ship.x = arg->x;
    ship.y = arg->y;
    ship.identifier = ENEMY;
    ship.serial = arg->serial;

    // Rilevazioni iniziali del tempo
    clock_gettime(CLOCK_REALTIME, &start);                  // Rilevazione per il tempo degli spari
    clock_gettime(CLOCK_REALTIME, &start2);                 // Rilevazione per l'incremento della velocità delle navi
    timeTravel = timeTravelEnemyRocket(ROCKET_DELAY);       // Valutazione tempo massimo di percorrenza di uno sparo

    insert(ship);
    while(enemy[id].lives){ // Loop movimento nave nemica
        switch (flag){
            case VERTICAL:                                  // Movimento verticale
                ship.y += direction;                       // Aggiornamento coordinata Y
                if (ship.y <= 2 || ship.y > MAX_Y - 1)    // Verifica collisione bordi
                    flag = HORIZONTAL;                      // Eventuale modifica del valore flag, che ci manderà al movimento orizzontale
                break;

            case HORIZONTAL:        // Movimento orizzontale
                ship.x -= 4;   // Aggiornamento coordinata X
                direction *= -1;    // Rimbalzo sul bordo
                flag = VERTICAL;    // Settiamo la flag per tornare al movimento verticale
                break;
        }

        // Sparo nemico 
        clock_gettime(CLOCK_REALTIME, &end);                            // Rilevazione del tempo trascorso dalla precedente rilevazione (spari)
        if (end.tv_sec - start.tv_sec >= timeTravel + 1){               // Controllo sul tempo trascorso dall'ultimo sparo
            if (rand()%ENEMIES == ship.serial){                        // Generazione numero casuale. Se uguale al serial della nave, può sparare
                shotArg.x = ship.x;
                shotArg.y = ship.y;
                shotArg.serial = ship.serial;
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
        insert(ship);
        usleep(delay);                              // Ritardo movimento navi
    }
    return NULL;
}

// Funzione per la gestione del movimento del razzo nemico
void* enemyShot(void* param){
    Object rocket;
    Args* arg;
    int id;
    
    arg = (Args*) param;
    id = arg->serial;

    // Inizializzazione razzo
    rocket.x = arg->x - 2;
    rocket.y = arg->y + 1;
    rocket.lives = 1;
    rocket.identifier = ENEMY_ROCKET;
    rocket.serial = arg->serial;

    insert(rocket);
    while (enemyRocket[id].lives && rocket.x > 0){                               // Loop movimento razzo
        rocket.x -= 1;                            // Movimento orizzontale
        insert(rocket);
        usleep(ROCKET_DELAY);                   // Ritardo movimento
    }
    return NULL;
}