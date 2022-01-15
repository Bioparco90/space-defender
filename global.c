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

int isRocket(Object item){
    return (item.identifier == ROCKET_UP || item.identifier == ROCKET_DOWN); //AGGIUNGERE CASO BOMBA NEMICA SUCCESSIVAMENTE
}

// Forse inutile
int isShip(Object item){
    return (item.identifier == PLAYER || item.identifier == ENEMY);
}

// Per un corretto utilizzo, il parametro "a" deve essere quello che "attacca"
// es: razzo (amico o nemico), nave nemica (collisione con nave giocatore)
// Il parametro "b" l'oggetto che subisce l'attacco di cui sopra.
int checkCollision(Object a, Object b){
    int row, col;
    int x = b.x;
    int y = b.y;

    if (isRocket(a)){
        for(row=0; row<3; row++){
            for(col=0; col<3; col++){
                if(a.x == x && a.y == y)
                    return 1;
                x++;
            }
            x=b.x;
            y++;
        }
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

// //Controlla le collisioni dei razzi, se c'è returna true se non c'è returna false
// int checkCollisionRocket(Object rocket){
    
//     if(mvinch(rocket.y,rocket.x)!=' '){
//         return true;
//     }
//     return false;
// }

// //controlla le collisioni di entità più complesse, sicuramente basta fare un controllo sull'identifier per incomporarle in un'unica funzione
// int checkCollisonEnemy(Object entity){
//     int row,col;
//     int x, y;
//     x=entity.x;
//     y=entity.y;

//     for(row=0;row<3;row++){
//         for(col=0;col<3;col++){
//             if(mvinch(y,x)!=' ')
//                 return true;
//             x++;
//         }
//         x=entity.x;
//         y++;
//     }
//     return false;
// }