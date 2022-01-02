#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ENEMIES 8 //Il valore che avevi messo era 25 ma l'ho rimpiazzato per semplicità momentaneamente
#define MAX_X 80
#define MAX_Y 20

// Funzioni libreria player.c
void playerShip(int fd);
void rocket(int fd);

// Funzioni libreria enemies.c
void enemiesGenerator(int *fd);
void enemyShip(int fd);

// Funzioni libreria gameplay.c
void gameArea(int fd);

/* Oggetto da rappresentare. Nave giocatore, nave nemica, oggetti di gioco quali siluri, proiettili, etc. */
struct Object {
    char identifier;    // Carattere dell'oggetto. Es: "-" come proiettile.
    int x;              // Posizione dell'oggetto nell'asse x
    int y;              // Posizione dell'oggetto nell'asse y
    int lives;          // Numero di vite disponibii dell'oggetto
};

