/* Created by Kenneth Emeremnu
 * University of Texas at Austin
 * EE 312 */

#include "RoboDealer.h"
#include <fstream>

extern ofstream gofish;

int RoboDealer::findIndexPlayer(Player *p) {
    for (unsigned int i = 0; i < plist.size(); i++) { // Go through the entire player list to find the player index
        if (plist[i].p == p) { // If the player is found, return i;
            return i;
        }
    }
    return -1;
}

void RoboDealer::CheckIfGameCanStart(){
    if (plist.size() < 2) { // If there isn't enough players to play Go Fish, end the program
        gofish << "There are not enough players to play." << endl;
        exit(0);
    }
    if (plist.size() > 10) { // If there are too many players to play Go Fish, end the program
        gofish << "There are too many players" << endl;
        exit(0);
    }
}

void RoboDealer::showPlayerHands() {
    for (unsigned int i = 0; i < plist.size(); i++) { // Go through the player list and show each player's hand
        gofish << plist[i].p->getName() << "'s hand: ";
        gofish << plist[i].p->showHand() << endl;
    }
    gofish << endl;
}

void RoboDealer::FindAllBooks() {
    Card a;
    Card b;
    for (unsigned int i = 0; i < plist.size(); i++) { // Go through the entire player list to find a pair for each player and book them together
        for (plist[i].p->checkHandForBook(a, b); plist[i].p->checkHandForBook(a, b) == true; plist[i].p->checkHandForBook(a, b)) {
            gofish << endl << plist[i].p->getName() << " looks at his hand and forms a book with " << a.toString() << " and " << b.toString() << endl;
            plist[i].p->bookCards(a, b); // If a pair exists, book them together and forget all memory associated with the card's rank
            plist[i].p->deleteAllMemoryWithRank(a);
        }
    }
}

void RoboDealer::dealHand(Deck &d) {
    for (unsigned int i = 0; i < plist.size(); i++) { // Go through the entire player list and deal them the appropriate number of cards
        for (int j = 0; j < getNumberOfCardsToDeal(); j++) {
            plist[i].p->addCard(d.dealCard());
        }
    }
}

void RoboDealer::DecideRotation(Deck *d) {
    for (unsigned int i = 0; i < plist.size(); i++) { // Go through the entire player list and deal them the top card of the deck
        Card j = d->dealCard();
        plist[i].turn = j.getRank();
    }
    for (unsigned int i = 0; i < (plist.size() - 1); i++) { // Sort the vector Rotation in ascending order to determine the rotation of the game
        PlayerBot min = plist[i];
        PlayerBot hold = plist[i];
        int y;
        for (unsigned int j = i + 1; j < plist.size(); j++) {
            if (min.turn > plist[j].turn) {
                min = plist[j];
                y = j;
            }
        }
        if (min.turn!= hold.turn) {
            plist[i] = min;
            plist[y] = hold;
        }
    }
}

void RoboDealer::PrintRotation() {
    for (unsigned int i = 0; i < plist.size(); i++) {
        if (i != plist.size() - 1) {
            gofish << plist[i].p->getName() <<  "->";
        } else {
            gofish << plist[i].p->getName();
        }
    }
}

Player* RoboDealer::whoIsFirst(){
    return plist[0].p;
}

Player* RoboDealer::whoIsNext(Player* p){
    unsigned int index;
    for (unsigned int i = 0; i < plist.size(); i++) { // Go through the entire player list to find the player index
        if (plist[i].p == p) { // If the player is found, save their index and break
            index = i;
            break;
        }
    }
    if (index == plist.size() - 1){ // If the last player in the rotation just went, the first player in the rotation will go next
        return plist[0].p;
    }
    return plist[index + 1].p; // The next player in the rotation will go next
}

Player* RoboDealer::randomPlayer(Player *asker) {
    int u = findIndexPlayer(asker); // Find the index of the player asking
    int y = rand() % plist.size() + 0; // Generate a random number
    if (onlyOneWithCards(asker) == true || asker->getHandSize() == 0) { // If the asker is the only one with cards or they have no cards,
        while (y == u) { // enter this while loop if they were about to ask themselves
            y = rand() % plist.size() + 0;
        }
    } else { // If the asker is not the only one with cards and they have cards, find someone with cards to ask
        while (y == u || plist[y].p->getHandSize() == 0) {
            y = rand() % plist.size() + 0;
        }
    }
    return plist[y].p; // Return the player to ask
}

int RoboDealer::TotalNumberOfBooks() {
    int booksize = 0;
    for (unsigned int i = 0; i < plist.size(); i++) { // Go through the entire player list and each player's number of books together
        booksize += plist[i].p->getBookSize();
    }
    return booksize; // return the total number of books formed in the game
}

Player* RoboDealer::choosePlayerToAsk(Player *p) {
    int max = 0;
    Player *player = randomPlayer(p); // Find a random player to ask just in case no one is safe to ask
    for (unsigned int i = 0; i < plist.size(); i++) { // Go through the entire player list to find someone safe to ask
        if (plist[i].p->getName() != p->getName()) { // If the player isn't the asker
            int u = plist[i].p->getHandSize() - (p->FindCardsNotToAskFor(*plist[i].p)).size(); // Find the number of cards the player has that the asker is safe to ask for
            if (u > max) { // If the number of cards the player has that the asker is safe to ask for is greater than the current max, set it equal to max and save the player
                max = u;
                player = plist[i].p;
            }
        }
    }
    return player;
}

bool RoboDealer::onlyOneWithCards(Player *p) {
    unsigned int l = 0;
    for (unsigned int i = 0; i < plist.size(); i++) { // Go through the entire player list and count the number of players with an empty hand
        if (plist[i].p->getHandSize() == 0) {
            l++;
        }
    }
    if (l == plist.size() - 1 && p->getHandSize() != 0) { // If l is the number of players - 1 and the player's hand isn't empty,
        return true; // the player is the only one with cards in the group
    }
    return false; // Return false if l isn't the number of players - 1 or the player's hand is empty,
}


void RoboDealer::createAndDeleteMemories(Player *p, Card r) {
    if (p->rankInHand(r) == false || p->twoRanksInHand(r) == true){ // If the player does not have the rank of the card asked or the player has two of the ranks in hand,
        for (unsigned int i = 0; i < plist.size(); i++) { // Go through the entire player list
            if (plist[i].p->getName() != p->getName()) { // (Make sure the player isn't creating memories of himself)
                plist[i].p->MemoryForShouldNotAsk(p, r); // to create memories to remember they should not ask and forget memories remembering they should ask if they have the rank of the card asked
            }
        }
    }
    else if (p->rankInHand(r) == true && p->twoRanksInHand(r) == false){ // If the player does have the rank of the card asked and the player does not have two of the ranks in hand,
        for (unsigned int i = 0; i < plist.size(); i++) { // Go through the entire player list
            if (plist[i].p->getName() != p->getName()) {  // (Make sure the player isn't creating memories of himself)
                plist[i].p->MemoryForShouldAsk(p, r); // to create memories to remember they should ask and forget memories remembering they should not ask if they have the rank of the card asked
            }
        }
    }
}

void RoboDealer::SortPlayerListByBooks() {
    for (unsigned int i = 0; i < (plist.size() - 1); i++) { // Iterate (number of players in the list) - 1 times
        PlayerBot max = plist[i]; // Set min equal to the current Player
        PlayerBot hold = plist[i]; // Set hold to the current Player
        int y;
        for (unsigned int j = i + 1; j < plist.size(); j++) { // Iterate (number of players in the list) times
            if (max.p->getBookSize() < plist[j].p->getBookSize()) { // If min is greater than the player's booksize
                max = plist[j]; // Set the min equal to the player's booksize
                y = j;
            }
        }
        if (max != hold) { // If min doesn't equal to the player's booksize the iteration started with, swap the current player's booksize and the player with the smallest booksize in the unsorted data
            plist[i] = max;
            plist[y] = hold;
        }
    }
}

string RoboDealer::FindPlacing(int i) {
    string z;
    if (i % 10 == 1){ // If i has a 1 at the end, return "st" for "1st"
        z += "st";
        return z;
    }
    if (i % 10 == 2){  // If i has a 2 at the end, return "nd" for "2nd"
        z += "nd";
        return z;
    }
    if (i % 10 == 3){ // If i has a 3 at the end, return "rd" for "3rd"
        z += "rd";
        return z;
    }
    z += "th"; // If i has anything else at the end, return "th" for "th" for "4th, 5th, 6th, etc
    return z;
}

void RoboDealer::PrintSpaces(Player *p) {
    for (unsigned int k = 0; k < 15 - p->getName().size() + 4; k++) { // Print the correct number of space for the format of the standings list
        gofish << " "; // by substracting 15 minus the player's name size + 4 (15 is the number of character of "Player Name | B"
    }
}

void RoboDealer::Rankings() {
    gofish << "There have been 26 books formed! The game is now over. Let's see the standings!" << endl << endl;
    SortPlayerListByBooks(); // Sort the list in descending order
    gofish << "Ranking | Player Name | Books Formed | Books" << endl; // Format for the standings list
    for (unsigned int i = 0; i < plist.size(); i++) { // Go through the entire player list and print their standings accordingly
        if (i +1 < 10) { // If the ranking is higher than 9 print one less space
            gofish <<  "  " << i + 1 << FindPlacing(i + 1) <<  "     " << plist[i].p->getName();
        } else {
            gofish <<  "  " << i + 1 << FindPlacing(i + 1) <<  "    " << plist[i].p->getName();
        }
        PrintSpaces(plist[i].p); // Print the correct number of space for the format of the standings list
        if (plist[i].p->getBookSize() > 9) { // If the book size is higher than 9, print one more space
            gofish << plist[i].p->getBookSize() << "       " << plist[i].p->showBooks() << endl;
        } else {
            gofish << plist[i].p->getBookSize() << "        " << plist[i].p->showBooks() << endl;
        }
        unsigned int u = i;
        for (unsigned int j = i + 1; j < plist.size() && plist[j].p->getBookSize() == plist[u].p->getBookSize(); j++,i++) { // If two or more players have the same number of books, give them the same ranking
            if (j < 10) {
                gofish <<  "  " << u + 1 << FindPlacing(u+1) <<  "     " << plist[j].p->getName();
            } else {
                gofish <<  "  " << u + 1 << FindPlacing(u+1) <<  "    " << plist[j].p->getName();
            }
            PrintSpaces(plist[j].p);
            if (plist[j].p->getBookSize() > 9) {
                gofish << plist[j].p->getBookSize() << "       " << plist[j].p->showBooks() << endl;
            } else {
                gofish << plist[j].p->getBookSize() << "        " << plist[j].p->showBooks() << endl;
            }
        }
    }
}

