#ifndef GLOBAL_H
#define GLOBAL_H

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ENEMIES 5 //Il valore che avevi messo era 25 ma l'ho rimpiazzato per semplicità momentaneamente
#define MAX_X 80
#define MAX_Y 20

// Macro per la direzione degli spari diagonali
#define DIR_UP 1
#define DIR_DOWN -1

// Macro per la rappresentazione degli oggetti
#define PLAYER "+"
#define ENEMY "<"
#define ROCKET "o"

/* Oggetto da rappresentare. Nave giocatore, nave nemica, oggetti di gioco quali siluri, proiettili, etc. */
struct Object {
    char identifier;    // Carattere dell'oggetto. Es: "-" come proiettile.
    int x;              // Posizione dell'oggetto nell'asse x
    int y;              // Posizione dell'oggetto nell'asse y
    int lives;          // Numero di vite disponibii dell'oggetto
    pid_t pid;          // Pid del processo di riferimento dell'oggetto
    int firstShow;
};

// Funzioni libreria player.c
void playerShip(int fd);
void shot(int fd, int direction);

// Funzioni libreria enemies.c
void enemiesGenerator(int *fd);
void enemyShip(int fd, struct Object enemy);

// Funzioni libreria gameplay.c
void gameArea(int fd);

// funzioni prova
void generatore(int fd, struct Object *enemy);


#endif /* GLOBAL_H */
