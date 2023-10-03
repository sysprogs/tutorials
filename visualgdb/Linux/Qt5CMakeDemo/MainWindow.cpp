#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *frame = new QWidget(this);
    frame->setGeometry(32, 32, 128, 128);
    frame->setStyleSheet("background-image: url(:test.png)");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ButtonClicked()
{
    QMessageBox msgBox;
    msgBox.setText("Hello, World!");
    msgBox.setWindowTitle("VisualGDB Qt Demo");
    msgBox.exec();
}
