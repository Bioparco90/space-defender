#include <curses.h>
#include <unistd.h>

#define ENEMIES 25

void playerShip(int fd);
void enemyShip(int fd);
void gameArea(int fd);

void enemiesGenerator(int *fd);