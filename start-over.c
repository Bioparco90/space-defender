#include "global.h"

char space[6][47]={
    {"  _____   _____                _____   ______ \n"},
    {" / ____| |  __ \\      /\\      / ____| |  ____|\n"},
    {"| (___   | |__) |    /  \\    | |      | |__   \n"},
    {" \\___ \\  |  ___/    / /\\ \\   | |      |  __|  \n"},
    {" ____) | | |       / ____ \\  | |____  | |____ \n"},
    {"|_____/  |_|      /_/    \\_\\  \\_____| |______|\n"}
};

char star[6][9]={
    {"    _    "},
    {" /\\| |/\\ "},
    {" \\ ` ' / "},
    {"|_     _|"},
    {" / , . \\ "},
    {" \\/|_|\\/ "}
};

char defender[6][70]={
    {" _____    ______   ______   ______   _   _   _____    ______   _____  "},
    {"|  __ \\  |  ____| |  ____| |  ____| | \\ | | |  __ \\  |  ____| |  __ \\ "},
    {"| |  | | | |__    | |__    | |__    |  \\| | | |  | | | |__    | |__) |"},
    {"| |  | | |  __|   |  __|   |  __|   | . ` | | |  | | |  __|   |  _  / "},
    {"| |__| | | |____  | |      | |____  | |\\  | | |__| | | |____  | | \\ \\ "},
    {"|_____/  |______| |_|      |______| |_| \\_| |_____/  |______| |_|  \\_\\"}
};

void countdownPrint(int x, int y, int count){

    init_pair(7,COLOR_BLACK,COLOR_RED);
    init_pair(8,COLOR_BLACK,COLOR_YELLOW);
    init_pair(9,COLOR_BLACK,COLOR_GREEN);

    switch(count){

        case 3:
            attron(COLOR_PAIR(7));
            mvprintw(y+9,x+21, "I");
            attroff(COLOR_PAIR(7));

            attron(COLOR_PAIR(7));
            mvprintw(y+9,x+23, "I");
            attroff(COLOR_PAIR(7));

            attron(COLOR_PAIR(7));
            mvprintw(y+9,x+25, "I");
            attroff(COLOR_PAIR(7));
            break;

        case 2:
            attron(COLOR_PAIR(8));
            mvprintw(y+9,x+22, "I");
            attroff(COLOR_PAIR(8));

            attron(COLOR_PAIR(8));
            mvprintw(y+9,x+24, "I");
            attroff(COLOR_PAIR(8));
            break;

        case 1:
            attron(COLOR_PAIR(9));
            mvprintw(y+9,x+23, "I");
            attroff(COLOR_PAIR(9));
            break;


    }

}

void startGame(){
    //char *message = "S P A C E * D E F E N D E R";
    int x, y;
    int i;
    int countdown=3;
    x = MAX_X/5;
    y = 4;

    init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_YELLOW,COLOR_BLACK);
    init_pair(3,COLOR_GREEN,COLOR_BLACK);
    init_pair(4,COLOR_CYAN,COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA,COLOR_BLACK);
    init_pair(6,COLOR_BLUE, COLOR_BLACK);

    
    for (i=countdown; i>0; i--){


        switch(i){
            case 3:
            attron(COLOR_PAIR(1));
            printSprite(x+19,y+6,6,9,star);
            attroff(COLOR_PAIR(1));

            attron(COLOR_PAIR(4));
            printSprite(x,y,6,47,space);
            attroff(COLOR_PAIR(4));

            attron(COLOR_PAIR(5));
            printSprite(x-10,y+12,6,70,defender);
            attroff(COLOR_PAIR(5));
            break;

        case 2:
            attron(COLOR_PAIR(2));
            printSprite(x+19,y+6,6,9,star);
            attroff(COLOR_PAIR(2));

            attron(COLOR_PAIR(5));
            printSprite(x,y,6,47,space);
            attroff(COLOR_PAIR(5));

            attron(COLOR_PAIR(3));
            printSprite(x-10,y+12,6,70,defender);
            attroff(COLOR_PAIR(3));
            break;

        case 1:
            attron(COLOR_PAIR(3));
             printSprite(x+19,y+6,6,9,star);
            attroff(COLOR_PAIR(3));

            attron(COLOR_PAIR(6));
            printSprite(x,y,6,47,space);
            attroff(COLOR_PAIR(6));

            attron(COLOR_PAIR(1));
            printSprite(x-10,y+12,6,70,defender);
            attroff(COLOR_PAIR(1));
            break;
        }

    
        //mvprintw(y, x, "%s", message);
        countdownPrint(x, y, i);
        refresh();
        system("aplay sounds/bip.wav 2> /dev/null &");
        sleep(1);
    }
    clear();
    refresh();
}


void gameOver(int winCondition, int score){
    int i, j;
    int maxStar=100;
    int maxUFO=15;
    int randomXL,randomXR, randomYU, randomYD, randomStar, randomXChooser, randomYChooser;
    int countdown=15;

    init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);

    if (winCondition == DEFEAT)
        system("aplay sounds/fail1.wav 2> /dev/null &");
    else 
        system("aplay sounds/win.wav 2> /dev/null &");
 
    for(j=countdown; j > 0; j--){
        clear();

        for(i=0; i<MAX_X;i++){
            mvprintw(2,i,"- ");
            mvprintw(18,i,"- ");
        }


        if(winCondition==WIN){
                        
            attron(COLOR_PAIR(3));
            for(i=0;i<maxStar;i++){
                        
                randomXL=rand()%(MAX_X/2)+1;
                randomXR=rand()%(MAX_X + 1 - 40) + 40;
                randomYU=rand()%(8 + 1 - 3) + 3;
                randomYD=rand()%(16 + 1 - 12) + 12;
                randomStar=rand()%(3) + 1;
                randomXChooser=rand()%(1000) + 1;
                randomYChooser=rand()%(1000) + 1;

                switch(randomStar){

                    case 1:
                        if(randomXChooser < 500 && randomYChooser < 500)
                            mvprintw(randomYU,randomXL,"* ");
                        else if(randomXChooser > 500 && randomYChooser < 500)
                            mvprintw(randomYU,randomXR,"* ");
                        else if(randomXChooser < 500 && randomYChooser > 500)
                            mvprintw(randomYD,randomXL,"* ");
                        else if(randomXChooser > 500 && randomYChooser > 500)
                            mvprintw(randomYD, randomXR,"* ");
                        break;

                    case 2:
                        if(randomXChooser < 500 && randomYChooser < 500)
                            mvprintw(randomYU,randomXL,"+ ");
                        else if(randomXChooser > 500 && randomYChooser < 500)
                            mvprintw(randomYU,randomXR,"+ ");
                        else if(randomXChooser < 500 && randomYChooser > 500)
                            mvprintw(randomYD,randomXL,"+ ");
                        else if(randomXChooser > 500 && randomYChooser > 500)
                            mvprintw(randomYD, randomXR,"+ ");
                        break;
                            
                    case 3:
                        if(randomXChooser < 500 && randomYChooser < 500)
                            mvprintw(randomYU,randomXL,"o ");
                        else if(randomXChooser > 500 && randomYChooser < 500)
                            mvprintw(randomYU,randomXR,"o ");
                        else if(randomXChooser < 500 && randomYChooser > 500)
                            mvprintw(randomYD,randomXL,"o ");
                        else if(randomXChooser > 500 && randomYChooser > 500)
                            mvprintw(randomYD, randomXR,"o ");
                        break;

                }

            }

                mvprintw(MAX_Y/2,MAX_X/2-10,"V I T T O R I A");
                attroff(COLOR_PAIR(3));
                mvprintw(MAX_Y/2+1,MAX_X/2-10,"Punteggio: %d", score);
                

        }
        else if(winCondition==DEFEAT){

            attron(COLOR_PAIR(1));
            


            for(i=0;i<maxUFO;i++){

                    randomXL=rand()%(MAX_X/2)+1;
                    randomXR=rand()%(MAX_X + 1 - 40) + 40;
                    randomYU=rand()%(8 + 1 - 3) + 3;
                    randomYD=rand()%(16 + 1 - 12) + 12;
                    randomXChooser=rand()%(1000) + 1;
                    randomYChooser=rand()%(1000) + 1;
                
                    if(randomXChooser < 500 && randomYChooser < 500)
                        mvprintw(randomYU,randomXL,"_/\\_ ");
                    else if(randomXChooser > 500 && randomYChooser < 500)
                        mvprintw(randomYU,randomXR,"_/\\_ ");
                    else if(randomXChooser < 500 && randomYChooser > 500)
                        mvprintw(randomYD,randomXL,"_/\\_ ");
                    else if(randomXChooser > 500 && randomYChooser > 500)
                        mvprintw(randomYD, randomXR,"_/\\_ ");
                                            

            }
        
            mvprintw(MAX_Y/2,MAX_X/2-10,"S E I  M O R T O");
            attroff(COLOR_PAIR(1));
            mvprintw(MAX_Y/2+1,MAX_X/2-10,"Punteggio: %d",score);
        
        }
        refresh();  
        sleep(1);
    }
    system("killall aplay");
}

