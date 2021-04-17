#include "controller.h"
#include <fstream>
#include <sstream>
//#include <bits/stdc++.h>

/**
 * @brief controller::controller
 * @param parent
 * @param board the chessboard being displayed in the UI
 * The controller aspect of the Chess Tutorial App
 */
controller::controller(QObject *parent, chessboard *board): QObject(parent)
{
    this->board = board;
    currentMessage = "Welcome to chess! Start a new tutorial or load a previous load.";
    nextButtonIsEnabled = true;
    for(auto icon : levelIconsActivatationList){
        icon = false;
    }

    QFile file(":/Files/Database.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in (&file);
    int counter = 0;
    while(!in.atEnd())
    {
       std::string line = in.readLine().toStdString();
       stages.push_back(parseData(std::string(line.substr(3))));
       counter++;
    }
    initializeView(stages.at(currentStage).board);
}

/**
 * @brief controller::mouseEvent
 * @param row: row of the square that was clicked
 * @param col: column of the square that was clicked
 * Sets currentPiece to be the piece in the square that was selected
 */
void controller::mouseEvent(int row, int col) {
    currentPiece = board->getSquare(row, col);
    QVector<QPair<int, int>> moves = possibleMoves(currentPiece, row, col);
    emit pieceSelected(moves, *board);
    emit updateBoard(*board);
}

void controller::undoHighlight() {
    emit updateBoard(*board);
}

/**
 * @brief controller::move
 * @param row: row of the desired square to move the piece to
 * @param col: column of the desired square to move the piece to
 * Moves currentPiece to row and col as long as it is a valid move
 */
void controller::move(int row, int col) {

    char type = currentPiece.pieceType();
    char owner = currentPiece.pieceOwner();
    int oldRow = currentPiece.getYCoordinate();
    int oldCol = currentPiece.getXCoordinate();

   // Check if it is a valid move for that type of piece on the board.
    if(!currentPiece.move(row, col, board->getSquare(row,col).isOccupied())){
        return;
    }
    else if(currentPiece.pieceOwner() == board->getSquare(row, col).pieceOwner()) // check if
        return;

    switch(type){
        case 'p':
           if(checkEnPessant(row, col)) {
               executeEnPessant(oldRow, oldCol, row, col);
           }
           else if(checkPathPawn(oldRow, oldCol, row, col)){
              executeMove(oldRow, oldCol, row, col);
              if(checkPromotion(row)){
                  executePromotion(row, col);
              }
           }
           emit legalMove();
           break;
        case 'K':
            if ((row == oldRow && col == oldCol - 2) && checkCastlingLeft())
                executeCastling('L');
            else if ((row == oldRow && col == oldCol + 2) && checkCastlingRight())
                executeCastling('R');
            else if (checkPathKing(owner, row, col)) {
                executeMove(oldRow, oldCol, row, col);
            }
            break;
        case 'Q':
               if(checkPathQueen(owner, oldRow, oldCol, row, col)){
                   executeMove(oldRow, oldCol, row, col);
               }
            break;
        case 'R':
            if(checkPathRook(owner, oldRow, oldCol, row, col)){
                executeMove(oldRow, oldCol, row, col);
            }
            break;
        case 'N':
            if(checkPathKnight(oldRow, oldCol, row, col)){
               executeMove(oldRow, oldCol, row, col);
            }
           break;
        case 'B':
            if(checkPathBishop(owner, oldRow, oldCol, row, col)){
               executeMove(oldRow, oldCol, row, col);
            }
            break;

        default:
            break;
    }
    checkStageConditions();
    updateView();
}

/**
 * @brief controller::updateView
 * Updates the chessboard to match the controller
 */
void controller::updateView(){
    // set the views board to what it should be
    emit clearBoard();
    emit updateBoard(*board);
}

/**
 * @brief controller::initializeView
 * @param b
 * Updates the entire UI to match the appropriate stage
 */
void controller::initializeView(chessboard b){
    // set the views board to what it should be
    chessboard temp(b);
    *board = temp;
    emit clearBoard();
    emit updateBoard(*board);
    emit updateTitle(QString::fromStdString((stages.at(currentStage).titleText)));
    emit updateMessage(QString::fromStdString((stages.at(currentStage).messageText)));
    emit updateNextButton(stages.at(currentStage).nextButtonSettingOn);
    emit updateBoardStatus(stages.at(currentStage).chessboardSettingOn);
    emit updateProgress(currentStage);
    // sets levelIconsActivationList  = levelIconsActivationList
}

/**
 * @brief controller::checkPathKing
 * @param owner owner of the selected piece
 * @param row row of the selected piece
 * @param col column of the selected piece
 * @return true if path is clear, false otherwise
 * Checks the path of the selected King piece to determine if there are any obstacles
 */
bool controller::checkPathKing(char owner, int row, int col) {
    chessSquare s = board->getSquare(row, col);
    if (s.isOccupied() && (owner == s.pieceOwner())) {
        return false;
    }
    return true;
}

/**
 * @brief controller::checkPathQueen
 * @param owner owner of the selected piece
 * @param oldRow original row of the selected piece
 * @param oldCol original column of the selected piece
 * @param row destination row of the selected piece
 * @param col destination column of the selected piece
 * @return true if the page is clear, false otherwise
 * Checks the path of the selected Queen piece to determine if there are any obstacles
 */
bool controller::checkPathQueen(char owner, int oldRow, int oldCol, int row, int col){
    return checkPathRook(owner, oldRow, oldCol, row, col) && checkPathBishop(owner, oldRow, oldCol, row, col);
}

/**
 * @brief controller::checkPathRook
 * @param owner owner of the selected piece
 * @param oldRow original row of the selected piece
 * @param oldCol original column of the selected piece
 * @param row destination row of the selected piece
 * @param col destination column of the selected piece
 * @return true if the page is clear, false otherwise
 * Checks the path of the selected Rook piece to determine if there are any obstacles
 */
bool controller::checkPathRook(char owner, int oldRow, int oldCol, int row, int col)
{
    // check for obstructions in path
    if (oldRow == row) {
        if (oldCol < col) {
            for (int c = oldCol + 1; c <= col; c++) {
                chessSquare s = board->getSquare(row, c);
                if (s.isOccupied() && (owner == s.pieceOwner() || (c != col && s.isOccupied())))
                    return false;
            }
        }
        else if (oldCol > col) {
            for (int c = oldCol - 1; c >= col; c--) {
                chessSquare s = board->getSquare(row, c);
                if (s.isOccupied() && (owner == s.pieceOwner() || (c != col && s.isOccupied())))
                    return false;
            }
        }
    }
    else if (oldCol == col) {
        if (oldRow < row) {
            for (int r = oldRow + 1; r <= row; r++) {
                chessSquare s =  board->getSquare(r, col);
                if (s.isOccupied() && (owner == s.pieceOwner() || (r != row && s.isOccupied())))
                    return false;
            }
        }
        else if (oldRow > row) {
            for (int r = oldRow-1; r >= row; r--) {
                chessSquare s =  board->getSquare(r, col);
                if (s.isOccupied() && (owner == s.pieceOwner() || (r != row && s.isOccupied())))
                    return false;
            }
        }
    }
    return true;
}

/**
 * @brief controller::checkPathBishop
 * @param owner owner of the selected piece
 * @param oldRow original row of the selected piece
 * @param oldCol original column of the selected piece
 * @param row destination row of the selected piece
 * @param col destination column of the selected piece
 * @return true if the page is clear, false otherwise
 * Checks the path of the selected Bishop to determine if there are any obstacles
 */
bool controller::checkPathBishop(char owner, int oldRow, int oldCol, int row, int col)
{
    int rowHolder = oldRow;
    int colHolder = oldCol;

    // check for obstructions in path
    if (oldRow < row && oldCol < col) { // diagonal down right
        while(rowHolder + 1 <= row) {
            chessSquare s = board->getSquare(rowHolder + 1, colHolder + 1);
            if (s.isOccupied() && owner == s.pieceOwner())
                return false;
            rowHolder++;
            colHolder++;
        }
    }
    else if (oldRow < row && oldCol > col) { // diagonal down left
        while(rowHolder + 1 <= row) {
            chessSquare s = board->getSquare(rowHolder + 1, colHolder - 1);
            if (s.isOccupied()  && owner == s.pieceOwner())
                return false;
            rowHolder++;
            colHolder--;
        }
    }
    else if (oldRow > row && oldCol < col) { // diagonal up right
        while(rowHolder - 1 >= row ) {
            chessSquare s = board->getSquare(rowHolder - 1, colHolder + 1);
            if (s.isOccupied()  && owner == s.pieceOwner())
                return false;
            rowHolder--;
            colHolder++;
        }
    }
    else if (oldRow > row && oldCol > col) { // diagonal up left
        while(rowHolder - 1 >= row) {
            chessSquare s = board->getSquare(rowHolder - 1, colHolder - 1);
            if (s.isOccupied()  && owner == s.pieceOwner())
                return false;
            rowHolder--;
            colHolder--;
        }
    }
    return true;
}

/**
 * @brief controller::checkPathKnight
 * @param oldRow original row of the selected piece
 * @param oldCol original column of the selected piece
 * @param row destination row of the selected piece
 * @param col destination column of the selected piece
 * @return true if the page is clear, false otherwise
 * Checks the path of selected Knight to determine if there are obstacles
 */
bool controller::checkPathKnight(int oldRow, int oldCol, int row, int col)
{
    chessSquare newLocation= board->getSquare(row, col);
    chessSquare oldLocation = board->getSquare(oldRow, oldCol);
    return newLocation.pieceOwner() != oldLocation.pieceOwner();
}

/**
 * @brief controller::checkPathPawn
 * @param oldRow original row of the selected piece
 * @param oldCol original column of the selected piece
 * @param row destination row of the selected piece
 * @param col destination column of the selected piece
 * @return true if the path is clear, false otherwise
 * Check the path of the selected Pawn to determine if there are obstacles
 */
bool controller::checkPathPawn(int oldRow, int oldCol, int row, int col)
{
    if(abs(oldRow - row) == 2 && oldCol == col){ // if moving forward 2 must check if there is a piece in the path.
        chessSquare checkSpace = board->getSquare(oldRow+(row-oldRow)/abs(row-oldRow), col); // calculates space in the gap.
        return !checkSpace.isOccupied();
    }
    return true;
}

/**
 * @brief controller::executeMove
 * @param oldRow the original row of the piece being moved
 * @param oldCol the original column of the piece being moved
 * @param row the destination row
 * @param col the destination column
 * Executes a move on the chessboard, including removing pieces if necessary and moving the piece that was selected
 */
void controller::executeMove(int oldRow, int oldCol, int row, int col)
{
    if (board->getSquare(row,col).isOccupied()) { // capture situation
        board->removePiece(row, col); // remove piece being captured
        board->removePiece(oldRow, oldCol); // remove piece being moved
        board->setPiece(row, col, currentPiece.getPiece()); // move piece to new location

    }
    else { // all other movement situations
        board->removePiece(oldRow, oldCol); // remove piece being moved
        board->setPiece(row, col, currentPiece.getPiece()); // move piece to new location
    }
    previousPiece.getPiece().setEnpessantOn(false); // remove en pessant as did not take turn.
    this->previousPiece = board->getSquare(row,col); // set the previous peice.
}

/**
 * @brief controller::checkPromotion
 * @param row the destination row of the pawn being moved
 * @return true if the pawn has reached the edge of the board, false otherwise
 * Checks the Pawn being moved to determine if it needs to be offered a promotion
 */
bool controller::checkPromotion(int row){
    char owner = currentPiece.getPiece().getOwner();
    if(owner == 'B')
        return row == 7; // is true if black piece has reached edge of the board.
    else return row == 0;
}

bool controller::checkCastlingLeft() {
    chessPiece rook = board->getSquare(7, 0).getPiece();
    // check that rook hasn't moved
    if (rook.getOwner() != 'W' && rook.getType() != 'R' && !rook.hasPieceMoved())
        return false;
    // check path between
    return checkPathRook('W', 7, 0, 7, 3);
}

bool controller::checkCastlingRight() {
    chessPiece rook = board->getSquare(7, 7).getPiece();
    // check that rook hasn't moved
    if (rook.getOwner() != 'W' && rook.getType() != 'R' && !rook.hasPieceMoved())
        return false;
    // check path between
    return checkPathRook('W', 7, 7, 7, 5);
}

void controller::executeCastling(char c) {
    chessPiece rook;
    board->removePiece(7, 4); // remove King
    if (c == 'L') {
        rook = board->getSquare(7, 0).getPiece();
        board->setPiece(7, 2, currentPiece.getPiece()); // move King 2 squares left
        board->removePiece(7, 0); // remove left rook that is castling
        board->setPiece(7, 3, rook);
    }
    else {
        rook = board->getSquare(7, 7).getPiece();
        board->setPiece(7, 6, currentPiece.getPiece()); // move King 2 squares right
        board->removePiece(7, 7); // remove right rook that is castling
        board->setPiece(7, 5, rook);
    }
}

/**
 * @brief controller::executePromotion
 * @param row the destination row of the promoted piece
 * @param col the destination column of the promoted piece
 * Executes the promotion by signalling MainWindow to open a dialog asking which piece the player desires
 */
void controller::executePromotion(int row, int col){
    currentPiece = board->getSquare(row, col); // set current square as current.
    emit openPromotionDialog();
}

/**
 * @brief controller::updatePromotionPiece
 * @param pieceCode
 * Updates the backing data structure with the promoted piece
 */
void controller::updatePromotionPiece(int pieceCode){
    int row = currentPiece.getYCoordinate();
    int col = currentPiece.getXCoordinate();
    switch(pieceCode){

        case 'Q':
            board->setPiece(row, col, chessPiece(currentPiece.pieceOwner(),'Q'));
            break;
        case 'R':
            board->setPiece(row, col, chessPiece(currentPiece.pieceOwner(),'R'));
            break;
        case 'B':
            board->setPiece(row, col, chessPiece(currentPiece.pieceOwner(),'B'));
            break;
        case 'N':
            board->setPiece(row, col, chessPiece(currentPiece.pieceOwner(),'N'));
            break;
        default:
            break;
    }
    updateView();
}

/**
 * @brief controller::checkEnPessant
 * @param row
 * @param col
 * @return
 * Checks if an En Passant move can occur
 */
bool controller::checkEnPessant(int row, int col)
{
    char owner = currentPiece.getPiece().getOwner();
    if(owner == 'B' && row == 6){
        chessSquare newLocation= board->getSquare(row-1, col);
        return newLocation.getPiece().getEnpessantOn();
    }
    else if(owner == 'W' && row == 2){
        chessSquare newLocation= board->getSquare(row+1, col);
        return newLocation.getPiece().getEnpessantOn();
    }
    else return false;
}

/**
 * @brief controller::executeEnPessant
 * @param oldRow
 * @param oldCol
 * @param row
 * @param col
 * Executes an En Passant move
 */
void controller::executeEnPessant(int oldRow, int oldCol, int row, int col)
{
     char owner = currentPiece.getPiece().getOwner();
     if(owner == 'B' && row == 5){
         board->removePiece(row-1, col); // remove piece being captured
     }
     else {
         board->removePiece(row+1, col); // remove piece being captured
     }
    board->setPiece(row, col, currentPiece.getPiece());
    board->removePiece(oldRow, oldCol);  // remove old peice.
    this->previousPiece = board->getSquare(row,col); // set the previous peice.
}

/**
 * @brief controller::parseData
 * @param data the line from StageDatabase.txt
 * @return a Data struct with information from data
 * Parses the StageDatabase to store information in stage structs
 */
controller::stage controller::parseData(std::string data){
    controller::stage temp;
    std::vector<std::string> splitString = this->splitString(data, '\\');
    temp.titleText = splitString[0];
    std::string stringHolder = splitString[1];
    std::vector<std::string> newString = this->splitString(stringHolder, '|');
    for (unsigned int i = 0; i < newString.size(); i++)
        temp.messageText = temp.messageText + newString[i] + "\n";

    temp.nextButtonSettingOn = stoi(splitString[2]);
    temp.chessboardSettingOn = stoi(splitString[3]);

    // get bool array for picture settings.
    std::vector<std::string> iconSettings = this->splitString(splitString[4], ',');
    for(unsigned int i = 0; i < iconSettings.size(); i++){
        temp.levelImageControl[i] = stoi(iconSettings[i]);
    }
    temp.methodHandling = stoi(splitString[5]);

    // create the chessboard.
    std::vector<std::string> boardStrings = this->splitString(splitString[6], ',');
    chessboard board(boardStrings);
    temp.board = board;
    return temp;
}

/**
 * @brief controller::splitString
 * @param line the string being split
 * @param delimiter the character to split line around
 * @return a vector splitString with the individual elements after being split
 * Splits the string line around the ' ' delimiter
 */
std::vector<std::string> controller::splitString(std::string line, char delimiter = ' '){
    std::vector<std::string> splitString;
    std::stringstream stream(line);

    std::string holder;

    while(std::getline(stream, holder, delimiter))
    {
        splitString.push_back(holder);
    }
    return splitString;
}

/**
 * @brief controller::incrementStage
 * Increments currentStage and updates the UI
 */
void controller::incrementStage(){
    currentStage++;
    // if the user is past the initial tutorial screen, the option to save is allowed
    if (currentStage > 0)
        emit updateSave(true);

    // if the user enters a new stage, a congratulations window pops up
    if (currentStage > 1 && stages.at(currentStage - 1).titleText != stages.at(currentStage).titleText)
        emit victory();

    // initializes the view with values for the current stage
    initializeView(stages.at(currentStage).board);

    if (currentStage == 3) {
        QVector<QPair<int, int>> selected;
        selected.push_back(qMakePair(7, 4));
        emit pieceSelected(selected, *board);
        emit updateBoard(*board);
    }

    if (currentStage == 5) {
        QVector<QPair<int, int>> selected;
        selected.push_back(qMakePair(4, 3));
        emit pieceSelected(selected, *board);
        emit updateBoard(*board);
    }

    if (currentStage == 9) {
        QVector<QPair<int, int>> selected;
        selected.push_back(qMakePair(7, 2));
        emit pieceSelected(selected, *board);
        emit updateBoard(*board);
    }

    if (currentStage == 13) {
        QVector<QPair<int, int>> selected;
        selected.push_back(qMakePair(7, 3));
        emit pieceSelected(selected, *board);
        emit updateBoard(*board);
    }

    if (currentStage == 16) {
        QVector<QPair<int, int>> selected;
        selected.push_back(qMakePair(7, 1));
        selected.push_back(qMakePair(7, 6));
        emit pieceSelected(selected, *board);
        emit updateBoard(*board);
    }
}

/**
 * @brief controller::checkStageConditions
 * Checks the stage conditions of the board to determine if a user can move on to the next stage
 */
void controller::checkStageConditions(){
    bool nextEnabled = true;
    switch(stages.at(currentStage).methodHandling)
    {
        case 0:     // standard case, continue.
            break;
        case 1: // capture enemy king to clear the level
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                if (board->getSquare(i, j).pieceType() == 'K')
                    nextEnabled = false;
        emit updateNextButton(nextEnabled);
            break;
        case 2: // capture a rook to clear the level
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                    if (board->getSquare(i, j).pieceType() == 'R')
                        nextEnabled = false;
            emit updateNextButton(nextEnabled);
            break;
        case 3: // capture a pawn to clear the level
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                    if (board->getSquare(i, j).pieceType() == 'p')
                        nextEnabled = false;
            emit updateNextButton(nextEnabled);
            break;
    }
}

/**
 * @brief controller::resetStage
 * Resets the stage to what it was at the beginning of the level
 */
void controller::resetStage(){
    initializeView(stages.at(currentStage).board);
}

/**
 * @brief controller::possibleMoves
 * @param square the current square
 * @param row the row of the current square
 * @param col the column of the current square
 * @return QVector<QPair<int, int>> with the possible squares that are legal destinations
 * Returns a vector of possible moves that a piece can make
 */
QVector<QPair<int, int>> controller::possibleMoves(chessSquare square, int row, int col)
{
    chessPiece piece = square.getPiece();
    chessSquare s;
    char type = piece.getType();
    char owner = piece.getOwner();
    QVector<QPair<int, int>> moves;
    int oldRow = square.getYCoordinate();
    int oldCol = square.getXCoordinate();
    int rowHolder = oldRow;
    int colHolder = oldCol;
    bool opponent = false;

    switch(type) {
        case 'K':
            if (!piece.hasPieceMoved() && currentStage > 32) // return possible moves for castling once that stage has started
            {
                if (checkCastlingLeft()) // queen side castling
                    moves.push_back(qMakePair(row, 2));
                if (checkCastlingRight())
                    moves.push_back(qMakePair(row, 6));
            }
            if (row > 0 && col > 0) { // going forward and left
                if (checkPathKing(owner, row - 1, col))
                    moves.push_back(qMakePair(row - 1, col));
                if (checkPathKing(owner, row - 1, col - 1))
                    moves.push_back(qMakePair(row - 1, col - 1));
                if (checkPathKing(owner, row, col - 1))
                    moves.push_back(qMakePair(row, col - 1));
            }
            if (row > 0 && col < 7) { // going forward and right
                if (checkPathKing(owner, row - 1, col))
                    moves.push_back(qMakePair(row - 1, col));
                if (checkPathKing(owner, row - 1, col + 1))
                    moves.push_back(qMakePair(row - 1, col + 1));
                if (checkPathKing(owner, row, col + 1))
                    moves.push_back(qMakePair(row, col + 1));
            }
            if (row < 7 && col > 0) { // going backward and left
                if (checkPathKing(owner, row + 1, col))
                    moves.push_back(qMakePair(row + 1, col));
                if (checkPathKing(owner, row + 1, col - 1))
                    moves.push_back(qMakePair(row + 1, col - 1));
                if (checkPathKing(owner, row, col - 1))
                    moves.push_back(qMakePair(row, col - 1));
            }
            if (row < 7 && col < 7) { // going backward and right
                if (checkPathKing(owner, row + 1, col))
                    moves.push_back(qMakePair(row + 1, col));
                if (checkPathKing(owner, row + 1, col + 1))
                    moves.push_back(qMakePair(row + 1, col + 1));
                if (checkPathKing(owner, row, col + 1))
                    moves.push_back(qMakePair(row, col + 1));
            }
            return moves;

        case 'Q':
            for (int a = row - 1; a > -1; a--) {
                if (checkPathRook(owner, oldRow, oldCol, a, col) && opponent == false)
                {
                    if (board->getSquare(a, col).pieceOwner() != 'e' && board->getSquare(a, col).pieceOwner() != owner) {
                        opponent = true;
                    }
                    moves.push_back(qMakePair(a, col));
                }
            }
            opponent = false;
            for (int b = row + 1; b < 8; b++) {
                if (checkPathRook(owner, oldRow, oldCol, b, col) && opponent == false)
                {
                    if (board->getSquare(b, col).pieceOwner() != 'e' && board->getSquare(b, col).pieceOwner() != owner) {
                        opponent = true;
                    }
                    moves.push_back(qMakePair(b, col));
                }
            }
            opponent = false;
            for (int c = col - 1; c > -1; c--) {
                if (checkPathRook(owner, oldRow, oldCol, row, c) && opponent == false)
                {
                    if (board->getSquare(row, c).pieceOwner() != 'e' && board->getSquare(row, c).pieceOwner() != owner) {
                        opponent = true;
                    }
                    moves.push_back(qMakePair(row, c));
                }
            }
            opponent = false;
            for (int d = col + 1; d < 8; d++) {
                if (checkPathRook(owner, oldRow, oldCol, row, d) && opponent == false)
                {
                    if (board->getSquare(row, d).pieceOwner() != 'e' && board->getSquare(row, d).pieceOwner() != owner) {
                        opponent = true;
                    }
                    moves.push_back(qMakePair(row, d));
                }
            }

            opponent = false;
            rowHolder = oldRow - 1;
            colHolder = oldCol - 1;
            while (rowHolder > -1 && colHolder > -1)
            {
                if (checkPathBishop(owner, oldRow, oldCol, rowHolder, colHolder) && opponent == false)
                {
                    if (board->getSquare(rowHolder, colHolder).pieceOwner() != 'e' && board->getSquare(rowHolder, colHolder).pieceOwner() != owner)
                        opponent = true;
                    moves.push_back(qMakePair(rowHolder, colHolder));
                }
                rowHolder--;
                colHolder--;
            }
            opponent = false;
            rowHolder = oldRow - 1;
            colHolder = oldCol + 1;
            while (rowHolder > -1 && colHolder < 8)
            {
                if (checkPathBishop(owner, oldRow, oldCol, rowHolder, colHolder) && opponent == false) {
                    if (board->getSquare(rowHolder, colHolder).pieceOwner() != 'e' && board->getSquare(rowHolder, colHolder).pieceOwner() != owner)
                        opponent = true;
                    moves.push_back(qMakePair(rowHolder, colHolder));
                }
                rowHolder--;
                colHolder++;
            }
            opponent = false;
            rowHolder = oldRow + 1;
            colHolder = oldCol + 1;
            while (rowHolder < 8 && colHolder < 8)
            {
                if (checkPathBishop(owner, oldRow, oldCol, rowHolder, colHolder) && opponent == false) {
                    if (board->getSquare(rowHolder, colHolder).pieceOwner() != 'e' && board->getSquare(rowHolder, colHolder).pieceOwner() != owner)
                        opponent = true;
                        moves.push_back(qMakePair(rowHolder, colHolder));
                }
                rowHolder++;
                colHolder++;
            }
            opponent = false;
            rowHolder = oldRow + 1;
            colHolder = oldCol - 1;
            while (rowHolder < 8 && colHolder > -1)
            {
                if (checkPathBishop(owner, oldRow, oldCol, rowHolder, colHolder) && opponent == false) {
                    if (board->getSquare(rowHolder, colHolder).pieceOwner() != 'e' && board->getSquare(rowHolder, colHolder).pieceOwner() != owner)
                        opponent = true;
                    moves.push_back(qMakePair(rowHolder, colHolder));
                }
                rowHolder++;
                colHolder--;
            }

            return moves;

        case 'N':
            if ((rowHolder-2 < 0) || (colHolder+1 > 7)) {

            }
            else {
                s = board->getSquare(rowHolder-2, colHolder+1);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder-2, colHolder+1));
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder-2, colHolder+1));
                }
            }

            if ((rowHolder-2 < 0) || (colHolder-1 < 0)) {

            }
            else {
                s = board->getSquare(rowHolder-2, colHolder-1);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder-2, colHolder-1));
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder-2, colHolder-1));
                }
            }

            if ((rowHolder-1 < 0) || (colHolder+2 > 7)) {

            }
            else {
                s = board->getSquare(rowHolder-1, colHolder+2);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder-1, colHolder+2));
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder-1, colHolder+2));
                }
            }

            if ((rowHolder-1 < 0) || (colHolder-2 < 0)) {

            }
            else {
                s = board->getSquare(rowHolder-1, colHolder-2);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder-1, colHolder-2));
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder-1, colHolder-2));
                }
            }

            if ((rowHolder+1 > 7) || (colHolder+2 > 7)) {

            }
            else {
                s = board->getSquare(rowHolder+1, colHolder+2);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder+1, colHolder+2));
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder+1, colHolder+2));
                }
            }

            if ((rowHolder+1 > 7) || (colHolder-2 < 0)) {

            }
            else {
                s = board->getSquare(rowHolder+1, colHolder-2);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder+1, colHolder-2));
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder+1, colHolder-2));
                }
            }

            if ((rowHolder+2 > 7) || (colHolder+1 > 7)) {

            }
            else {
                s = board->getSquare(rowHolder+2, colHolder+1);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder+2, colHolder+1));
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder+2, colHolder+1));
                }
            }

            if ((rowHolder+2 > 7) || (colHolder-1 < 0)) {

            }
            else {
                s = board->getSquare(rowHolder+2, colHolder-1);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder+2, colHolder-1));
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder+2, colHolder-1));
                }
            }

            return moves;

        case 'R':
            for (int a = row - 1; a > -1; a--)
                if (checkPathRook(owner, oldRow, oldCol, a, col))
                    moves.push_back(qMakePair(a, col));
            for (int b = row + 1; b < 8; b++)
                if (checkPathRook(owner, oldRow, oldCol, b, col))
                    moves.push_back(qMakePair(b, col));
            for (int c = col - 1; c > -1; c--)
                if (checkPathRook(owner, oldRow, oldCol, row, c))
                    moves.push_back(qMakePair(row, c));
            for (int d = col + 1; d < 8; d++)
                if (checkPathRook(owner, oldRow, oldCol, row, d))
                    moves.push_back(qMakePair(row, d));
            return moves;

        case 'B':
            // check for obstructions in path
            while(rowHolder + 1 < 8) {
                chessSquare s = board->getSquare(rowHolder+1, colHolder+1);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                        break;
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder+1, colHolder+1));
                        break;
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder+1, colHolder+1));
                }
                rowHolder = rowHolder + 1;
                colHolder = colHolder + 1;
            }

            rowHolder = oldRow;
            colHolder = oldCol;

            while(rowHolder + 1 < 8) {
                chessSquare s= board->getSquare(rowHolder+1, colHolder-1);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                        break;
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder+1, colHolder-1));
                        break;
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder+1, colHolder-1));
                }
                rowHolder = rowHolder + 1;
                colHolder = colHolder - 1;
            }

            rowHolder = oldRow;
            colHolder = oldCol;

            while(rowHolder - 1 >= 0) {
                chessSquare s= board->getSquare(rowHolder-1, colHolder+1);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                        break;
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder-1, colHolder+1));
                        break;
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder-1, colHolder+1));
                }
                rowHolder = rowHolder - 1;
                colHolder = colHolder + 1;
            }

            rowHolder = oldRow;
            colHolder = oldCol;

            while(rowHolder - 1 >= 0) {
                chessSquare s= board->getSquare(rowHolder-1, colHolder-1);
                if (s.isOccupied()) {
                    if (s.pieceOwner() == square.pieceOwner()) {
                        break;
                    }
                    else {
                        moves.push_back(qMakePair(rowHolder-1, colHolder-1));
                        break;
                    }
                }
                else {
                    moves.push_back(qMakePair(rowHolder-1, colHolder-1));
                }
                rowHolder = rowHolder - 1;
                colHolder = colHolder - 1;
            }
            return moves;



        case 'p':
            if (owner == 'W')
            {
                if (!piece.hasPieceMoved()) // if the pawn hasn't moved, then it can move 2 spaces forward
                {
                    if ((!board->getSquare(row - 1, col).isOccupied()) && (!board->getSquare(row - 2, col).isOccupied()))
                        moves.push_back(qMakePair(row - 2, col));
                }
                if (!board->getSquare(row - 1, col).isOccupied())
                    moves.push_back(qMakePair(row - 1, col));
                if (board->getSquare(row - 1, col + 1).isOccupied() && board->getSquare(row - 1, col + 1).pieceOwner() != owner)
                    moves.push_back(qMakePair(row - 1, col + 1));
                if (board->getSquare(row - 1, col - 1).isOccupied() && board->getSquare(row - 1, col - 1).pieceOwner() != owner)
                    moves.push_back(qMakePair(row - 1, col - 1));
            }
            if (owner == 'B')
            {
                if (!piece.hasPieceMoved())
                {
                    if ((!board->getSquare(row + 1, col).isOccupied()) && (!board->getSquare(row + 2, col).isOccupied()))
                        moves.push_back(qMakePair(row + 2, col));
                }
                if (!board->getSquare(row + 1, col).isOccupied())
                    moves.push_back(qMakePair(row + 1, col));
                if (board->getSquare(row + 1, col + 1).isOccupied() && board->getSquare(row + 1, col + 1).pieceOwner() != owner)
                    moves.push_back(qMakePair(row + 1, col + 1));
                if (board->getSquare(row + 1, col - 1).isOccupied() && board->getSquare(row + 1, col - 1).pieceOwner() != owner)
                    moves.push_back(qMakePair(row + 1, col - 1));
            }
            return moves;


        default:
            return moves;
    }
}

int controller::getCurrentStage() {
    return currentStage;
}

void controller::setCurrentStage(int stage) {
    currentStage = stage;
}
