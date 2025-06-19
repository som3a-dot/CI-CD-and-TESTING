#include "playernamewindow.h"
#include "ui_playernamewindow.h"
#include <QDebug>

PlayerNameWindow::PlayerNameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerNameWindow)
{
    ui->setupUi(this);
}

PlayerNameWindow::~PlayerNameWindow()
{
    delete ui;
}

QString PlayerNameWindow::getPlayer1Name() const
{
    return ui->lineEditPlayer1->text().trimmed();
}

QString PlayerNameWindow::getPlayer2Name() const
{
    return ui->lineEditPlayer2->text().trimmed();
}

void PlayerNameWindow::on_clearButton_clicked()
{
    ui->lineEditPlayer1->clear();
    ui->lineEditPlayer2->clear();
}


void PlayerNameWindow::on_okButton_2_clicked()
{
    QString player1 = getPlayer1Name();
    QString player2 = getPlayer2Name();
    if (!player1.isEmpty() && !player2.isEmpty()) {
        emit namesEntered(player1, player2);
        this->hide(); // Hide after emitting names
    } else {
        qDebug() << "Player names cannot be empty";
    }
}

