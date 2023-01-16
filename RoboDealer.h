/* Created by Kenneth Emeremnu
 * University of Texas at Austin
 * EE 312 */

#ifndef GOFISH_ROBODEALER_H
#define GOFISH_ROBODEALER_H

#include "player.h"
#include "deck.h"

class RoboDealer {
private:
    struct PlayerBot{
        Player* p;
        int turn;
        bool operator != (const PlayerBot a) const{
            if (p == a.p && turn == a.turn){
                return false;
            }
            return true;
        }
    };
    vector<PlayerBot> plist;
    int findIndexPlayer(Player *p);
public:
    RoboDealer()= default;
    void addPlayer(Player *p) {
        PlayerBot a{};
        a.p = p;
        a.turn = -1;
        plist.push_back(a);
    }
    int getNumberOfCardsToDeal() const {
        if (plist.size() > 2){
            return 5;
        }
        return 7;
    }
    // Find every single pair and book that pair for each player
    void CheckIfGameCanStart();

    // Show every player's hand
    void showPlayerHands();

    // Find every single pair and book that pair for each player
    void FindAllBooks();

    // Deal the appropiate number of cards to each player
    void dealHand(Deck &d);

    // Determine the rotation of the game
    void DecideRotation(Deck *d);

    // Print out the rotation of the game
    void PrintRotation();

    // Find the player who will go first
    Player* whoIsFirst();

    // Find the player who will go next
    Player* whoIsNext(Player* p);

    // Choose a random player to ask
    Player* randomPlayer(Player *asker);

    // Return the total number of books formed in the entire game
    int TotalNumberOfBooks();

    // Ask the player with the most oppurtune possibilities
    Player* choosePlayerToAsk(Player *p);

    // Check if the player is the only one with a non empty hand
    bool onlyOneWithCards(Player *p);

    // Form memories what and what not to ask of certain players
    void createAndDeleteMemories(Player *p, Card r);

    // Sort players by the number of books they formed
    void SortPlayerListByBooks();

    // Find the appropiate placing end of a for a number "1st, 2nd, 3rd, 4th, 5th, etc"
    string FindPlacing(int i);

    // Print the appropiate number of spaces for the format of the standings
    void PrintSpaces(Player *p);

    // Print the rankings
    void Rankings();
};

#endif //GOFISH_ROBODEALER_H
