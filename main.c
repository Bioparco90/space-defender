#include "global.h"
#include "curses.h"

int main(){
    pid_t pidPlayerShip;         // Pid processo figlio "nave giocatore" 
    pid_t pidEnemyShip[ENEMIES]; // Pid processo figlio "nave nemica"
    int fd[2];                   // Descrittore pipe
    int i,j;

    initscr();         // Inizializza schermo di gioco
    noecho();          // Disabilita visualizzazione tasti premuti
    keypad(stdscr, 1); // Abilita tasti funzione (frecce)
    curs_set(0);       // Disabilita visualizzazione cursore

    // Creazione della pipe per la comunicazione tra i processi
    pipe(fd);   

    // Creazione primo processo figlio - Nave giocatore
    pidPlayerShip = fork();

    switch (pidPlayerShip){
        case -1:
            printw("Errore creazione processo nave giocatore\n");
            return 1;
            break;

        case 0:
            mvprintw(MAX_Y / 2, 1, "+"); // Prima rappresenzazione nave giocatore
            close(fd[0]);       // Chiudiamo descrittore in lettura
            playerShip(fd[1]);  // Gestiamo movimento giocatore passando descrittore in scrittura
            _exit(0);
            //break;
       /* default:
            //wait(NULL); // di debug, valutare successiva rimozione (impedisce l'uscita prematura dal programma)
            // Generiamo processi multipli relativi alle navi nemiche
            break;*/
    }
    enemiesGenerator(fd); 

    // int c = getch(); // di debug, da rimuovere poi
    endwin();   // Ripristino del terminale

    return 0;
}