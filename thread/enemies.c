#include "global.h"

// Funzione per l'inizializzazione, la generazione e il posizionamento iniziale delle navi nemiche.
void fleetEnlister(int mainPipe){
    Object enemy[ENEMIES];
    pid_t pidEnemyShip[ENEMIES];
    int posX = MAX_X - 2;
    int posY = 2;
    int i;
    
    // Ciclo per la generazione dei processi di tutte le navi nemiche e la relativa inizializzazione
    for(i=0; i<ENEMIES; i++){
        pidEnemyShip[i] = fork();
        switch(pidEnemyShip[i]){
            case -1:
                endwin();
                printf("Errore creazione processo nemico #%d", i);
                exit(1);

            case 0:
                // Inizializzazione singola nave
                enemy[i].x = posX;
                enemy[i].y = posY;
                enemy[i].pid = getpid();
                enemy[i].serial = i;
                enemy[i].identifier = ENEMY;
                enemyShip(mainPipe, enemy[i]);  // Chiamata a gestore movimento nave
        }
        posY += 4;              // Posizione asse Y della nave successiva
        if(posY >= MAX_Y -1) {  // Controllo posizionamento massimo asse Y
            posX -= 8;          // Prossima colonna di navi
            posY = 2;           // Prima posizione della colonna (nave di testa)
        }
    }

    // Ciclo per l'attesa della terminazione di tutti i processi figli (navi)
    for (i=0; i<ENEMIES; i++)
        waitpid(enemy[i].pid, NULL, 0);
}

// Funzione per la gestione delle azioni della singola nave nemica
void enemyShip(int mainPipe,  Object enemy){
    int direction = 1;              // Direzione nave: 1 -> basso, -1 -> alto
    int flag = VERTICAL;            // Flag da sfruttare per gestire il movimento verticale
    struct timespec start, end;     // Utilizzate per scandire il tempo minimo tra uno sparo e l'altro
    struct timespec start2, end2;   // Utilizzate come countdown per il cambio di velocità del movimento nemico
    int timeTravel;                 // Tempo massimo di percorrenza dell'area di gioco
    int delay = ENEMY_DELAY;        // Ritardo movimento navi

    // Rilevazioni iniziali del tempo
    clock_gettime(CLOCK_REALTIME, &start);                  // Rilevazione per il tempo degli spari
    clock_gettime(CLOCK_REALTIME, &start2);                 // Rilevazione per l'incremento della velocità delle navi
    timeTravel = timeTravelEnemyRocket(ROCKET_DELAY);       // Valutazione tempo massimo di percorrenza di uno sparo

    write(mainPipe, &enemy, sizeof(enemy));                 // Prima scrittura nella mainPipe
    while(true){                                            // Loop movimento nave nemica
        switch (flag){
            case VERTICAL:                                  // Movimento verticale
                enemy.y += direction;                       // Aggiornamento coordinata Y
                if (enemy.y <= 2 || enemy.y > MAX_Y - 1)    // Verifica collisione bordi
                    flag = HORIZONTAL;                      // Eventuale modifica del valore flag, che ci manderà al movimento orizzontale
                break;

            case HORIZONTAL:        // Movimento orizzontale
                enemy.x -= 4;       // Aggiornamento coordinata X
                direction *= -1;    // Rimbalzo sul bordo
                flag = VERTICAL;    // Settiamo la flag per tornare al movimento verticale
                break;
        }

        // Sparo nemico 
        clock_gettime(CLOCK_REALTIME, &end);                            // Rilevazione del tempo trascorso dalla precedente rilevazione (spari)
        if (end.tv_sec - start.tv_sec >= timeTravel + 1){               // Controllo sul tempo trascorso dall'ultimo sparo
            if (rand()%ENEMIES == enemy.serial){                        // Generazione numero casuale. Se uguale al serial della nave, può sparare
                enemyShot(mainPipe, enemy.x, enemy.y, enemy.serial);    // Chiamata a generazione sparo
                start = end;                                            // Il valore dell'ultima rilevazione viene salvato come "rilevazione precedente"
            }
        }

        clock_gettime(CLOCK_REALTIME, &end2);       // Rilevazione del tempo trascorso dalla precedente rilevazione (movimento)
        if (end2.tv_sec - start2.tv_sec >= 7){      // Controllo sul tempo trascorso dall'ultimo incremento della velocità
            delay *= 0.8;                           // Incremento della velocità di movimento: 20%
            start2 = end2;                          // Il valore dell'ultima rilevazione viene salvato come "rilevazione precedente"
        }
        write(mainPipe, &enemy, sizeof(enemy));     // Scrittura ciclica nella mainPipe passata al loop di gioco
        usleep(delay);                              // Ritardo movimento navi
    }
}

// Funzione per la gestione del movimento del razzo nemico
void enemyShot(int mainPipe, int x, int y, int serial){
    pid_t pidEnemyShot;
    Object shot;

    pidEnemyShot = fork();  // Creazione processo per il razzo nemico
    switch (pidEnemyShot){
        case -1:
            endwin();
            printf("Errore creazione missile nemico\n");
            exit(1);
    
        case 0:
            // Inizializzazione razzo
            shot.x = x - 2;
            shot.y = y + 1;
            shot.identifier = ENEMY_ROCKET;
            shot.pid = getpid();
            shot.serial = serial;

            write(mainPipe, &shot, sizeof(shot));       // Prima scrittura nella mainPipe
            while (true){                               // Loop movimento razzo
                shot.x -= 1;                            // Movimento orizzontale
                write(mainPipe, &shot, sizeof(shot));   // Scrittura ciclica nella mainPipe passata al loop di gioco
                usleep(ROCKET_DELAY);                   // Ritardo movimento
            }
            _exit(0);                                   // Chiusura processo
    }
}