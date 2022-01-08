#include "global.h"

// Funzione per la generazione delle navi.
// Si occupa della posizione iniziale di spawn e di settare tutti i valori iniziali
// di ogni singola nave. 
struct Object generator(int enemyCounter){
    struct Object enemy;

    enemy.y= 2* ((enemyCounter-1) % (MAX_ENEMY_COL))+2;      // 1 è il numero di spazi tra una nave e l'altra
    enemy.x= MAX_X - (((enemyCounter-1)/(MAX_ENEMY_COL))-2); // 1 è il numero di spazi tra una colonna di navi e l'altra
    enemy.identifier = ENEMY;
    enemy.lives = 3; // lives = 3: nave di primo livello, lives < 3: nave di secondo livello
    enemy.pid = getpid();

    return enemy;
}

void enemyShip(int mainPipe, int enemyPipe, struct Object enemy){
    int direction=1; 

    write(mainPipe, &enemy, sizeof(enemy));
    while (true){
        read(enemyPipe, &enemy, sizeof(enemy));
        if(enemy.y < 2 || enemy.y > MAX_Y - 1) {
            enemy.x -= 1;
            direction *= -1;
        }   
        enemy.y += direction;
        write(mainPipe, &enemy, sizeof(enemy));
        usleep(500000);
    }
}
