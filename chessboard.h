#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "chesssquare.h"
#include <vector>

class chessboard
{
public:
    chessboard();
    chessboard & operator=(chessboard);
    chessboard (const chessboard & other);
    chessboard(std::vector<std::string>);
    void setPiece(int row,int col, chessPiece piece);
    void removePiece(int row, int col);
    chessSquare** getBoard();
    chessSquare getSquare(int row, int col);
    void clearBoard();

private:
    chessSquare** board;
};

#endif // CHESSBOARD_H
