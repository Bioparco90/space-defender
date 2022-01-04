#include "global.h"

/* La funzione si occupa di generare le navi nemiche e di richiamare la funzione
   che si occupa della rappresentazione della schermata di gioco.
   Si è reso necessario optare per un if-else in modo da poter sfruttare la break 
   nei processi padre, al fine di evitare la creazione di ^n processi nel ciclo. 
 */
void enemiesGenerator(int *fd){
    struct Object enemy;
    int i;
    int pidEnemy[ENEMIES];
    for(i=0; i<ENEMIES; i++){
        pidEnemy[i] = fork();
        if (pidEnemy[i] == -1){
            printf("Errore creazione processo nemico\n");
            exit(2);
        } else{
            if (!pidEnemy[i]){
                close(fd[0]);       // Chiudiamo descrittore in lettura
                generatore(fd[0], &enemy);
                enemyShip(fd[1]);   // Gestiamo movimento nemici passano descrittore in scrittura
            } else{
                close(fd[1]);       // Chiudiamo descrittore in scrittura 
                gameArea(fd[0]);    // Gestiamo rappresentazione area di gioco passando descrittore in lettura
                break;
            }
        }
    }
}


// Versione 2
void enemyShip(int fd, struct Object enemy){
    int passo=1;

    // Coordinate debug
    // enemy.x = MAX_X - 1;
    // enemy.y = MAX_Y / 2;
    // enemy.identifier = '<';

    write(fd, &enemy, sizeof(enemy));

    while (true){
        //mvaddch(enemy.y, enemy.x, ' ');        
        if(enemy.y <= 2 || enemy.y > MAX_Y - 1) {
            enemy.x -= 1;
            passo *= -1;
            //mvprintw(enemy.y, enemy.x, &enemy.identifier);
            // mvaddch(enemy.y, enemy.x, ' ');
        }   
        enemy.y += passo;

        //mvprintw(enemy.y, enemy.x, &enemy.identifier);
        write(fd, &enemy, sizeof(enemy));

        usleep(500000);
        refresh();
    }
}
