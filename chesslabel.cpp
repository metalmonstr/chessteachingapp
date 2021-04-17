#include "chesslabel.h"

#include <iostream>

/**
 * @brief ChessLabel::ChessLabel
 * @param parent
 * ChessLabel has all functionality regarding the chess board, including
 * a mouse listener and painting events
 */
ChessLabel::ChessLabel(QFrame *parent) : QLabel(parent)
{
    image = QImage(boardSize * squareSize,boardSize * squareSize,QImage::Format_RGB32);
    clearBoard();
    brush = Qt::black;
}


/**
 * @brief ChessLabel::mousePressEvent
 * @param event
 * when the chessLabel is clicked on
 */
void ChessLabel::mousePressEvent(QMouseEvent *event)
{
    // x, y is the coordinates of the corner of the square that was clicked on
    int x = nearestCorner(event->x());
    int y = nearestCorner(event->y());

    if(event->button() == Qt::LeftButton)
    {
        if (!moving) {
            emit mouseClick(y / 50, x / 50);
            xOrig = x;
            yOrig = y;
            moving = true;
            legal = false;
        }
        else {
            if (x / 50 == xOrig / 50 && y / 50 == yOrig / 50) // if they reclick the same piece, the possible moves should go away
            {
                emit undoHighlight();
                //moving = false;
            }
            else
                emit pieceMoved(y / 50, x / 50);
            moving = false;
            //legal ? moving = false : moving = true;

//            emit pieceMoved(y / 50, x / 50);
//            if (legal) // if the user made a legal move, the piece should move to the desired location
//            {
//                moving = false;
//            }
        }
    }
}

void ChessLabel::legalMove()
{
    legal = true;
}

/**
 * @brief pixel::nearestCorner
 * @param point
 * @return
 * finds the coordinate of nearest top left corner of a square
 */
int ChessLabel::nearestCorner(int point)
{
    int corner = point % squareSize;
    return point - corner;
}

/**
 * @brief ChessLabel::paintEvent
 * @param event
 * this paints the image onto the chesslabel in the ui: NEVER TOUCH THIS
 */
void ChessLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
      QRect dirtyRect = event->rect();
      painter.drawImage(dirtyRect, image, dirtyRect);
}


/**
 * @brief ChessLabel::clearBoard
 * changes the board in the ui to be an empty chessboard
 */
void ChessLabel::clearBoard()
{
    for(int row = 0; row < boardSize;row++)
    {
        for(int col = 0;col < boardSize;col++)
        {
            clearSquare(row,col);
        }
    }
}

/**
 * @brief ChessLabel::clearSquare
 * @param row
 * @param col
 * clears an individual square at col,row to its original empty state.
 */
void ChessLabel::clearSquare(int row, int col)
{
//    if (test == true)
//        brush = Qt::magenta;
    //what color square is it
    if((row + col) % 2 == 0)
    {
        brush = Qt::white;
    }
    else
    {
//       brush = Qt::black;
        // a brownish color
        brush = QColor(110, 55, 1);
    }
    //paint the square
    int x = col * squareSize;
    int y = row * squareSize;
    QPoint endPoint = QPoint (x + squareSize, y + squareSize);
    QPainter painter(&image);
    painter.setPen(brush);
    painter.fillRect(x,y,squareSize, squareSize, brush);
    int rad = (8 / 2) + 2;
        update(QRect(endPoint, endPoint).normalized()
                                         .adjusted(-rad, -rad, +rad, +rad));
}

void ChessLabel::highlightMoves(QVector<QPair<int, int>> moves)
{
    for (int i = 0; i < moves.size(); i++)
    {
        int row = moves[i].first;
        int col = moves[i].second;
        if((row + col) % 2 == 0)
        {
            brush = QColor(171, 207, 255);
        }
        else
        {
            brush = QColor(65, 105, 225);
        }
        //paint the square
        int x = col * squareSize;
        int y = row * squareSize;
        QPoint endPoint = QPoint (x + squareSize, y + squareSize);
        QPainter painter(&image);
        painter.setPen(brush);
        painter.fillRect(x, y, squareSize, squareSize, brush);
        int rad = (8 / 2) + 2;
            update(QRect(endPoint, endPoint).normalized()
                                             .adjusted(-rad, -rad, +rad, +rad  ));
    }

}

/**
 * @brief ChessLabel::placePieces
 * @param board
 * clear board and then place all the pieces.
 */
void ChessLabel::placePieces(chessboard board) {
    //clearBoard();

    for(int row = 0; row < boardSize;row++)
    {
        for(int col = 0;col < boardSize;col++)
        {
            if (board.getBoard()[row][col].isOccupied()) {
                char owner = board.getBoard()[row][col].pieceOwner();
                char type = board.getBoard()[row][col].pieceType();

                if (owner == 'B') {
                    if (type == 'K') {
                        drawPiece(col, row, "../A8EducationApplication/blackKing.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/blackKing.png");
                        //drawPiece(col, row, "blackKing.png");
                    }
                    else if (type == 'Q') {
                        drawPiece(col, row, "../A8EducationApplication/blackQueen.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/blackQueen.png");
                        //drawPiece(col, row, "blackQueen.png");
                    }
                    else if (type == 'R') {
                        drawPiece(col, row, "../A8EducationApplication/blackRook.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/blackRook.png");
                        //drawPiece(col, row, "blackRook.png");
                    }
                    else if (type == 'N') {
                        drawPiece(col, row, "../A8EducationApplication/blackKnight.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/blackKnight.png");
                        //drawPiece(col, row, "blackKnight.png");
                    }
                    else if (type == 'B') {
                        drawPiece(col, row, "../A8EducationApplication/blackBishop.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/blackBishop.png");
                        //drawPiece(col, row, "blackBishop.png");
                    }
                    else if (type == 'p') {
                        drawPiece(col, row, "../A8EducationApplication/blackPawn.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/blackPawn.png");
                        //drawPiece(col, row, "blackPawn.png");
                    }
                }
                else if (owner == 'W') {
                    if (type == 'K') {
                        drawPiece(col, row, "../A8EducationApplication/whiteKing.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/whiteKing.png");
                        //drawPiece(col, row, "whiteKing.png");
                    }
                    else if (type == 'Q') {
                        drawPiece(col, row, "../A8EducationApplication/whiteQueen.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/whiteQueen.png");
                        //drawPiece(col, row, "whiteQueen.png");
                    }
                    else if (type == 'R') {
                        drawPiece(col, row, "../A8EducationApplication/whiteRook.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/whiteRook.png");
                        //drawPiece(col, row, "whiteRook.png");
                    }
                    else if (type == 'N') {
                        drawPiece(col, row, "../A8EducationApplication/whiteKnight.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/whiteKnight.png");
                        //drawPiece(col, row, "whiteKnight.png");
                    }
                    else if (type == 'B') {
                        drawPiece(col, row, "../A8EducationApplication/whiteBishop.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/whiteBishop.png");
                        //drawPiece(col, row, "whiteBishop.png");
                    }
                    else if (type == 'p') {
                        drawPiece(col, row, "../A8EducationApplication/whitePawn.png");
//                        drawPiece(col, row, "/Users/kayleemartin/a8-an-educational-app-f18-readgirl52/A8EducationApplication/whitePawn.png");
                        //drawPiece(col, row, "whitePawn.png");
                    }
                }
                else {

                }
            }
        }
    }
    update();
}

void ChessLabel::drawPiece(int col, int row, QString filename) {
    QPainter painter(&image);
    int x = col * squareSize;
    int y = row * squareSize;
    QPoint corner = QPoint(x, y);
    QPoint endPoint = QPoint (x + squareSize, y + squareSize);
    QRect rectangle = QRect(corner, endPoint);
    QPixmap King = QPixmap(filename);
    painter.drawPixmap(rectangle, King);
}

void ChessLabel::openPromotionDialog(){
    QDialog dlg;
    QVBoxLayout layout(&dlg);

    QPushButton *queen = new QPushButton(tr("Queen"));
    QPushButton *rook = new QPushButton(tr("Rook"));
    QPushButton *bishop = new QPushButton(tr("Bishop"));
    QPushButton *knight = new QPushButton(tr("Knight"));


    //Create the button container. Add all the buttons to the dialog box.
    QDialogButtonBox *box = new QDialogButtonBox();

    // Ensure buttons close the dialog.
    connect(queen,  &QPushButton::clicked, [&dlg](){dlg.done(1);});
    connect(rook,   &QPushButton::clicked, [&dlg](){dlg.done(2);});
    connect(bishop, &QPushButton::clicked, [&dlg](){dlg.done(3);});
    connect(knight, &QPushButton::clicked, [&dlg](){dlg.done(4);});

    connect(queen, SIGNAL(clicked()),  &dlg, SLOT(close()));
    connect(rook, SIGNAL(clicked()),   &dlg, SLOT(close()));
    connect(bishop, SIGNAL(clicked()), &dlg, SLOT(close()));
    connect(knight, SIGNAL(clicked()), &dlg, SLOT(close()));


    box->addButton(queen, QDialogButtonBox::QDialogButtonBox::ButtonRole(1));
    box->addButton(rook, QDialogButtonBox::QDialogButtonBox::ButtonRole(1));
    box->addButton(bishop, QDialogButtonBox::QDialogButtonBox::ButtonRole(1));
    box->addButton(knight, QDialogButtonBox::QDialogButtonBox::ButtonRole(1));


    layout.addWidget(box);
    dlg.setLayout(&layout);

    int result = dlg.exec();
    switch(result) {
        case 1:
            updatePromotionPiece('Q');
            break;
        case 2:
            updatePromotionPiece('R');
            break;
        case 3:
            updatePromotionPiece('B');
            break;
        case 4:
            updatePromotionPiece('N');
            break;
        default:
            break;
    }
    dlg.close();
}
