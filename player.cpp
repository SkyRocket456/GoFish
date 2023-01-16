/* Created by Kenneth Emeremnu
 * University of Texas at Austin
 * EE 312 */

#include "player.h"
#include <fstream>

extern ofstream gofish;

void Player::addCard(Card c) {
    myHand.push_back(c); // Put the card into the player's hand
}

void Player::bookCards(Card c1, Card c2) {
    myBook.push_back(c1); // Push the cards onto the player's book
    myBook.push_back(c2);
    myHand.erase(myHand.begin() + findIndex(c1)); // Take the cards away from the player's hand
    myHand.erase(myHand.begin() + findIndex(c2));
}

bool Player::checkHandForBook(Card &c1, Card &c2) {
    for (unsigned int i = 0; i < myHand.size(); i++) { // Search through the player's hand
        for (unsigned int j = 0; j < myHand.size(); j++) { // Check each card in the player's hand with the other cards
            if (myHand[j].getRank() == myHand[i].getRank() && j != i) { // If a pair is found, equal c1 and c2 to both those cards and return true
                c1 = myHand[j];
                c2 = myHand[i];
                return true;
            }
        }
    }
    return false; // If a pair not is found, return false
}

bool Player::rankInHand(Card c) const {
    for (unsigned int i = 0; i < myHand.size(); i++) { // Search through the player's hand
        if (myHand[i].getRank() == c.getRank()) { // If a player's card's rank matches the card's rank, return true
            return true;
        }
    }
    return false; // If a player's card's rank does not matches the card's rank, return false
}

bool Player::twoRanksInHand(Card c) const {
    int u = 0;
    for (unsigned int i = 0; i < myHand.size(); i++) { // Search through the player's hand
        if (myHand[i].getRank() == c.getRank()) { // If a player's card's rank matches the card's rank, increment u
            u++;
            if (u == 2) { // If two cards have the same rank, return true
                return true;
            }
        }
    }
    return false; // If two cards do not have the same rank, return false
}

Card Player::chooseCardFromHand(Player p) {
    vector<Card> SafeToAskFor = FindCardsToAskFor(FindCardsNotToAskFor(p)); // Find the cards safe to ask for by taking away the cards not to ask for
    if (SafeToAskFor.empty()) { // If there is no cards safe to ask, give a deep sigh and just ask them any card
        return myHand[rand() % myHand.size() + 0];
    }
    return SafeToAskFor[rand() % SafeToAskFor.size() + 0]; // If there are cards safe to ask, choose a random one and ask
}


void Player::drawCard(Deck *d) {
    if (d->size() != 0) { // If the deck isn't empty
        Card u = d->dealCard();
        addCard(u); // Take a card from the deck and add it to the player's hand
        if (u.rankString(u.getRank()) == "A" || u.rankString(u.getRank()) == "8") { // If the rank is an Ace or an eight, use "an" for grammar
            gofish << getName() << " draws an " << u.toString() << endl;
        } else {
            gofish << getName() << " draws a " << u.toString() << endl;
        }
    }
}

vector<Card> Player::removeCertainRanks(Card a) {
    vector<Card> cardsToGive; // Create a new vector of cards to give to the asker
    for (unsigned int i = 0; i < myHand.size(); i++) { // Search through the player's hand
        if (myHand[i].getRank() == a.getRank()) { // If a card in the player's hand equals the card the rank of the card asked
            cardsToGive.push_back(myHand[i]); // Add it to the list of cards to give
            myHand.erase(myHand.begin() + findIndex(myHand[i])); // Take the card away from the player's hand
        }
    }
    if (cardsToGive.size() == 1) { // If there is only one card to give, print the following
        gofish << getName() << " gives one " << a.rankString(a.getRank());
    } else { // If there is more than one card to give, print the following
        gofish << getName() << " gives " << cardsToGive.size() << " " << a.rankString(a.getRank()) << "'s";
    }
    return cardsToGive;
}

void Player::addCertainRanks(vector<Card> cards) {
    for (unsigned int i = 0; i < cards.size(); i++) { // Give every card in the cardToGive to the asker
        addCard(cards.at(i));
    }
    gofish << " to " << getName() << endl;
}

void Player::MemoryForShouldNotAsk(Player *p, Card c) {
    memory t;
    t.c = c;
    t.p = p;
    if (rankInHand(c) == true && twoRanksInHand(c) == false && isItInMyMemory(p, c, b.IShouldNotAsk) == false) { // If the observer has the rank
        b.IShouldNotAsk.push_back(t); // of the card, doesn't have two of the same rank of the card, and doesn't already have it in her memory, create a memory not to ask the player for that rank
    }
    if (isItInMyMemory(p, c, b.IShouldAsk) == true) { // If the player has the memory of remembering they should ask the player for that rank, forget it
        b.IShouldAsk.erase(b.IShouldAsk.begin() + findIndexMemory(t, b.IShouldAsk));
    }
}

void Player::MemoryForShouldAsk(Player *p, Card c) {
    memory t;
    t.c = c;
    t.p = p;
    if (rankInHand(c) == true && isItInMyMemory(p, c, b.IShouldAsk) == false) { // If the observer has the rank
        b.IShouldAsk.push_back(t); // of the card and doesn't already have it in her memory, create a memory to ask the player for that rank
    }
    if (isItInMyMemory(p, c, b.IShouldNotAsk) == true) { // If the player has the memory of remembering they should not ask the player for that rank, forget it
        b.IShouldNotAsk.erase(b.IShouldNotAsk.begin() + findIndexMemory(t, b.IShouldNotAsk));
    }
}


void Player::deleteAllMemoryWithRank(Card c) {
    for (unsigned int i = 0; i < b.IShouldAsk.size(); i++) { // Go through the entire memory telling the player they should ask and forget the memories with the rank of Card c
        if (b.IShouldAsk[i].c.getRank() == c.getRank()) {
            memory m;
            m.c = b.IShouldAsk[i].c;
            m.p = b.IShouldAsk[i].p;
            b.IShouldAsk.erase(b.IShouldAsk.begin() + findIndexMemory(m, b.IShouldAsk));
            i--;
        }
    }
    for (unsigned int i = 0; i < b.IShouldNotAsk.size(); i++) { // Go through the entire memory telling the player they should not ask and forget the memories with the rank of Card c
        if (b.IShouldNotAsk[i].c.getRank() == c.getRank()) {
            memory m;
            m.c = b.IShouldNotAsk[i].c;
            m.p = b.IShouldNotAsk[i].p;
            b.IShouldNotAsk.erase(b.IShouldNotAsk.begin() + findIndexMemory(m, b.IShouldNotAsk));
            i--;
        }
    }
}

bool Player::isThereAnyoneIShouldAsk() const {
    if (b.IShouldAsk.empty()) { // If there is no one to ask, return false;
        return false;
    } else {
        return true;
    }
}

Player::memory Player::recallWhoToAsk() {
    return b.IShouldAsk[rand() % b.IShouldAsk.size() + 0]; // Return a random memory of a player and rank to ask
}

vector<Card> Player::FindCardsNotToAskFor(Player p) {
    vector<Card> DontAskFor; // Create a new vector of a list of cards not to ask for
    for (unsigned int i = 0; i < b.IShouldNotAsk.size(); i++) {
        if (b.IShouldNotAsk[i].p->getName() == p.getName()) { // If a memory has the player's name, add to to the list of cards not to ask for
            DontAskFor.push_back(b.IShouldNotAsk[i].c);
        }
    }
    return DontAskFor; // return the vector
}

vector<Card> Player::FindCardsToAskFor(vector<Card> DontAskFor) {
    vector<Card> SafeToAskFor; // Create a new vector of a list of cards safe to ask for
    for (unsigned int j = 0; j < myHand.size(); j++) {
        if (isRankInThisCardVector(myHand[j], DontAskFor) == false) { // If the card's rank isn't in the list of cards not to ask for, add to the list of cards safe to ask for
            SafeToAskFor.push_back(myHand[j]);
        }
    }
    return SafeToAskFor;
}

bool Player::isRankInThisCardVector(Card r, vector<Card> temp) const {
    for (unsigned int i = 0; i < temp.size(); i++) { // Search through the card vector
        if (temp[i].getRank() == r.getRank()) { // If the card vector's card's rank matches the card's rank, return true
            return true;
        }
    }
    return false; // If the card's rank is not found, return false
}

bool Player::isItInMyMemory(Player *p, Card r, vector<memory> temp) {
    for (unsigned int i = 0; i < temp.size(); i++) { // Search through the memory vector
        if (temp[i].p->getName() == p->getName() && temp[i].c.getRank() == r.getRank()) { // If the memory is found with the player's name and the card's rank, return true
            return true;
        }
    }
    return false; // If the memory isn't found, return false
}

string Player::showHand() const {
    string s;
    for (unsigned int i = 0; i < myHand.size(); i++) { // Go through the player's hand and add every card into a string
        if (i != myHand.size() - 1) {
            s += myHand.at(i).toString() + ",";
        } else {
            s += myHand.at(i).toString();
        }
    }
    return s;
}


string Player::showBooks() const {
    string s;
    for (unsigned int i = 0; i < myBook.size(); i++) { // Go through the player's books and add them into a string
        if (i != myBook.size() - 1) {
            if (i % 2 == 0){
                s += "{" + myBook.at(i).toString() + ",";
            }
            else{
                s += myBook.at(i).toString() + "},";
            }
        } else {
            s += myBook.at(i).toString() + "}";
        }
    }

    return s; // return the string
}

int Player::getHandSize() const {
    return myHand.size(); // return the number of cards in the player's hand
}

int Player::getBookSize() const {
    unsigned int a = myBook.size();
    return a / 2; // return the number of books the player has
}

int Player::findIndex(Card c) {
    for (unsigned int i = 0; i < myHand.size(); i++) { // Find the index of the card in the myHand vector and return it
        if (myHand[i] == c) {
            return i;
        }
    }
    return -1;
}

int Player::findIndexMemory(memory m, vector<memory> temp) {
    for (unsigned int i = 0; i < temp.size(); i++) { // Find the index of the card in the memory vector and return it
        if (temp[i].c.getRank() == m.c.getRank() && temp[i].p->getName() == m.p->getName()) {
            return i;
        }
    }
    return -1;
}

