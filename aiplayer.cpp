#include "aiplayer.h"
#include <QDebug>
#include <QtGlobal> // Ensure this is present for qrand()
#include <QVector>

AIPlayer::AIPlayer(char aiSymbol, char playerSymbol, const QString &difficulty)
    : aiSymbol(aiSymbol), playerSymbol(playerSymbol), difficulty(difficulty)
{
}

void AIPlayer::makeMove(QVector<QVector<char>> &board, int &row, int &col)
{
    int bestRow = -1, bestCol = -1;
    int bestScore = INT_MIN;
    int depth = (difficulty == "Hard") ? 9 : (difficulty == "Medium" ? 2 : 0);

    if (difficulty == "Easy") {
        QVector<QPair<int, int>> emptyCells;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (board[i][j] == ' ') emptyCells.append({i, j});
        if (!emptyCells.isEmpty()) {
            int index = rand() % emptyCells.size();
            bestRow = emptyCells[index].first;
            bestCol = emptyCells[index].second;
        }
    } else {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (board[i][j] == ' ') {
                    board[i][j] = aiSymbol;
                    int score = minimax(board, depth - 1, false, INT_MIN, INT_MAX);
                    board[i][j] = ' ';
                    if (score > bestScore) {
                        bestScore = score;
                        bestRow = i;
                        bestCol = j;
                    }
                }
    }

    if (bestRow != -1 && bestCol != -1) {
        row = bestRow;
        col = bestCol;
        board[row][col] = aiSymbol;
    }
}

int AIPlayer::evaluateBoard(const QVector<QVector<char>> &board)
{
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == aiSymbol && board[i][1] == aiSymbol && board[i][2] == aiSymbol) return 10;
        if (board[0][i] == aiSymbol && board[1][i] == aiSymbol && board[2][i] == aiSymbol) return 10;
    }
    if (board[0][0] == aiSymbol && board[1][1] == aiSymbol && board[2][2] == aiSymbol) return 10;
    if (board[0][2] == aiSymbol && board[1][1] == aiSymbol && board[2][0] == aiSymbol) return 10;

    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == playerSymbol && board[i][1] == playerSymbol && board[i][2] == playerSymbol) return -10;
        if (board[0][i] == playerSymbol && board[1][i] == playerSymbol && board[2][i] == playerSymbol) return -10;
    }
    if (board[0][0] == playerSymbol && board[1][1] == playerSymbol && board[2][2] == playerSymbol) return -10;
    if (board[0][2] == playerSymbol && board[1][1] == playerSymbol && board[2][0] == playerSymbol) return -10;

    return 0;
}

int AIPlayer::minimax(QVector<QVector<char>> &board, int depth, bool isMaximizing, int alpha, int beta)
{
    int score = evaluateBoard(board);

    if (score == 10 || score == -10 || isBoardFull(board) || depth == 0) return score;

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (board[i][j] == ' ') {
                    board[i][j] = aiSymbol;
                    int eval = minimax(board, depth - 1, false, alpha, beta);
                    board[i][j] = ' ';
                    maxEval = qMax(maxEval, eval);
                    alpha = qMax(alpha, eval);
                    if (beta <= alpha) break;
                }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (board[i][j] == ' ') {
                    board[i][j] = playerSymbol;
                    int eval = minimax(board, depth - 1, true, alpha, beta);
                    board[i][j] = ' ';
                    minEval = qMin(minEval, eval);
                    beta = qMin(beta, eval);
                    if (beta <= alpha) break;
                }
        return minEval;
    }
}

bool AIPlayer::isBoardFull(const QVector<QVector<char>> &board)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == ' ') return false;
    return true;
}

bool AIPlayer::checkWin(const QVector<QVector<char>> &board, char symbol)
{
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) return true;
        if (board[0][i] == symbol && board[1][i] == symbol && board[2][i] == symbol) return true;
    }
    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) return true;
    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) return true;
    return false;
}
