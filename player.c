#include "global.h"
#include <sys/types.h>
#include <unistd.h>

void playerShip(int fdMain){
    struct Object ship;
    
    // Inizializzazione nave giocatore
    ship.x = 1;
    ship.y = MAX_Y / 2;
    ship.identifier = PLAYER;
    ship.lives = 3;
    ship.pid = getpid();
    ship.serial = 777;

    write(fdMain, &ship, sizeof(ship)); // Prima scrittura nella mainPipe

    while(true){
        // read(fdMain, &ship, sizeof(ship)); // Lettura dei dati provenienti dal loop di gioco 
        int c = getch();  // Input che verrà valutato nello switch case
        switch (c){
            case KEY_UP:  // Pressione del tasto freccia SU, movimento verso l'alto
                if(ship.y > 2) // Il moviimento avviene solo qualora non si sia raggiunto il limite alto dello schermo
                    ship.y -= 1;
                break;
            
            case KEY_DOWN:  // Pressione del tasto freccia GIU, movimento verso il basso
                if (ship.y < MAX_Y-1) // Il movimento avviene solo se non è stato raggiunto il limite basso dello schermo
                    ship.y += 1;
                break;

            case ' ':
                playerShotInit(fdMain, ship.x, ship.y);
                break;
        }
        write(fdMain, &ship, sizeof(ship)); // Scrittura ciclica sulla mainPipe passata al loop di gioco
    }
}

// Funzione da rivedere. Prossimo compito
void shot(int mainPipe, int x, int y, int direction){
    struct Object rocket;

	rocket.x = x + 1;
	rocket.y = y + direction;
    rocket.identifier = ROCKET;
    rocket.lives = 1;
    rocket.pid = getpid();

    write(mainPipe, &rocket, sizeof(rocket));
    while(rocket.x < MAX_X - 1 && (rocket.y >= 2 && rocket.y <= MAX_Y - 1)){
		rocket.x += 9;
        rocket.y += direction;
        write(mainPipe, &rocket, sizeof(rocket));
		usleep(100000);
    }
}

void playerShotInit(int mainPipe, int x, int y){
    pid_t pidRocketUp, pidRocketDown;

    switch (pidRocketUp = fork()){
        case -1:
            _exit(-1);
            break;

        case 0:
            shot(mainPipe, x, y, DIR_UP);
            _exit(0);
            break;
    }

    switch (pidRocketDown = fork()){
        case -1:
            _exit(-1);
            break;

        case 0:
            shot(mainPipe, x, y, DIR_DOWN);
            _exit(0);
            break;
    }
}