#include "gamemodewindow.h"
#include "symbolwindow.h"
#include "ui_gamemodewindow.h"
#include <QDebug>

GameModeWindow::GameModeWindow(const QString &username, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameModeWindow),
    gameBoard(nullptr),
    playerNameWindow(nullptr),
    aiDifficultyWindow(nullptr),
    settingsWindow(nullptr),
    currentUsername(username)
{
    ui->setupUi(this);
}

GameModeWindow::~GameModeWindow()
{
    delete ui;
    delete gameBoard;
    delete playerNameWindow;
    delete aiDifficultyWindow;
    delete settingsWindow;
}

void GameModeWindow::on_playerVsPlayer_clicked()
{
    playerNameWindow = new PlayerNameWindow(this);
    if (playerNameWindow) {
        connect(playerNameWindow, &PlayerNameWindow::namesEntered, this, &GameModeWindow::onPlayerNamesEntered);
        playerNameWindow->show();
        this->hide();
    } else {
        qDebug() << "Failed to create PlayerNameWindow";
    }
}

void GameModeWindow::on_playerVsAI_clicked()
{
    aiDifficultyWindow = new AIDifficultyWindow(this);
    if (aiDifficultyWindow) {
        connect(aiDifficultyWindow, &AIDifficultyWindow::difficultySelected, this, &GameModeWindow::onAIDifficultySelected);
        aiDifficultyWindow->show();
        this->hide();
    } else {
        qDebug() << "Failed to create AIDifficultyWindow";
    }
}

void GameModeWindow::onPlayerNamesEntered(const QString &player1, const QString &player2)
{
    gameBoard = new GameBoard(currentUsername, this); // Set GameModeWindow as parent
    if (gameBoard) {
        gameBoard->setGameMode(GameBoard::PlayerVsPlayer);
        gameBoard->setPlayerNames(player1, player2);
        gameBoard->show();
        this->hide();
    } else {
        qDebug() << "Failed to create GameBoard";
    }
}

void GameModeWindow::onAIDifficultySelected(const QString &difficulty)
{
    SymbolWindow *symbolWindow = new SymbolWindow(this);
    if (symbolWindow) {
        connect(symbolWindow, &SymbolWindow::symbolSelected, this, [this, difficulty](const QString &symbol) {
            gameBoard = new GameBoard(currentUsername, this); // Set GameModeWindow as parent
            if (gameBoard) {
                gameBoard->setGameMode(GameBoard::PlayerVsAI);
                gameBoard->setAIDifficulty(difficulty);
                gameBoard->setPlayerSymbol(symbol.toUpper()[0].toLatin1());
                gameBoard->setPlayerNames("Player", "AI");
                gameBoard->show();
            } else {
                qDebug() << "Failed to create GameBoard";
            }
            delete aiDifficultyWindow;
            aiDifficultyWindow = nullptr;
        });
        symbolWindow->show();
    } else {
        qDebug() << "Failed to create SymbolWindow";
    }
}

void GameModeWindow::on_settingButton_clicked()
{
    settingsWindow = new SettingsWindow(currentUsername, this);
    if (settingsWindow) {
        settingsWindow->show();
    } else {
        qDebug() << "Failed to create SettingsWindow";
    }
}
