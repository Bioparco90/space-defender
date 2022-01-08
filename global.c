#include "global.h"


void gameAreaV3(int mainPipe, int playerPipe, int enemyPipe[][2]){
	struct Object data, dataEnemy[ENEMIES], dataEnemyPrev[ENEMIES], player, enemy, playerRocket, enemyRocket;
	int collision = 0;
    int i;
    char actualChar;


	player.x = -1;
	enemy.y = -1;
	playerRocket.x = -1;
    enemyRocket.x=-1;
    
	do{
        read(mainPipe, &data, sizeof(data));
        if (data.identifier == PLAYER){
            if(player.x >= 0)
                mvaddch(player.y, player.x, ' ');
            player = data;
            write(playerPipe, &player, sizeof(player));
        } else{
            if (data.serial < ENEMIES){
                if(enemy.y >= 0)
                    mvaddch(enemy.y, enemy.x, ' ');
                enemy = data;
                write(enemyPipe[enemy.serial][WRITE], &enemy, sizeof(enemy)); // VERIFICARE
                dataEnemy[enemy.serial] = enemy; //test
            } else{
                if (data.identifier == ROCKET){
                    if(playerRocket.x >= 0)
                        mvaddch(playerRocket.y, playerRocket.x, ' ');
                playerRocket = data;
                }
                else{
                    if (data.identifier==ENEMY_ROCKET){
                        if(enemyRocket.x>=0)
                             actualChar=mvinch(enemyRocket.y,enemyRocket.x);
                             if(actualChar!=' '){
                                mvaddch(enemyRocket.y,enemyRocket.x,actualChar);
                            }
                            mvaddch(enemyRocket.y,enemyRocket.x,' ');
                        enemyRocket= data;
                    }
                }
            }	
        }
        clear();
        // mvaddch(data.y, data.x, data.identifier);
        mvaddch(player.y, player.x, player.identifier);
        for(i=0;i<ENEMIES; i++)
            mvaddch(dataEnemy[i].y, dataEnemy[i].x, dataEnemy[i].identifier);
        mvaddch(playerRocket.y,playerRocket.x,playerRocket.identifier);
        mvaddch(enemyRocket.y,enemyRocket.x,enemyRocket.identifier);
        mvprintw(0, 1, "Vite: %d", player.lives);
        refresh();
    
	} while (!collision);
}

