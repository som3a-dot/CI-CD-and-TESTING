#include "historywindow.h"
#include "ui_historywindow.h"
#include <QListWidget>
#include <QPushButton>
#include <QDebug>

historywindow::historywindow(const QString &username, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::historywindow)
    , gameHistory(username)
    , currentReplayIndex(-1)
    , currentMoveIndex(-1)
    , replayTimer(new QTimer(this))
{
    ui->setupUi(this);
    connect(ui->historyList, &QListWidget::itemClicked, this, &historywindow::on_historyList_itemClicked);
    connect(ui->replayButton, &QPushButton::clicked, this, &historywindow::on_replayButton_clicked);
    connect(replayTimer, &QTimer::timeout, this, &historywindow::replayNextMove);

    displayHistory();
}

historywindow::~historywindow()
{
    delete ui;
    delete replayTimer;
}

void historywindow::displayHistory()
{
    ui->historyList->clear();
    games = gameHistory.loadGames();
    if (games.isEmpty()) {
        ui->historyList->addItem("Please create an account to save your played games");
        ui->replayButton->setEnabled(false);
        ui->clearHistoryButton_2->setEnabled(false);
        return;
    }

    for (int i = 0; i < games.size(); ++i) {
        const GameSession &session = games[i];
        QString itemText = QString("Game %1: %2 vs %3 (%4) - %5 [%6]")
                               .arg(i + 1)
                               .arg(session.player1Name, session.player2Name, session.gameMode, session.outcome, session.timestamp.toString("yyyy-MM-dd hh:mm:ss"));
        ui->historyList->addItem(itemText);
    }
    ui->clearHistoryButton_2->setEnabled(true);
}

void historywindow::on_historyList_itemClicked()
{
    currentReplayIndex = ui->historyList->currentRow();
    resetReplayBoard();
    ui->replayButton->setEnabled(currentReplayIndex >= 0 && !games.isEmpty());
}

void historywindow::on_replayButton_clicked()
{
    if (currentReplayIndex < 0 || currentReplayIndex >= games.size()) return;

    currentMoveIndex = -1;
    resetReplayBoard();
    replayTimer->start(1000); // Replay moves every 1 second
}

void historywindow::replayNextMove()
{
    if (currentReplayIndex < 0 || currentReplayIndex >= games.size()) {
        replayTimer->stop();
        return;
    }

    const GameSession &session = games[currentReplayIndex];
    currentMoveIndex++;

    if (currentMoveIndex >= session.moves.size()) {
        replayTimer->stop();
        return;
    }

    int row = session.moves[currentMoveIndex].first;
    int col = session.moves[currentMoveIndex].second;
    QString symbol = session.winnerSymbol.isEmpty() ? (currentMoveIndex % 2 == 0 ? "X" : "O") :
                         (currentMoveIndex % 2 == 0 ? (session.player1Name == session.winnerSymbol ? "X" : "O") :
                              (session.player1Name == session.winnerSymbol ? "O" : "X"));

    // Update the replay board
    if (row == 0 && col == 0) ui->replay_0_0->setText(symbol);
    else if (row == 0 && col == 1) ui->replay_0_1->setText(symbol);
    else if (row == 0 && col == 2) ui->replay_0_2->setText(symbol);
    else if (row == 1 && col == 0) ui->replay_1_0->setText(symbol);
    else if (row == 1 && col == 1) ui->replay_1_1->setText(symbol);
    else if (row == 1 && col == 2) ui->replay_1_2->setText(symbol);
    else if (row == 2 && col == 0) ui->replay_2_0->setText(symbol);
    else if (row == 2 && col == 1) ui->replay_2_1->setText(symbol);
    else if (row == 2 && col == 2) ui->replay_2_2->setText(symbol);
}

void historywindow::resetReplayBoard()
{
    // Clear the replay board
    ui->replay_0_0->setText("");
    ui->replay_0_1->setText("");
    ui->replay_0_2->setText("");
    ui->replay_1_0->setText("");
    ui->replay_1_1->setText("");
    ui->replay_1_2->setText("");
    ui->replay_2_0->setText("");
    ui->replay_2_1->setText("");
    ui->replay_2_2->setText("");
}

void historywindow::on_clearHistoryButton_2_clicked()
{
    gameHistory.clearHistory();
    displayHistory();
    resetReplayBoard();
    ui->replayButton->setEnabled(false);
}
