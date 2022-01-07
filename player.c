#include "global.h"

void playerShip(int fdShip, int fdMain){
    struct Object ship, dataMain;
    pid_t pidShot;
    //pid_t pidShotUp;
    //pid_t pidShotDown;
    
    ship.y = MAX_Y / 2;
    ship.x = 1;
    ship.identifier = PLAYER;
    ship.lives = 3;
    ship.pid = getpid();
    ship.serial = 777;

    write(fdMain, &ship, sizeof(ship));

    while(true){
        read(fdShip, &dataMain, sizeof(dataMain));
        int c = getch();
        switch (c){
            case KEY_UP:
                if(ship.y > 2) 
                    ship.y -= 1;
                break;
            
            case KEY_DOWN:
                if (ship.y < MAX_Y-1)
                    ship.y += 1;
                break;


            //Questa versione spara un proiettile verso il basso ma poi la nave si blocca e non si può muovere, era un test per vedere se bastava usare una fork con il processo padre e il figlio senza usarne due.
            //Da riprovare.
            
             case ' ':      
                pidShot=fork();
                
                if(pidShot==-1){
                    printf("Errore Sparo");
                }
                else if(pidShot==0){
                      shot(fdMain, DIR_UP);
                      
                }
                else{
                    shot(fdMain, DIR_DOWN);
                   
                }
                break;     



                //Questo è una prova per gli shot, questo li spara senza bloccare la nave ma flickerano nello schermo quando vengono stampati e sopratutto ne parte uno a caso tra i due spammando.
                //Probabilmente ha a che fare col fatto che non vi sia un delay tra i due processi e si intervallino tra loro
                
               /* pidShotUp=fork();
                pidShotDown=fork();
                
                if(pidShotUp==-1 || pidShotDown==-1){
                    printf("Errore Sparo");
                }
                else if(pidShotUp==0){
                      shot(fdMain, DIR_UP);
                        _exit(0);
                }
                else if(pidShotDown==0){
                    shot(fdMain, DIR_DOWN);
                    _exit(0);
                }
                break;*/
            
            // case ' ':                
            //     pidShotUp = fork();
            //     if(pidShotUp == -1){
            //         printf("errore sparo");
            //         exit(1);
            //     } else{
            //         if (!pidShotUp){
            //             shot(fd, DIR_UP);
            //         } else{
            //             pidShotDown = fork();
            //             if(pidShotDown == -1){
            //                 printf("errore sparo");
            //             } else{
            //                 if(!pidShotDown){
            //                     shot(fd, DIR_DOWN);
            //                 } else{
            //                     wait(NULL);
            //                     exit(1);
            //                 }
            //             }
            //         }
            //     }
            //     break;

            default:
                break;
        }
        
        // mvprintw(ship.y, ship.x, &ship.identifier);
        write(fdMain, &ship, sizeof(ship));
    }
}

void shot(int fdMain, int direction){
    struct Object rocket;

	rocket.x = 2;
	rocket.y = MAX_Y / 2;
    rocket.identifier = ROCKET;
    rocket.lives = 1;
    rocket.pid = getpid();

    write(fdMain, &rocket, sizeof(rocket));

    while(true){
		rocket.x +=8;
        rocket.y += direction;
		// mvaddch(rocket.y, rocket.x-1, ' ');
        // mvprintw(rocket.y, rocket.x, "o");
		// refresh();
         write(fdMain, &rocket, sizeof(rocket));
	    usleep(100000);
       
    }
}