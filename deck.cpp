/* Created by Kenneth Emeremnu
 * University of Texas at Austin
 * EE 312 */

#include "deck.h"

Deck::Deck() {
    // Create a deck of cards by constructing 13 cards for each suit and adding them onto the myCards vector
    for (int i = 0; i < SIZE; i++) {
        if (i < 13) {
            Card a(i + 1, Card::spades);
            myCards[i] = a;
        }
        if (i > 12 && i < 26) {
            Card a(i - 12, Card::hearts);
            myCards[i] = a;
        }
        if (i > 25 && i < 39) {
            Card a(i - 25, Card::diamonds);
            myCards[i] = a;
        }
        if (i > 38 && i < 52) {
            Card a(i - 38, Card::clubs);
            myCards[i] = a;
        }
    }
    myIndex = SIZE - 1;
}

void Deck::shuffle() {
    // Create a new random_device e and seed rand() with the random value from e
    myIndex = SIZE - 1;
    for (int i = 0; i < 100; i++) { // Swap two random cards in the deck 100 times
        int y = rand() % SIZE + 0;
        int z = rand() % SIZE + 0;

        // Swap two random cards in the deck
        Card a = myCards[y];
        myCards[y] = myCards[z];
        myCards[z] = a;
    }
}

Card Deck::dealCard() {
    int myTemp = myIndex; // Save the top card's index
    myIndex--; // Take off the top card from the deck
    return myCards[myTemp]; // Return the top card
}

int Deck::size() const {
    return myIndex + 1; // Return the number of cards left in the deck
}