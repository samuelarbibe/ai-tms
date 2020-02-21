#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SimulatorEngine = new Engine(ui->SimulatorFrame);

    DistanceUnits currentDistanceUnit = static_cast<DistanceUnits>(ui->DistanceUnitComboBox->currentIndex());
    VelocityUnits currentUnit = static_cast<VelocityUnits>(ui->VelocityUnitComboBox->currentIndex());

    // load presets
    ui->LaneWidthSlider->setMinimum(int(Settings::MinLaneWidth));
    ui->LaneWidthSlider->setMaximum(int(Settings::MaxLaneWidth));
    ui->LaneWidthSlider->setSliderPosition(int(Settings::LaneWidth));
    ui->ZoomSlider->setSliderPosition(int(Settings::Zoom * 99));
    ui->LaneWidthValueEdit->setText(QString::number(Settings::GetLaneWidthAs(currentDistanceUnit)));
    ui->CarMaxSpeed->setText(QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::CAR, currentUnit)));
    ui->MotorcycleMaxSpeed->setText(
            QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::MOTORCYCLE, currentUnit)));
    ui->TruckMaxSpeed->setText(QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::TRUCK, currentUnit)));

    reloadOptionData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);

    // things that can be done only after complete init
    SimulatorEngine->ResizeFrame(ui->SimulatorFrame->size() * Settings::SFMLRatio);
    reloadOptionData();
}

void MainWindow::reloadOptionData()
{
    // set intersection number range for future use
    set<QString> intersectionIdList = SimulatorEngine->map->GetIntersectionIdList();

    ui->FromIntersectionComboBox->clear();
    ui->ToIntersectionComboBox->clear();
    ui->IntersectionComboBox->clear();

    for(const QString s : intersectionIdList)
    {
        ui->FromIntersectionComboBox->addItem(s);
        ui->ToIntersectionComboBox->addItem(s);
        ui->IntersectionComboBox->addItem(s);
    }

    ui->ToRoadComboBox->clear();

    for(const QString sd : SimulatorEngine->map->GetRoadIdList())
    {
        ui->ToRoadComboBox->addItem(sd);
    }

    ui->FromLaneComboBox->clear();
    ui->ToLaneComboBox->clear();

    for(const QString sd : SimulatorEngine->map->GetLaneIdList())
    {
        ui->FromLaneComboBox->addItem(sd);
        ui->ToLaneComboBox->addItem(sd);
    }
}


// When mouse is clicked, use click coordinates in Map setup
void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {

        QPoint clickPoint;
        Vector2f point;

        clickPoint = SimulatorEngine->mapFromGlobal(QCursor::pos());

        point.x = clickPoint.x();
        point.y = clickPoint.y();

        //cout << point.x << point.y << endl;

        if (point.x > 0 && point.y > 0) {
            // draw a point on simulator canvas to indicate last clicked position

            point = SimulatorEngine->DrawPoint(point);

            ui->IntersectionXEdit->setText(QString::number(int(point.x)));
            ui->IntersectionYEdit->setText(QString::number(int(point.y)));

            ui->statusbar->showMessage(
                    tr("You can now Click 'Add Intersection' to add an intersection at the clicked position "));

            Lane *selectedLane = SimulatorEngine->map->SelectedLane;
            bool isSelected = (selectedLane != nullptr);
            ui->DeleteButton->setEnabled(isSelected);
            if (isSelected) {
                QString selectionText = "Selected: Lane {";
                selectionText.append(QString::number(selectedLane->GetLaneNumber()));
                selectionText.append("}, Road {");
                selectionText.append(QString::number(selectedLane->GetRoadNumber()));
                selectionText.append("}, Intersection {");
                selectionText.append(QString::number(selectedLane->GetIntersectionNumber()));
                selectionText.append("}, Direction {");
                selectionText.append(QString::number(selectedLane->GetDirection()));
                selectionText.append("}");

                ui->statusbar->showMessage(selectionText);

            }

        }

    }
}

void MainWindow::on_AddIntersectionButton_clicked()
{
    if (ui->IntersectionXEdit->text().length() > 0 && ui->IntersectionYEdit->text().length() > 0) {
        int x = ui->IntersectionXEdit->text().toInt();
        int y = ui->IntersectionYEdit->text().toInt();

        // check for usable data
        if (x > 0 && x < SimulatorEngine->map->GetSize().x && y > 0 && y < SimulatorEngine->map->GetSize().y) {
            Vector2f position(x, y);
            if (SimulatorEngine->map->AddIntersection(0, position) != nullptr) {
                // set data for future use
                reloadOptionData();
                ui->statusbar->clearMessage();
                ui->statusbar->showMessage(tr("Intersection Successfully added."), 5000);
                return; // success
            }
        }
    }
    ui->statusbar->showMessage(tr("Could not add Intersection. please check that the entered values are correct. "));
}


void MainWindow::on_AddConnectingRoadButton_clicked()
{
    int intersection1 = ui->FromIntersectionComboBox->currentText().toInt();
    int intersection2 = ui->ToIntersectionComboBox->currentText().toInt();

    // check for usable data
    if (intersection1 != intersection2)
    {
        if (SimulatorEngine->map->AddConnectingRoad(0, intersection1, intersection2) != nullptr)
        {
            // refresh spinboxes data
            reloadOptionData();

            ui->statusbar->clearMessage();
            ui->statusbar->showMessage(tr("Connecting Road Successfully added."), 5000);
            return; // success
        }
    }

    ui->statusbar->showMessage(tr("Could not add Connecting Road. please check that the entered values are correct. "));
}

void MainWindow::on_AddRoadButton_clicked()
{
    int intersectionNumber = ui->IntersectionComboBox->currentText().toInt();
    int connectionSide = ui->ConSideComboBox->currentIndex() + 1;

    if (SimulatorEngine->map->AddRoad(0, intersectionNumber, connectionSide, Settings::DefaultLaneLength)) {
        // refresh spin-boxes data
        reloadOptionData();

        ui->statusbar->clearMessage();
        ui->statusbar->showMessage(tr("Road Successfully added."), 5000);
        reloadOptionData();
        return; // success
    }
    ui->statusbar->showMessage(tr("Could not add Road. please check that the entered values are correct. "));
}

void MainWindow::on_AddLanePushButton_clicked()
{
    int roadNumber = ui->ToRoadComboBox->currentText().toInt();
    bool isInRoadDirection = ui->InDirectionCheckBox->isChecked();

    if (SimulatorEngine->map->AddLane(0, roadNumber, isInRoadDirection)) {
        ui->statusbar->clearMessage();
        ui->statusbar->showMessage(tr("Lane Successfully added."), 5000);
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
    Settings::DrawGrid = arg1;
}

void MainWindow::on_LaneWidthSlider_sliderMoved(int position)
{
    DistanceUnits unit = static_cast<DistanceUnits>(ui->DistanceUnitComboBox->currentIndex());
    // setting the value will cut the value to the current range
    ui->LaneWidthSlider->setValue(position);
    Settings::LaneWidth = ui->LaneWidthSlider->value();
    ui->LaneWidthValueEdit->setText(QString::number(Settings::GetLaneWidthAs(unit)));
    // check if init was called, if was then reload map
    if (SimulatorEngine->map != nullptr)SimulatorEngine->map->ReloadMap();
}

void MainWindow::on_DistanceUnitComboBox_currentIndexChanged(int index)
{
    // get slider position -> lane with as px
    ui->LaneWidthValueEdit->setText(QString::number(Settings::GetLaneWidthAs(static_cast<DistanceUnits>(index))));
}

void MainWindow::on_LaneWidthValueEdit_editingFinished()
{
    // get the entered value in the current unit
    float enteredValue = ui->LaneWidthValueEdit->text().toFloat();

    // get the current unit
    DistanceUnits currentUnit = static_cast<DistanceUnits>(int(ui->DistanceUnitComboBox->currentIndex()));

    // convert the entered unit to PX
    enteredValue = Settings::ConvertSize(currentUnit, DistanceUnits::PX, enteredValue);
    // send it to this function that simulate a slider movement
    on_LaneWidthSlider_sliderMoved(enteredValue);
}

void MainWindow::on_ZoomSlider_valueChanged(int value)
{
    float zoomValue = 1.f - value / 100.f;
    SimulatorEngine->UpdateView(Vector2f(0, 0), zoomValue);
}

void MainWindow::on_CarMaxSpeed_editingFinished()
{
    // get the entered value for the max speed
    float enteredValue = ui->CarMaxSpeed->text().toFloat();

    // get the current unit
    VelocityUnits currentUnit = static_cast<VelocityUnits>(int(ui->VelocityUnitComboBox->currentIndex()));

    // convert the entered value to px
    enteredValue = Settings::ConvertVelocity(currentUnit, VelocityUnits::PXS, enteredValue);

    // save the changes
    Settings::MaxSpeeds[VehicleTypeOptions::CAR] = enteredValue;
}

void MainWindow::on_MotorcycleMaxSpeed_editingFinished()
{
    // get the entered value for the max speed
    float enteredValue = ui->CarMaxSpeed->text().toFloat();

    // get the current unit
    VelocityUnits currentUnit = static_cast<VelocityUnits>(int(ui->VelocityUnitComboBox->currentIndex()));

    // convert the entered value to px
    enteredValue = Settings::ConvertVelocity(currentUnit, VelocityUnits::PXS, enteredValue);

    // save the changes
    Settings::MaxSpeeds[VehicleTypeOptions::MOTORCYCLE] = enteredValue;
}

void MainWindow::on_TruckMaxSpeed_editingFinished()
{
    // get the entered value for the max speed
    float enteredValue = ui->CarMaxSpeed->text().toFloat();

    // get the current unit
    VelocityUnits currentUnit = static_cast<VelocityUnits>(int(ui->VelocityUnitComboBox->currentIndex()));

    // convert the entered value to px
    enteredValue = Settings::ConvertVelocity(currentUnit, VelocityUnits::PXS, enteredValue);

    // save the changes
    Settings::MaxSpeeds[VehicleTypeOptions::TRUCK] = enteredValue;
}

void MainWindow::on_VelocityUnitComboBox_currentIndexChanged(int index)
{
    VelocityUnits currentUnit = static_cast<VelocityUnits>(index);
    // re-display all the velocities
    ui->CarMaxSpeed->setText(QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::CAR, currentUnit)));
    ui->MotorcycleMaxSpeed->setText(
            QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::MOTORCYCLE, currentUnit)));
    ui->TruckMaxSpeed->setText(QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::TRUCK, currentUnit)));
}

void MainWindow::on_MultiColorCheckBox_stateChanged(int arg1)
{
    Settings::MultiColor = ui->MultiColorCheckBox->isChecked();
}

void MainWindow::on_DeleteButton_clicked()
{
    Lane *selectedLane = SimulatorEngine->map->SelectedLane;
    if (selectedLane != nullptr) {
        int laneNumber = selectedLane->GetLaneNumber();
        // if deletion was successful
        if (SimulatorEngine->map->DeleteLane(selectedLane->GetLaneNumber())) {
            QString text = "Lane ";
            text.append(QString::number(laneNumber));
            text.append(" has been deleted. ");
            ui->statusbar->showMessage(text);

            // set spinbox ranges
            reloadOptionData();
        }
    }
}

void MainWindow::on_ResetButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to reset map?");
    msgBox.setInformativeText("The map and all the active vehicles will be deleted.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    switch (ret) {
        case QMessageBox::Ok:
            SimulatorEngine->ResetMap();
            ui->statusbar->showMessage(tr("Map has been reset."));
            break;
        case QMessageBox::Cancel:
            break;
    }
}

void MainWindow::on_LoadMapButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();

        SimulatorEngine->LoadMap(fileNames.front().toStdString());
    }
}

void MainWindow::on_SaveMapButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "map.json",
                                                    tr("JSON Files (*.json"));
    SimulatorEngine->SaveMap(fileName.toStdString());
}

void MainWindow::on_ShowDataBoxesCheckBox_stateChanged(int arg1)
{
    Settings::DrawRoadDataBoxes = arg1;
    Settings::DrawVehicleDataBoxes = arg1;
}

void MainWindow::on_FasterButton_clicked()
{
    Settings::Speed *= 2;
    QString text = "Running speed: x";
    text.append(QString::number(Settings::Speed));
    ui->RunningSpeedLabel->setText(text);
}

void MainWindow::on_SlowerButton_clicked()
{
    Settings::Speed /= 2.f;
    QString text = "Running speed: x";
    text.append(QString::number(Settings::Speed));
    ui->RunningSpeedLabel->setText(text);
}

void MainWindow::on_PauseButton_clicked()
{
    static float prev_speed = 1.f;
    if(Settings::Speed != 0.f)
    {
        prev_speed = Settings::Speed;
        Settings::Speed = 0.f;
        ui->PauseButton->setText(tr(">"));
        ui->RunningSpeedLabel->setText(tr("Paused."));
    }
    else
    {
        Settings::Speed = prev_speed;
        ui->PauseButton->setText(tr("||"));
        QString text = "Running speed: x";
        text.append(QString::number(Settings::Speed));
        ui->RunningSpeedLabel->setText(text);
    }
}

void MainWindow::on_RunSimulationButton_clicked()
{
    int amount = ui->CarCountSpinBox->value();

    for(int i = 0; i < amount; i++)
    {
        SimulatorEngine->AddVehicleRandomly();
    }
}

void MainWindow::on_AddRouteButton_clicked()
{
    int lane1 = ui->FromLaneComboBox->currentText().toInt();
    int lane2 = ui->ToLaneComboBox->currentText().toInt();

    if(lane1 != 0 && lane2 != 0)
    {
        if(SimulatorEngine->map->AddRoute(lane1, lane2))
        {
            ui->statusbar->showMessage(tr("Route Added Successfully."));
            return;
        }
    }
    ui->statusbar->showMessage(tr("Could not add Route"));
}

void MainWindow::on_ReloadButton_clicked()
{
     reloadOptionData();
}

void MainWindow::on_ShowRoutesCheckBox_stateChanged(int arg1)
{
    Settings::DrawRoutes = arg1;
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    SimulatorEngine->ResizeFrame(ui->SimulatorFrame->size() * Settings::SFMLRatio);
}

