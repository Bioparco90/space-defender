#include "global.h"

void playerShip(int fdShip, int fdMain){
    struct Object ship, dataMain;
    
    // Inizializzazione nave giocatore
    ship.x = 1;
    ship.y = MAX_Y / 2;
    ship.identifier = PLAYER;
    ship.lives = 3;
    ship.pid = getpid();
    ship.serial = 777;

    write(fdMain, &ship, sizeof(ship)); // Prima scrittura nella mainPipe

    while(true){
        read(fdShip, &dataMain, sizeof(dataMain)); // Lettura dei dati provenienti dal loop di gioco 
        int c = getch();  // Input che verrà valutato nello switch case
        switch (c){
            case KEY_UP:  // Pressione del tasto freccia SU, movimento verso l'alto
                if(ship.y > 2) // Il moviimento avviene solo qualora non si sia raggiunto il limite alto dello schermo
                    ship.y -= 1;
                break;
            
            case KEY_DOWN:  // Pressione del tasto freccia GIU, movimento verso il basso
                if (ship.y < MAX_Y-1) // Il moviimento avviene solo se non è stato raggiunto il limite basso dello schermo
                    ship.y += 1;
                break;
        }
        write(fdMain, &ship, sizeof(ship)); // Scrittura ciclica sulla mainPipe passata al loop di gioco
    }
}

// Funzione da rivedere. Prossimo compito
void shot(int fd, int direction){
    struct Object rocket;

	rocket.x = 2;
	rocket.y = MAX_Y / 2;
    rocket.identifier = 'o';
    rocket.lives = 1;
    rocket.pid = getpid();

    write(fd, &rocket, sizeof(rocket));

    while(true){
		rocket.x++;
        rocket.y += direction;
		// mvaddch(rocket.y, rocket.x-1, ' ');
        // mvprintw(rocket.y, rocket.x, "o");
		// refresh();
		// usleep(15000);
        write(fd, &rocket, sizeof(rocket));
    }
}