#include "global.h"

void printSprite(int posX, int posY, char sprite[3][1]){

    int row,col;
    int x, y;
    x=posX;
    y=posY;

    for(row=0;row<3;row++){
        for(col=0;col<1;col++){
            mvaddch(y,x,sprite[row][col]);
            // x++;
        }
        // x=posX;
        y++;
    }
}
