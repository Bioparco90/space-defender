#include "global.h"

//globali
int spawnX = MAX_X;
int spawnY = 2;

// codice non globale
void generatore(struct Object *enemy){
    enemy->x = spawnX;
    enemy->y = spawnY;
    enemy->identifier = ENEMY;
    enemy->lives = 2; // o quante sono
    enemy->pid = getpid(); // siamo nel for delle fork in teoria
    // ora possiamo modificare i valori
    spawnY += 2; //spazio vuoto
    if(enemy->y == MAX_Y - 1 || spawnY >= MAX_Y - 1)
        spawnX -= 2; // spazio vuoto
}
// dovrebbe essere una roba del genere, senza manco usare il modulo
