all: ringmaster player

ringmaster: ringmaster.cpp potato.h potato.cpp
	g++ -g -o ringmaster ringmaster.cpp potato.cpp
player: player.cpp potato.h potato.cpp
	g++ -g -o player player.cpp potato.cpp

.PHONY:
	clean
clean:
	rm -rf *.o ringmaster player
