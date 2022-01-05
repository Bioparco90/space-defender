a.out:	main2.o global.o player.o enemies.o gameplay.o
	cc main2.o global.o player.o enemies.o gameplay.o -o spacedefender.exe -lncurses

main2.o:	main2.c global.h
	cc -c main2.c

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

