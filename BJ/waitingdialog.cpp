#include "waitingdialog.h"
#include "ui_waitingdialog.h"

#include <QPropertyAnimation>

// ============================================
//
//               WaitingPage
//
// ============================================

WaitingDialog::WaitingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WaitingDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint);

    // Creo l'oggetto Animation
    m_animation1 = new QPropertyAnimation(ui->progressBar, "value", this);
    m_animation1->setDuration(700);
    m_animation1->setStartValue(ui->progressBar->minimum());
    m_animation1->setEndValue(ui->progressBar->maximum());
    m_animation1->setEasingCurve(QEasingCurve::InOutQuad);

    // Creo l'oggetto Animation
    m_animation2 = new QPropertyAnimation(ui->progressBar2, "value", this);
    m_animation2->setDuration(700);
    m_animation2->setStartValue(ui->progressBar->minimum());
    m_animation2->setEndValue(ui->progressBar->maximum());
    m_animation2->setEasingCurve(QEasingCurve::InOutQuad);

    // Aggiungo le animazione al gruppo
    m_animationGroup.addAnimation(m_animation1);
    m_animationGroup.addAnimation(m_animation2);

    // Connect per invertire l'animazione
    connect(&m_animationGroup, &QParallelAnimationGroup::finished, this, &WaitingDialog::subAnimationEnded);

    // Connect per comunicare Abort
    connect(ui->abort_qPushButton, &QPushButton::clicked, this, &WaitingDialog::subAbortClicked);
    connect(ui->abort_qPushButton, &QPushButton::clicked, this, &WaitingDialog::pubAbortClicked);
}

WaitingDialog::~WaitingDialog()
{
    delete ui;
}



// ===================================
//
//          Click Abort
//
// ===================================

void WaitingDialog::subAbortClicked()
{
    // Aggiorno label
    ui->label->setText("Aborting...");

    // Disabilito tasto abort
    ui->abort_qPushButton->setEnabled(false);
}



// ===================================
//
//          Fine animazione
//
// ===================================

void WaitingDialog::subAnimationEnded()
{
    // Memorizzo i valori di inizio e fine
    static int start1 = ui->progressBar->minimum();
    static int end1   = ui->progressBar->maximum();

    // Inverto
    int temp1 = start1;
    start1 = end1;
    end1 = temp1;

    m_animation1->setStartValue(start1);
    m_animation1->setEndValue(end1);

    // Memorizzo i valori di inizio e fine
    static int start2 = ui->progressBar2->minimum();
    static int end2   = ui->progressBar2->maximum();

    // Inverto
    int temp2  = start2;
    start2 = end2;
    end2  = temp2;

    m_animation2->setStartValue(start2);
    m_animation2->setEndValue(end2);

    // Riavvio animazioni
    m_animationGroup.start();
}



// ===================================
//
//           Override
//
// ===================================

void WaitingDialog::showEvent(QShowEvent *event)
{
    // Reset label
    ui->label->setText("Processing...");

    // Reset pulsante
    ui->abort_qPushButton->setEnabled(true);

    // Avvio animazione
    m_animationGroup.start();

    // Richiamo metodo padre
    QDialog::showEvent(event);
}

void WaitingDialog::hideEvent(QHideEvent *event)
{
    // Chiamo metodo padre
    QDialog::hideEvent(event);

    // Fermo animazione
    m_animationGroup.stop();
}
