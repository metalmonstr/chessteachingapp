#include "chesssquare.h"

chessSquare::chessSquare()
{

}

chessSquare::chessSquare(int row, int col)
{
    location = QPoint(col,row);
}

/**
 * @brief chessSquare::setPiece
 * @param owner
 * @param type
 * sets the piece on this square to a piece owned by owner and of type type
 */
void chessSquare::setPiece(char owner, char type)
{
    piece = chessPiece(owner,type);
}

/**
 * @brief chessSquare::setPiece
 * @param piece
 * sets the piece on this square to piece
 */
void chessSquare::setPiece(chessPiece piece)
{
    this->piece = piece;
}

/**
 * @brief chessSquare::clearPiece
 * clears the piece on this square
 */
void chessSquare::clearPiece()
{
    piece = chessPiece();
}

/**
 * @brief chessSquare::isOccupied
 * @return
 * returns true if this space has an active piece on it, false otherwise
 */
bool chessSquare::isOccupied()
{
    return piece.isPiece();
}

/**
 * @brief chessSquare::pieceOwner
 * @return owner
 * returns the owner of the piece on this square, 'B' if black, 'W' if white, 'e' if there is no piece there
 */
char chessSquare::pieceOwner()
{
    if(piece.isPiece())
    {
        return piece.getOwner();
    }
    return 'e';
}

/**
 * @brief chessSquare::pieceType
 * @return type
 * returns the type of the piece on this square, 'p' if pawn, 'K' if king, 'Q' if queen, 'R' if rook, 'N' if knight,
 * 'B' if bishop.
 */
char chessSquare::pieceType() {
    if(piece.isPiece())
    {
        return piece.getType();
    }
    return 'e';
}

chessPiece chessSquare::getPiece() {
    return piece;
}

int chessSquare::getXCoordinate()
{
    return location.x();
}

int chessSquare::getYCoordinate()
{
    return location.y();
}

bool chessSquare::move(int newRow, int newCol, bool isOccupied)
{
    return piece.move(newRow,newCol, this->getXCoordinate(), this->getYCoordinate(), isOccupied);
}
