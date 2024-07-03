#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "classes.h"
#include "pickmenu.h"
#include "ingame.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    this->setCentralWidget(ui->stackedWidget);
    Storage::load();
    Storage::shuffle();
    Board::initBoard();
    ui->stackedWidget->addWidget(new pickmenu(this));
    ingame* ig = new ingame(ui->stackedWidget);
    ui->stackedWidget->addWidget(ig);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


