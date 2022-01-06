#include "global.h"

void gameAreaV2(int fd, int enemyPipe[][2]){
	struct Object data, dataEnemy[ENEMIES], player, enemy, rocket;
	int collision = 0;

	player.x = -1;
	enemy.y = -1;
	rocket.x = -1;
	
	int i;
	for(i=0;i<ENEMIES; i++){
		read(*enemyPipe[i], &dataEnemy[i], sizeof(dataEnemy[i]));
	}

	do{
		read(fd, &data, sizeof(data));
		if (data.identifier == PLAYER){
			if(player.x >= 0)
				mvaddch(player.y, player.x, ' ');
			player = data;
		} else{
			for(i=0; i<ENEMIES; i++)
				enemy = dataEnemy[i];
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
	} while (!collision);

	
}

/* Tentativo da fare:
	usare unica pipe assegnando ad ogni nave un numero di serie.
	Es: Nave gioatore = 0, nave nemici 1->ENEMIES
*/