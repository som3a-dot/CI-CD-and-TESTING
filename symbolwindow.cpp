#include "symbolwindow.h"
#include "ui_symbolwindow.h"
#include <QDebug>

SymbolWindow::SymbolWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SymbolWindow),
    selectedSymbol("X")
{
    ui->setupUi(this);
}

SymbolWindow::~SymbolWindow()
{
    delete ui;
}

QString SymbolWindow::getSelectedSymbol() const
{
    return selectedSymbol;
}

void SymbolWindow::on_xButton_clicked()
{
    selectedSymbol = "X";
    emit symbolSelected(selectedSymbol);
    this->hide();
}


void SymbolWindow::on_oButton_2_clicked()
{
    selectedSymbol = "O";
    emit symbolSelected(selectedSymbol);
    this->hide();
}

