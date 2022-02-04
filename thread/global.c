#include "global.h"

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

Object buffer[DIM_BUFFER];

// Object player;
// Object enemy[ENEMIES];
// Object rocketUp[MAX_ROCKET];
// Object rocketDown[MAX_ROCKET];
// Object enemyRocket[ENEMIES];
// int enemyCounter;

int rocketFrame = 1;

// Funzione per la stampa su schermo della sprite
void printSprite(int posX, int posY, int dimRow, int dimCol, char sprite[dimRow][dimCol]){
    int row, col;
    int x, y;
    x = posX;
    y = posY;

    for(row=0; row<dimRow; row++){
        for(col=0; col<dimCol; col++){
            mvaddch(y, x, sprite[row][col]);
            x++;
        }
        x = posX;
        y++;
    }
}

// Funzione per la cancellazione dallo schermo della sprite
void deleteSprite(Object item){
    int row,col;
    int x, y;
    x = item.x;
    y = item.y;

    for(row=0; row<3; row++){
        for(col=0; col<3; col++){
            mvaddch(y, x, ' ');
            x++;
        }
        x = item.x;
        y++;
    }
}


// Funzione per il riconoscimento di razzi amici e nemici
int isRocket(Object item){
    return (item.identifier == ROCKET_UP || item.identifier == ROCKET_DOWN || item.identifier == ENEMY_ROCKET);
}

// Funzione per la valutazione delle collisioni tra spari e navi.
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
            x = b.x;
            y++;
        }
    }
    return 0;
}

// Funzione per il reset di un oggetto non più utile al gioco (es. un razzo che ha esaurito il suo ciclo vitale)
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

// Funzione per il calcolo del tempo, in secondi, impiegato da un razzo a coprire l'intera area di gioco. 
int timeTravelEnemyRocket(int microSeconds){
    return ((microSeconds * MAX_X) / 1000000);
}

// Funzione per l'animazione del razzo del giocatore
void rocketAnimation(int x, int y){
    switch(rocketFrame){
        case 1:
            mvaddch(y, x, '_');
            rocketFrame++;
            break;

        case 2:
            mvaddch(y, x, '\\');
            rocketFrame++;
            break;

        case 3:
            mvaddch(y, x, '/');
            rocketFrame = 1;
            break;
    }
}

//Funzione per la stampa delle vite del giocatore
void printLives(int lives){
    int x = 0;
    int y = 0;

    mvprintw(y ,x, "Vite: ");
    switch(lives){
        case 3:
            attron(COLOR_PAIR(2));
            mvprintw(y,x+7,"<3");
            mvprintw(y,x+11,"<3");
            mvprintw(y,x+15,"<3");   
            attroff(COLOR_PAIR(2));   
            break;

        case 2:
            attron(COLOR_PAIR(2));
            mvprintw(y,x+7,"<3");
            mvprintw(y,x+11,"<3");
            attroff(COLOR_PAIR(2));
            mvprintw(y,x+15,"  ");  
            break;

        case 1:
            attron(COLOR_PAIR(2));
            mvprintw(y,x+7,"<3");
            attroff(COLOR_PAIR(2));
            mvprintw(y,x+11,"  ");
            mvprintw(y,x+15,"  ");  
            break;
    }
}

int prod_index = 0;
void insert(Object item){
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    buffer[prod_index] = item;
    prod_index = (prod_index + 1) % DIM_BUFFER;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

int cons_index = 0;
Object extract(){
    Object tmp;

    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    tmp = buffer[cons_index];
    cons_index = (cons_index + 1) % DIM_BUFFER;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);

    return tmp;
}