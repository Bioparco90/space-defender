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
        attron(A_REVERSE);
        mvprintw(y, x, "%s", message);
        attroff(A_REVERSE);
        mvprintw(y + 2, MAX_X / 2 -5, "%d", i);
        refresh();  
        sleep(1);
    }
    clear();
}


void gameOver(int winCondition, int score){
    int i;
    int maxStar=100;
    int maxUFO=15;
    int randomXL,randomXR, randomYU, randomYD, randomStar, randomXChooser, randomYChooser;
    int countdown=15;


 
    for(i=countdown; i > 0; i--){
        clear();

        for(i=0; i<MAX_X;i++){
            mvprintw(2,i,"- ");
            mvprintw(18,i,"- ");
        }


        if(winCondition==1){
                        
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
                            mvprintw(randomYU,randomXL,"+ ");
                        else if(randomXChooser > 500 && randomYChooser < 500)
                            mvprintw(randomYU,randomXR,"+ ");
                        else if(randomXChooser < 500 && randomYChooser > 500)
                            mvprintw(randomYD,randomXL,"+ ");
                        else if(randomXChooser > 500 && randomYChooser > 500)
                            mvprintw(randomYD, randomXR,"+ ");
                        break;

                }

            }

                mvprintw(MAX_Y/2,MAX_X/2-10,"V I T T O R I A");
                attroff(COLOR_PAIR(3));
                mvprintw(MAX_Y/2+1,MAX_X/2-10,"Punteggio: %d", score);
                

        }
        else if(winCondition==0){

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
}