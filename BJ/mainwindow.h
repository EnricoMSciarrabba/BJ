#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "manager.h"
#include "waitingdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QSvgRenderer;

// ============================================
//
//              MainWindow
//
// ============================================

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



    // ========================
    //
    //  Metodi interni

private:
    // Inizializzazione
    void init();
    void initIcon();

    // Azioni in Commands
    Q_SLOT void subNumberOfDecksChanged(int numberOfDecks);
    Q_SLOT void subOptimizationChanged(int optimization);
    Q_SLOT void subContinueClicked();
    Q_SLOT void subCalculateClicked();

    // Click in CardBox
    Q_SLOT void subAddPlayerCardClicked(unsigned short id);
    Q_SLOT void subAddDealerCardClicked(unsigned short id);
    Q_SLOT void subMinusCardClicked(unsigned short id);
    Q_SLOT void subPlusCardClicked(unsigned short id);

    // Risultati del calcolo probabilità
    Q_SLOT void subProbStopped();
    Q_SLOT void subProbCalculated(const double& hitProbability,
                                  const double& standProbability);

    // Utility grafiche
    void resetPlayerCard();
    void setDealerCard(unsigned short id = -1);
    void setProbabilityLabel(const double& hitProbability= -1.,
                             const double& standProbability= -1.);



    // ========================
    //
    //  Variabili

private:
    Ui::MainWindow *ui;

    QList<QSvgRenderer*> m_cardIcon;
    Manager m_manager;

    WaitingDialog m_waitingDialog;
};
#endif // MAINWINDOW_H
