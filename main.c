#include "global.h"

int main(){
    pid_t pidPlayerShip;    // Pid processo figlio "nave giocatore" 
    pid_t pidEnemyFleet;    // Pid processo generatore flotta nemica
    int mainPipe[2];        // Pipe generale

    srand(time(NULL));      // Inizializza seed random

    initscr();              // Inizializza schermo di gioco
    start_color();
    noecho();               // Disabilita visualizzazione tasti premuti
    keypad(stdscr, 1);      // Abilita tasti funzione (frecce)
    curs_set(0);            // Disabilita visualizzazione cursore

    startGame();            // Schermata iniziale

    // Creazione delle pipe per la comunicazione tra i processi
    if (pipe(mainPipe) < 0) {
        endwin();
        printf("Errore creazione pipe\n");
        return -1;
    }

    // Creazione primo processo figlio - Nave giocatore
    pidPlayerShip = fork();
    switch (pidPlayerShip){
        case -1:
            endwin();
            printf("Errore creazione processo nave giocatore\n");
            return -1;

        case 0:
            close(mainPipe[READ]);        // Chiudiamo pipe principale in lettura
            playerShip(mainPipe[WRITE]);  // Gestore movimento nave giocatore
            _exit(0);

        default:
        
            // Creazione secondo processo figlio "flotta nemica"
            pidEnemyFleet = fork();
            switch(pidEnemyFleet){
                case -1:
                    endwin();
                    printf("Errore creazione flotta nemica\n");
                    return -1;

                case 0:
                    close(mainPipe[READ]);          // Chiusura pipe in lettura
                    fleetEnlister(mainPipe[WRITE]); // Generatore flotta
                    _exit(0);
            }
    }

    // Processo principale
    close(mainPipe[WRITE]);     // Chiusura pipe principale in scrittura
    gameArea(mainPipe[READ]);   // Gestore principale del gioco

    endwin();                   // Ripristino del terminale

    // debug terminazione processi
    // pid_t log;
    // log = fork();
    // switch (log){
    //     case -1:
    //         _exit(1);
        
    //     case 0:
    //         execl("/bin/ps", "" ,NULL);
    // }

    return 0;
}