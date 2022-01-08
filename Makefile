a.out:	main.o global.o player.o enemies.o gameplay.o
	cc main.o global.o player.o enemies.o gameplay.o -o spacedefender.exe -lncurses

main.o:	main.c global.h
	cc -c main.c

global.o: global.c global.h
	cc -c global.c

player.o: player.c global.h
	cc -c player.c

enemies.o: enemies.c global.h
	cc -c enemies.c

gameplay.o: gameplay.c global.h
	cc -c gameplay.c

clean:
	rm -f *.o

