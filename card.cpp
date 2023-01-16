/* Created by Kenneth Emeremnu
 * University of Texas at Austin
 * EE 312 */

#include "card.h"

Card::Card() {
    myRank = 1;
    mySuit = spades;
}

Card::Card(int rank, Suit s) {
    myRank = rank;
    mySuit = s;
}

string Card::toString() const {
    string s; // Create a string and add the string equivalents of the card's rank and suit
    s += rankString(myRank) + suitString(mySuit);
    return s; // return the string
}

bool Card::sameSuitAs(const Card &c) const {
    if (mySuit == c.mySuit) { // If the suits are the same, return true
        return true;
    }
    return false; // If the suits aren't the same, return true
}

int Card::getRank() const {
    return myRank; // Return the card's rank
}

string Card::suitString(Suit s) const {
    // Find the string equivalent of the suit and return it
    if (s == spades) {
        return "s";
    }
    if (s == hearts) {
        return "h";
    }
    if (s == diamonds) {
        return "d";
    }
    return "c";
}

string Card::rankString(int r) const {
    // Find the string equivalent of the rank and return it
    string digits[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    return digits[r - 1];
}

bool Card::operator==(const Card &rhs) const {
    if (myRank == rhs.myRank && mySuit == rhs.mySuit) { // If the suit and rank are the same, return true
        return true;
    }
    return false;  // else, return false
}

bool Card::operator!=(const Card &rhs) const {
    if (myRank == rhs.myRank && mySuit == rhs.mySuit) {  // If the suit and rank are the same, return false
        return false;
    }
    return true; // else, return true
}
