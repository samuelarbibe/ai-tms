#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SimulatorEngine = new Engine(ui->SimulatorFrame);

    // load presets
    ui->LaneWidthSlider->setMinimum(Settings::MinLaneWidth);
    ui->LaneWidthSlider->setMaximum(Settings::MaxLaneWidth);
    ui->ZoomSlider->setSliderPosition(Settings::Zoom * 99);
    this->on_LaneWidthSlider_sliderMoved(Settings::LaneWidth);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// When mouse is clicked, use click coordinates in Map setup
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint clickPoint;
    Vector2f point;

    clickPoint = SimulatorEngine->mapFromGlobal(QCursor::pos());

    point.x = clickPoint.x();
    point.y = clickPoint.y();

    //cout << point.x << point.y << endl;

    if(point.x > 0 && point.y > 0)
    {
        // draw a point on simulator canvas to indicate last clicked position
        point = SimulatorEngine->DrawPoint(point);

        ui->IntersectionXEdit->setText(QString::number(int(point.x)));
        ui->IntersectionYEdit->setText(QString::number(int(point.y)));

        ui->statusbar->showMessage(tr("You can now Click 'Add Intersection' to add an intersection at the clicked position "));
    }
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
    int intersectionNumber = ui->IntersectionSpinBox->value();
    int connectionSide     = ui->ConSideComboBox->currentIndex()+1;

    if(SimulatorEngine->map->AddRoad(0, intersectionNumber, connectionSide, Settings::DefaultLaneLength / Settings::Scale))
    {
        ui->ToRoadSpinBox->setRange(1, Road::RoadCount);

        ui->statusbar->clearMessage();
        ui->statusbar->showMessage(tr("Road Succefully added."), 5000);
        return; // success
    }
    ui->statusbar->showMessage(tr("Could not add Road. please check that the entered values are correct. "));
}

void MainWindow::on_AddLanePushButton_clicked()
{
    int roadNumber         = ui->ToRoadSpinBox->value();
    bool isInRoadDirection = ui->InDirectionCheckBox->isChecked();

    if(SimulatorEngine->map->AddLane(0, roadNumber, isInRoadDirection))
    {
        ui->statusbar->clearMessage();
        ui->statusbar->showMessage(tr("Lane Succefully added."), 5000);
        return; // success
    }
    ui->statusbar->showMessage(tr("Could not add Lane. please check that the entered values are correct. "));

}

void MainWindow::on_SnapToGridCheckBox_stateChanged(int arg1)
{
    bool isChecked = ui->SnapToGridCheckBox->isChecked();

    ui->ShowGridCheckBox->setChecked(isChecked);
    ui->ShowGridCheckBox->setEnabled(isChecked);
    SimulatorEngine->SetSnapToGrid(isChecked);
}

void MainWindow::on_ShowGridCheckBox_stateChanged(int arg1)
{
    bool isChecked = ui->ShowGridCheckBox->isChecked();

    SimulatorEngine->ShowGrid(isChecked);
}



void MainWindow::on_LaneWidthSlider_sliderMoved(int position)
{
    Units unit = static_cast<Units>(ui->UnitComboBox->currentIndex());
    ui->LaneWidthSlider->setValue(position);
    ui->LaneWidthValueEdit->setText(QString::number(position * Settings::UnitScales[unit]));
    Settings::LaneWidth = position * Settings::Scale;
    if(SimulatorEngine->map != nullptr)SimulatorEngine->map->ReloadMap(); // check if init was called
}

void MainWindow::on_UnitComboBox_currentIndexChanged(int index)
{
    int position = ui->LaneWidthSlider->value();
    ui->LaneWidthValueEdit->setText(QString::number(position * Settings::UnitScales[index]));
}

void MainWindow::on_LaneWidthValueEdit_editingFinished()
{
    float value = ui->LaneWidthValueEdit->text().toFloat();

    // convert to slider units (cm)
    Units unit = static_cast<Units>(ui->UnitComboBox->currentIndex());
    int position = int(value / Settings::UnitScales[unit]);

    this->on_LaneWidthSlider_sliderMoved(position);
}

void MainWindow::on_ZoomSlider_valueChanged(int value)
{
    float zoomValue = 1.f - value/100.f;

    SimulatorEngine->SetView(zoomValue, Vector2f(0,0));
}
