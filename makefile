final: go_fish RoboDealer player deck card
	g++ -o gofish go_fish.o RoboDealer.o player.o deck.o card.o
go_fish: 
	g++ -std=c++11 -O0 -g -Wall -Werror -o go_fish.o -c go_fish.cpp
RoboDealer:
	g++ -std=c++11 -O0 -g -Wall -Werror -o RoboDealer.o -c RoboDealer.cpp
player:
	g++ -std=c++11 -O0 -g -Wall -Werror -o player.o -c player.cpp
deck:
	g++ -std=c++11 -O0 -g -Wall -Werror -o deck.o -c deck.cpp
card:
	g++ -std=c++11 -O0 -g -Wall -Werror -o card.o -c card.cpp