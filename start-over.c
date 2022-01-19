#include "global.h"
#include <curses.h>
#include <sys/types.h>
#include <string.h>

void startGame(){
    char *message = "S P A C E * D E F E N D E R";
    int countdown = 3;
    int x, y;
    int i;

    x = MAX_X / 2 - (strlen(message) / 2 + 3);
    y = MAX_Y / 2;

    for (i=countdown; i>0; i--){
        mvprintw(y, x, "%s", message);
        mvprintw(y + 2, MAX_X / 2 -5, "%d", i);
        refresh();  
        sleep(1);
    }
    clear();
}