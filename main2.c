#include "global.h"

int main(){
    struct Object enemy;
    pid_t pidPlayerShip;            // Pid processo figlio "nave giocatore" 
    pid_t pidEnemyShip[ENEMIES];    // Pid processo figlio "nave nemica"
    int fd[2];                      // Descrittore pipe generale
    int allShipsPipe[ENEMIES+1][2]; // Descrittore pipe nave giocatore (0) e nemici
    int i;

    initscr();         // Inizializza schermo di gioco
    noecho();          // Disabilita visualizzazione tasti premuti
    keypad(stdscr, 1); // Abilita tasti funzione (frecce)
    curs_set(0);       // Disabilita visualizzazione cursore

    // Creazione della pipe per la comunicazione tra i processi
    pipe(fd);

    // Pipe dei nemici
    for(i=0; i<ENEMIES+1; i++)
        pipe(allShipsPipe[i]);   

    // Creazione primo processo figlio - Nave giocatore
    switch (pidPlayerShip = fork()){
        case -1:
            printf("Errore creazione processo nave giocatore\n");
            _exit(-1);

        case 0:
            close(allShipsPipe[0][READ]);       // Chiudiamo descrittore in lettura
            playerShip(allShipsPipe[0][WRITE]); // Gestiamo movimento giocatore passando descrittore in scrittura
            _exit(0);
    }

    for(i=0; i<ENEMIES; i++){
        switch(pidEnemyShip[i] = fork()){
            case -1:
                printf("Errore creazione processo nave nemica\n");
                _exit(-1);

            case 0:
                // enemy = generatore2(enemy, i+1);
                close(allShipsPipe[i+1][READ]);
                enemy = generatore();
                enemy.serial = i;
                enemyShip(allShipsPipe[i+1][WRITE], enemy);
                _exit(0);
        }
    }

    // close(fd[1]);
    for (i=0; i<ENEMIES; i++){
        close(allShipsPipe[i][WRITE]);
    }
    gameAreaV3(allShipsPipe);
    // gameAreaV2(fd[0], allShipsPipe);

    // int c = getch(); // di debug, da rimuovere poi
    endwin();   // Ripristino del terminale

    return 0;
}