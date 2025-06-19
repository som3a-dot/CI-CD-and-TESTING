#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "mainwindow.h"
#include "historywindow.h"
#include <QDebug>

SettingsWindow::SettingsWindow(const QString &username, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow),
    currentUsername(username)
{
    ui->setupUi(this);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_switchaccountButton_3_clicked()
{
    MainWindow *mainWindow = new MainWindow(nullptr);
    if (mainWindow) {
        mainWindow->show();
        this->hide();
    } else {
        qDebug() << "Failed to create MainWindow";
    }
}

void SettingsWindow::on_historyButton_2_clicked()
{
    historywindow *historyWindow = new historywindow(currentUsername, nullptr);
    if (historyWindow) {
        historyWindow->show();
    } else {
        qDebug() << "Failed to create historywindow";
    }
}
