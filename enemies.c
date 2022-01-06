#include "global.h"

//globali
int spawnX = MAX_X-1;
int spawnY = 2;

// codice non globale
struct Object generatore(){
    struct Object enemy;
    enemy.x = spawnX;
    enemy.y = spawnY;
    enemy.identifier = ENEMY;
    enemy.lives = 2; // o quante sono
    enemy.pid = getpid(); // siamo nel for delle fork in teoria
    // ora possiamo modificare i valori
    spawnY += 2; //spazio vuoto
    if(enemy.y == MAX_Y - 1 || spawnY >= MAX_Y - 1)
        spawnX -= 2; // spazio vuoto

    return enemy;
}

struct Object generatore2(struct Object enemy, int enemyCounter){

    enemy.y= 2* ((enemyCounter-1) % (MAX_ENEMY_COL));//2 è il numero di spazi tra una nave e l'altra
    enemy.x= MAX_X - (((enemyCounter-1)/(MAX_ENEMY_COL))*2);//2 è il numero di spazi tra una colonna di navi e l'altra
    enemy.identifier = ENEMY;
    enemy.lives = 2; // o quante sono
    enemy.pid = getpid();

    return enemy;
}


void enemyShip(int fd, struct Object enemy){
    int direction=1;

    write(fd, &enemy, sizeof(enemy));
    while (true){
        if(enemy.y < 2 || enemy.y > MAX_Y - 1) {
            enemy.x -= 1;
            direction *= -1;
        }   
        enemy.y += direction;
        write(fd, &enemy, sizeof(enemy));
        usleep(500000);
    }
}
