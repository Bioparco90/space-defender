#include <curses.h>
#include <unistd.h>

/* Oggetto da rappresentare. Nave giocatore, nave nemica, oggetti di gioco quali siluri, proiettili, etc. */
struct object {
    char identifier;    // Carattere dell'oggetto. Es: "-" come proiettile.
    int x;              // Posizione dell'oggetto nell'asse x
    int y;              // Posizione dell'oggetto nell'asse y
};

int main(){
    pid_t pidPlayerShip;    // Pid processo figlio "nave giocatore" 
    pid_t pidEnemyShip;     // Pid processo figlio "nave nemica"
    int fd[2];              // Descrittore pipe

    initscr();              // Inizializza schermo di gioco
    noecho();               // Disabilita visualizzazione tasti premuti
    keypad(stdscr, 1);      // Abilita tasti funzione (frecce)
    curs_set(0);            // Disabilita visualizzazione cursore

    endwin();               // Ripristino del terminale

    return 0;
}