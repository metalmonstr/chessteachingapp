#include "chesspiece.h"

/**
 * @brief chessPiece::chessPiece
 * constructs an undefined chesspiece
 */
chessPiece::chessPiece()
{
    //e for empty
    owner = 'e';
    type = 'e';
    hasMoved = false;
}

/**
 * @brief chessPiece::chessPiece
 * @param owner
 * @param type
 * creates a chessPiece with the specified owner and type
 * B or W for owner, and p for pawn, K for king, Q for queen, N for knight, R for rook, and B for bishop
 */
chessPiece::chessPiece(char owner, char type)
{
    if(owner == 'B' || owner == 'W')
    {
        this->owner = owner;
    }
    else
    {
        //error, invalid owner, given e for empty, could also throw error instead
        this->owner = 'e';
    }
    if(type == 'p' || type == 'K' ||type == 'Q'||type == 'R' || type == 'N' || type == 'B' )
    {
        this->type = type;
    }
    else
    {
        //error, invalid type given e for empty, could also throw error instead
        this->type = 'e';
    }
    hasMoved = false;
}

/**
 * @brief chessPiece::chessPiece
 * @param owner the owner of the piece, either 'B' or 'W'
 * @param type the type of the piece {'p', 'K', 'Q', 'N', 'R', 'B'}
 * @param newRow the row of the chess piece
 * @param newCol the column of the chess piece
 */
chessPiece::chessPiece(char owner, char type, int newRow, int newCol)
{
    if(owner == 'B' || owner == 'W')
    {
        this->owner = owner;
    }
    else
    {
        //error, invalid owner, given e for empty, could also throw error instead
        this->owner = 'e';
    }
    if(type == 'p' || type == 'K' ||type == 'Q'||type == 'R' || type == 'N' || type == 'B' )
    {
        this->type = type;
    }
    else
    {
        //error, invalid type given e for empty, could also throw error instead
        this->type = 'e';
    }
    row = newRow;
    col = newCol;
    if (type == 'p' && ((owner == 'W' && row != 6) || (owner == 'B' && row != 1)))
        hasMoved = true;
    else
        hasMoved = false;
}

/**
 * @brief chessPiece::isPiece
 * @return true if the piece is defined, false otherwise
 */
bool chessPiece::isPiece()
{
    //if type and owner are valid
    if((type == 'p' || type == 'K' ||type == 'Q'||type == 'R' || type == 'N' || type == 'B') && (owner == 'W' || owner == 'B'))
    {
        return true;
    }
    return false;
}

bool chessPiece::hasPieceMoved()
{
    return hasMoved;
}

/**
 * @brief chessPiece::move
 * @param newRow the new row of the chess piece, if move is valid
 * @param newCol the new column of the chess piece, if move is valid
 * @param isOccupied boolean to check if new chessSquare is occupied; used for some capture scenarios
 * @return true if a move is valid, false otherwise
 *
 * Calculates if a move is valid, returning true if so and false otherwise.
 */
bool chessPiece::move(int newRow, int newCol, int oldCol, int oldRow, bool isOccupied) {
    if (type == 'p') { // pawn
        char owner = this->getOwner();
        if((owner == 'B' && oldRow > newRow) || (owner == 'W' && oldRow < newRow)) // prevent pawns going backwards.
            return false;
        else if (isOccupied && abs(newRow-oldRow) == 1 && abs(newCol-oldCol) == 1) // capture scenario
        {
            hasMoved = true;
            return true;
        }
        else if (!isOccupied && hasMoved && abs(newRow-oldRow) == 1 && abs(newCol-oldCol) == 0){ // if hasMoved, can only move forward 1
              return true;
        }
        else if (!isOccupied && !hasMoved && (abs(newRow-oldRow) <= 2) && abs(newCol-oldCol) == 0) // if !hasMoved, can move forward 1 or 2.
        {
            hasMoved = true;
            if(abs(newRow-oldRow) == 2) // if moved forward 2, en pessant is turned on.
                  enPessantOn = true;
            return true;
        }
        else if(!isOccupied && hasMoved && abs(newRow-oldRow) == 1 && abs(newCol-oldCol) == 1){ // checks if could be a enPessant move.
            if(owner == 'B' && newRow == 6 )
                return true;
            else if(owner == 'W' && newRow == 2)
                return true;
            else return false;
        }
        else // illegal move
            return false;
    }

    else if (type == 'K') // king
    {
        if (!hasMoved && newRow == 7 && (newCol == 2 || newCol == 6)) { // castling is a valid move
            hasMoved = true;
            return true;
        }
        if (abs(oldRow-newRow) == 1 || abs(oldCol-newCol) == 1) {
            hasMoved = true;
            return true;
        }
        return false;
    }

    else if (type == 'Q') { // queen. Check statement for rook or bishop must be true.
        return abs(newRow - oldRow) == abs(newCol - oldCol) // bishop test
                || (newRow == oldRow || newCol == oldCol); // rook test.
    }

    else if (type == 'R') { // rook
        if (newRow == oldRow || newCol == oldCol) {
            hasMoved = true;
            return true;
        }
        return false;
    }

    else if (type == 'N') { // knight
        int rowDifference = abs(newRow - oldRow);
        int colDifference = abs(newCol - oldCol);

        if(rowDifference == 2 && colDifference == 1)  // moving right or left
            return true;
        else if(rowDifference == 1 && colDifference == 2) // moving up or down.
            return true;
        else return false;
    }

    else if (type == 'B') { // bishop
        return abs(newRow - oldRow) == abs(newCol - oldCol);
    }

    else { // illegal move
        return false;
    }

}

/**
 * @brief chessPiece::getOwner
 * @return the owner of this piece
 */
char chessPiece::getOwner() {
    return owner;
}

/**
 * @brief chessPiece::getType
 * @return the type of this piece
 */
char chessPiece::getType() {
    return type;
}

/**
 * @brief chessPiece::setRow
 * @param newRow the new row for the chess piece
 * Sets the row of the chess piece
 */
void chessPiece::setRow(int newRow) {
    row = newRow;
}

/**
 * @brief chessPiece::setCol
 * @param newCol the new column for the chess piece
 * Sets the column of the chess piece
 */
void chessPiece::setCol(int newCol) {
    col = newCol;
}

/**
 * @brief chessPiece::getRow
 * @return the row of the chess piece
 * Returns the row of the chess piece
 */
int chessPiece::getRow() {
    return row;
}

/**
 * @brief chessPiece::getCol
 * @return the column of the chess piece
 * Returns the column of the chess piece
 */
int chessPiece::getCol() {
    return col;
}

/**
 * @brief chessPiece::getEnpessantOn
 * @return the value of if the peice can be taken by enPessant.
 */
bool chessPiece::getEnpessantOn(){
    return enPessantOn;
}

/**
 * @brief chessPiece::setEnpessantOn
 * @param val Sets the value of enPessantOn.
 */
void chessPiece::setEnpessantOn(bool val) {
    enPessantOn = val;
}



