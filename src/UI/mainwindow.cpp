#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SimulatorEngine = new Engine(ui->frame);
}

MainWindow::~MainWindow()
{
    delete ui;
}
