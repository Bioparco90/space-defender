#include "global.h"


void gameAreaV3(int mainPipe, int playerPipe, int enemyPipe[][2]){
	struct Object data, dataEnemy[ENEMIES], player, enemy, rocket;
	int collision = 0;
    int i;

	player.x = -1;
	enemy.y = -1;
	rocket.x = -1;
    
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
            } else{
                if (data.identifier == ROCKET){
                    if(rocket.x >= 0)
                        mvaddch(rocket.y, rocket.x, ' ');
                rocket = data;
                }
            }	
        }
        mvaddch(data.y, data.x, data.identifier);
        mvprintw(0, 1, "Vite: %d", player.lives);
        refresh();
    
	} while (!collision);
}