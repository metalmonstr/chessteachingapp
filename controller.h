#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QWidget>
#include <iostream>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <vector>
#include <QTimer>
#include <chessboard.h>

class controller: public QObject
{
    Q_OBJECT
public:

    struct stage{
        std::string titleText;
        std::string messageText;
        bool nextButtonSettingOn;
        bool chessboardSettingOn;
        bool levelImageControl[8];
        int methodHandling;
        chessboard board;
    };

    std::vector<stage> stages;
    explicit controller(QObject *parent = nullptr, chessboard *board = new chessboard());
    void updateView();
    void initializeView(chessboard);

private:
    chessboard *board;
    QString currentMessage;
    int currentStage = 0;
    bool nextButtonIsEnabled = true;
    bool levelIconsActivatationList[8];
    chessSquare currentPiece;
    chessSquare previousPiece;
    QVector<QPair<int, int>> possibleMoves(chessSquare piece, int row, int col);

    void loadStage(int stage);
    void updateKing();
    bool checkPathKing(char, int, int);
    bool checkPathQueen(char, int, int, int, int);
    bool checkPathBishop(char, int, int, int, int);
    bool checkPathRook(char, int, int, int, int);
    bool checkPathKnight(int, int, int, int);
    bool checkPathPawn(int, int, int, int);
    bool checkCastlingLeft();
    bool checkCastlingRight();
    void executeCastling(char);
    bool checkEnPessant(int, int);
    bool checkPromotion(int);
    void executeMove(int, int, int, int);
    void executeEnPessant(int,int, int, int);
    void executePromotion(int,int);
    void checkStageConditions();
    stage parseData(std::string data);
    std::vector<std::string>splitString(std::string, char);

signals:
    void clearBoard();
    void updateBoard(chessboard);
    void updateTitle(QString);
    void updateMessage(QString);
    void updateNextButton(bool);
    void updateBoardStatus(bool);
    void updateProgress(int);
    // void updateLevelIcons(bool[8]);
    void openPromotionDialog();
    void pieceSelected(QVector<QPair<int, int>> moves, chessboard);
    void legalMove();
    void victory();
    void updateSave(bool);

public slots:
    //void recieveMove();
    void mouseEvent(int row, int col);
    void move(int row, int col);
    void updatePromotionPiece(int pieceCode);
    void incrementStage();
    void resetStage();
    void undoHighlight();
    int getCurrentStage();
    void setCurrentStage(int stage);
};



#endif // CONTROLLER_H
