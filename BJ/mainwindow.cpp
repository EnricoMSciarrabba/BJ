#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QSvgRenderer>

#include "cardwidget.h"
#include "cardblock.h"

// Template per caricare Iconne
#define HEARTS_ICON_TEMPLATE "://card/hearts_%1.svg"
#define DIAMONDS_ICON_TEMPLATE  "://card/diamonds_%1.svg"
#define CLUBS_ICON_TEMPLATE  "://card/clubs_%1.svg"
#define SPADES_ICON_TEMPLATE  "://card/spades_%1.svg"

// Template della probabilità
#define PROB_TEMPLATE "%1%"

// ============================================
//
//                 MainWindow
//
// ============================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inizializzo icone
    init();
    initIcon();
}

MainWindow::~MainWindow()
{
    delete ui;

    // Elimino le icone
    for(QSvgRenderer* svg : m_cardIcon)
    {
        delete svg;
    }
}



// ===================================
//
//              Init
//
// ===================================

void MainWindow::init()
{
    // Inizializzo il manager con i valori di default della grafica
    m_manager.setNumberOfDecks(ui->numberOfDecks_qSpinBox->value());
    m_manager.setOptimization(ui->optimization_qHorizontalSlider->value());

    // Imposto la WaitingDialog Modale in modo che blocchi le interazioni della UI quando visibile
    m_waitingDialog.setModal(true);

    // Richiamo il metodo reset per la carta del Dealer in modo da forzare l'inserimento di
    // una carta vuota per dimensionare correttamente il widget
    setDealerCard();

    // Connect pannello Commands
    connect(ui->numberOfDecks_qSpinBox,         &QSpinBox::valueChanged, this, &MainWindow::subNumberOfDecksChanged);
    connect(ui->optimization_qHorizontalSlider, &QSlider::valueChanged,  this, &MainWindow::subOptimizationChanged);
    connect(ui->continue_qPushButton,           &QPushButton::clicked,   this, &MainWindow::subContinueClicked);
    connect(ui->calculate_qPushButton,          &QPushButton::clicked,   this, &MainWindow::subCalculateClicked);

    // Connect con Manager
    connect(&m_manager, &Manager::pubProbCalculated, this, &MainWindow::subProbCalculated);
    connect(&m_manager, &Manager::pubProbStopped,    this, &MainWindow::subProbStopped);

    // Quando viene premuto abort sulla WaitingDialog forzo lo stop del Thread
    connect(&m_waitingDialog, &WaitingDialog::pubAbortClicked, &m_manager, &Manager::stopCalculate);
}

void MainWindow::initIcon()
{
    // Creo template
    QList<QString> cardTemp;
    cardTemp << HEARTS_ICON_TEMPLATE;
    cardTemp << DIAMONDS_ICON_TEMPLATE;
    cardTemp << CLUBS_ICON_TEMPLATE;
    cardTemp << SPADES_ICON_TEMPLATE;

    // Per ogni template
    for(QString& iconTemplate : cardTemp)
    {
        // Carico Asso
        m_cardIcon.append(new QSvgRenderer(iconTemplate.arg("ace")));

        // Carico i numeri da 2 a 10
        for(unsigned short num=2; num<=10; num++)
        {
            m_cardIcon.append(new QSvgRenderer(iconTemplate.arg(QString::number(num))));
        }

        // Carico le figure
        m_cardIcon.append(new QSvgRenderer(iconTemplate.arg("jack")));
        m_cardIcon.append(new QSvgRenderer(iconTemplate.arg("queen")));
        m_cardIcon.append(new QSvgRenderer(iconTemplate.arg("king")));
    }

    // Inserisco le carte nel deck
    for(unsigned int i=0; i<SUITS; i++)
    {
        for(int j=0; j<MAX_CARD; j++)
        {
            // Creo la pixmap da mostrare
            QPixmap pixmap(DECK_ICON_SIZE);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            m_cardIcon[MAX_CARD*i + j]->render(&painter);

            // Creo la card
            CardBlock* card = new CardBlock(MAX_CARD*i + j, pixmap, CARDS_FOR_NUMBER, ui->deck_qScrollArea);

            // Mostro la card
            ui->gridLayout->addWidget(card, i, j);

            // Connect con metodi interni
            connect(card, &CardBlock::pubPlayerClicked, this, &MainWindow::subAddPlayerCardClicked);
            connect(card, &CardBlock::pubDealerClicked, this, &MainWindow::subAddDealerCardClicked);
            connect(card, &CardBlock::pubPlusClicked,   this, &MainWindow::subPlusCardClicked);
            connect(card, &CardBlock::pubMinusClicked,  this, &MainWindow::subMinusCardClicked);
        }
    }
}



// ===================================
//
//          Click in Commands
//
// ===================================

void MainWindow::subNumberOfDecksChanged(int numberOfDecks)
{
    // Aggiorno il manager
    m_manager.setNumberOfDecks(numberOfDecks);

    // Aggiorno il numero di carte nei vari CardBlock
    CardBlock* cardBlock;
    for(int index=0; index<ui->deck_scrollAreaWidgetContents->layout()->count(); index++)
    {
        // Recupero la card e aggiorno il numero totale di carte
        cardBlock = static_cast<CardBlock*>(ui->deck_scrollAreaWidgetContents->layout()->itemAt(index)->widget());
        cardBlock->setTotalCardInGroup(numberOfDecks * SUITS);
    }

    // Reset Grafico delle carte del Player e del Dealer
    resetPlayerCard();
    setDealerCard();

    // Reset delle probabilità
    setProbabilityLabel();
}

void MainWindow::subOptimizationChanged(int optimization)
{
    // Aggiorno il manager
    m_manager.setOptimization(optimization);
}

void MainWindow::subContinueClicked()
{
    // Svuoto la mano del Player e del Dealer nel Manager
    m_manager.resetPlayerCard();
    m_manager.resetDealerCard();

    // Elimino le carte del Player e del Dealer
    resetPlayerCard();
    setDealerCard();

    // Reset delle probabilità
    setProbabilityLabel();
}

void MainWindow::subCalculateClicked()
{
    // Disabilito il widget in modo che l'utente non possa più interagirvi
    setEnabled(false);

    // Reset delle probabilità
    setProbabilityLabel();

    // Provo ad effettuare il calcolo
    // Se non c'è bisogno del calcolo
    if(not m_manager.startCalculate())
    {
        // Vuol dire che l'utente deve sempre pescare
        setProbabilityLabel(1., 0.);

        // Riabilito il widget
        setEnabled(true);

        return;
    }

    // Altrimenti se il calcolo è iniziato mostro la WaitingPage
    m_waitingDialog.show();

    // Attendo la risposta dal Thread
}



// ===================================
//
//         Click in CardBlock
//
// ===================================


void MainWindow::subAddPlayerCardClicked(unsigned short id)
{
    // Aggiungo la carta al manager
    m_manager.addPlayerCard(id%MAX_CARD +1);

    // Creo un nuovo widget
    CardWidget* card = new CardWidget(m_cardIcon[id], ui->playerCard_qScrollArea);

    // Lo inserisco nella View
    QHBoxLayout* layout = static_cast<QHBoxLayout*>(ui->playerCard_scrollAreaWidgetContents->layout());
    layout->insertWidget(ui->playerCard_scrollAreaWidgetContents->layout()->count()-1, card);

    // Resetto le probabilità
    setProbabilityLabel();
}

void MainWindow::subAddDealerCardClicked(unsigned short id)
{
    // Aggiungo la carta al Dealer
    m_manager.addDealerCard(id%MAX_CARD +1);

    // Aggiorno la card del Dealer
    setDealerCard(id);

    // Resetto le probabilità
    setProbabilityLabel();
}

void MainWindow::subMinusCardClicked(unsigned short id)
{
    // Rimuovo la carta dal manager
    m_manager.discardCard(id%MAX_CARD +1);

    // Resetto le probabilità
    setProbabilityLabel();
}

void MainWindow::subPlusCardClicked(unsigned short id)
{
    // Aggiungo la carta nel manager
    m_manager.insertCard(id%MAX_CARD +1);

    // Resetto le probabilità
    setProbabilityLabel();
}



// ===================================
//
//             Utility
//
// ===================================

void MainWindow::resetPlayerCard()
{
    // Per ogni item nella ScrollArea delle carte del Player
    QLayoutItem* item;
    while(ui->playerCard_scrollAreaWidgetContents->layout()->count() != 0)
    {
        // Se il primo item non è una card vuol dire che è lo spacer (termino)
        if(dynamic_cast<CardWidget*>(ui->playerCard_scrollAreaWidgetContents->layout()->itemAt(0)->widget()) == nullptr)
        {
            break;
        }

        // Recupero l'item
        item = ui->playerCard_scrollAreaWidgetContents->layout()->takeAt(0);

        // Elimino item e widget
        delete item->widget();
        delete item;
    }
}

void MainWindow::setDealerCard(unsigned short id)
{
    // Se è presente una carta del Dealer
    QLayoutItem* item;
    if(ui->dealerCard_qGroupBox->layout()->count() != 0)
    {
        // Recupero la carta
        item = ui->dealerCard_qGroupBox->layout()->takeAt(0);

        // Elimino item e widget
        delete item->widget();
        delete item;
    }

    // Controllo se è un reset
    CardWidget* card;
    if(id == static_cast<unsigned short>(-1))
    {
        // Inserisco una carta vuota per mantenere la size
        card = new CardWidget(nullptr, ui->dealerCard_qGroupBox);
    }

    // Altrimenti mostro la carta richiesta
    else
    {
        card = new CardWidget(m_cardIcon[id], ui->dealerCard_qGroupBox);
    }

    // La inserisco nella View
    ui->dealerCard_qGroupBox->layout()->addWidget(card);
}

void MainWindow::setProbabilityLabel(const double &hitProbability, const double &standProbability)
{
    // Se sono entrambi -1 voglio il reset
    if(hitProbability == -1. and standProbability == -1.)
    {
        ui->hit_qLabel->setText("-");
        ui->stand_qLabel->setText("-");
    }

    // Altrimenti aggiorno i valori
    else
    {
        ui->hit_qLabel->setText(QString(PROB_TEMPLATE).arg(QString::number(hitProbability*100., 'g', 4)));
        ui->stand_qLabel->setText(QString(PROB_TEMPLATE).arg(QString::number(standProbability*100., 'g', 4)));
    }
}



// ===================================
//
//     Calcolo delle probabilità
//
// ===================================


void MainWindow::subProbCalculated(const double &hitProbability, const double &standProbability)
{
    // Aggiorno le probabilità
    setProbabilityLabel(hitProbability, standProbability);

    // Chiudo la WaitingDialog
    m_waitingDialog.accept();

    // Riabilito il widget
    setEnabled(true);
}

void MainWindow::subProbStopped()
{
    // Reset delle probabilità
    setProbabilityLabel();

    // Chiudo la WaitingDialog
    m_waitingDialog.reject();

    // Riabilito il widget
    setEnabled(true);
}
