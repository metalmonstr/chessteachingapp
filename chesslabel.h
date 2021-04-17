#ifndef CHESSLABEL_H
#define CHESSLABEL_H

#include <QWidget>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include "chessboard.h"
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QtCore>

class ChessLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ChessLabel(QFrame *parent = nullptr);

signals:
    void pieceMoved(int row, int col);
    void mouseClick(int row, int col);
    void updatePromotionPiece(int pieceCode);
    void undoHighlight();

public slots:
    void clearBoard();
    void openPromotionDialog();
    void placePieces(chessboard board);
    void drawPiece(int col, int row, QString filename);
    void highlightMoves(QVector<QPair<int, int>>);
    void legalMove();

protected:
    void mousePressEvent(QMouseEvent *ev) override;
    void paintEvent(QPaintEvent *) override;

private:
    const int boardSize = 8;
    //if you want to change the size of the board you have to change this
    const int squareSize = 50;
    QImage image;
    QColor brush;
    int nearestCorner(int point);
    void clearSquare(int row, int col);
    bool moving = false;
    bool legal = false;
    int xOrig;
    int yOrig;
};

#endif // CHESSLABEL_H
