#include "global.h"

// Funzione per la generazione delle navi.
// Si occupa della posizione iniziale di spawn e di settare tutti i valori iniziali
// di ogni singola nave. 
// PROMEMORIA: spiegare la logica del posizionamento x e y, che attualmente inizializza
// le navi oltre il limite dello schermo (probabile fix con +2 anzichè l'attuale -2)
struct Object generator(int enemyCounter){
    struct Object enemy;

    enemy.y = 2* ((enemyCounter-1) % (MAX_ENEMY_COL))+2;      // 1 è il numero di spazi tra una nave e l'altra
    enemy.x = MAX_X - ((enemyCounter-1)/(MAX_ENEMY_COL))-2; // 1 è il numero di spazi tra una colonna di navi e l'altra
    enemy.identifier = ENEMY; // Carattere identificativo dell'astronave nemica
    enemy.lives = 3; // lives = 3: nave di primo livello, lives < 3: nave di secondo livello
    enemy.pid = getpid();

    return enemy;
}

void enemyShip(int mainPipe, struct Object enemy){
    int direction = 1;      // Direzione nave: 1 -> basso, -1 -> alto
    int flag = VERTICAL;    // Flag da sfruttare per gestire il movimento verticale senza il fastidioso movimento diagonale       

    write(mainPipe, &enemy, sizeof(enemy));             // Prima scrittura nella mainPipe
    while(true){                                        // Loop movimento nave nemica
        // read(mainPipe, &enemy, sizeof(enemy));         // Lettura dei dati provenienti dal loop di gioco
        switch (flag){
            case VERTICAL:                              // Movimento verticale
                enemy.y += direction + direction;                   // Aggiornamento coordinata Y
                if (enemy.y <= 2 || enemy.y > MAX_Y - 1) // Verifica collisione bordi
                    flag = HORIZONTAL;  // Eventuale modifica del valore flag, che ci manderà al movimento orizzontale
                break;

            case HORIZONTAL:        // Movimento orizzontale
                enemy.x -= 2;       // Aggiornamento coordinata X
                direction *= -1;    // Rimbalzo sul bordo
                flag = VERTICAL;    // Settiamo la flag per tornare al movimento verticale
                break;
        }

        write(mainPipe, &enemy, sizeof(enemy));
        usleep(ENEMY_DELAY);
    }
}
