#include "global.h"

//globali
int spawnX = MAX_X;
int spawnY = 2;

// codice non globale
void generatore(int fd, struct Object *nemico){
    nemico->x = spawnX;
    nemico->y = spawnY;
    nemico->lives = 2; // o quante sono
    nemico->pid = getpid(); // siamo nel for delle fork in teoria
    // ora possiamo modificare i valori
    spawnY += 2 //spazio vuoto
    if(nemico->y == MAX_Y - 1 || spawnY >= MAX_Y - 1)
        spawnX -= 2; // spazio vuoto
}
// dovrebbe essere una roba del genere, senza manco usare il modulo
