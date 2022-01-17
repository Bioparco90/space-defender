#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <bits/time.h>
#include <unistd.h>

// Funzione per la generazione delle navi.
// Si occupa della posizione iniziale di spawn e di settare tutti i valori iniziali
// di ogni singola nave. 
void fleetEnlister(int mainPipe){
    Object enemy[ENEMIES];
    pid_t pidEnemyShip[ENEMIES];
    int posX = MAX_X;
    int posY = 2;
    int i;
    

    for(i=0; i<ENEMIES; i++){
        pidEnemyShip[i] = fork();
        switch(pidEnemyShip[i]){
            case -1:
                endwin();
                printf("Errore creazione processo nemico #%d", i);
                exit(1);

            case 0:
                enemy[i].x = posX;
                enemy[i].y = posY;
                enemy[i].pid = getpid();
                enemy[i].lives = 3;
                enemy[i].serial = i;
                enemy[i].identifier = ENEMY;
                enemyShip(mainPipe, enemy[i]);
        }
        posY += 4;
        if(posY >= MAX_Y -1) {
            posX -= 8;
            posY = 2;
        }
        
    }
}

void enemyShip(int mainPipe,  Object enemy){
    int direction = 1;      // Direzione nave: 1 -> basso, -1 -> alto
    int flag = VERTICAL;    // Flag da sfruttare per gestire il movimento verticale senza il fastidioso movimento diagonale       
    int randomBomb[ENEMIES];
    int i;
    int bombSerial=enemy.serial;

    struct timespec time,checker;

    clock_gettime(CLOCK_REALTIME,&time); 

    write(mainPipe, &enemy, sizeof(enemy));             // Prima scrittura nella mainPipe
    while(true){                                        // Loop movimento nave nemica
        switch (flag){
            case VERTICAL:                              // Movimento verticale
                enemy.y += direction;                   // Aggiornamento coordinata Y
                if (enemy.y <= 2 || enemy.y > MAX_Y - 1) // Verifica collisione bordi
                    flag = HORIZONTAL;  // Eventuale modifica del valore flag, che ci manderà al movimento orizzontale
                break;

            case HORIZONTAL:        // Movimento orizzontale
                enemy.x -= 4;       // Aggiornamento coordinata X
                direction *= -1;    // Rimbalzo sul bordo
                flag = VERTICAL;    // Settiamo la flag per tornare al movimento verticale
                break;
        }

        for(i=0;i<ENEMIES;i++){
                randomBomb[i]=RANDOM_BOMB_START + (rand()%RANDOM_BOMB_FINISH);  
        }
        
        if(randomBomb[bombSerial]<RANDOM_BOMB_FINISH/DELAY_BOMB_RANDOM){
            clock_gettime(CLOCK_REALTIME, &checker);    
            if(checker.tv_sec - time.tv_sec >=1){
                
                enemyBombInit(mainPipe,enemy.x,enemy.y,bombSerial);
                bombSerial++;
                time = checker;
        
            }
        }

        if(bombSerial >=MAX_BOMB)
            bombSerial=0;
        
        write(mainPipe, &enemy, sizeof(enemy));
        usleep(ENEMY_DELAY);
    }
}



void bomb(int mainPipe, int x ,int y, int bombSerial){
        Object bomb;
        
        bomb.y = y+1;    
        bomb.x= x-2;    
        bomb.identifier = BOMB;    
        bomb.lives = 1;    
        bomb.pid = getpid();    
        bomb.serial=bombSerial;

        write(mainPipe, &bomb, sizeof(bomb));    
        while(true){        
            bomb.x-=1;        
            write(mainPipe,&bomb,sizeof(bomb));   
             
            usleep(BOMB_DELAY);    
        }

}

void enemyBombInit(int mainPipe, int x, int y, int bombSerial){
        pid_t pidEnemyBomb;    
        
        switch(pidEnemyBomb=fork()){   

            case -1:            
                _exit(-1);            
                break;        
            
            case 0:            
                bomb(mainPipe, x, y, bombSerial);            
                _exit(0);            
                break;    
        }
}