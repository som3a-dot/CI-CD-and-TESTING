#include "gamehistory.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QDebug>

GameHistory::GameHistory(const QString &username) {
    if (username.isEmpty()) {
        historyFilePath = "";
    } else {
        historyFilePath = QDir::homePath() + "/tictactoe_" + username + "_history.json";
    }
}

void GameHistory::saveGame(const GameSession &session) {
    if (historyFilePath.isEmpty()) {
        qDebug() << "Guest mode: Game history not saved";
        return;
    }

    QFile file(historyFilePath);
    QJsonArray historyArray;

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isNull() && doc.isArray()) {
            historyArray = doc.array();
        }
        file.close();
    }

    QJsonObject sessionObj;
    sessionObj["player1Name"] = session.player1Name;
    sessionObj["player2Name"] = session.player2Name;
    sessionObj["gameMode"] = session.gameMode;
    sessionObj["outcome"] = session.outcome;
    sessionObj["timestamp"] = session.timestamp.toString(Qt::ISODate);
    sessionObj["winnerSymbol"] = session.winnerSymbol;

    QJsonArray movesArray;
    for (const auto &move : session.moves) {
        QJsonObject moveObj;
        moveObj["row"] = move.first;
        moveObj["col"] = move.second;
        movesArray.append(moveObj);
    }
    sessionObj["moves"] = movesArray;

    historyArray.append(sessionObj);

    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(historyArray);
        file.write(doc.toJson());
        file.close();
    } else {
        qDebug() << "Failed to open history file for writing:" << historyFilePath;
    }
}

QVector<GameSession> GameHistory::loadGames() {
    QVector<GameSession> games;
    if (historyFilePath.isEmpty()) {
        qDebug() << "Guest mode: No game history loaded";
        return games;
    }

    QFile file(historyFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isNull() && doc.isArray()) {
            QJsonArray historyArray = doc.array();
            for (const QJsonValue &value : historyArray) {
                if (value.isObject()) {
                    QJsonObject obj = value.toObject();
                    GameSession session;
                    session.player1Name = obj["player1Name"].toString();
                    session.player2Name = obj["player2Name"].toString();
                    session.gameMode = obj["gameMode"].toString();
                    session.outcome = obj["outcome"].toString();
                    session.timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
                    session.winnerSymbol = obj["winnerSymbol"].toString();

                    QJsonArray movesArray = obj["moves"].toArray();
                    for (const QJsonValue &moveValue : movesArray) {
                        QJsonObject moveObj = moveValue.toObject();
                        session.moves.append({moveObj["row"].toInt(), moveObj["col"].toInt()});
                    }
                    games.append(session);
                }
            }
        }
        file.close();
    }
    return games;
}

void GameHistory::clearHistory() {
    if (historyFilePath.isEmpty()) {
        qDebug() << "Guest mode: No history to clear";
        return;
    }

    QFile file(historyFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(QJsonArray()).toJson());
        file.close();
    } else {
        qDebug() << "Failed to clear history file:" << historyFilePath;
    }
}
