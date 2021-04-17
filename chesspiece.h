#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <cmath>
#include <cstdlib>

class chessPiece
{
public:
    chessPiece();
    chessPiece(char owner, char type);
    chessPiece(char owner, char type, int newRow, int newCol);
    bool hasPieceMoved();
    bool isPiece();
    char getOwner();
    char getType();
    void setRow(int newRow);
    void setCol(int newCol);
    void setEnpessantOn(bool);
    bool getEnpessantOn();
    int getRow();
    int getCol();
    bool move(int newRow, int newCol, int oldRow, int oldCol, bool isOccupied);

private:
    int row;
    int col;
    char owner;
    char type;
    bool hasMoved;
    bool enPessantOn;
};

#endif // CHESSPIECE_H
