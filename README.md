space-defender-2

1) Verificare correttezza del ciclo for per la creazione dei processi delle navi nemiche


2) Prototipo: **void playerShip(int fd);**
    Creare funzione playerShip per gestire il movimento della nave giocatore. La funzione riceve in ingresso 
    il descrittore aperto in scrittura


3) Prototipo: **void enemyShip(int fd);**
    Creare funzione enemyShip per gestire il movimento delle navi nemiche. La funzione riceve in ingresso 
    il descrittore aperto in scrittura


4) Prototipo: **void gameArea(int fd);**
    Creare funzione controllo per gestire la rappresentazione grafica degli oggetti in gioco. La funzione riceve in ingresso 
    il descrittore aperto in lettura


Valutare la creazione di un processo "flotta nemica" che faccia da padre a tutti i processi "nave nemica". 
Potrebbe esser necessario per ovviare ad alcuni eventuali errori (info collega) ma anche per mantenere il codice del main più pulito