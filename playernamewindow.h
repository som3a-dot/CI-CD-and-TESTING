#ifndef PLAYERNAMEWINDOW_H
#define PLAYERNAMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class PlayerNameWindow;
}

class PlayerNameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerNameWindow(QWidget *parent = nullptr);
    ~PlayerNameWindow();
    QString getPlayer1Name() const;
    QString getPlayer2Name() const;

signals:
    void namesEntered(const QString &player1, const QString &player2);

private slots:
    void on_okButton_clicked();
    void on_clearButton_clicked();
    void on_okButton_2_clicked();

private:
    Ui::PlayerNameWindow *ui;
};

#endif // PLAYERNAMEWINDOW_H
