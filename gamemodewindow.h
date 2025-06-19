#ifndef GAMEMODEWINDOW_H
#define GAMEMODEWINDOW_H

#include <QMainWindow>
#include "gameboard.h"
#include "playernamewindow.h"
#include "aidifficultywindow.h"
#include "settingswindow.h"

namespace Ui {
class GameModeWindow;
}

class GameModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameModeWindow(const QString &username = "", QWidget *parent = nullptr); // Added username parameter
    ~GameModeWindow();

private slots:
    void on_playerVsPlayer_clicked();
    void on_playerVsAI_clicked();
    void onPlayerNamesEntered(const QString &player1, const QString &player2);
    void onAIDifficultySelected(const QString &difficulty);
    void on_settingButton_clicked();

private:
    Ui::GameModeWindow *ui;
    GameBoard *gameBoard;
    PlayerNameWindow *playerNameWindow;
    AIDifficultyWindow *aiDifficultyWindow;
    SettingsWindow *settingsWindow;
    QString currentUsername; // Added to store the current username
};

#endif // GAMEMODEWINDOW_H
