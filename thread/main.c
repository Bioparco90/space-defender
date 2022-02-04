#include "global.h"

int main(){
    pthread_t thPlayerShip;
    int i; 

    srand(time(NULL));      // Inizializza seed random

    initscr();              // Inizializza schermo di gioco
    start_color();
    noecho();               // Disabilita visualizzazione tasti premuti
    keypad(stdscr, 1);      // Abilita tasti funzione (frecce)
    curs_set(0);            // Disabilita visualizzazione cursore

    // startGame();            // Schermata iniziale

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, DIM_BUFFER);
    sem_init(&full, 0, 0);

    if (pthread_create(&thPlayerShip, NULL, &playerShip, NULL)){
        endwin();
        exit(1);
    }

    fleetEnlister();

    gameArea();   // Gestore principale del gioco

    endwin();                   // Ripristino del terminale

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}