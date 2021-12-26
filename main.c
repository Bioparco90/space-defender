#include <curses.h>
#include <unistd.h>

#define ENEMIES 25

/* Oggetto da rappresentare. Nave giocatore, nave nemica, oggetti di gioco quali siluri, proiettili, etc. */
struct Object {
    char identifier;    // Carattere dell'oggetto. Es: "-" come proiettile.
    int x;              // Posizione dell'oggetto nell'asse x
    int y;              // Posizione dell'oggetto nell'asse y
};

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

    if (pidPlayerShip < 0){                     
        printf("Errore creazione processo nave giocatore\n");
        return 1;
    } else{
        if (pidPlayerShip == 0){                
            close(fd[0]);       // Chiudiamo dscrittore in lettura
            playerShip(fd[1]);  // Gestiamo movimento giocatore passando descrittore in scrittura
        } else {                                
            int i;
            for (i=0; i<ENEMIES, i++){          
                pidEnemyShip[i] = fork();   // Creazione processi navi nemiche
                if (pidEnemyShip[i] < 0){
                    printf("Errore creazione processo nave nemica\n");
                    return 2;
                } else{
                    if (pidEnemyShip == 0){
                        close(fd[0]);       // Chiudiamo descrittore in lettura
                        enemyShip(fd[1]);   // Gestiamo movimento nemici passano descrittore in scrittura
                    } else{
                        close(fd[1]);       // Chiudiamo descrittore in scrittura 
                        controllo(fd[0]);   // Gestiamo rappresentazione area di gioco passando descrittore in lettura
                    }
                }
            }
        }
    }
    

    endwin();               // Ripristino del terminale

    return 0;
}