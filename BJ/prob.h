#ifndef PROB_H
#define PROB_H

#include <vector>

#define DECKS 8
#define SUITS 4
#define CARDS_FOR_NUMBER SUITS*DECKS
#define MAX_CARD 13

namespace BJProbability {

double dfsDealer(std::vector<unsigned short>* deck,
                 std::vector<unsigned short>* dealerCards,
                 unsigned short playerPoints);

double dfsPlayer(std::vector<unsigned short>* deck,
                 std::vector<unsigned short>* playerCards,
                 unsigned short dealerCard);

double calculateHitProbability(std::vector<unsigned short> deck,
                               std::vector<unsigned short> playerCards,
                               unsigned short dealerCard);

double calculateStandProbability(std::vector<unsigned short> deck,
                                 std::vector<unsigned short> playerCards,
                                 unsigned short dealerCard);

}

#endif // PROB_H
