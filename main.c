#include "global.h"
#include <curses.h>
#include <unistd.h>

int main(){
    struct Object enemy;
    pid_t pidPlayerShip;            // Pid processo figlio "nave giocatore" 
    pid_t pidEnemyShip[ENEMIES];    // Pid processo figlio "nave nemica"
    int mainPipe[2];                // Pipe generale
    int playerShipPipe[2];          // Pipe giocatore
    int enemyShipsPipe[ENEMIES][2]; // Pipe nemici
    int i, j;

    initscr();         // Inizializza schermo di gioco
    noecho();          // Disabilita visualizzazione tasti premuti
    keypad(stdscr, 1); // Abilita tasti funzione (frecce)
    curs_set(0);       // Disabilita visualizzazione cursore

    // Creazione delle pipe per la comunicazione tra i processi
    if (pipe(mainPipe) < 0) {
        endwin();
        return 1;
    }
    if (pipe(playerShipPipe) < 0) {
        endwin();
        return 2;
    }

    // Creazione delle pipe dei nemici
    for(i=0; i<ENEMIES; i++)
        if (pipe(enemyShipsPipe[i]) < 0){
            endwin();
            return 3;
        }

    // Creazione primo processo figlio - Nave giocatore
    switch (pidPlayerShip = fork()){
        case -1:
            endwin();
            printf("Errore creazione processo nave giocatore\n");
            return 4;

        case 0:
            // Chiudiamo tutte le pipe nemico sia in lettura che in scrittura
            for(i=0; i<ENEMIES; i++){
                close(enemyShipsPipe[i][READ]);
                close(enemyShipsPipe[i][WRITE]);
            }
            close(playerShipPipe[WRITE]); // Chiudiamo pipe giocatore in scrittura
            close(mainPipe[READ]);        // Chiudiamo pipe principale in lettura
            playerShip(playerShipPipe[READ], mainPipe[WRITE]);  // Gestore movimento nave giocatore
            _exit(0);
    }

    // Creazione processi navi nemiche
    for(i=0; i<ENEMIES; i++){
        switch(pidEnemyShip[i] = fork()){
            case -1:
                endwin();
                printf("Errore creazione processo nave nemica\n");
                return 5;

            case 0:
                close(mainPipe[READ]);              // Chiusura pipe principale in lettura
                close(playerShipPipe[READ]);        // Chiusura pipe giocatore in lettura
                close(playerShipPipe[WRITE]);       // Chiusura pipe giocatore in scrittura
                
                // Chiusura di tutte le pipe nemici in lettura e scrittura 
                for(j=0; j<ENEMIES; j++){
                    close(enemyShipsPipe[i][WRITE]);
                    if(j!=i) close(enemyShipsPipe[j][READ]); // La pipe del processo attuale rimane aperta in lettura
                }

                // Generazione e posizionamento iniziale della nave
                // Chiedere motivo del passaggio di i+1 quando poi la funzione gestisce i-1. 
                // Dovrebbe esser sufficiente passare i.
                enemy = generator(i+1);

                // Assegnazione del seriale univoco della nave (possibilità di spostarlo dentro la generatore)
                enemy.serial = i;

                // Gestione movimento della nave
                enemyShip(mainPipe[WRITE], enemyShipsPipe[i][READ], enemy); 
                _exit(0);
        }
    }

    // Processo principale
    close(mainPipe[WRITE]);             // Chiudiamo pipe principale in scrittura
    for (i=0; i<ENEMIES; i++){
        close(enemyShipsPipe[i][READ]); // Chiudiamo le pipe nemici in lettura
    }

    // Gestore principale del gioco
    gameArea(mainPipe[READ], playerShipPipe[WRITE], enemyShipsPipe);

    endwin(); // Ripristino del terminale

    return 0;
}