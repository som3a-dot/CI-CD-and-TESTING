#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <QVector>

class AIPlayer {
public:
    AIPlayer(char aiSymbol, char playerSymbol, const QString &difficulty);
    void makeMove(QVector<QVector<char>> &board, int &row, int &col);
    bool isBoardFull(const QVector<QVector<char>> &board);
    bool checkWin(const QVector<QVector<char>> &board, char symbol);

private:
    char aiSymbol;
    char playerSymbol;
    QString difficulty;
    int evaluateBoard(const QVector<QVector<char>> &board);
    int minimax(QVector<QVector<char>> &board, int depth, bool isMaximizing, int alpha, int beta);
};

#endif // AIPLAYER_H
