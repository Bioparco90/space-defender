#include "global.h"
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
    if (pipe(mainPipe) < 0) return 1;
    if (pipe(playerShipPipe) < 0) return 2;

    // Pipe dei nemici
    for(i=0; i<ENEMIES; i++)
        if (pipe(enemyShipsPipe[i]) < 0)
            return 3;   

    // Creazione primo processo figlio - Nave giocatore
    switch (pidPlayerShip = fork()){
        case -1:
            printf("Errore creazione processo nave giocatore\n");
            return 4;

        case 0:
            for(i=0; i<ENEMIES; i++){
                close(enemyShipsPipe[i][READ]);
                close(enemyShipsPipe[i][WRITE]);
            }
            close(playerShipPipe[WRITE]);       // Chiudiamo descrittore in lettura
            close(mainPipe[READ]);              // Chiudiamo descrittore in lettura
            playerShip(playerShipPipe[READ], mainPipe[WRITE]);  // Gestore movimento nave giocatore
            _exit(0);
    }

    for(i=0; i<ENEMIES; i++){
        switch(pidEnemyShip[i] = fork()){
            case -1:
                printf("Errore creazione processo nave nemica\n");
                return 5;

            case 0:
                // enemy = generatore2(enemy, i+1);
                close(mainPipe[READ]);
                close(playerShipPipe[READ]);
                close(playerShipPipe[WRITE]);
                close(enemyShipsPipe[i][WRITE]);
                for(j=0; j<ENEMIES; j++){
                    if(j!=i) close(enemyShipsPipe[j][READ]);
                }
                enemy = generatore(i+2);
                enemy.serial = i;
                enemyShip(mainPipe[WRITE], enemyShipsPipe[i][READ], enemy);
                _exit(0);
        }
    }

    close(mainPipe[WRITE]);
    for (i=0; i<ENEMIES; i++){
        close(enemyShipsPipe[i][READ]);
    }
    gameAreaV3(mainPipe[READ], playerShipPipe[WRITE], enemyShipsPipe);

    endwin();   // Ripristino del terminale

    return 0;
}