#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QVector>
#include <QThread>

#include "prob.h"

// ============================================
//
//                  Manager
//
// ============================================

class Manager : public QObject
{
    Q_OBJECT
public:
    Manager();
    virtual ~Manager();



    // ========================
    //
    //  Interfacce esterne

public:
    // Reset
    void reset();

    // Settings
    Q_SLOT void setNumberOfDecks(unsigned short numberOfDecks);
    Q_SLOT void setOptimization(unsigned short optimization);

    // Operazioni su deck
    Q_SLOT void discardCard(unsigned short card);
    Q_SLOT void insertCard(unsigned short card);

    // Operazioni giocatori
    void addPlayerCard(unsigned short playerCard);
    void addDealerCard(unsigned short dealerCard);
    void resetPlayerCard();
    void resetDealerCard();

    // Gestione calcolo della probabilità
             bool startCalculate();
      Q_SLOT void stopCalculate();
    Q_SIGNAL void pubProbCalculated(const double& hitProbability, const double& standProbability);
    Q_SIGNAL void pubProbStopped();


    // ========================
    //
    //  Metodi interni

private:
    // Metodo per calcolare le probabilità
    void calculateProb();



    // ========================
    //
    //  Variabili

private:
    // Settings
    unsigned short m_numberOfDecks;
    unsigned short m_optimization;

    // Strutture per le carte
    QList<unsigned short> m_deck;
    QList<unsigned short> m_playerCards;
    unsigned short m_dealerCard = 0;

    // Thread per gestire il calcolo
    QThread* m_thread;

};

#endif // MANAGER_H
