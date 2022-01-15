#include "global.h"
#include <curses.h>
#include <unistd.h>

int main(){
    pid_t pidPlayerShip;            // Pid processo figlio "nave giocatore" 
    pid_t pidEnemyFleet;
    int mainPipe[2];                // Pipe generale

    srand(time(NULL));

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

    // Processo principale
    close(mainPipe[WRITE]);     // Chiudiamo pipe principale in scrittura
    gameArea(mainPipe[READ]);   // Gestore principale del gioco

    endwin(); // Ripristino del terminale

    return 0;
}