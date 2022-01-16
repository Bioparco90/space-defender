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
#define ROCKET '*'
#define BOMB 'o'

// Macro proiettili giocatore
#define MAX_ROCKET 50
#define ROCKET_UP 'u'
#define ROCKET_DOWN 'd'

//Macro bombe nemiche
#define MAX_BOMB 100
#define RANDOM_BOMB_START 1
#define RANDOM_BOMB_FINISH 1000000


// Macro movimento nemici
#define HORIZONTAL 1
#define VERTICAL 0

// Macro per i ritardi
#define ENEMY_DELAY 300000
#define ROCKET_DELAY 20000
#define BOMB_DELAY 25000
#define DELAY_BOMB_RANDOM 35


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
void playerShotInit(int mainPipe, int x, int y, int serial);
void shot(int mainPipe, int x, int y, int direction, int serial);

// Funzioni libreria enemies.c
void fleetEnlister(int mainPipe);
void enemyShip(int mainPipe, Object enemy);
void enemyBombInit(int mainPipe, int x, int y, int bombSerial);
void bomb(int mainPipe, int x, int y, int bombSerial);

// Funzioni libreria gameplay.c
void gameArea(int mainPipe);
void gameOver(int score, int collision);

// Funzioni di utilità globale (global.c)
void printSprite(int posX, int posY, char sprite[3][3]);
void deleteSprite(Object item);

// funzioni prova
int isWeapon(Object item);
int checkCollision(Object a, Object b);
Object resetItem();
// int checkCollisionRocket(Object rocket);
// int checkCollisonEnemy(Object entity);



#endif /* GLOBAL_H */
