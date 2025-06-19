#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class registerwindow;
}

class registerwindow : public QDialog
{
    Q_OBJECT

public:
    explicit registerwindow(QWidget *parent = nullptr);
    ~registerwindow();

signals:
    void userRegistered(const QString &username, const QString &hashedPassword);

private slots:
    void on_registerButton_clicked();
    void on_cancelButton_2_clicked();
    void onPasswordTextChanged(const QString &text);


private:
    Ui::registerwindow *ui;
    QString hashPassword(const QString &password);
    QString checkPasswordStrength(const QString &password);
};

#endif // REGISTERWINDOW_H
