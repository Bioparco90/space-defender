#include "global.h"

// Funzione per l'inizializzazione e la gestione delle azioni della nave del giocatore
void* playerShip(){
    struct timespec time, checker;
    Object ship;
    int serial;

    clock_gettime(CLOCK_REALTIME, &time);   // Rilevazione iniziale del tempo
    
    // Inizializzazione nave giocatore
    ship.x = 2;
    ship.y = MAX_Y / 2;
    ship.identifier = PLAYER;
    ship.pid = pthread_self();
    ship.serial = 777;
    
    serial = 0;     // Inizializzazione serial razzi giocatore

    insert(ship);   // Prima scrittura nel buffer

    // Ciclo movimento e sparo
    while(true){
        int c = getch();

        switch (c){
            case KEY_UP:                // Movimento verso l'alto
                if(ship.y > 2)          // Verifica collisione bordo
                    ship.y -= 1;
                break;
            
            case KEY_DOWN:              // Movimento verso il basso
                if (ship.y < MAX_Y-1)   // Verifica collisione bordo
                    ship.y += 1;
                break;

            case ' ':                                                   // Sparo alla pressione del tasto "spazio"
                clock_gettime(CLOCK_REALTIME, &checker);                // Rilevazione del tempo trascorso dalla precedente rilevazione
                if (serial == 0){                                       // Primo sparo della partita
                    system("aplay ../sounds/laser.wav 2> /dev/null &");    // Riproduce suono sparo
                    playerShotInit(ship.x, ship.y, serial);             // Generazione sparo
                    serial++;                                           // Incremento del serial (verrà utilizzato come indice nella funzione gestore di gioco)
                    time = checker;                                     // Il valore dell'ultima rilevazione viene salvato come "rilevazione precedente"
                }
                if (checker.tv_sec - time.tv_sec >= 1){                 // Spari successivi al primo. Valuta se sia passato ALMENO un secondo dallo sparo precedente
                    system("aplay ../sounds/laser.wav 2> /dev/null &");    // Riproduce suono sparo
                    playerShotInit(ship.x, ship.y, serial);             // Generazione sparo
                    serial++;                                           // Incremento del serial (verrà utilizzato come indice nel gameLoop)
                    time = checker;                                     // Il valore dell'ultima rilevazione viene salvato come "rilevazione precedente"
                }
                break;
        }
        if(serial >= MAX_ROCKET)            // Verifica se sia stato raggiunto il limite di spari esistenti in uno stesso momento
            serial = 0;                     // L'indicizzazione riparte da 0
        insert(ship);                       // Scrittura ciclica nel buffer
    }
    return NULL;
}

// Funzione per l'inizializzazione degli spari diagonali
void playerShotInit(int x, int y, int serial){
    pthread_t thRocketUp[MAX_ROCKET], thRocketDown[MAX_ROCKET];

    // Parametri da passare ai thread
    Args argsUp = {x, y, DIR_UP, serial};
    Args argsDown = {x, y, DIR_DOWN, serial};

    // Creazione thread spari
    if (pthread_create(&thRocketUp[serial], NULL, &shotUp, &argsUp)){
        endwin();
        exit(1);
    }

    if (pthread_create(&thRocketDown[serial], NULL, &shotDown, &argsDown)){
        endwin();
        exit(1);
    }
}

// Funzione per la gestione del movimento del singolo razzo
void* shotUp(void* param){
    struct timespec start, end;
    int timeTravel;
    Object shot;
    Args* args;
    int id;
    int direction;
    
    args = (Args*) param;

    id = args->serial;
    direction = args->dir;

    shot.x = args->x + 2;
   	shot.y = args->y + 1 + args->dir;
    shot.identifier = ROCKET_UP;
    shot.pid = pthread_self();
    shot.serial = args->serial;

    // La terminazione del thread avviene grazie alla rilevazione del tempo massimo di vita dello sparo
    timeTravel = timeTravelEnemyRocket(ROCKET_DELAY);
    clock_gettime(CLOCK_REALTIME, &start);

    insert(shot);   // Prima scrittura nel buffer

    // Ciclo movimento razzo
    while(true){
        clock_gettime(CLOCK_REALTIME, &end);                    // Verifica tempo trascorso
        if (end.tv_sec - start.tv_sec >= timeTravel + 1) break; // Esci dal loop e concludi l'esecuzione del thread
        if((shot.y < 2 || shot.y > MAX_Y)){                     // Verifica collisione bordi
            direction *= -1;                                    // Cambio direzione
        }
        shot.x += 1;            // Spostamento orizzontale
        shot.y += direction;    // Spostamento verticale, direzione dipendente dal valore di "direction"

        insert(shot);           // Scrittura ciclica nel buffer
		usleep(ROCKET_DELAY);   // Delay movimento razzo
    }
    return NULL;
}

void* shotDown(void* param){
    struct timespec start, end;
    int timeTravel;
    Object shot;
    Args* args;
    int id;
    int direction;
    
    args = (Args*) param;

    id = args->serial;
    direction = args->dir;

    shot.x = args->x + 2;
   	shot.y = args->y + 1 + args->dir;
    shot.identifier = ROCKET_DOWN;
    shot.pid = pthread_self();
    shot.serial = args->serial;

    // La terminazione del thread avviene grazie alla rilevazione del tempo massimo di vita dello sparo
    timeTravel = timeTravelEnemyRocket(ROCKET_DELAY);
    clock_gettime(CLOCK_REALTIME, &start);

    insert(shot);   // Prima scrittura nel buffer

    // Ciclo movimento razzo
    while(true){
        clock_gettime(CLOCK_REALTIME, &end);                    // Verifica tempo trascorso
        if (end.tv_sec - start.tv_sec >= timeTravel + 1) break; // Esci dal loop e concludi l'esecuzione del thread

        if((shot.y < 2 || shot.y > MAX_Y)){     // Verifica collisione bordi
            direction *= -1;                    // Cambio direzione
        }
        shot.x += 1;            // Spostamento orizzontale
        shot.y += direction;    // Spostamento verticale, direzione dipendente dal valore di "direction"

        insert(shot);           // Scrittura ciclica nel buffer
		usleep(ROCKET_DELAY);   // Delay movimento razzo
    }
    return NULL;
}
