#ifndef CHESSSQUARE_H
#define CHESSSQUARE_H

#include <QString>
#include "chesspiece.h"
#include <QPoint>


class chessSquare
{
public:
    chessSquare();
    chessSquare(int x, int y);
    void setPiece(char owner, char type);
    void setPiece(chessPiece piece);
    void clearPiece();
    bool isOccupied();
    char pieceOwner();
    char pieceType();
    chessPiece getPiece();
    int getXCoordinate();
    int getYCoordinate();
    bool move(int newRow, int newCol, bool isOccupied);



private:
    QPoint location;
    chessPiece piece;

};

#endif // CHESSSQUARE_H
