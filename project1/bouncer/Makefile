#authors: Maks Cegielski-Johnson and John Ballard
#cs3505 - Project 1 Checkpoint 3 makefile

all: bouncer

bouncer:	bouncer.o ball.o
	g++ -o bouncer bouncer.o ball.o `pkg-config --cflags --libs libswscale libavformat libavcodec`

bouncer.o:	bouncer.cpp ball.h
	g++ -c bouncer.cpp `pkg-config --cflags --libs libswscale libavformat libavcodec`

ball.o: ball.cpp ball.h
	g++ -c ball.cpp

clean:
	rm -f *.mpff *.o *.mp4 bouncer