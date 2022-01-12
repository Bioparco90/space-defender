#include "global.h"
#include <curses.h>
#include <unistd.h>

int main(){
    struct Object enemy[ENEMIES];
    pid_t pidPlayerShip;            // Pid processo figlio "nave giocatore" 
    pid_t pidEnemyShip[ENEMIES];    // Pid processo figlio "nave nemica"
    pid_t pidEnemyFleet;
    int mainPipe[2];                // Pipe generale
    // int playerShipPipe[2];          // Pipe giocatore
    // int enemyShipsPipe[ENEMIES][2]; // Pipe nemici
    int i, j, row, col = MAX_X - 1;

    initscr();         // Inizializza schermo di gioco
    noecho();          // Disabilita visualizzazione tasti premuti
    keypad(stdscr, 1); // Abilita tasti funzione (frecce)
    curs_set(0);       // Disabilita visualizzazione cursore

    // Creazione delle pipe per la comunicazione tra i processi
    if (pipe(mainPipe) < 0) {
        endwin();
        return 1;
    }

    // Creazione primo processo figlio - Nave giocatore
    pidPlayerShip = fork();
    switch (pidPlayerShip){
        case -1:
            endwin();
            printf("Errore creazione processo nave giocatore\n");
            return 2;

        case 0:
            // Chiudiamo tutte le pipe nemico sia in lettura che in scrittura
            // for(i=0; i<ENEMIES; i++){
            //     close(enemyShipsPipe[i][READ]);
            //     close(enemyShipsPipe[i][WRITE]);
            // }
            // close(playerShipPipe[WRITE]); // Chiudiamo pipe giocatore in scrittura
            close(mainPipe[READ]);        // Chiudiamo pipe principale in lettura
            playerShip(mainPipe[WRITE]);  // Gestore movimento nave giocatore
            _exit(0);

        default:
            pidEnemyFleet = fork();
            switch(pidEnemyFleet){
                case -1:
                    endwin();
                    printf("Errore creazione flotta nemica\n");
                    return 3;

                case 0:
                    close(mainPipe[READ]);
                    fleetEnlister(mainPipe[WRITE]);
                    _exit(0);
            }
    }

    // Creazione processi navi nemiche
    // for(i=0; i<ENEMIES; i++){
    //     row = i+3;
         
    //     pidEnemyShip[i] = fork();
    //     switch(pidEnemyShip[i]){
    //         case -1:
    //             endwin();
    //             printf("Errore creazione processo nave nemica\n");
    //             return 3;

    //         case 0:
    //             close(mainPipe[READ]);              // Chiusura pipe principale in lettura
    //             // enemy[i] = generator(i+1);
    //             enemy[i].identifier = ENEMY;
                
                
    //             // Assegnazione del seriale univoco della nave (possibilità di spostarlo dentro la generatore)
    //             enemy[i].serial = i;

    //             // Gestione movimento della nave
    //             enemyShip(mainPipe[WRITE], enemy[i]); 
    //             _exit(0);
    //     }
    // }

    // Processo principale
    close(mainPipe[WRITE]);     // Chiudiamo pipe principale in scrittura
    gameArea(mainPipe[READ]);   // Gestore principale del gioco

    endwin(); // Ripristino del terminale

    return 0;
}