/* Created by Kenneth Emeremnu
 * University of Texas at Austin
 * EE 312 */

#include <cstdlib>     // Provides EXIT_SUCCESS
#include "card.h"
#include "player.h"
#include "RoboDealer.h"
#include <ostream>
#include <fstream>
#include <ctime>


using namespace std;

ofstream gofish;

void increment(RoboDealer *R2D2, Deck *d) {
    R2D2->FindAllBooks(); // Find all the books for each player
    gofish << endl << "--------------------------------------------------------------------------------------" << endl << endl;
    R2D2->showPlayerHands(); // Show each player's hand
    gofish << "Cards left in the deck: " << d->size() << endl << endl;
    if (R2D2->TotalNumberOfBooks() == 26) {
        gofish << "--------------------------------------------------------------------------------------" << endl << endl;
    }
}

void initialization(RoboDealer *R2D2, Deck *d, Player **first) {
    R2D2->CheckIfGameCanStart();

    d->shuffle(); // Shuffle the deck twice for randomness
    d->shuffle();

    gofish << "The deck is shuffled twice to decide the rotation of the game" << endl << endl;

    gofish << "Each player takes one card from the top of the deck" << endl << endl;

    R2D2->DecideRotation(d);
    Player *temp = (R2D2->whoIsFirst()); // Find the player who will go first
    *first = temp;
    gofish << "The rotation of the game is as follows:" << endl;
    R2D2->PrintRotation();
    gofish << endl << endl;

    d->shuffle(); // Shuffle the deck twice for randomness
    d->shuffle();

    R2D2->dealHand(*d); // Deal the appropiate number of cards to each player

    gofish << "Each player puts their card back on the deck. The deck is shuffled twice again" << endl << endl;

    gofish << "Each player is given " << R2D2->getNumberOfCardsToDeal() << " cards to start with" << endl << endl;

    gofish << "THE GAME BEGINS" << endl;
    gofish << endl << "--------------------------------------------------------------------------------------" << endl << endl;
    R2D2->showPlayerHands(); // Show each player's hand
    gofish << "Cards left in the deck: " << d->size() << endl;
    increment(R2D2,d);
}

int main() {
    ofstream outfile("gofish_results.txt"); // Create a new file called "gofish_results.txt" if it doesn't exist already
    gofish.open("gofish_results.txt"); // open gofish_results.txt using the ofstream gofish
    srand(time(nullptr));

    // Create the electronic dealer
    RoboDealer R2D2;

    // Create players for the game
    Player p1("Kenny");
    Player p2("Malik");
    Player p3("Colby");
    Player p4("Alex");


    // Add players to the electronic dealer's playerbase
    R2D2.addPlayer(&p1);
    R2D2.addPlayer(&p2);
    R2D2.addPlayer(&p3);
    R2D2.addPlayer(&p4);

    // Create a new deck and two player initializers for the game
    Deck d;
    Player *asker;
    Player *asked;
    for (initialization(&R2D2, &d, &asker); R2D2.TotalNumberOfBooks() < 26; increment(&R2D2, &d)) {
        while (asker->getHandSize() == 0) { // If the person asking has an empty hand, draw a card if there are cards left and pass the turn to another player
            gofish << asker->getName() << " has no cards in his hand" << endl;
            asker->drawCard(&d);
            asker = R2D2.whoIsNext(asker);
            gofish << endl << asker->getName() << " is the next player in the rotation" << endl << endl;
        }
        Card r;
        if ((asker->isThereAnyoneIShouldAsk())) { // If the person asking has someone to ask in mind, recall that memory
            Player::memory y = asker->recallWhoToAsk();
            asked = y.p; // The player who they remembered to ask
            r = y.c; // The card they remember them having
        }
        else { // If the person asking doesn't have someone to ask in mind, choose a player who is the safest to ask
            asked = R2D2.choosePlayerToAsk(asker); // The player safest to ask
            r = asker->chooseCardFromHand(*asked); // The rank they are going to ask for
        }
        string z = r.rankString(r.getRank()); // The rank they are going to ask for as a string

        gofish << asker->getName() << ": " << asked->getName();
        if (z != "A" && z != "8") {
            gofish << ", do you have a " << z << "?" << endl;
        } else {
            gofish << ", do you have an " << z << "?" << endl;
        }
        gofish << asked->getName() << ": ";
        if (asked->rankInHand(r)) { // If the person asked has the rank the player asking asked for, give all the ranks to the asker.
            if (z != "A" && z != "8") {
                gofish << "Yeah, I got a " << z << endl;
            } else {
                gofish << "Yeah, I got an " << z << endl;
            }
            asker->addCertainRanks(asked->removeCertainRanks(r));
            // This portion creates new memories and forgets old memories for the players observing
            // If the observer has the rank of the card being asked, they create a new memory indicating they should not ask the asker and player being asked for that rank
            // and forget a memory saying they should ask the asker and player being asked for that rank
            asked->deleteAllMemoryWithRank(r); // The asked erases all memory of the rank they lost
            R2D2.createAndDeleteMemories(asked, r);
            R2D2.createAndDeleteMemories(asker, r);
        } else { // If the person asked does not have the rank the player asking asked for, they say Go Fish! and the asker must draw a card if there are cards left
            gofish << "Go Fish! " << endl;
            asker->drawCard(&d);
            // This portion creates new memories and forgets old memories for the players observing
            // If the observer has the rank of the card being asked, they create a new memory indicating they should not ask the player being asked for that rank
            // and forget a memory saying they should ask the player being asked for that rank
            // They also create a new memory indicating they should ask the asker for that rank and forget a memory saying they should not ask for that rank
            R2D2.createAndDeleteMemories(asked, r); // They will delete the memories reminding them they Should Ask for the player being asked
            R2D2.createAndDeleteMemories(asker, r); // They will create memories reminding them they Should Ask for the player asking
            asker = R2D2.whoIsNext(asker); // The next person in the rotation takes the turn
        }
    }
    R2D2.Rankings(); // Print the standings of the game after it is over
    cout << endl << "The game has been printed onto gofish_results.txt" << endl << endl;
    gofish.close(); // Close the ofstream gofish
    return EXIT_SUCCESS;
}