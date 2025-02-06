#ifndef BJ_CARDBLOCK_H
#define BJ_CARDBLOCK_H

#include <QWidget>

namespace Ui {
class CardBlock;
}

// ============================================
//
//              CardBlock
//
// ============================================

class CardBlock : public QWidget
{
    Q_OBJECT

public:
    explicit CardBlock(unsigned short id, QPixmap& icon, unsigned short totalCardInGroup, QWidget *parent = nullptr);
    ~CardBlock();



    // ========================
    //
    //  Override

protected:
    // QWidget
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event)      override;
    virtual void paintEvent(QPaintEvent *event) override;



    // ========================
    //
    //  Interfacce esterne

public:
    // Reset del widget
    void reset();

    // Aggiornamento esterno del numero di carte
    void setTotalCardInGroup(unsigned short totalCardInGroup);

    // Segnali per comunicare esternamente il click dei pulsanti
    Q_SIGNAL void pubPlayerClicked(unsigned short id);
    Q_SIGNAL void pubDealerClicked(unsigned short id);
    Q_SIGNAL void pubMinusClicked(unsigned short id);
    Q_SIGNAL void pubPlusClicked(unsigned short id);



    // ========================
    //
    //  Metodi interni

private:
    // Slot per il click dei pulsanti
    Q_SLOT void subPlayerClicked();
    Q_SLOT void subDealerClicked();
    Q_SLOT void subMinusClicked();
    Q_SLOT void subPlusClicked();

    // Controllo dei pulsanti
    void checkButtons();
    void updateLabels();



    // ========================
    //
    //  Variabili

private:
    Ui::CardBlock *ui;

    // Valori delle carte
    unsigned short m_id;
    unsigned short m_totalCardInGroup;
    unsigned short m_cardInGroup;

    // Variabili di stato
    bool m_isHovered;
};

#endif // BJ_CARDBLOCK_H
