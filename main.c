#include "global.h"
#include <curses.h>

int main(){
    pid_t pidPlayerShip;         // Pid processo figlio "nave giocatore" 
    pid_t pidEnemyShip[ENEMIES];
    struct Object datoRead ;              // Pid processo figlio "nave nemica"
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
            printf("Errore creazione processo nave giocatore\n");
            return 1;
            break;

        case 0:
            mvprintw(MAX_Y / 2, 1, "+"); // Prima rappresenzazione nave giocatore
            close(fd[0]);       // Chiudiamo descrittore in lettura
            playerShip(fd[1]);  // Gestiamo movimento giocatore passando descrittore in scrittura
            break;

        default:
            wait(NULL); // di debug, valutare successiva rimozione (impedisce l'uscita prematura dal programma)
            enemiesGenerator(fd); // Generiamo processi multipli relativi alle navi nemiche
            break;
    }
    

    //ALGORITMO PROTOTIPO SPAWN INIZIALE NEMICI
       /* for(i=0;i<ENEMIES;i++){    
         for(j=0;j<i; j++){ 
            if(i==ENEMIES-1){
                for(k=0;k<ENEMIES;k++){
                printw("<");
                printw("\n\n");
                }
                break;
            }          
        }
        printw("\n");
   }*/
  

    int c = getch(); // di debug, da rimuovere poi
    endwin();   // Ripristino del terminale

    return 0;
}