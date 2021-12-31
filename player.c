#include "global.h"
#include <curses.h>
#include <stdbool.h>

void playerShip(int fd){
    struct Object ship;
    
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
                mvprintw(ship.y, ship.x+1, "-");
        }
        
        mvprintw(ship.y, ship.x, &ship.identifier);
        write(fd, &ship, sizeof(ship));
    }
}