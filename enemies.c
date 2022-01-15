#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
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
            posX += 8;
            posY = 2;
        }
        
    }
}

void enemyShip(int mainPipe,  Object enemy){
    int direction = 1;      // Direzione nave: 1 -> basso, -1 -> alto
    int flag = VERTICAL;    // Flag da sfruttare per gestire il movimento verticale senza il fastidioso movimento diagonale       

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

        write(mainPipe, &enemy, sizeof(enemy));
        usleep(ENEMY_DELAY);
    }
}
