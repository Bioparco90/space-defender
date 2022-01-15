#include "global.h"
#include <sys/types.h>
#include <unistd.h>

void playerShip(int fdMain){
    Object ship;
    int rocketSerial;
 
    
    // Inizializzazione nave giocatore
    ship.x = 1;
    ship.y = MAX_Y / 2;
    ship.identifier = PLAYER;
    ship.lives = 3;
    ship.pid = getpid();
    ship.serial = 777;

    // Inizializzazione serial razzi giocatore
    rocketSerial = 0;

    write(fdMain, &ship, sizeof(ship)); // Prima scrittura nella mainPipe

    while(true){
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
                playerShotInit(fdMain, ship.x, ship.y, rocketSerial);
                rocketSerial++;
                break;
        }
        if(rocketSerial >= MAX_ROCKET)
            rocketSerial = 0;
        write(fdMain, &ship, sizeof(ship)); // Scrittura ciclica sulla mainPipe passata al loop di gioco
    }
}

void playerShotInit(int mainPipe, int x, int y, int rocketSerial){
    pid_t pidRocketUp, pidRocketDown;

    pidRocketUp = fork();
    switch (pidRocketUp){
        case -1:
            _exit(-1);
            break;

        case 0:
            shot(mainPipe, x, y, DIR_UP, rocketSerial);
            _exit(0);
            break;

        default:
            pidRocketDown = fork();
            switch (pidRocketDown){
                case -1:
                    _exit(-1);
                    break;

                case 0:
                    shot(mainPipe, x, y, DIR_DOWN, rocketSerial);
                    _exit(0);
                    break;
            }
    }
}

// Funzione da rivedere. Prossimo compito
void shot(int mainPipe, int x, int y, int direction, int rocketSerial){
    Object rocket;

    if(direction == DIR_UP)
        rocket.identifier = ROCKET_UP;
    else
        rocket.identifier = ROCKET_DOWN;

    rocket.x = x + 2;
   	rocket.y = y + 1 + direction;
    rocket.lives = 1;
    rocket.pid = getpid();
    rocket.serial = rocketSerial;

    write(mainPipe, &rocket, sizeof(rocket));
    while(true){
        if((rocket.y < 2 || rocket.y > MAX_Y)){
            direction *= -1;
        }
        rocket.x += 1;
        rocket.y += direction;
        write(mainPipe, &rocket, sizeof(rocket));
		usleep(ROCKET_DELAY);
    }
}
