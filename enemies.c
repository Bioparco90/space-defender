#include "global.h"

//globali
int spawnX = MAX_X-1;
int spawnY = 2;
int flagAvanzata=0;

// codice non globale
struct Object generatore(int i){
    struct Object enemy;
    enemy.x = spawnX;
    // enemy.y = spawnY;
    enemy.y = i;
    // enemy.identifier = ENEMY;
    enemy.identifier = '<';//i-2+97;
    enemy.lives = 2; // o quante sono
    enemy.pid = getpid(); // siamo nel for delle fork in teoria
    // ora possiamo modificare i valori
    spawnY += 2; //spazio vuoto
    if(enemy.y == MAX_Y - 1 || spawnY >= MAX_Y - 1)
        spawnX -= 2; // spazio vuoto

    return enemy;
}

struct Object generatore2(int enemyCounter){
    struct Object enemy;
    enemy.y= 2* ((enemyCounter-1) % (MAX_ENEMY_COL))+2;//1 è il numero di spazi tra una nave e l'altra
    enemy.x= MAX_X - (((enemyCounter-1)/(MAX_ENEMY_COL))*2)-2;//1 è il numero di spazi tra una colonna di navi e l'altra
    enemy.identifier = ENEMY;
    enemy.lives = 2; // o quante sono
    enemy.pid = getpid();

    return enemy;
}


void enemyShip(int mainPipe, int enemyPipe, struct Object enemy){
    int direction=1;
    char controlCharater;
    pid_t pidEnemyRocket;

    //TL;DR stavo provando a testare la funzione enemyRocket in due maniere, in una il fork viene fatto direttamente all'interno della funzione e l'altro in enemyShip
    //Lo switch qua dentro lo fa vedere, un fork è il processo del razzo l'altro fork è quello dei nemici. Non worka perché i cicli di movimento vanno in conflitto,
    //bloccando le navi e lo sparo. Stessa cosa se facessi il fork in enemyRocket. Forse la soluzione potrebbe essere fare tanti processi razzo quanti sono i processi nemici ma non ho avuto tempo di testarla.

   /* switch(pidEnemyRocket=fork()){
        case -1:
            printw("Errore generazione razzo");
        case 0:
            enemyRocket(mainPipe, enemyPipe);
        default:
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
    }*/

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


void enemyRocket(int mainPipe, int enemyPipe){
    struct Object rocket;
    struct Object enemy;
    read(enemyPipe, &enemy, sizeof(enemy));
    rocket.x=enemy.x+1;
    rocket.y=enemy.y;
    rocket.identifier=ENEMY_ROCKET;
    rocket.serial=enemy.serial+ENEMIES;
    rocket.lives=1;
    rocket.pid=getpid();
    
    {

        write(mainPipe,&rocket,sizeof(rocket));

        while(true){
           
            if(random() < RAND_MAX/4){
                do
                {
                    if(rocket.x <= 2){
                        mvprintw(rocket.y,rocket.x," ");
                        break;
                    }
                    rocket.x+=1;
                } while (true);

                 write(mainPipe, &rocket, sizeof(rocket));
                  usleep(600000);
            }

        }
    }
}