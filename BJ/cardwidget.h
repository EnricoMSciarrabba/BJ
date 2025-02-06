#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>

#define DECK_ICON_SIZE QSize(78, 111)

class QSvgRenderer;
class QLabel;

// ============================================
//
//                 CardWidget
//
// ============================================

class CardWidget : public QWidget
{
public:
    CardWidget(QSvgRenderer* icon, QWidget* parent);



    // ========================
    //
    //  Override

protected:
    // QWidget
    virtual void resizeEvent(QResizeEvent *event) override;



    // ========================
    //
    //  Variabili

private:
    QLabel* m_label;
    QSvgRenderer* m_cardIcon;

    bool m_updateIcon = false;
};

#endif // CARDWIDGET_H
