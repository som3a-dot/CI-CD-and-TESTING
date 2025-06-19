#include "aidifficultywindow.h"
#include "ui_aidifficultywindow.h"
#include <QDebug>

AIDifficultyWindow::AIDifficultyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AIDifficultyWindow)
{
    ui->setupUi(this);
}

AIDifficultyWindow::~AIDifficultyWindow()
{
    delete ui;
}

QString AIDifficultyWindow::getDifficulty() const
{
    return selectedDifficulty;
}

void AIDifficultyWindow::on_easyButton_clicked()
{
    selectedDifficulty = "Easy";
    emit difficultySelected(selectedDifficulty);
    this->hide();
}

void AIDifficultyWindow::on_mediumButton_clicked()
{
    selectedDifficulty = "Medium";
    emit difficultySelected(selectedDifficulty);
    this->hide();
}

void AIDifficultyWindow::on_hardButton_clicked()
{
    selectedDifficulty = "Hard";
    emit difficultySelected(selectedDifficulty);
    this->hide();
}
