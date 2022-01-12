#include <stdio.h>
#include <string.h>

    char enemySpriteLv1[3][3]={
        {" S "},
        {"<o>"},
        {" S "}
    };

    char enemySpriteLv2[3][3]={
        {"[\\ "},
        {"[@>"},
        {"[/ "}
    };

    char playerSprite[3][3]={
        {"/\\ "},
        {"[]>"},
        {"/\\ "}
    };

int main(){

    int i,j;

    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            printf("%c",enemySpriteLv2[i][j]);
        }
        printf("\n");
    }
    
    // int posY = 2, y;
    // int ENEMIES = 5;
    // int posX;
    // for(i=0; i<ENEMIES; i++){
    //     y = posY;
    //     if(posY >= MAX_Y -1) posX += 2;
    //     x = posX;
    //     posY += 3;
    // }

    return 0;
}