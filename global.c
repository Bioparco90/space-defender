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


void deleteSprite(Object item){
    int row,col;
    int x, y;
    x = item.x;
    y = item.y;

    for(row=0;row<3;row++){
        for(col=0;col<3;col++){
            mvaddch(y,x,' ');
            x++;
        }
        x = item.x;
        y++;
    }
}

int isWeapon(Object item){
    return (item.identifier == ROCKET_UP || item.identifier == ROCKET_DOWN || item.identifier == BOMB); //AGGIUNGERE CASO BOMBA NEMICA SUCCESSIVAMENTE
}

// Per un corretto utilizzo, il parametro "a" deve essere quello che "attacca"
// es: razzo (amico o nemico), nave nemica (collisione con nave giocatore)
// Il parametro "b" l'oggetto che subisce l'attacco di cui sopra.
int checkCollision(Object a, Object b){
    int row, col;
    int x = b.x;
    int y = b.y;

    if (isWeapon(a)){
        for(row=0; row<3; row++){
            for(col=0; col<3; col++){
                if(a.x == x && a.y == y)
                    return 1;
                x++;
            }
            x = b.x;
            y++;
        }
    }
    else{
        if(a.x==b.x || a.x==0)
            return 1;
    }
    return 0;
}

Object resetItem(){
    Object item;

    item.identifier = 'n';
    item.x = -1;
    item.y = -1;
    item.lives = -1;
    item.pid = -1;
    item.serial = -1;

    return item;
}
