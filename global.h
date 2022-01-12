#ifndef GLOBAL_H
#define GLOBAL_H

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ENEMIES 50
#define MAX_X 80
#define MAX_Y 20
#define MAX_ENEMY_COL (MAX_Y/3)

// Macro gestione pipe
#define READ 0
#define WRITE 1

// Macro per la direzione degli spari diagonali
#define DIR_UP 1
#define DIR_DOWN -1

// Macro per la rappresentazione degli oggetti
#define PLAYER '+'
#define ENEMY '<'
#define ROCKET 'o'

// Macro movimento nemici
#define HORIZONTAL 1
#define VERTICAL 0

// Macro per i ritardi
#define ENEMY_DELAY 300000

/* Oggetto da rappresentare. Nave giocatore, nave nemica, oggetti di gioco quali siluri, proiettili, etc. */
struct Object {
    char identifier;    // Carattere dell'oggetto. Es: "-" come proiettile.
    int x;              // Posizione dell'oggetto nell'asse x
    int y;              // Posizione dell'oggetto nell'asse y
    int lives;          // Numero di vite disponibii dell'oggetto
    pid_t pid;          // Pid del processo di riferimento dell'oggetto
    int serial;         // Numero univoco della nave
};

// Funzioni libreria player.c
void playerShip(int fdMain);
void playerShotInit(int mainPipe, int x, int y);
void shot(int mainPipe, int x, int y, int direction);

// Funzioni libreria enemies.c
void enemyShip(int mainPipe, struct Object enemy);

// Funzioni libreria gameplay.c
void gameArea(int mainPipe);

// funzioni prova
struct Object generator(int enemyCounter);
void printSprite(int posX, int posY, char sprite[3][1]);
void fleetEnlister(int mainPipe);

#endif /* GLOBAL_H */
