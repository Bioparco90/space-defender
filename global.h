#ifndef GLOBAL_H
#define GLOBAL_H

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ENEMIES 5
#define MAX_X 80
#define MAX_Y 20
#define MAX_ENEMY_COL (MAX_Y/3)
#define AMMO_CYCLE 64

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
#define ROCKET_DELAY 10000

/* Oggetto da rappresentare. Nave giocatore, nave nemica, oggetti di gioco quali siluri, proiettili, etc. */
typedef struct {
    char identifier;    // Carattere dell'oggetto. Es: "-" come proiettile.
    int x;              // Posizione dell'oggetto nell'asse x
    int y;              // Posizione dell'oggetto nell'asse y
    int lives;          // Numero di vite disponibii dell'oggetto
    pid_t pid;          // Pid del processo di riferimento dell'oggetto
    int serial;         // Numero univoco della nave
} Object;

// Funzioni libreria player.c
void playerShip(int fdMain);
void playerShotInit(int mainPipe, int x, int y, int ammoSerialUp, int ammoSerialDown);
void shot(int mainPipe, int x, int y, int direction,  int ammoSerialUp, int ammoSerialDown);

// Funzioni libreria enemies.c
void fleetEnlister(int mainPipe);
void enemyShip(int mainPipe, Object enemy);

// Funzioni libreria gameplay.c
void gameArea(int mainPipe);

// Funzioni di utilità globale (global.c)
void printSprite(int posX, int posY, char sprite[3][3]);
void deleteSprite(int posX, int posY);

// funzioni prova
int checkCollisionRocket(Object rocket);
int checkCollisonEnemy(Object entity);


#endif /* GLOBAL_H */
