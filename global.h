#ifndef GLOBAL_H
#define GLOBAL_H

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ENEMIES 8 //Il valore che avevi messo era 25 ma l'ho rimpiazzato per semplicità momentaneamente
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
#define ENEMY_ROCKET '-'

/* Oggetto da rappresentare. Nave giocatore, nave nemica, oggetti di gioco quali siluri, proiettili, etc. */
struct Object {
    char identifier;    // Carattere dell'oggetto. Es: "-" come proiettile.
    int x;              // Posizione dell'oggetto nell'asse x
    int y;              // Posizione dell'oggetto nell'asse y
    int lives;          // Numero di vite disponibii dell'oggetto
    pid_t pid;          // Pid del processo di riferimento dell'oggetto
    int serial;
};

// Funzioni libreria player.c
void playerShip(int fdShip, int fdMain);
void shot(int fdMain, int direction);

// Funzioni libreria enemies.c
void enemyShip(int mainPipe, int enemyPipe, struct Object enemy);

// Funzioni libreria gameplay.c
void gameAreaV2(int fd, int enemyPipe[][2]);

// funzioni prova
struct Object generatore(int i);
struct Object generatore2( int enemyCounter);
void gameAreaV3(int mainPipe, int playerPipe, int enemyPipe[][2]);
void enemyRocket(int mainPipe, int enemyPipe);

#endif /* GLOBAL_H */
