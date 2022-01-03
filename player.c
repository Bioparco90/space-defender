#include "global.h"

void playerShip(int fd){
    struct Object ship;
    pid_t pidShot;
    
    ship.y = MAX_Y / 2;
    ship.x = 1;
    ship.identifier = '+';

    write(fd, &ship, sizeof(ship));

    while(true){
        int c = getch();
        mvaddch(ship.y, ship.x, ' ');
        switch (c){
            case KEY_UP:
                if(ship.y > 2) 
                    ship.y -= 1;
                break;
            
            case KEY_DOWN:
                if (ship.y < MAX_Y-1)
                    ship.y += 1;
                break;
            
            case ' ':                
                pidShot = fork();
                if(pidShot == -1){
                    printf("errore sparo");
                } else{
                    if (!pidShot){
                        rocket(fd);
                    } else{
                        wait(NULL);
                    }
                }
                // mvprintw(ship.y, ship.x+1, "o");
                break;

            default:
                break;
        }
        
        mvprintw(ship.y, ship.x, &ship.identifier);
        write(fd, &ship, sizeof(ship));
    }
}

void rocket(int fd){
    struct Object rocketUp;
    struct Object rocketDown;

	rocketUp.x = 2;
	rocketUp.y = MAX_Y / 2;

    write(fd, &rocketUp, sizeof(rocketUp));

    while(rocketUp.x < MAX_X){
		rocketUp.x++;
		mvaddch(rocketUp.y, rocketUp.x-1, ' ');
        mvprintw(rocketUp.y, rocketUp.x, "o");
		refresh();
		usleep(15000);
    }
}