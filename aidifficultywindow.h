#ifndef AIDIFFICULTYWINDOW_H
#define AIDIFFICULTYWINDOW_H

#include <QMainWindow>
class GameBoard;

namespace Ui {
class AIDifficultyWindow;
}

class AIDifficultyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AIDifficultyWindow(QWidget *parent = nullptr);
    ~AIDifficultyWindow();
    QString getDifficulty() const;

signals:
    void difficultySelected(const QString &difficulty);

private slots:
    void on_easyButton_clicked();
    void on_mediumButton_clicked();
    void on_hardButton_clicked();
private:
    Ui::AIDifficultyWindow *ui;
    QString selectedDifficulty;
    GameBoard *gameBoard = nullptr;
};

#endif // AIDIFFICULTYWINDOW_H

