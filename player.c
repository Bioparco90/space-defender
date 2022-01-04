#include "global.h"

void playerShip(int fd){
    struct Object ship;
    pid_t pidShotUp;
    pid_t pidShotDown;
    
    ship.y = MAX_Y / 2;
    ship.x = 1;
    ship.identifier = '+';

    write(fd, &ship, sizeof(ship));

    while(true){
        int c = getch();
        //mvaddch(ship.y, ship.x, ' ');
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
                pidShotUp = fork();
                if(pidShotUp == -1){
                    printf("errore sparo");
                    exit(1);
                } else{
                    if (!pidShotUp){
                        shot(fd, DIR_UP);
                    } else{
                        pidShotDown = fork();
                        if(pidShotDown == -1){
                            printf("errore sparo");
                        } else{
                            if(!pidShotDown){
                                shot(fd, DIR_DOWN);
                            } else{
                                wait(NULL);
                                exit(1);
                            }
                        }
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

void shot(int fd, int direction){
    struct Object rocket;

	rocket.x = 2;
	rocket.y = MAX_Y / 2;
    rocket.identifier = 'o';
    rocket.lives = 1;
    rocket.pid = getpid();

    write(fd, &rocket, sizeof(rocket));

    while(true){
		rocket.x++;
        rocket.y += direction;
		// mvaddch(rocket.y, rocket.x-1, ' ');
        // mvprintw(rocket.y, rocket.x, "o");
		// refresh();
		// usleep(15000);
        write(fd, &rocket, sizeof(rocket));
    }
}