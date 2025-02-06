# Probabilità Condizionata e Algoritmo Ricorsivo per Decisioni Ottimali

💡 Analisi delle scelte probabilistiche in scenari decisionali incerti

Questo progetto implementa un algoritmo ricorsivo basato sulle probabilità condizionate per valutare la scelta ottimale tra due azioni possibili. L'obiettivo è determinare la probabilità di successo scegliendo tra pescare o stare, utilizzando un modello basato sulla teoria delle probabilità e un approccio iterativo per l'aggiornamento delle decisioni.

## 📊 Algoritmo di Calcolo delle Probabilità

Per determinare la probabilità di vincere pescando e la probabilità di vincere stando, il sistema si basa su un modello probabilistico ricorsivo.

### 1️⃣ Probabilità di vincere pescando

Per calcolare la probabilità di successo pescando, dobbiamo considerare ogni possibile carta che può essere estratta. Se chiamiamo 
𝑋 l'insieme delle carte disponibili nel mazzo, la probabilità complessiva è data dalla somma ponderata delle probabilità condizionate di vittoria per ogni carta possibile:

$$
P_{hit} = \sum_{x \in X} P(x) \cdot P_{win | x}
$$

Dove: 
- *P(x)* è la probabilità di pescare la carta 𝑥
- *P<sub>win | x</sub>* è la probabilità di vincere dato che abbiamo pescato la carta 𝑥

La probabilità condizionata *P<sub>win | x</sub>* viene determinata confrontando due possibilità:
1. Vincere se si sta con la nuova mano (probabilità di vincere senza pescare ulteriormente)
2. Vincere continuando a pescare (ripetendo il processo con una nuova iterazione ricorsiva)

L'algoritmo sceglie il valore massimo tra queste due probabilità:

$$
P_{win | x} = \max(P_{stand | x}, P_{hit | x})
$$

Questa struttura rende il modello ricorsivo, poiché ogni chiamata al calcolo della probabilità di vincere pescando può portare a un nuovo calcolo ricorsivo con la mano aggiornata.

### 2️⃣ Probabilità di vincere stando

Se il giocatore decide di non pescare, la probabilità di vincere è calcolata considerando la distribuzione delle mani possibili per l'avversario (dealer) e confrontandole con la mano corrente del giocatore.

$$
P_{stand} = \sum_{y \in Y} P(y) \cdot P_{win | y}
$$

Dove: 
- *Y* rappresenta tutte le possibili mani del dealer
- *P(y)* è la probabilità che il dealer abbia una mano 𝑦
- *P<sub>win | y</sub>* è la probabilità di vincere contro quella mano

Anche in questo caso, la probabilità *P<sub>win | y</sub>* iene calcolata in modo ricorsivo, perché il dealer può anche decidere di pescare in base alle regole del gioco.

## 🔁 Approccio Ricorsivo

L'algoritmo esplora tutte le possibilità, aggiornando dinamicamente le probabilità a ogni iterazione e garantendo un calcolo accurato delle decisioni ottimali. Grazie a questo approccio:
✅ Si tiene conto di tutte le possibili carte che possono essere pescate
✅ Si valuta il comportamento del dealer in base alla sua strategia di gioco
✅ Si ottiene una decisione ottimizzata che massimizza le probabilità di vittoria

## ⚡ Implementazione Tecnica

📌 Linguaggio: C++ con Qt per la gestione dell’interfaccia grafica
📌 Gestione dell’input: Selezione dinamica delle carte e configurazione personalizzata del mazzo
📌 Ottimizzazione: Parametri configurabili per migliorare l’accuratezza delle decisioni

## 📂 Struttura del Progetto

📁 BJ/ → Contiene l'implementazione dell'algoritmo probabilistico
📁 Portable/ → Versione precompilata dell’applicazione

## 🎯 Applicazione all'Analisi del Blackjack

Questo modello è stato applicato per analizzare le probabilità di vincita in un gioco di carte, calcolando se conviene pescare o stare in base a un'analisi stocastica del mazzo rimanente e del comportamento dell’avversario.

Grazie all’algoritmo ricorsivo, il sistema è in grado di simulare scenari realistici, fornendo suggerimenti ottimali basati su calcoli probabilistici.

![Alt text](Img/Gui.png)
