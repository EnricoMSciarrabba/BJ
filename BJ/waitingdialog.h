#ifndef WAITINGDIALOG_H
#define WAITINGDIALOG_H

#include <QDialog>
#include <QParallelAnimationGroup>

class QPropertyAnimation;

namespace Ui {
class WaitingDialog;
}

// ============================================
//
//               WaitingPage
//
// ============================================

class WaitingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingDialog(QWidget *parent = nullptr);
    ~WaitingDialog();



    // ========================
    //
    //  Override

protected:
    // QWidget
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);



    // ========================
    //
    //  Interfacce esterne

public:
    Q_SIGNAL void pubAbortClicked();



    // ========================
    //
    //  Metodi interni

private:
    Q_SLOT void subAbortClicked();
    Q_SLOT void subAnimationEnded();



    // ========================
    //
    //  Variabili

private:
    Ui::WaitingDialog *ui;

    QPropertyAnimation* m_animation1;
    QPropertyAnimation* m_animation2;
    QParallelAnimationGroup m_animationGroup;
};

#endif // WAITINGDIALOG_H
