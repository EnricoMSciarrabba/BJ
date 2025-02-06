#include "prob.h"

#include <deque>
#include <algorithm>
#include <future>
#include <cmath>

namespace BJProbability {

namespace {

unsigned short calculatePoints(const std::vector<unsigned short> *hand)
{
    // Variabile punteggio
    unsigned short points = 0;

    // Flag per memorizzare un eventuale asso in mano
    bool aceInHand = false;

    // Calcolo punti in mano
    for(size_t cardIndex=0; cardIndex<hand->size(); cardIndex++)
    {
        // Incremento il punteggio
        points += std::min((*hand)[cardIndex], static_cast<unsigned short>(10));

        // Se è un asso lo memorizzo
        if((*hand)[cardIndex] == 1) aceInHand = true;
    }

    // Se ho un asso provo se può valere 11
    if(aceInHand and points+10 <= 21)
    {
        points += 10;
    }

    return points;
}

}

double dfsDealer(std::vector<unsigned short> *deck, std::vector<unsigned short> *dealerCards, unsigned short playerPoints)
{
    // Calcolo il punteggio
    unsigned short dealerPoints = calculatePoints(dealerCards);

    // Se il Dealer supera 21 ha perso (il Player vince)
    if(dealerPoints > 21)
    {
        // Il Player vince quindi salvo una vittoria
        return 1.;
    }

    // Se il Dealer ha 21 ha vinto (il Player perde)
    if(dealerPoints == 21)
    {
        // Il Player perde quindi salvo una sconfitta
        return 0.;
    }

    // Se supero 17
    if(dealerPoints >= 17)
    {
        // Il Dealer non chiama più e controllo se ha vinto o perso
        // Se il player ha un punteggio maggiore segno vittoria altrimenti perdita
        if(playerPoints >= dealerPoints) return 1.;
        else                             return 0.;
    }

    // Calcolo le carte nel deck
    unsigned int cardInDeck = 0;
    for(const unsigned short& card : (*deck)) cardInDeck += card;

    // Continuo calcolando la probabilità di vincere se chiamo
    double probability = 0.;
    double conditionalProbability;
    double cardProbability;
    for(size_t cardIndex=0; cardIndex<deck->size(); cardIndex++)
    {
        // Se non ho un valore, continuo
        if((*deck)[cardIndex] == 0) continue;

        // Inserisco la carta in mano e la tolgo dal deck
        dealerCards->push_back(cardIndex + 1);
        (*deck)[cardIndex] -= 1;

        // Calcolola probabilità per il Player di vincere se al dilere esce questa carta
        conditionalProbability = dfsDealer(deck, dealerCards, playerPoints);

        // Appena risale sistemo
        dealerCards->pop_back();
        (*deck)[cardIndex] += 1;

        // Calcolo la probabilità che esca la carta
        cardProbability = static_cast<double>((*deck)[cardIndex])/cardInDeck;

        // Aggiungo la probabilità di vincere se pesco la carta i-esima
        probability += conditionalProbability*cardProbability;
    }

    return probability;
}

double dfsPlayer(std::vector<unsigned short> *deck, std::vector<unsigned short> *playerCards, unsigned short dealerCard)
{
    // Calcolo il punteggio
    unsigned short playerPoints = calculatePoints(playerCards);

    // Se il Player supera 21
    if(playerPoints > 21)
    {
        // La probabilità di vincere è 0
        return 0.;
    }

    // Se il player ha 21 vince in automatico
    if(playerPoints == 21)
    {
        // La probabilità di vincere è 1
        return 1.;
    }

    // Inizio calcolando la probabilità di vincere se non chiamiamo
    std::vector<unsigned short> dealerCards{dealerCard};
    double standProbability = dfsDealer(deck, &dealerCards, playerPoints);

    // Calcolo le carte nel deck
    unsigned int cardInDeck = 0;
    for(const unsigned short& card : *deck) cardInDeck += card;

    // Continuo calcolando la probabilità di vincere se chiamo
    double hitProbability = 0.;
    double conditionalProbability;
    double cardProbability;
    for(size_t cardType=0; cardType<deck->size(); cardType++)
    {
        // Se non ho un valore, continuo
        if((*deck)[cardType] == 0) continue;

        // Inserisco la carta in mano e la tolgo dal deck
        playerCards->push_back(cardType + 1);
        (*deck)[cardType] -= 1;

        // Calcolo la probabilità condizionata di vincere se esce la carta
        conditionalProbability = dfsPlayer(deck, playerCards, dealerCard);

        // Appena risale sistemo
        playerCards->pop_back();
        (*deck)[cardType] += 1;

        // Calcolo la probabilità che esca la carta
        cardProbability = static_cast<double>((*deck)[cardType])/cardInDeck;

        // Aggiungo la probabilità di vincere se pesco la carta i-esima
        hitProbability += conditionalProbability*cardProbability;
    }

    // Restituisco la probabilità migliore
    return fmax(standProbability, hitProbability);
}

double calculateHitProbability(std::vector<unsigned short> deck, std::vector<unsigned short> playerCards, unsigned short dealerCard)
{
    // Calcolo il punteggio del player
    unsigned short playerPoints = calculatePoints(&playerCards);

    // Se il Player ha o supera 21 non può chiamare
    if(playerPoints >= 21)
    {
        // Deve stare
        return 0.;
    }

    // Calcolo le carte nel deck
    unsigned int cardInDeck = 0;
    for(const unsigned short& card : deck) cardInDeck += card;

    // Vettore contenente le probabilità di uscita di una carta
    std::vector<double> cardProbability;

    // Calcolo in parallelo le probabilità condizionate di vincere se esce una carta X
    std::vector<std::future<double>> futureVector;
    std::deque<std::vector<unsigned short>> copyDecks;
    std::deque<std::vector<unsigned short>> copyPlayerCards;
    for(size_t cardType=0; cardType<deck.size(); cardType++)
    {
        // Se non ho una carta, continuo
        if(deck[cardType] == 0)
        {
            continue;
        }

        // Inserisco la carta in mano e la tolgo dal deck
        playerCards.push_back(cardType + 1);
        deck[cardType] -= 1;

        // Creo le copie per lanciare gli altri thread
        copyDecks.push_back(deck);
        copyPlayerCards.push_back(playerCards);

        // Calcolo la probabilità condizionata di vincere se esce la carta in un nuovo thread
        futureVector.emplace_back(std::async(std::launch::async,
                                             dfsPlayer,
                                             &(copyDecks.back()),
                                             &(copyPlayerCards.back()),
                                             dealerCard)
                                  );

        // Appena risale sistemo
        playerCards.pop_back();
        deck[cardType] += 1;

        // Calcolo la probabilità che esca la carta
        cardProbability.push_back(static_cast<double>(deck[cardType])/cardInDeck);
    }

    // Attendo tutti i thread per recuperare le probabilità condizionate
    // e calcolare la probabilità di vincere pescando
    double hitProbability = 0.;
    for(size_t index=0; index<futureVector.size(); index++)
    {
        hitProbability += futureVector[index].get() * cardProbability[index];
    }

    // Ritorno la probabilità di vincere se viene pescata una carta
    return hitProbability;
}

double calculateStandProbability(std::vector<unsigned short> deck, std::vector<unsigned short> playerCards, unsigned short dealerCard)
{
    // Calcolo il punteggio del player
    unsigned short playerPoints = calculatePoints(&playerCards);

    // Se il Player supera 21
    if(playerPoints > 21)
    {
        // Deve stare
        return 1.;
    }

    // Se il Player ha 21
    if(playerPoints == 21)
    {
        // Se il Player ha più di due carte vince sempre
        if(playerCards.size() > 2) return 1.;

        // Se il Player ha 2 carte e il Dealer non ha Asso vince sempre
        if(dealerCard != 1) return 1.;

        // Se il Dealer ha Asso calcolo la probabilità che abbia 10 come seconda carta
        unsigned int allCardsCounter = 0;
        unsigned int winCardsCounter = 0;
        for(size_t index=0; index<deck.size(); index++)
        {
            // Incremento il numero totale di carte
            allCardsCounter += deck[index];

            // Se è una carta con valore 10
            if(index+1 >= 10) winCardsCounter += deck[index];
        }

        // La probabilità di vincere non chiamando è di 1 - (la probabilità che vinca il Dealer)
        return 1. - static_cast<double>(winCardsCounter)/allCardsCounter;
    }

    // Creo il vector con le carte del Dealer
    std::vector<unsigned short> dealerCards{dealerCard};

    // Calcolo la probabilità di vincere se sto
    return dfsDealer(&deck, &dealerCards, playerPoints);
}

} // namespace BJProbability
