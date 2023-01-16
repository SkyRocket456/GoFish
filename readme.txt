Go Fish!
EE 312 
Programming Project 6
By Kenneth Emeremnu
University of Texas at Austin

What does this program do?
This program randomly generates a game of Go Fish between the players Kenny, Malik, Andrew, and Jonah.

The rules of Go Fish for this program are as listed:
-Before the game starts, each player picks a card from the top of the deck to establish a rotation. The player who picked the 
 lowest card goes first, 2nd lowest goes second, etc

-5 cards are dealt to each player in the beginning. (This would be 7 cards if there were two players)

-If there are no more cards in the deck, the game is played as normal except the players can't fish for more cards

-A player asks another player for a rank 
	1.If the player asked has the rank asked for, the asker takes all the cards of that rank
	   from the player asked and takes another turn.
	2. If the player asked does not have the rank asked for, the player asked says "Go Fish!" and the asker 
	   has to take a card from the deck if there are cards left. The turn is then passed to the next player in the rotation. 

-If a player takes a turn and their hand is empty, they take a card from the deck if there are cards left and the turn is passed to 
 the next player in the rotation

-If a player is the only one with cards in the circle, they can ask anyone for a random rank and take a card from 
 the deck if there are cards left. The next player in the rotation goes next and the game is then continued as normal

Contents:
go_fish.cpp: contains the main function that automates the game which calls functions defined in other files 

RoboDealer.h: header file that defines the class RoboDealer, a class that allows the creation of an electronic dealer that is 
essential for automation of the game for the main function, and contains the declarations for its functions

RoboDealer.cpp: contains defined functions for the class RoboDealer

player.h: header file that defines the class Player, a class that allows the creation of a player, and contains the declarations for its functions

player.cpp: contains defined functions for the class Player

Deck.h: header file that defines the class Deck, a class that allows the creation of a deck of cards, and contains the declarations for its functions

Deck.cpp: contains defined functions for the class Deck 

card.h: header file that defines the class Card, a class that allows the creation of a card with a rank and suit, and contains the declarations for its functions

card.cpp: contains defined functions for the class Card 

makefile: file that is used on unix systems to prepare the file for execution

Installation:
1. Unzip the file on the command line
    Syntax: "unzip <file_name.zip>" (for this program, file_name.zip == prog06_kie226.zip)
2. Run the command "make" in the command line, preparing the program for execution. (Make sure you are in the folder where you unzip the file when typing "make")
3.type ./gofish to begin!

Notes:
-The game will be printed out onto gofish_results.txt. If the file doesn't exist, the program will create a new file called gofish_results.txt and print it there. If the
file does exist, the program will overwrite everything inside that file with the Go Fish game

-This program does not allow user input. You can, however, add or take out players manually in go_fish.cpp and the program will function the same
 (minimum number of players is 2 and max number of players is 10).

-The players Kenny, Malik, Andrew, and Jonah in the automation hold memories of events in the game that could benefit them. Consider these two scenarios:
	Andrew: Kenny, do you have a 2?
	Kenny: Yeah, I got a 2
	Kenny gives one 2 to Andrew
	-If a player bot  who is witnessing this event has a card with the rank 2, they remember they should not ask Andrew and Kenny for a 2 and forget 
	 that they should ask Andrew and Kenny for a 2 if they had that memory 
	
	Andrew: Kenny, do you have an A?
	Kenny: Go Fish! 
	Andrew draws a 3c
	-If a player bot  who is witnessing this event has a card with the rank A, they remember they should ask Andrew for an A and forget that they should 
	 not ask Andrew for an A if they had that memory and they remember they should not ask Kenny for a A and forget that they should 
	 ask Kenny for a A if they had that memory 

-When deciding on who to ask, the asker first recalls if there is anyone in their memory they should ask for rank they have. If there isn't, the asker finds the most
 oppurtune player to ask (the player with the most amount of cards and the least amount of cards the asker should not to ask for) for a rank they have.

-When deciding on what rank to ask for, the asker creates a list of cards safe to ask for from the player they are about to ask in their brain. The asker does this 
 by distinguishing the cards they should not ask from the player they are about to ask from their hand (If there are no cards safe to ask for, the asker has to take 
 the L and ask for any card in their hand).


