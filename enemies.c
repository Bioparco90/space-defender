#include "global.h"

/* La funzione si occupa di generare le navi nemiche e di richiamare la funzione
   che si occupa della rappresentazione della schermata di gioco.
   Si è reso necessario optare per un if-else in modo da poter sfruttare la break 
   nei processi padre, al fine di evitare la creazione di ^n processi nel ciclo. 
 */
void enemiesGenerator(int *fd){

    int i;
    int pidEnemy[ENEMIES];
    for(i=0; i<ENEMIES; i++){
        pidEnemy[i] = fork();
        if (pidEnemy[i] == -1){
            printf("Errore creazione processo nemico\n");
            exit(2);
        } else{
            if (!pidEnemy[i]){
                close(fd[0]);               // Chiudiamo descrittore in lettura
                enemyShipMovement(fd[1]);   // Gestiamo movimento nemici passano descrittore in scrittura
            } else{
                close(fd[1]);       // Chiudiamo descrittore in scrittura 
                gameArea(fd[0]);    // Gestiamo rappresentazione area di gioco passando descrittore in lettura
                break;
            }
        }
    }
}

void enemyShipMovement(int fd){
    struct Object enemy;
    
    enemy.y = MAX_Y / 2;
    enemy.x = MAX_X / 1.5;
    enemy.identifier = '<';
    //un flag che controlla la direzione del gruppo di navicelle se deve andare verso l'alto o verso il basso e cambia al primo rimbalzo contro i bordi della finestra di gioco di una nave nemica.
    int direzioneAlto=true;

    write(fd, &enemy, sizeof(enemy));


    while(true){
        //Se la y della navicella nemica è superiore a 2, limite superiore dello schermo, allora la abbassa di 3 facendola salire.
        if(enemy.y > 2 && direzioneAlto=true)
        {
            enemy.y-=1;
        }
        //Se la y della navicella nemica è minore a 2, ha quindi superato il limite superiore dello schermo, la navicella riscenderà automaticemente di 3 e si spostera in avanti di uno verso il player
        else if(enemy.y <= 2 && direzioneAlto=true)
        {
            enemy.x -=1;
            direzioneAlto=false;
        }

        //Se la y della navicella nemica è inferiore a MAX_Y-1, limite inferiore dello schermo, allora la alza di 1 facendola scendere.
        if(enemy.y < MAX_Y-1) && direzioneAlto=false)
        {
            enemy.y +=1;
        }
        //Se la y della navicella nemica è inferiore a MAX_Y-1, ha quindi superato il limite inferiore dello schermo, la navicella risalirà automaticemente di 3 e si spostera in avanti di uno verso il player
        else if(enemy.y >= MAX_Y-1 && direzioneAlto=false) 
        {
            enemy.x -=1;
            direzioneAlto=true; 
        }
              
        mvprintw(enemy.y, enemy.x, &enemy.identifier);
        write(fd, &enemy, sizeof(enemy));
    }
}






