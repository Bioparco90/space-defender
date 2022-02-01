#include "global.h"

int main(){
    pthread_t thPlayerShip;   

    srand(time(NULL));      // Inizializza seed random

    initscr();              // Inizializza schermo di gioco
    start_color();
    noecho();               // Disabilita visualizzazione tasti premuti
    keypad(stdscr, 1);      // Abilita tasti funzione (frecce)
    curs_set(0);            // Disabilita visualizzazione cursore

    // startGame();            // Schermata iniziale

    pthread_mutex_init(&mutex, NULL);

    // Inizializzare vite oggetti globali qui
    player.lives = 3;

    if (pthread_create(&thPlayerShip, NULL, &playerShip, NULL)){
        endwin();
        exit;
    }


    gameArea();   // Gestore principale del gioco

    endwin();                   // Ripristino del terminale

    pthread_mutex_destroy(&mutex);

    return 0;
}