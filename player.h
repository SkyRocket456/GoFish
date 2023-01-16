// FILE:  player.h
// written by Roger Priebe
// 1/22/08 (revised 9/2/08)
// This class represents a player in a card game that takes "tricks"
// The "Books" represent a container for holding tricks

/* This header file was revamped by Kenneth Emeremnu
 * University of Texas at Austin
 * EE 312 */

#ifndef _PLAYER_H
#define _PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "card.h"
#include "deck.h"
using namespace std;


class Player
{
public:
    struct memory{
        Card c;
        Player* p;
    };
private:
    struct myBrain{
        vector <memory> IShouldAsk;
        vector <memory> IShouldNotAsk;
    };

    vector <Card> myHand;
    vector <Card> myBook;

    myBrain b;
    string myName;

    // Finds the index of a card in the myHand vector
    int findIndex(Card c);

    // Finds the index of a memory in a memory vector for IShouldAsk and IShouldNotAsk
    int findIndexMemory(memory m, vector<memory> temp);
public:
    Player(){
        myName = "None";
    }
    Player(string name) {
        myName = name;
    }
    string getName() const {
        return myName;
    }
    void addCard(Card c);  //adds a card to the hand

    void bookCards(Card c1, Card c2); // books a pair of cards

    //this function will check a players hand for a pair.
    //If a pair is found, it returns true and populates the two variables with the cards tha make the pair.
    bool checkHandForBook(Card &c1, Card &c2);

    //Does the player have a card with the same rank as c in her hand?
    bool rankInHand(Card c) const;

    // Checks if the player has two cards of the exact same rank as Card c in their hand
    bool twoRanksInHand(Card c) const;

    //uses some strategy to choose one card from the player's
    //hand so they can say "Do you have a 4?"
    Card chooseCardFromHand(Player p);

    // Draw a card from the deck
    void drawCard(Deck *d);

    // Remove all the cards of the same rank as the card the asker asked for and returns them via vector
    vector<Card> removeCertainRanks(Card a);

    // Add all the cards from the person asking to the asker's hand via vectro
    void addCertainRanks(vector<Card> cards);

    // Create a memory indicating they should not ask and forget a memory indicating they should ask Player p for the rank of Card C if they have the same rank
    void MemoryForShouldNotAsk(Player *p, Card c);

    // Create a memory indicating they should ask and forget a memory indicating they should not ask Player p for the rank of Card C if they have the same rank
    void MemoryForShouldAsk(Player *p, Card c);

    // Forget all memory of the rank of Card c
    void deleteAllMemoryWithRank(Card c);

    // Search the card vector and try and find the rank of Card r
    bool isRankInThisCardVector(Card r, vector<Card> temp)const;

    // Search through the memory vector and try to find the rank of Card r
    bool isItInMyMemory(Player *p, Card r,vector<memory> temp);

    // Remember if there is anyone the player should ask
    bool isThereAnyoneIShouldAsk()const;

    // Recall the player to ask and the rank they should ask for
    memory recallWhoToAsk();

    // Find the cards you should not ask Player p
    vector<Card> FindCardsNotToAskFor(Player p);

    // Find the cards that are safe to ask Player p
    vector<Card> FindCardsToAskFor(vector <Card> DontAskFor);


    string showHand() const; // Return a player's hand as a string
    string showBooks() const; // Return the books of the player as a string

    int getHandSize() const; // Return the player's hand size
    int getBookSize() const; // Return the number of books the player formed
};


#endif
