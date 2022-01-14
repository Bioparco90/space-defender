#include "global.h"
#include <sys/types.h>
#include <unistd.h>

void playerShip(int fdMain){
     Object ship;
    int ammoSerialUp=-1;
    int ammoSerialDown=0;
 
    
    // Inizializzazione nave giocatore
    ship.x = 1;
    ship.y = MAX_Y / 2;
    ship.identifier = PLAYER;
    ship.lives = 3;
    ship.pid = getpid();
    ship.serial = 777;

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
                //serve per la pausa per l'input, forse non lo conta neanche o l'intervallo è troppo piccolo bisogna vedere.
                usleep(20000);
                //Usiamo due serial differenti per il razzo di su o di giù, potrebbe bastarne uno, rimasuglio di quando ho testato con un vettore statico
                ammoSerialUp+=1;
                ammoSerialDown+=1;
                playerShotInit(fdMain, ship.x, ship.y, ammoSerialUp,ammoSerialDown);
                break;
        }
        
        write(fdMain, &ship, sizeof(ship)); // Scrittura ciclica sulla mainPipe passata al loop di gioco
    }
}

// Funzione da rivedere. Prossimo compito
void shot(int mainPipe, int x, int y, int direction, int ammoSerialUp, int ammoSerialDown){
     Object rocket;
   

	
	rocket.y = y+ 1 + direction;
    rocket.identifier = ROCKET;
    rocket.lives = 1;
    rocket.pid = getpid();

    //Da una serial differente in base se è il razzo di già o di su (rimasuglio di un test con un vettore statico, forse inutile)
    switch(direction){
        case DIR_UP:
            rocket.serial=ammoSerialUp;
            rocket.x = x + 2;
            break;
        case DIR_DOWN:
            rocket.serial=ammoSerialDown;
            rocket.x = x + 2;
            break;
    }

    write(mainPipe, &rocket, sizeof(rocket));//&& (rocket.y >= 2 && rocket.y <= MAX_Y - 1)
    while(rocket.x < MAX_X - 1){

        if((rocket.y < 2 || rocket.y > MAX_Y - 1)){
            direction=(-1*direction);
        }
        rocket.x += 1;
        rocket.y+=direction;
        write(mainPipe, &rocket, sizeof(rocket));
		usleep(ROCKET_DELAY);
    }
}

void playerShotInit(int mainPipe, int x, int y, int ammoSerialUp, int ammoSerialDown){
    pid_t pidRocketUp, pidRocketDown;


    switch (pidRocketUp = fork()){
        case -1:
            _exit(-1);
            break;

        case 0:
            shot(mainPipe, x, y, DIR_UP, ammoSerialUp, ammoSerialDown);
            _exit(0);
            break;
    }

    switch (pidRocketDown = fork()){
        case -1:
            _exit(-1);
            break;

        case 0:
            shot(mainPipe, x, y, DIR_DOWN, ammoSerialUp, ammoSerialDown);
            _exit(0);
            break;
    }
}