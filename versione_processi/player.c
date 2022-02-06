#include "global.h"

// Funzione per l'inizializzazione e la gestione delle azioni della nave del giocatore
void playerShip(int mainPipe){
    struct timespec time, checker;
    Object ship;
    int serial;

    clock_gettime(CLOCK_REALTIME, &time);   // Rilevazione iniziale del tempo
    
    // Inizializzazione nave giocatore
    ship.x = 2;
    ship.y = MAX_Y / 2;
    ship.identifier = PLAYER;
    ship.pid = getpid();
    ship.serial = 777;
    
    write(mainPipe, &ship, sizeof(ship)); // Prima scrittura nella mainPipe
    serial = 0;                         // Inizializzazione serial razzi giocatore

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
                    playerShotInit(mainPipe, ship.x, ship.y, serial);     // Generazione sparo
                    serial++;                                           // Incremento del serial (verrà utilizzato come indice nella funzione gestore di gioco)
                    time = checker;                                     // Il valore dell'ultima rilevazione viene salvato come "rilevazione precedente"
                }
                if (checker.tv_sec - time.tv_sec >= 1){                 // Spari successivi al primo. Valuta se sia passato ALMENO un secondo dallo sparo precedente
                    system("aplay ../sounds/laser.wav 2> /dev/null &");    // Riproduce suono sparo
                    playerShotInit(mainPipe, ship.x, ship.y, serial);     // Generazione sparo
                    serial++;                                           // Incremento del serial (verrà utilizzato come indice nel gameLoop)
                    time = checker;                                     // Il valore dell'ultima rilevazione viene salvato come "rilevazione precedente"
                }
                break;
        }
        if(serial >= MAX_ROCKET)            // Verifica se sia stato raggiunto il limite di spari esistenti in uno stesso momento
            serial = 0;                     // L'indicizzazione riparte da 0
        write(mainPipe, &ship, sizeof(ship)); // Scrittura ciclica nella mainPipe passata al loop di gioco
    }
}

// Funzione per l'inizializzazione degli spari diagonali
void playerShotInit(int mainPipe, int x, int y, int serial){
    pid_t pidRocketUp, pidRocketDown;

    pidRocketUp = fork();                                       // Inizializzazione processo razzo diagonale superiore
    switch (pidRocketUp){                                       
        case -1:                                                // Fallimento fork
            _exit(-1);                                          // Gestione errore
            break;

        case 0:                                                 // Processo razzo diagonale superiore
            shot(mainPipe, x, y, DIR_UP, serial);               // Inizializzazione e gestore movimento razzo
            _exit(0);                                           // Chiusura processo
            break;

        default:                                                // Processo padre
            pidRocketDown = fork();                             // Inizializzazione processo razzo diagonale inferiore
            switch (pidRocketDown){
                case -1:                                        // Fallimento fork
                    _exit(-1);                                  // Gestione errore
                    break;

                case 0:                                         // Processo razzo diagonale inferiore
                    shot(mainPipe, x, y, DIR_DOWN, serial);     // Inizializzazione e gestore movimento razzo
                    _exit(0);                                   // Chiusura processo
                    break;
            }
    }
}

// Funzione per la gestione del movimento del singolo razzo
void shot(int mainPipe, int x, int y, int direction, int serial){
    Object rocket;

    // Inizializzazione razzo
    if(direction == DIR_UP)                 // Valutazione direzione sparo
        rocket.identifier = ROCKET_UP;      // Razzo giocatore alto
    else
        rocket.identifier = ROCKET_DOWN;    // Razzo giocatore basso

    rocket.x = x + 2;
   	rocket.y = y + 1 + direction;
    rocket.lives = 1;
    rocket.pid = getpid();
    rocket.serial = serial;

    write(mainPipe, &rocket, sizeof(rocket));       // Prima scrittura nella mainPipe

    // Ciclo movimento razzo
    while(true){
        if((rocket.y < 2 || rocket.y > MAX_Y)){     // Verifica collisione bordi
            direction *= -1;                        // Cambio direzione
        }
        rocket.x += 1;                              // Spostamento orizzontale
        rocket.y += direction;                      // Spostamento verticale, direzione dipendente dal valore di "direction"
        write(mainPipe, &rocket, sizeof(rocket));   // Scrittura ciclica nella mainPipe passata al loop di gioco
		usleep(ROCKET_DELAY);                       // Delay movimento razzo
    }
}
