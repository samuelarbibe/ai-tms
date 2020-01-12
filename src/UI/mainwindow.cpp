#include "mainwindow.h"
#include "ui_mainwindow.h"

//int Road::RoadCount;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SimulatorEngine = new Engine(ui->SimulatorFrame);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_AddIntersectionButton_clicked()
{
    if(ui->IntersectionXEdit->text().length() > 0 && ui->IntersectionYEdit->text().length() > 0 )
    {
        int x = ui->IntersectionXEdit->text().toInt();
        int y = ui->IntersectionYEdit->text().toInt();

        // check for usable data
        if(x > 0 && x < SimulatorEngine->map->GetSize().x && y > 0 && y < SimulatorEngine->map->GetSize().y)
        {
            Vector2f position(x, y);
            if(SimulatorEngine->map->AddIntersection(0, position) != nullptr)
            {
                // set intersection number range for future use
                ui->FromIntersectionSpinBox->setRange(1, Intersection::IntersectionCount);
                ui->ToIntersectionSpinBox->setRange(1, Intersection::IntersectionCount);
                ui->IntersectionSpinBox->setRange(1, Intersection::IntersectionCount);
                ui->statusbar->clearMessage();
                ui->statusbar->showMessage(tr("Intersection Succefully added."), 5000);
                return; // success
            }
        }
    }
    ui->statusbar->showMessage(tr("Could not add Intersection. please check that the entered values are correct. "));
}


void MainWindow::on_AddConnectingRoadButton_clicked()
{

    int intersection1 = ui->FromIntersectionSpinBox->value();
    int intersection2 = ui->ToIntersectionSpinBox->value();

    // check for usable data
    if(intersection1 != intersection2)
    {
        if(SimulatorEngine->map->AddConnectingRoad(0, intersection1, intersection2) != nullptr)
        {
            ui->ToRoadSpinBox->setRange(1, Road::RoadCount);

            ui->statusbar->clearMessage();
            ui->statusbar->showMessage(tr("Connecting Road Succefully added."), 5000);
            return; // success
        }
    }

    ui->statusbar->showMessage(tr("Could not add Connecting Road. please check that the entered values are correct. "));
}

void MainWindow::on_AddRoadButton_clicked()
{

}
