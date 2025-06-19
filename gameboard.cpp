#include "gameboard.h"
#include "ui_gameboard.h"
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include "gamemodewindow.h"

GameBoard::GameBoard(const QString &username, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameBoard),
    currentPlayer(1),
    gameMode(PlayerVsPlayer),
    aiDifficulty("Easy"),
    aiPlayer(nullptr),
    playerSymbol('X'),
    aiSymbol('O'),
    player1Name("Player 1"),
    player2Name("Player 2"),
    player1Score(0),
    player2Score(0),
    gameHistory(username)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = 0;

    ui->setupUi(this);
    connect(ui->button_0_0, &QPushButton::clicked, this, &GameBoard::onButtonClicked);
    connect(ui->button_0_1, &QPushButton::clicked, this, &GameBoard::onButtonClicked);
    connect(ui->button_0_2, &QPushButton::clicked, this, &GameBoard::onButtonClicked);
    connect(ui->button_1_0, &QPushButton::clicked, this, &GameBoard::onButtonClicked);
    connect(ui->button_1_1, &QPushButton::clicked, this, &GameBoard::onButtonClicked);
    connect(ui->button_1_2, &QPushButton::clicked, this, &GameBoard::onButtonClicked);
    connect(ui->button_2_0, &QPushButton::clicked, this, &GameBoard::onButtonClicked);
    connect(ui->button_2_1, &QPushButton::clicked, this, &GameBoard::onButtonClicked);
    connect(ui->button_2_2, &QPushButton::clicked, this, &GameBoard::onButtonClicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &GameBoard::on_resetButton_clicked);
    connect(ui->ReturnButton, &QPushButton::clicked, this, &GameBoard::on_ReturnButton_clicked);

    resetBoard();
}

GameBoard::~GameBoard()
{
    delete ui;
    delete aiPlayer;
}

void GameBoard::onButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button || !button->text().isEmpty()) return;

    QString moveSymbol = (gameMode == PlayerVsPlayer) ? (currentPlayer == 1 ? "X" : "O") :
                             (currentPlayer == 1 ? QString(playerSymbol) : QString(aiSymbol));
    int row = button == ui->button_0_0 || button == ui->button_0_1 || button == ui->button_0_2 ? 0 :
                  (button == ui->button_1_0 || button == ui->button_1_1 || button == ui->button_1_2 ? 1 : 2);
    int col = (button == ui->button_0_0 || button == ui->button_1_0 || button == ui->button_2_0 ? 0 :
                   (button == ui->button_0_1 || button == ui->button_1_1 || button == ui->button_2_1 ? 1 : 2));
    board[row][col] = (currentPlayer == 1) ? 1 : 2;
    button->setText(moveSymbol);
    currentGameMoves.append({row, col});
    currentPlayer = 3 - currentPlayer;

    if (checkWin()) {
        QString winner = (currentPlayer == 2) ? player1Name : player2Name;
        QString winnerSymbol = (currentPlayer == 2) ? (gameMode == PlayerVsPlayer ? "X" : QString(playerSymbol)) :
                                   (gameMode == PlayerVsPlayer ? "O" : QString(aiSymbol));
        QMessageBox::information(this, "Winner", winner + " wins!");
        if (currentPlayer == 2) player1Score++;
        else player2Score++;
        updateScore();
        saveGameHistory(winner + " wins");
        resetBoard();
        return;
    }

    bool tie = true;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == 0) tie = false;
    if (tie) {
        QMessageBox::information(this, "Game Over", "It's a tie!");
        saveGameHistory("Tie");
        resetBoard();
        return;
    }

    if (gameMode == PlayerVsAI && currentPlayer == 2 && aiPlayer) {
        int aiRow = -1, aiCol = -1;
        QVector<QVector<char>> aiBoard(3, QVector<char>(3, ' '));
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (board[i][j] == 1) aiBoard[i][j] = playerSymbol;
                else if (board[i][j] == 2) aiBoard[i][j] = aiSymbol;
        aiPlayer->makeMove(aiBoard, aiRow, aiCol);

        if (aiRow != -1 && aiCol != -1) {
            board[aiRow][aiCol] = 2;
            QString aiMoveSymbol = QString(aiSymbol);
            if (aiRow == 0 && aiCol == 0) ui->button_0_0->setText(aiMoveSymbol);
            else if (aiRow == 0 && aiCol == 1) ui->button_0_1->setText(aiMoveSymbol);
            else if (aiRow == 0 && aiCol == 2) ui->button_0_2->setText(aiMoveSymbol);
            else if (aiRow == 1 && aiCol == 0) ui->button_1_0->setText(aiMoveSymbol);
            else if (aiRow == 1 && aiCol == 1) ui->button_1_1->setText(aiMoveSymbol);
            else if (aiRow == 1 && aiCol == 2) ui->button_1_2->setText(aiMoveSymbol);
            else if (aiRow == 2 && aiCol == 0) ui->button_2_0->setText(aiMoveSymbol);
            else if (aiRow == 2 && aiCol == 1) ui->button_2_1->setText(aiMoveSymbol);
            else if (aiRow == 2 && aiCol == 2) ui->button_2_2->setText(aiMoveSymbol);
            currentGameMoves.append({aiRow, aiCol});
            currentPlayer = 1;
        }

        if (checkWin()) {
            QMessageBox::information(this, "Game Over", "AI wins!");
            player2Score++;
            updateScore();
            saveGameHistory("AI wins");
            resetBoard();
            return;
        }
        tie = true;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (board[i][j] == 0) tie = false;
        if (tie) {
            QMessageBox::information(this, "Game Over", "It's a tie!");
            saveGameHistory("Tie");
            resetBoard();
            return;
        }
    }
    updateStatus();
}

void GameBoard::on_resetButton_clicked()
{
    resetBoard();
}

void GameBoard::resetBoard()
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            board[i][j] = 0;
            if (i == 0 && j == 0) ui->button_0_0->setText("");
            else if (i == 0 && j == 1) ui->button_0_1->setText("");
            else if (i == 0 && j == 2) ui->button_0_2->setText("");
            else if (i == 1 && j == 0) ui->button_1_0->setText("");
            else if (i == 1 && j == 1) ui->button_1_1->setText("");
            else if (i == 1 && j == 2) ui->button_1_2->setText("");
            else if (i == 2 && j == 0) ui->button_2_0->setText("");
            else if (i == 2 && j == 1) ui->button_2_1->setText("");
            else if (i == 2 && j == 2) ui->button_2_2->setText("");
        }
    currentPlayer = 1;
    currentGameMoves.clear();
    updateStatus();
}

bool GameBoard::checkWin()
{
    for (int i = 0; i < 3; ++i)
        if (board[i][0] && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return true;

    for (int j = 0; j < 3; ++j)
        if (board[0][j] && board[0][j] == board[1][j] && board[1][j] == board[2][j])
            return true;

    if (board[0][0] && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return true;
    if (board[0][2] && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return true;

    return false;
}

void GameBoard::updateStatus()
{
    if (ui->statusLabel) {
        QString currentName = (gameMode == PlayerVsAI && currentPlayer == 2) ? "AI" : (currentPlayer == 1 ? player1Name : player2Name);
        QString symbol = (gameMode == PlayerVsPlayer) ? (currentPlayer == 1 ? "X" : "O") :
                             (currentPlayer == 1 ? QString(playerSymbol) : QString(aiSymbol));
        ui->statusLabel->setText("Current Player: " + currentName + " (" + symbol + ")");
    }
}

void GameBoard::updateScore()
{
    if (ui->scoreLabel) {
        ui->scoreLabel->setText(QString("Score: %1: %2, %3: %4")
                                    .arg(player1Name).arg(player1Score)
                                    .arg(gameMode == PlayerVsAI ? "AI" : player2Name).arg(player2Score));
    }
}

void GameBoard::saveGameHistory(const QString &outcome)
{
    GameSession session;
    session.player1Name = player1Name;
    session.player2Name = player2Name;
    session.gameMode = (gameMode == PlayerVsPlayer) ? "Player vs Player" : "Player vs AI";
    session.outcome = outcome;
    session.timestamp = QDateTime::currentDateTime();
    session.moves = currentGameMoves;
    session.winnerSymbol = (outcome == "Tie") ? "" : ((currentPlayer == 2) ? (gameMode == PlayerVsPlayer ? "X" : QString(playerSymbol)) :
                                                          (gameMode == PlayerVsPlayer ? "O" : QString(aiSymbol)));
    gameHistory.saveGame(session);
}

void GameBoard::setGameMode(GameMode mode)
{
    gameMode = mode;
    if (gameMode == PlayerVsAI) {
        char aiSym = (playerSymbol == 'X') ? 'O' : 'X';
        aiSymbol = aiSym;
        if (aiPlayer) delete aiPlayer;
        aiPlayer = new AIPlayer(aiSymbol, playerSymbol, aiDifficulty);
        player2Name = "AI";
    } else if (gameMode == PlayerVsPlayer) {
        delete aiPlayer;
        aiPlayer = nullptr;
        playerSymbol = 'X';
        aiSymbol = 'O';
        player2Name = "Player 2";
    }
    resetBoard();
    updateStatus();
    updateScore();
}

void GameBoard::setAIDifficulty(const QString &difficulty)
{
    aiDifficulty = difficulty;
    if (gameMode == PlayerVsAI) {
        char aiSym = (playerSymbol == 'X') ? 'O' : 'X';
        aiSymbol = aiSym;
        if (aiPlayer) delete aiPlayer;
        aiPlayer = new AIPlayer(aiSymbol, playerSymbol, aiDifficulty);
    }
    resetBoard();
}

void GameBoard::setPlayerNames(const QString &player1, const QString &player2)
{
    player1Name = player1.isEmpty() ? "Player 1" : player1;
    player2Name = (gameMode == PlayerVsAI) ? "AI" : (player2.isEmpty() ? "Player 2" : player2);
    updateStatus();
    updateScore();
}

void GameBoard::setPlayerSymbol(char symbol)
{
    if (symbol != 'X' && symbol != 'O') {
        qDebug() << "Invalid symbol:" << symbol;
        return;
    }

    if (gameMode == PlayerVsAI) {
        playerSymbol = symbol;
        aiSymbol = (playerSymbol == 'X') ? 'O' : 'X';
        if (aiPlayer) delete aiPlayer;
        aiPlayer = new AIPlayer(aiSymbol, playerSymbol, aiDifficulty);
        resetBoard();
        updateStatus();
    } else {
        qDebug() << "Symbol selection ignored in PlayerVsPlayer mode; Player 1 is X, Player 2 is O";
        playerSymbol = 'X';
        aiSymbol = 'O';
    }
}

void GameBoard::on_ReturnButton_clicked()
{
    this->hide();
    GameModeWindow *gameModeWindow = qobject_cast<GameModeWindow*>(parent());
    if (gameModeWindow) {
        gameModeWindow->show();
        gameModeWindow->activateWindow();
    } else {
        gameModeWindow = new GameModeWindow("", nullptr);
        if (gameModeWindow) {
            gameModeWindow->show();
            gameModeWindow->activateWindow();
        }
    }
}
