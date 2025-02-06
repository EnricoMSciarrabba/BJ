#include "cardblock.h"
#include "ui_cardblock.h"

#include <QPainter>

#define CARD_NUM_TEMPLATE "x%1"

// ============================================
//
//              CardBlock
//
// ============================================

CardBlock::CardBlock(unsigned short id, QPixmap& icon, unsigned short totalCardInGroup, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CardBlock)
    , m_id(id)
    , m_totalCardInGroup(totalCardInGroup)
    , m_cardInGroup(m_totalCardInGroup)
    , m_isHovered(false)
{
    ui->setupUi(this);

    // Imposto l'icona
    ui->icon_qLabel->setPixmap(icon);

    // Mostro il numero di carte
    updateLabels();

    // Forzo la visualizzazione del numero di carte
    ui->stackedWidget->setCurrentIndex(0);

    // Aggiorno i pulsanti
    checkButtons();

    // Connect
    connect(ui->player_qPushButton, &QPushButton::clicked, this, &CardBlock::subPlayerClicked);
    connect(ui->dealer_qPushButton, &QPushButton::clicked, this, &CardBlock::subDealerClicked);
    connect(ui->minus_qPushButton,  &QPushButton::clicked, this, &CardBlock::subMinusClicked);
    connect(ui->plus_qPushButton,   &QPushButton::clicked, this, &CardBlock::subPlusClicked);
}

CardBlock::~CardBlock()
{
    delete ui;
}



// ===================================
//
//              Reset
//
// ===================================

void CardBlock::reset()
{
    // Reset delle carte
    m_cardInGroup = m_totalCardInGroup;

    // Mostro il numero di carte
    updateLabels();

    // Aggiorno i pulsanti
    checkButtons();
}



// ===================================
//
//              Setter
//
// ===================================

void CardBlock::setTotalCardInGroup(unsigned short totalCardInGroup)
{
    // Aggiorno il numero totale di carte
    m_totalCardInGroup = totalCardInGroup;

    // Chiamo il reset
    reset();
}



// ===================================
//
//             Pulsanti
//
// ===================================

void CardBlock::subPlayerClicked()
{
    // Descremento il numero di card disponibili
    m_cardInGroup--;

    // Aggiorno i pulsanti
    checkButtons();

    // Mostro il numero di carte
    updateLabels();

    emit pubPlayerClicked(m_id);
}

void CardBlock::subDealerClicked()
{
    // Descremento il numero di card disponibili
    m_cardInGroup--;

    // Aggiorno i pulsanti
    checkButtons();

    // Mostro il numero di carte
    updateLabels();

    emit pubDealerClicked(m_id);
}

void CardBlock::subMinusClicked()
{
    // Descremento il numero di card disponibili
    m_cardInGroup--;

    // Aggiorno i pulsanti
    checkButtons();

    // Mostro il numero di carte
    updateLabels();

    emit pubMinusClicked(m_id);
}

void CardBlock::subPlusClicked()
{
    // Descremento il numero di card disponibili
    m_cardInGroup++;

    // Aggiorno i pulsanti
    checkButtons();

    // Mostro il numero di carte
    updateLabels();

    emit pubPlusClicked(m_id);
}



// ===================================
//
//             Utility
//
// ===================================

void CardBlock::checkButtons()
{
    // Aggiorno i pulsanti
    ui->player_qPushButton->setEnabled(m_cardInGroup != 0);
    ui->dealer_qPushButton->setEnabled(m_cardInGroup != 0);
    ui->minus_qPushButton->setEnabled(m_cardInGroup != 0);
    ui->plus_qPushButton->setEnabled(m_cardInGroup < m_totalCardInGroup);
}

void CardBlock::updateLabels()
{
    ui->number_qLabel->setText(QString(CARD_NUM_TEMPLATE).arg(QString::number(m_cardInGroup)));
    ui->number2_qLabel->setText(QString(CARD_NUM_TEMPLATE).arg(QString::number(m_cardInGroup)));
}



// ===================================
//
//             Override
//
// ===================================

void CardBlock::enterEvent(QEnterEvent*)
{
    // Salvo stato
    m_isHovered = true;

    // Mostro i pulsanti
    ui->stackedWidget->setCurrentIndex(1);

    // Aggiorno
    update();
}

void CardBlock::leaveEvent(QEvent*)
{
    // Salvo stato
    m_isHovered = false;

    // Mostro i pulsanti
    ui->stackedWidget->setCurrentIndex(0);

    // Aggiorno
    update();
}

void CardBlock::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    // Cambia colore quando il mouse è sopra
    if (m_isHovered)
    {
        painter.fillRect(rect(), QColor(255, 255, 255, 50));  // Colore chiaro
    }
    else
    {
        painter.fillRect(rect(), Qt::transparent);  // Colore normale
    }
}

