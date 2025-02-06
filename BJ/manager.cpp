#include "manager.h"

#include <QDebug>

// ============================================
//
//                  Manager
//
// ============================================

Manager::Manager()
    : m_numberOfDecks(DECKS)
    , m_deck(QVector<unsigned short>(MAX_CARD, DECKS*SUITS))
{

}

Manager::~Manager()
{

}



// ============================================
//
//               Reset
//
// ============================================

void Manager::reset()
{
    // Reset del Deck
    m_deck = QVector<unsigned short>(MAX_CARD, m_numberOfDecks*SUITS);

    // Clear della mano del player
    m_playerCards.clear();

    // Reset mano del Dealer
    m_dealerCard = 0;
}



// ============================================
//
//               Settings
//
// ============================================

void Manager::setNumberOfDecks(unsigned short numberOfDecks)
{
    // Salvo il numero di Deck
    m_numberOfDecks = numberOfDecks;

    // Effettuo il reset di tutto il manager
    reset();
}

void Manager::setOptimization(unsigned short optimization)
{
    m_optimization = optimization;
}



// ============================================
//
//           Operazioni su Deck
//
// ============================================

void Manager::discardCard(unsigned short card)
{
    // Decremento di 1 la quantità di carte in input
    m_deck[card-1]--;
}

void Manager::insertCard(unsigned short card)
{
    // Incremento di 1 la quantità di carte in input
    m_deck[card-1]++;
}



// ============================================
//
//           Operazioni giocatori
//
// ============================================

void Manager::addPlayerCard(unsigned short playerCard)
{
    // Aggiungo una carta al Player
    m_playerCards.push_back(playerCard);

    // Rimuovo la carta dal Deck
    discardCard(playerCard);
}

void Manager::addDealerCard(unsigned short dealerCard)
{
    // Associo la carta al Dealer
    m_dealerCard = dealerCard;

    // Rimuovo la carta dal Deck
    discardCard(dealerCard);
}

void Manager::resetPlayerCard()
{
    // Svuoto le carte del Player
    m_playerCards.clear();
}

void Manager::resetDealerCard()
{
    // Azzero la carta del Dealer
    m_dealerCard = 0;
}



// ============================================
//
//           Calcolo probabilità
//
// ============================================

bool Manager::startCalculate()
{
    // Variabile punteggio
    unsigned short playerPoints = 0;

    // Flag per memorizzare un eventuale asso in mano
    bool aceInHand = false;

    // Calcolo punti in mano al Player
    for(const unsigned short& card : m_playerCards)
    {
        // Incremento il punteggio
        playerPoints += std::min(card, static_cast<unsigned short>(10));

        // Se è un asso lo memorizzo
        if(card == 1) aceInHand = true;
    }

    // Se ho un asso provo se può valere 11
    if(aceInHand and playerPoints+10 <= 21)
    {
        playerPoints += 10;
    }

    // Se non supero la soglia
    if(playerPoints < m_optimization)
    {
        // Comunico sempre di chiamare
        return false;
    }

    // In tutti gli altri casi lancio un Thread per calcolare la probabilità
    m_thread = QThread::create([this]{this->calculateProb();});

    // Connect per distruggere il Thread una volta terminato
    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);

    // Avvio il Thread
    m_thread->start();

    return true;
}

void Manager::stopCalculate()
{
    // Connect per comunicare lo stop
    connect(m_thread, &QThread::finished, this, &Manager::pubProbStopped);

    // Forzo la terminazione del Thread
    m_thread->quit();
}

void Manager::calculateProb()
{
    // Creo strutture di supporto
    std::vector<unsigned short> deckStd(m_deck.begin(), m_deck.end());
    std::vector<unsigned short> playerStd(m_playerCards.begin(), m_playerCards.end());

    // Calcolo la probabilità di vincere pescando
    double hitProbability = BJProbability::calculateHitProbability(deckStd, playerStd, m_dealerCard);

    // Calcolo la probabilità di vincere se non pesco
    double standProbability = BJProbability::calculateStandProbability(deckStd, playerStd, m_dealerCard);

    // Comunico la fine
    emit pubProbCalculated(hitProbability, standProbability);
}
