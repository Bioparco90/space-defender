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

// Funzione per la stampa dei caratteri di countdown
void countdownPrint(int x, int y, int count){

    // Inizializzazione colori
    init_pair(7,COLOR_BLACK,COLOR_RED);
    init_pair(8,COLOR_BLACK,COLOR_YELLOW);
    init_pair(9,COLOR_BLACK,COLOR_GREEN);

    // Valuta quanti secondi mancano all'inizio del gioco e stampa di conseguenza
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

// Funzione per la schermata iniziale del gioco
void startGame(){
    int x, y;
    int i;
    int countdown=3;
    x = MAX_X/5;
    y = 4;

    // Inizializzazione colori
    init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_YELLOW,COLOR_BLACK);
    init_pair(3,COLOR_GREEN,COLOR_BLACK);
    init_pair(4,COLOR_CYAN,COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA,COLOR_BLACK);
    init_pair(6,COLOR_BLUE, COLOR_BLACK);

    // Loop che si occupa della scansione del tempo e della stampa del titolo
    for (i=countdown; i>0; i--){

        // Il contatore del ciclo corrisponde al secondo del countdown
        // Ad ogni secondo i colori della schermata del titolo cambiano
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

        countdownPrint(x, y, i);                        // Richiama la stampa dei secondi mancanti
        refresh();
        system("aplay ../sounds/bip.wav 2> /dev/null &");  // Riproduce un "bip" ogni secondo
        sleep(1);                                       // Scansione del tempo (1 secondo)
    }
    clear();
    refresh();
}

// Funzione per la gestione della schermata di fine gioco
void gameOver(int winCondition, int score){
    int i, j;
    int countdown = 15;     // Durata della schermata di fine gioco
    int maxStar = 100;      // Massimo numero di stelle a schermo
    int maxUFO = 15;        // Massimo numero di UFO a schermo
    int randomXL, randomXR; // Posizioni random asse X a sinistra e destra della schermata
    int randomYU, randomYD; // Posizioni random asse Y in alto e in basso della schermata
    int randomStar;         // Numero corrispondente del tipo di stella da stampare
    
    // Variabili per la scelta della posizione
    int randomXChooser, randomYChooser;
    
    // Inizializzazione colori
    init_pair(3, COLOR_YELLOW,COLOR_BLACK); // Vittoria
    init_pair(1, COLOR_RED,COLOR_BLACK);    // Sconfitta

    // Riproduzione del corretto suono a seconda che il giocatore abbia vinto o perso
    if (winCondition == DEFEAT)
        system("aplay ../sounds/fail1.wav 2> /dev/null &");
    else 
        system("aplay ../sounds/win.wav 2> /dev/null &");

    // Questo ciclo permette di rappresentare la schermata per "countdown" secondi
    for(j=countdown; j > 0; j--){
        clear();

        // Stampa delle righe di delimitazione superiore e inferiore
        for(i=0; i<MAX_X; i++){
            mvprintw(2,i,"- ");
            mvprintw(18,i,"- ");
        }

        // Gestione del caso "vittoria"
        if(winCondition){
            attron(COLOR_PAIR(3));
            for(i=0;i<maxStar;i++){

                // Assegnazione ciclica dei valori alle variabili per successiva scelta            
                randomXL=rand()%(MAX_X/2)+1;
                randomXR=rand()%(MAX_X + 1 - 40) + 40;
                randomYU=rand()%(8 + 1 - 3) + 3;
                randomYD=rand()%(16 + 1 - 12) + 12;
                randomStar=rand()%(3) + 1;
                randomXChooser=rand()%(1000) + 1;
                randomYChooser=rand()%(1000) + 1;

                // Scelta del tipo di stella da rappresentare e conseguente scelta del suo posizionamento
                switch(randomStar){
                    case 1:
                        if(randomXChooser < 500 && randomYChooser < 500)
                            mvprintw(randomYU, randomXL, "* ");
                        else if(randomXChooser > 500 && randomYChooser < 500)
                            mvprintw(randomYU, randomXR, "* ");
                        else if(randomXChooser < 500 && randomYChooser > 500)
                            mvprintw(randomYD, randomXL, "* ");
                        else if(randomXChooser > 500 && randomYChooser > 500)
                            mvprintw(randomYD, randomXR, "* ");
                        break;

                    case 2:
                        if(randomXChooser < 500 && randomYChooser < 500)
                            mvprintw(randomYU, randomXL, "+ ");
                        else if(randomXChooser > 500 && randomYChooser < 500)
                            mvprintw(randomYU, randomXR, "+ ");
                        else if(randomXChooser < 500 && randomYChooser > 500)
                            mvprintw(randomYD, randomXL, "+ ");
                        else if(randomXChooser > 500 && randomYChooser > 500)
                            mvprintw(randomYD, randomXR, "+ ");
                        break;
                            
                    case 3:
                        if(randomXChooser < 500 && randomYChooser < 500)
                            mvprintw(randomYU, randomXL, "o ");
                        else if(randomXChooser > 500 && randomYChooser < 500)
                            mvprintw(randomYU, randomXR, "o ");
                        else if(randomXChooser < 500 && randomYChooser > 500)
                            mvprintw(randomYD, randomXL, "o ");
                        else if(randomXChooser > 500 && randomYChooser > 500)
                            mvprintw(randomYD, randomXR, "o ");
                        break;
                }
            }

            // Stampa del messaggio di vittoria e punteggio
            mvprintw(MAX_Y/2,MAX_X/2-10,"V I T T O R I A");
            attroff(COLOR_PAIR(3));
            mvprintw(MAX_Y/2+1,MAX_X/2-10,"Punteggio: %d", score);
        }

        // Gestione del caso "sconfitta".
        else if(!winCondition){
            attron(COLOR_PAIR(1));
            for(i=0;i<maxUFO;i++){

                // Assegnazione ciclica dei valori alle variabili per successiva scelta            
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
    system("killall aplay");    // Allo scadere del countdown interrompe anche la riproduzione dell'audio
}

