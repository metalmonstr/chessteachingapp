#include "chessboard.h"

/**
 * @brief chessboard::chessboard
 * creates an empty 8x8 chessboard
 */
chessboard::chessboard()
{
    this->board = new chessSquare*[8];
    for(int i = 0;i < 8; i++)
    {
        board[i] = new chessSquare[8];
    }
    //names each square
    for( int row = 0; row < 8; row++) {
        for( int col = 0; col < 8; col++) {
            board[row][col] = chessSquare(row,col);
        }
    }
}

/**
 * @brief chessboard::chessboard
 * Constructs a chessboard given an array of strings.
 */
chessboard::chessboard(std::vector<std::string>input){
    this->board = new chessSquare*[8];
    for(int i = 0;i < 8; i++)
    {
        board[i] = new chessSquare[8];
    }
    //names each square
    for( int row = 0; row < 8; row++) {
        for( int col = 0; col < 8; col++) {
            board[row][col] = chessSquare(row,col);
        }
    }

    int size = 8;
    // Read the input into the board.
    for(int index = 0; index < 64; index++){
        int row = index / size;
        int col = index % size;
        std::string current = input[index];
        char owner = current[0];
        char type = current[1];
        if(owner == 'W'){
            switch(type)
            {
                case 'K':
                    setPiece(row, col, chessPiece('W', 'K', row, col));
                    break;
                case 'Q':
                    setPiece(row, col, chessPiece('W', 'Q', row, col));
                    break;
                case 'R':
                    setPiece(row, col, chessPiece('W', 'R', row, col));
                    break;
                case 'B':
                    setPiece(row, col, chessPiece('W', 'B', row, col));
                    break;
                case 'N':
                    setPiece(row, col, chessPiece('W', 'N', row, col));
                    break;
                case 'P':
                    setPiece(row, col, chessPiece('W', 'p', row, col));
                    break;
                default:
                    break;

            }
        }
        else if(owner == 'B')
        {
            switch(type)
            {
            case 'K':
                setPiece(row, col, chessPiece('B', 'K', row, col));
                break;
            case 'Q':
                setPiece(row, col, chessPiece('B', 'Q', row, col));
                break;
            case 'R':
                setPiece(row, col, chessPiece('B', 'R', row, col));
                break;
            case 'B':
                setPiece(row, col, chessPiece('B', 'B', row, col));
                break;
            case 'N':
                setPiece(row, col, chessPiece('B', 'N', row, col));
                break;
            case 'P':
                setPiece(row, col, chessPiece('B', 'p', row, col));
                break;
            default:
                break;

            }
        }
        else continue;
    }
}

/**
 * Copy constructor for chessboard.
 * @brief chessboard::operator =
 * @param other
 * @return
 */
chessboard& chessboard::operator=(chessboard other){
    std::swap(board, other.board);
    return *this;
}

/**
 * Used to reset the board. Creates copy of other board.
 * @brief chessboard::chessboard
 * @param other
 */
chessboard::chessboard (const chessboard & other)
{
    this->board = new chessSquare*[8];
    for(int i = 0;i < 8; i++)
    {
        board[i] = new chessSquare[8];
    }
    //names each square
    for( int row = 0; row < 8; row++) {
        for( int col = 0; col < 8; col++) {
            board[row][col] = chessSquare(row,col);
        }
    }

    //names each square
    for( int row = 0; row < 8; row++) {
        for( int col = 0; col < 8; col++) {
            char owner = other.board[row][col].pieceOwner();
            char type = other.board[row][col].pieceType();
            board[row][col].setPiece(owner, type);
        }
    }
}


/**
 * @brief chessboard::setPiece
 * @param row
 * @param col
 * @param piece
 * places a piece at col,row
 */
void chessboard::setPiece(int row, int col, chessPiece piece)
{
    board[row][col].setPiece(piece);
    piece.setRow(row);
    piece.setCol(col);
}

/**
 * @brief chessboard::removePiece
 * @param row
 * @param col
 * removes the piece at col,row
 */
void chessboard::removePiece(int row, int col)
{
    board[row][col].clearPiece();
}

/**
 * @brief chessboard::clearBoard
 * clears all pieces from board.
 */
void chessboard::clearBoard() {
    for(int row = 0; row < 8; row++)
    {
        for(int col = 0; col < 8; col++)
        {
            board[row][col].clearPiece();
        }
    }
}

/**
 * @brief chessboard::getBoard
 * @return board
 * return chessSquare double array.
 */
chessSquare** chessboard::getBoard() {
    return board;
}

chessSquare chessboard::getSquare(int row, int col) {
    return board[row][col];
}

