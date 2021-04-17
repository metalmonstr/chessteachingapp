#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "chesslabel.h"
#include "controller.h"
#include "congratulations.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, controller *control = new controller());
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    controller *control;

    private slots:
    void congratsWindow();
    void openFile();
    void saveFile();

    //chessboard board;
};
#endif // MAINWINDOW_H
