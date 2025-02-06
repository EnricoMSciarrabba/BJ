#include "cardwidget.h"

#include <QSvgRenderer>
#include <QVBoxLayout>
#include <QPainter>
#include <QLabel>

// ============================================
//
//                 CardWidget
//
// ============================================

CardWidget::CardWidget(QSvgRenderer* icon, QWidget* parent)
    : QWidget(parent)
    , m_cardIcon(icon)
{
    // Inserisco un layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);

    // Creo una label per inserire la pixmap
    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);

    // Inserisco label nel layout
    layout->addWidget(m_label);
}



// ===================================
//
//             Override
//
// ===================================

void CardWidget::resizeEvent(QResizeEvent *event)
{
    // Calcolo la nuova width
    int newWidth = size().toSizeF().height() * DECK_ICON_SIZE.width() / DECK_ICON_SIZE.height();

    // Aggiorno width
    setMinimumWidth(newWidth);

    // Se l'icona non è nulla
    if(m_cardIcon != nullptr)
    {
        // Creo la pixmap da mostrare
        QPixmap pixmap(QSize(newWidth, size().height()));
        pixmap.fill(Qt::transparent);

        // Creo il painter
        QPainter painter(&pixmap);
        m_cardIcon->render(&painter);

        // Mostro la pixmap
        m_label->setPixmap(pixmap);
    }

    // Chiamo il metodo padre
    QWidget::resizeEvent(event);
}

