#include "global.h"

/* La funzione si occupa di generare le navi nemiche e di richiamare la funzione
   che si occupa della rappresentazione della schermata di gioco.
   Si è reso necessario optare per un if-else in modo da poter sfruttare la break 
   nei processi padre, al fine di evitare la creazione di ^n processi nel ciclo. 
 */
void enemiesGenerator(int *fd){
    int pidEnemy[ENEMIES];
    int enemyCounter;


    for(enemyCounter=0; enemyCounter<ENEMIES; enemyCounter++){
        pidEnemy[enemyCounter] = fork();
        if (pidEnemy[enemyCounter] == -1){
            printf("Errore creazione processo nemico\n");
            exit(2);
        } else{
            if (!pidEnemy[enemyCounter]){
                struct Object enemy=generatore2(enemy,enemyCounter+1);
                close(fd[0]);       // Chiudiamo descrittore in lettura
                enemyShip(fd[1], enemy);   // Gestiamo movimento nemici passano descrittore in scrittura
                close(fd[1]);
                gameAreaV2(fd[0]);
                _exit(0);
            } //else{
                //close(fd[1]);       // Chiudiamo descrittore in scrittura 
                // gameArea(fd[0]);    // Gestiamo rappresentazione area di gioco passando descrittore in lettura
                //gameAreaV2(fd[0]);
               // break;
            //}
        }
    }
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
