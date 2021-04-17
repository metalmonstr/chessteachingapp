#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chesslabel.h"

MainWindow::MainWindow(QWidget *parent, controller* app)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    chessboard board;
    control = app;
    ui->progressBar->setRange(0, 40);

    //connections dealling with files
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::openFile);
    connect(control, &controller::updateSave, ui->actionSave, &QAction::setEnabled);

    // Connections to update the board.
    connect(control, &controller::clearBoard, ui->label, &ChessLabel::clearBoard);
    connect(control, &controller::updateBoard, ui->label, &ChessLabel::placePieces);
    connect(control, &controller::updateTitle, ui->titleLabel, &QLabel::setText);
    connect(control, &controller::updateMessage, ui->MessageBox, &QLabel::setText);
    connect(control, &controller::updateNextButton, ui->NextButton, &QPushButton::setEnabled);
    connect(control, &controller::updateBoardStatus, ui->label, &QLabel::setEnabled);
    connect(control, &controller::updateProgress, ui->progressBar, &QProgressBar::setValue);
    connect(control, &controller::openPromotionDialog, ui->label, &ChessLabel::openPromotionDialog);
    connect(control, &controller::pieceSelected, ui->label, &ChessLabel::highlightMoves);
    connect(control, &controller::legalMove, ui->label, &ChessLabel::legalMove);
    connect(control, &controller::victory, this, &MainWindow::congratsWindow);

    connect(ui->NextButton, &QPushButton::clicked, control, &controller::incrementStage);
    connect(ui->ResetButton, &QPushButton::clicked, control, &controller::resetStage);
    connect(ui->label, &ChessLabel::pieceMoved, control, &controller::move);
    connect(ui->label, &ChessLabel::mouseClick, control, &controller::mouseEvent);
    connect(ui->label, &ChessLabel::updatePromotionPiece, control, &controller::updatePromotionPiece);
    connect(ui->label, &ChessLabel::undoHighlight, control, &controller::undoHighlight);

    control->initializeView(control->stages.at(0).board);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveFile() {
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Save Board"), "", tr("All Files (*)"), nullptr, QFileDialog::DontUseNativeDialog);

    if (saveFile.isEmpty()) {
       return;
    }
    else {
       QFile file(saveFile);
       if (!file.open(QIODevice::WriteOnly)) {
           std::cout << "Unable to open file\n";
           return;
       }

       QJsonObject firstJSON;
       firstJSON["level"] = control->getCurrentStage();

       QJsonDocument documentJSON(firstJSON);
       QString jsonString = documentJSON.toJson(QJsonDocument::Indented);
       QTextStream jsonStream(&file);
       jsonStream << jsonString;
    }
}

void MainWindow::openFile() {
    QString board = QFileDialog::getOpenFileName(this, tr("Open Board"), "", tr("All Files (*)"), nullptr, QFileDialog::DontUseNativeDialog);

    if(board.isEmpty()) {
        std::cout << "No file provided!";
        return;
    }
    else {
        QFile file(board);
        if(!file.open(QIODevice::ReadOnly)) {
            std::cout << "Unable to open file";
            return;
        }

        int level = 0;
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            line = line.trimmed();

            if(line.startsWith("\"level\":")) {
                std::string L = std::string(line);
                L = L.substr(9, 2);
                level = stoi(L);
                control->setCurrentStage(level);
                control->resetStage();
            }
        }

        file.close();
    }
}

void MainWindow::congratsWindow(){
//    congratulations *congrats = new congratulations;
//    congrats->show();
//    congrats->raise();
//    congrats->activateWindow();
}
