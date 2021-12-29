#include "global.h"

int main(){
    pid_t pidPlayerShip;         // Pid processo figlio "nave giocatore" 
    pid_t pidEnemyShip[ENEMIES]; // Pid processo figlio "nave nemica"
    int fd[2];                   // Descrittore pipe

    initscr();         // Inizializza schermo di gioco
    noecho();          // Disabilita visualizzazione tasti premuti
    keypad(stdscr, 1); // Abilita tasti funzione (frecce)
    curs_set(0);       // Disabilita visualizzazione cursore

    // Creazione primo processo figlio - Nave giocatore
    pidPlayerShip = fork();

    switch (pidPlayerShip){
        case -1:
            printf("Errore creazione processo nave giocatore\n");
            return 1;
            break;

        case 0:
            close(fd[0]);       // Chiudiamo dscrittore in lettura
            playerShip(fd[1]);  // Gestiamo movimento giocatore passando descrittore in scrittura
            break;

        default:
            int i;
            for (i=0; i<ENEMIES; i++){
                pidEnemyShip[i] = fork();
                switch (pidEnemyShip[i]){
                    case -1:
                        printf("Errore creazione processo nave nemica\n");
                        return 2;
                        break;

                    case 0:
                        close(fd[0]);       // Chiudiamo descrittore in lettura
                        enemyShip(fd[1]);   // Gestiamo movimento nemici passano descrittore in scrittura
                        break;

                    default:
                        close(fd[1]);       // Chiudiamo descrittore in scrittura 
                        gameArea(fd[0]);    // Gestiamo rappresentazione area di gioco passando descrittore in lettura
                        break;
                }
            }
            break;
    }
  
    endwin();   // Ripristino del terminale

    return 0;
}