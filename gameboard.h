#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QMainWindow>
#include <QVector>
#include "aiplayer.h"
#include "gamehistory.h"

namespace Ui {
class GameBoard;
}

class GameBoard : public QMainWindow
{
    Q_OBJECT

public:
    enum GameMode { PlayerVsPlayer, PlayerVsAI };
    explicit GameBoard(const QString &username = "", QWidget *parent = nullptr); // Added username parameter
    ~GameBoard();

    void setGameMode(GameMode mode);
    void setAIDifficulty(const QString &difficulty);
    void setPlayerNames(const QString &player1, const QString &player2);
    void setPlayerSymbol(char symbol);

private slots:
    void onButtonClicked();
    void on_resetButton_clicked();
    void on_ReturnButton_clicked();

private:
    void resetBoard();
    bool checkWin();
    void updateStatus();
    void updateScore();
    void saveGameHistory(const QString &outcome);

    Ui::GameBoard *ui;
    int board[3][3];
    int currentPlayer;
    GameMode gameMode;
    QString aiDifficulty;
    AIPlayer *aiPlayer;
    char playerSymbol;
    char aiSymbol;
    QString player1Name;
    QString player2Name;
    int player1Score;
    int player2Score;
    GameHistory gameHistory; // Initialized with username in constructor
    QVector<QPair<int, int>> currentGameMoves; // Track moves for the current game
};

#endif // GAMEBOARD_H
