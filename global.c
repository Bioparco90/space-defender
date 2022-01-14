#include "global.h"
#include <curses.h>

// Le righe commentate servono in caso di sprite 3x3
void printSprite(int posX, int posY, char sprite[3][3]){

    int row,col;
    int x, y;
    x=posX;
    y=posY;

    for(row=0;row<3;row++){
        for(col=0;col<3;col++){
            mvaddch(y,x,sprite[row][col]);
            x++;
        }
        x=posX;
        y++;
    }
}


void deleteSprite(int posX, int posY){

    int row,col;
    int x, y;
    x=posX;
    y=posY;

    for(row=0;row<3;row++){
        for(col=0;col<3;col++){
            mvaddch(y,x,' ');
            x++;
        }
        x=posX;
        y++;
    }
}


//Controlla le collisioni dei razzi, se c'è returna true se non c'è returna false
int checkCollisionRocket(Object rocket){
    
    if(mvinch(rocket.y,rocket.x)!=' '){
        return true;
    }
    return false;
}

//controlla le collisioni di entità più complesse, sicuramente basta fare un controllo sull'identifier per incomporarle in un'unica funzione
int checkCollisonEnemy(Object entity){
    int row,col;
    int x, y;
    x=entity.x;
    y=entity.y;

    for(row=0;row<3;row++){
        for(col=0;col<3;col++){
            if(mvinch(y,x)!=' ')
                return true;
            x++;
        }
        x=entity.x;
        y++;
    }
    return false;
}

Object resetItem(){
    Object item;

    item.identifier = 'n';
    item.x = MAX_X;
    item.y = MAX_Y +1;
    item.lives = -1;
    item.pid = -1;
    item.serial = -1;

    return item;
}