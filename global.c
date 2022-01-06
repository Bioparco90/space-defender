#include "global.h"


void gameAreaV3(int pipe[][2]){
	struct Object data, dataEnemy[ENEMIES], player, enemy, rocket;
	int collision = 0;
    int i;

	player.x = -1;
	enemy.y = -1;
	rocket.x = -1;
    
	do{
        for(i=0; i<ENEMIES+1; i++){
            read(**pipe, &data, sizeof(data));
            if (data.identifier == PLAYER){
                if(player.x >= 0)
                    mvaddch(player.y, player.x, ' ');
                player = data;
            } else{
                if (data.identifier == ENEMY){
                    if(enemy.y >= 0)
                        mvaddch(enemy.y, enemy.x, ' ');
                    enemy = data;
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
        }
	} while (!collision);
}