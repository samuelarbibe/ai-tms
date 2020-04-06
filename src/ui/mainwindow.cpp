#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow) {
	ui->setupUi(this);

	sf::ContextSettings settings;

	SimulatorEngine = new Engine(ui->SimulatorFrame);

	DistanceUnits currentDistanceUnit = static_cast<DistanceUnits>(ui->DistanceUnitComboBox->currentIndex());
	VelocityUnits currentUnit = static_cast<VelocityUnits>(ui->VelocityUnitComboBox->currentIndex());

	// load presets
	ui->LaneWidthSlider->setMinimum(int(Settings::MinLaneWidth));
	ui->LaneWidthSlider->setMaximum(int(Settings::MaxLaneWidth));
	ui->LaneWidthSlider->setSliderPosition(int(Settings::LaneWidth));
    ui->VehicleSpawnRateSlider->setMinimum(0);
    ui->VehicleSpawnRateSlider->setMaximum(2000);
    ui->VehicleSpawnRateTextBox->setText(QString::number(Settings::VehicleSpawnRate * 1000.f));
    ui->VehicleSpawnRateSlider->setSliderPosition(int(Settings::VehicleSpawnRate * 1000.f));
	ui->ZoomSlider->setSliderPosition(int(Settings::Zoom * 99));
	ui->LaneWidthValueEdit->setText(QString::number(Settings::GetLaneWidthAs(currentDistanceUnit)));
	ui->CarMaxSpeed->setText(QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::SMALL_CAR, currentUnit)));
	ui->MotorcycleMaxSpeed->setText(
		QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::LONG_CAR, currentUnit)));
	ui->TruckMaxSpeed->setText(QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::MEDIUM_CAR, currentUnit)));

	ui->PhaseDelayLineEdit->setText(QString::number(Settings::PhaseDelay));
	ui->PhaseDelaySlider->setSliderPosition(Settings::PhaseDelay);
	ui->PhaseTimeSlider->setMaximum(int(Settings::MaxCycleTime));
	ui->PhaseTimeSlider->setMinimum(int(Settings::MinCycleTime));

	model_ = new SimModel(this);
	selected_row_ = 99999;

	// connect the simulation finished event to its slot here
	QObject::connect(SimulatorEngine, SIGNAL(SimulationFinished()), this, SLOT(on_SimulationFinished()));
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::on_SimulationFinished() {
	ui->AbortButton->setEnabled(false);
	reload_sim_table();
	resize_sim_table();
}

void MainWindow::reload_sim_table() {

	model_ = new SimModel(this);
	model_->populateData(SimulatorEngine->GetSimulations());
	ui->SimTable->setModel(model_);
}

void MainWindow::showEvent(QShowEvent *ev) {
	QMainWindow::showEvent(ev);

	// things that can be done only after complete init
	SimulatorEngine->ResizeFrame(ui->SimulatorFrame->size() * Settings::SFMLRatio);

	reloadOptionData();
}

void MainWindow::reloadOptionData() {
	// set intersection number range for future use

	ui->FromIntersectionComboBox->clear();
	ui->ToIntersectionComboBox->clear();
	ui->IntersectionComboBox->clear();
    ui->IntersectionNumberComboBox->clear();
	ui->NearRoadComboBox->clear();
	ui->ToPhaseComboBox->clear();
    ui->ShowLanesForPhaseComboBox->clear();
    ui->ToCycleComboBox->clear();
	ui->PhaseTimeComboBox->clear();
	ui->FromLaneComboBox->clear();
	ui->ToLaneComboBox->clear();
	ui->AssignLaneToPhaseComboBox->clear();
    ui->ToRoadComboBox->clear();

	for (const QString s : SimulatorEngine->map->GetIntersectionIdList())
	{
		ui->FromIntersectionComboBox->addItem(s);
		ui->ToIntersectionComboBox->addItem(s);
		ui->IntersectionComboBox->addItem(s);
        ui->IntersectionNumberComboBox->addItem(s);
	}

	for (const QString sd : SimulatorEngine->map->GetRoadIdList())
	{
		ui->ToRoadComboBox->addItem(sd);
		ui->NearRoadComboBox->addItem(sd);
	}

	for (const QString sd : SimulatorEngine->map->GetLaneIdList())
	{
		ui->FromLaneComboBox->addItem(sd);
		ui->ToLaneComboBox->addItem(sd);
	}

	for (const QString p : SimulatorEngine->map->GetPhaseIdList())
	{
        ui->ShowLanesForPhaseComboBox->addItem(p);
		ui->PhaseTimeComboBox->addItem(p);
		ui->ToPhaseComboBox->addItem(p);
		ui->AssignLaneToPhaseComboBox->addItem(p);
	}

    for (const QString p : SimulatorEngine->map->GetCycleIdList())
    {
        ui->ToCycleComboBox->addItem(p);
    }

    reload_lane_options();
}

void MainWindow::reload_lane_options() {

	ui->AssignedLanesListView->clear();

    int phaseNumber = ui->ShowLanesForPhaseComboBox->currentText().toInt();
	if (phaseNumber != 0)
	{
		for (auto &p : SimulatorEngine->map->GetLaneIdList(phaseNumber))
		{
			QString s;
			s.append("Lane ");
			s.append(p);
			ui->AssignedLanesListView->addItem(s);
		}
	}
}

// When mouse is clicked, use click coordinates in map setup
void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
	if (event->buttons() == Qt::LeftButton)
	{

		QPoint clickPoint;
		Vector2f point;

		clickPoint = SimulatorEngine->mapFromGlobal(QCursor::pos());

		point.x = clickPoint.x();
		point.y = clickPoint.y();

		if (point.x > 0 && point.y > 0)
		{
			// draw a point on simulator canvas to indicate last clicked position

			point = SimulatorEngine->DrawPoint(point);

			ui->IntersectionXEdit->setText(QString::number(int(point.x)));
			ui->IntersectionYEdit->setText(QString::number(int(point.y)));

			ui->statusbar->showMessage(
				tr("You can now Click 'Add Intersection' to add an intersection at the clicked position "));

			// check for lane selection
			Lane *selectedLane = SimulatorEngine->map->SelectedLane;
			bool isLaneSelected = (selectedLane != nullptr);
			ui->DeleteButton->setEnabled(isLaneSelected);
			if (isLaneSelected)
			{
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

			// check for car selection
			Vehicle *selectedVehicle = Vehicle::SelectedVehicle;
			bool isVehicleSelected = (selectedVehicle != nullptr);
			if (isVehicleSelected)
			{
				QString selectionText = "Selected: Vehicle {";
				selectionText.append(QString::number(selectedVehicle->GetVehicleNumber()));
				selectionText.append("}");
				/*
				selectionText.append("Status : {");
				selectionText.append(QString::number(selectedVehicle->GetState()));
				selectionText.append("}");
				 */

				ui->statusbar->showMessage(selectionText);
			}

		}

	}
}

void MainWindow::on_AddIntersectionButton_clicked() {
	if (ui->IntersectionXEdit->text().length() > 0 && ui->IntersectionYEdit->text().length() > 0)
	{
		int x = ui->IntersectionXEdit->text().toInt();
		int y = ui->IntersectionYEdit->text().toInt();

		// check for usable data
		if (x > 0 && x < SimulatorEngine->map->GetSize().x && y > 0 && y < SimulatorEngine->map->GetSize().y)
		{
			Vector2f position(x, y);
			if (SimulatorEngine->map->AddIntersection(0, position) != nullptr)
			{
				ui->statusbar->clearMessage();
				ui->statusbar->showMessage(tr("Intersection Successfully added."), 5000);
				// set data for future use
				reloadOptionData();
				return; // success
			}
		}
	}
	ui->statusbar->showMessage(tr("Could not add Intersection. please check that the entered values are correct. "));
}

void MainWindow::on_AddConnectingRoadButton_clicked() {
	int intersection1 = ui->FromIntersectionComboBox->currentText().toInt();
	int intersection2 = ui->ToIntersectionComboBox->currentText().toInt();

	// check for usable data
	if (intersection1 != intersection2)
	{
		if (SimulatorEngine->map->AddConnectingRoad(0, intersection1, intersection2) != nullptr)
		{
			ui->statusbar->clearMessage();
			ui->statusbar->showMessage(tr("Connecting Road Successfully added."), 5000);
			// refresh spinboxes data
			reloadOptionData();
			return; // success
		}
	}

	ui->statusbar->showMessage(tr("Could not add Connecting Road. please check that the entered values are correct. "));
}

void MainWindow::on_AddRoadButton_clicked() {
	int intersectionNumber = ui->IntersectionComboBox->currentText().toInt();
	int connectionSide = ui->ConSideComboBox->currentIndex() + 1;

	if (SimulatorEngine->map->AddRoad(0, intersectionNumber, connectionSide, Settings::DefaultLaneLength))
	{
		// refresh spin-boxes data
		reloadOptionData();

		ui->statusbar->clearMessage();
		ui->statusbar->showMessage(tr("Road Successfully added."), 5000);
		reloadOptionData();
		return; // success
	}
	ui->statusbar->showMessage(tr("Could not add Road. please check that the entered values are correct. "));
}

void MainWindow::on_AddLanePushButton_clicked() {
	int roadNumber = ui->ToRoadComboBox->currentText().toInt();
	bool isInRoadDirection = ui->InDirectionCheckBox->isChecked();

	if (SimulatorEngine->map->AddLane(0, roadNumber, isInRoadDirection))
	{
		ui->statusbar->clearMessage();
		ui->statusbar->showMessage(tr("Lane Successfully added."), 5000);
		reloadOptionData();
		return; // success
	}
	ui->statusbar->showMessage(tr("Could not add Lane. please check that the entered values are correct. "));

}

void MainWindow::on_SnapToGridCheckBox_stateChanged(int arg1) {
	bool isChecked = ui->SnapToGridCheckBox->isChecked();

	ui->ShowGridCheckBox->setChecked(isChecked);
	ui->ShowGridCheckBox->setEnabled(isChecked);
	SimulatorEngine->SetSnapToGrid(isChecked);
}

void MainWindow::on_ShowGridCheckBox_stateChanged(int arg1) {
	Settings::DrawGrid = arg1;
}

void MainWindow::on_LaneWidthSlider_sliderMoved(int position) {
	DistanceUnits unit = static_cast<DistanceUnits>(ui->DistanceUnitComboBox->currentIndex());
	// setting the value will cut the value to the current range
	ui->LaneWidthSlider->setValue(position);
	Settings::LaneWidth = ui->LaneWidthSlider->value();
	ui->LaneWidthValueEdit->setText(QString::number(Settings::GetLaneWidthAs(unit)));
	// check if init was called, if was then reload map
	if (SimulatorEngine->map != nullptr)
		SimulatorEngine->map->ReloadMap();
}

void MainWindow::on_DistanceUnitComboBox_currentIndexChanged(int index) {
	// get slider position -> lane with as px
	ui->LaneWidthValueEdit->setText(QString::number(Settings::GetLaneWidthAs(static_cast<DistanceUnits>(index))));
}

void MainWindow::on_LaneWidthValueEdit_editingFinished() {
	// get the entered value in the current unit
	float enteredValue = ui->LaneWidthValueEdit->text().toFloat();

	// get the current unit
	DistanceUnits currentUnit = static_cast<DistanceUnits>(int(ui->DistanceUnitComboBox->currentIndex()));

	// convert the entered unit to PX
	enteredValue = Settings::ConvertSize(currentUnit, DistanceUnits::PX, enteredValue);
	// send it to this function that simulate a slider movement
	on_LaneWidthSlider_sliderMoved(enteredValue);
}

void MainWindow::on_ZoomSlider_valueChanged(int value) {
	float zoomValue = 1.f - value / 100.f;
	SimulatorEngine->UpdateView(Vector2f(0, 0), zoomValue);
}

void MainWindow::on_CarMaxSpeed_editingFinished() {
	// get the entered value for the max speed
	float enteredValue = ui->CarMaxSpeed->text().toFloat();

	// get the current unit
	VelocityUnits currentUnit = static_cast<VelocityUnits>(int(ui->VelocityUnitComboBox->currentIndex()));

	// convert the entered value to px
	enteredValue = Settings::ConvertVelocity(currentUnit, VelocityUnits::PXS, enteredValue);

	// save the changes
	Settings::MaxSpeeds[VehicleTypeOptions::SMALL_CAR] = enteredValue;
}

void MainWindow::on_MotorcycleMaxSpeed_editingFinished() {
	// get the entered value for the max speed
	float enteredValue = ui->CarMaxSpeed->text().toFloat();

	// get the current unit
	VelocityUnits currentUnit = static_cast<VelocityUnits>(int(ui->VelocityUnitComboBox->currentIndex()));

	// convert the entered value to px
	enteredValue = Settings::ConvertVelocity(currentUnit, VelocityUnits::PXS, enteredValue);

	// save the changes
	Settings::MaxSpeeds[VehicleTypeOptions::LONG_CAR] = enteredValue;
}

void MainWindow::on_TruckMaxSpeed_editingFinished() {
	// get the entered value for the max speed
	float enteredValue = ui->CarMaxSpeed->text().toFloat();

	// get the current unit
	VelocityUnits currentUnit = static_cast<VelocityUnits>(int(ui->VelocityUnitComboBox->currentIndex()));

	// convert the entered value to px
	enteredValue = Settings::ConvertVelocity(currentUnit, VelocityUnits::PXS, enteredValue);

	// save the changes
	Settings::MaxSpeeds[VehicleTypeOptions::MEDIUM_CAR] = enteredValue;
}

void MainWindow::on_VelocityUnitComboBox_currentIndexChanged(int index) {
	VelocityUnits currentUnit = static_cast<VelocityUnits>(index);
	// re-display all the velocities
	ui->CarMaxSpeed->setText(QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::SMALL_CAR, currentUnit)));
	ui->MotorcycleMaxSpeed->setText(
		QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::LONG_CAR, currentUnit)));
	ui->TruckMaxSpeed->setText(QString::number(Settings::GetMaxSpeedAs(VehicleTypeOptions::MEDIUM_CAR, currentUnit)));
}

void MainWindow::on_MultiColorCheckBox_stateChanged(int arg1) {
	Settings::MultiColor = ui->MultiColorCheckBox->isChecked();
}

void MainWindow::on_DeleteButton_clicked() {
	Lane *selectedLane = SimulatorEngine->map->SelectedLane;
	if (selectedLane != nullptr)
	{
		int laneNumber = selectedLane->GetLaneNumber();
		// if deletion was successful
		if (SimulatorEngine->map->DeleteLane(selectedLane->GetLaneNumber()))
		{

			QString text = "Lane ";
			text.append(QString::number(laneNumber));
			text.append(" has been deleted. ");
			ui->statusbar->showMessage(text);

			// set spinbox ranges
			reloadOptionData();
		}
	}
}

void MainWindow::on_ResetButton_clicked() {
	QMessageBox msgBox;
	msgBox.setText("Are you sure you want to reset map?");
	msgBox.setInformativeText("The map and all the active vehicles will be deleted.");
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Ok);
	int ret = msgBox.exec();

	switch (ret)
	{
	case QMessageBox::Ok:SimulatorEngine->ResetMap();
		ui->statusbar->showMessage(tr("map has been reset."));
		reloadOptionData();
		break;
	case QMessageBox::Cancel:break;
	}
}

void MainWindow::on_LoadMapButton_clicked() {
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(tr("JSON Files (*.json)"));
	dialog.setViewMode(QFileDialog::Detail);

	QStringList fileNames;
	if (dialog.exec())
	{
		fileNames = dialog.selectedFiles();
		SimulatorEngine->LoadMap(fileNames.front().toStdString());
		reloadOptionData();
	}
}

void MainWindow::on_SaveMapButton_clicked() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
	                                                "map.json",
	                                                tr("JSON Files (*.json"));
	SimulatorEngine->SaveMap(fileName.toStdString());
}

void MainWindow::on_ShowDataBoxesCheckBox_stateChanged(int arg1) {
	Settings::DrawRoadDataBoxes = arg1;
	Settings::DrawLightDataBoxes = arg1;
	Settings::DrawVehicleDataBoxes = arg1;
}

void MainWindow::on_FasterButton_clicked() {
	Settings::Speed *= 2;
	QString text = "Running speed: x";
	text.append(QString::number(Settings::Speed));
	ui->RunningSpeedLabel->setText(text);
}

void MainWindow::on_SlowerButton_clicked() {
	Settings::Speed /= 2.f;
	QString text = "Running speed: x";
	text.append(QString::number(Settings::Speed));
	ui->RunningSpeedLabel->setText(text);
}

void MainWindow::on_PauseButton_clicked() {
	static float prev_speed = 1.f;
	if (Settings::Speed != 0.f)
	{
		prev_speed = Settings::Speed;
		Settings::Speed = 0.f;
		ui->PauseButton->setText(tr(">"));
		ui->RunningSpeedLabel->setText(tr("Paused."));
	} else
	{
		Settings::Speed = prev_speed;
		ui->PauseButton->setText(tr("||"));
		QString text = "Running speed: x";
		text.append(QString::number(Settings::Speed));
		ui->RunningSpeedLabel->setText(text);
	}
}

void MainWindow::on_RunSimulationButton_clicked() {
	int amount = ui->CarCountSpinBox->value();
	bool timed = ui->TimedSimCheckBox->isChecked();

	if (!Simulation::SimRunning)
	{
		if(timed)
			SimulatorEngine->RunSimulation(1000, amount);
		else
			SimulatorEngine->RunSimulation(amount);
		ui->AbortButton->setEnabled(true);
	} else
	{
		ui->statusbar->showMessage(tr("Another simulation is currently running, please wait for it to finish"), 5000);
	}
}

void MainWindow::on_AddRouteButton_clicked() {
	int lane1 = ui->FromLaneComboBox->currentText().toInt();
	int lane2 = ui->ToLaneComboBox->currentText().toInt();

	if (lane1 != 0 && lane2 != 0)
	{
		if (SimulatorEngine->map->AddRoute(lane1, lane2))
		{
			ui->statusbar->showMessage(tr("Route Added Successfully."));
			reloadOptionData();
			return;
		}
	}
	ui->statusbar->showMessage(tr("Could not add Route"));
}

void MainWindow::on_ReloadButton_clicked() {
	reloadOptionData();
}

void MainWindow::on_ShowRoutesCheckBox_stateChanged(int arg1) {
	Settings::DrawRoutes = arg1;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	// resize simulation frame
	SimulatorEngine->ResizeFrame(ui->SimulatorFrame->size() * Settings::SFMLRatio);

	resize_sim_table();
}

void MainWindow::resize_sim_table() {
	// resize simulation table
	if (ui->SimTable->model() != nullptr)
	{
		int colCount = ui->SimTable->model()->columnCount();
		int colWidth = (ui->SimTable->size().width() - 20) / colCount;

		for (int i = 0; i < colCount; i++)
		{
			ui->SimTable->setColumnWidth(i, colWidth);
		}
	}
}

void MainWindow::on_ShowLanesForPhaseComboBox_currentTextChanged(const QString &arg1) {
    reload_lane_options();
    int phaseNumber = ui->ShowLanesForPhaseComboBox->currentText().toInt();

	if (phaseNumber != 0 && Settings::ShowSelectedPhaseLanes)
	{
		SimulatorEngine->map->SelectLanesByPhase(phaseNumber);
	}
}

void MainWindow::on_AssignedLanesListView_itemClicked(QListWidgetItem *item) {
	//int selectedLane = item->text().toInt();

}


void MainWindow::on_AddPhaseButton_clicked() {

    int cycleNumber = ui->ToCycleComboBox->currentText().toInt();

    SimulatorEngine->map->AddPhase(0, cycleNumber, Settings::DefaultCycleTime);
	reloadOptionData();
}


void MainWindow::on_AddLightButton_clicked() {
	int phaseNumber = ui->ToPhaseComboBox->currentText().toInt();
	int roadNumber = ui->NearRoadComboBox->currentText().toInt();

	if (phaseNumber != 0 && roadNumber != 0)
	{
		if (SimulatorEngine->map->AddLight(0, phaseNumber, roadNumber))
		{
			ui->statusbar->showMessage(tr("Route Added Successfully."));
			reloadOptionData();
			return;
		}
	}
	ui->statusbar->showMessage(tr("Could not add Traffic Light. View console for details"));
}

void MainWindow::on_PhaseTimeSlider_sliderMoved(int position) {
	int phaseNumber = ui->PhaseTimeComboBox->currentText().toInt();

	if (phaseNumber != 0)
	{
		if (SimulatorEngine->map->SetPhaseTime(phaseNumber, position))
		{
			ui->PhaseTimeSlider->setValue(position);
			ui->statusbar->showMessage(tr("Phase time set successfully"));
			ui->PhaseTimeLineEdit->setText(QString::number(position));
			return;
		}
	}
	ui->statusbar->showMessage(tr("Could not set phase time."));
}

void MainWindow::on_PhaseDelaySlider_sliderMoved(int position) {
	ui->PhaseDelaySlider->setValue(position);
	Settings::PhaseDelay = position;
	ui->PhaseDelayLineEdit->setText(QString::number(position));
	ui->statusbar->showMessage(tr("Phase delay changed."));
}

void MainWindow::on_AssignLaneButton_clicked() {
	int phaseNumber = ui->AssignLaneToPhaseComboBox->currentText().toInt();
	Lane *lane = SimulatorEngine->map->SelectedLane;

	if (phaseNumber != 0 && lane != nullptr)
	{
		if (SimulatorEngine->map->AssignLaneToPhase(phaseNumber, lane->GetLaneNumber()))
		{
			ui->statusbar->showMessage(tr("Orange Light delay changed."));
			reloadOptionData();
			return;
		}
	}

	ui->statusbar->showMessage(tr("To Assign a lane to a phase, please select click on a lane to select it."));
}

void MainWindow::on_PhaseTimeComboBox_currentTextChanged(const QString &arg1) {
	int phaseNumber = ui->PhaseTimeComboBox->currentText().toInt();
	if (phaseNumber != 0)
	{
		Phase *p = SimulatorEngine->map->GetPhase(phaseNumber);
		if (p != nullptr)
		{
			ui->PhaseTimeLineEdit->setText(QString::number(p->GetCycleTime()));
			ui->PhaseTimeSlider->setSliderPosition(p->GetCycleTime());
		}
	}
}

void MainWindow::on_PhaseTimeLineEdit_editingFinished() {
	int phaseNumber = ui->PhaseTimeComboBox->currentText().toInt();
	float value = ui->PhaseTimeLineEdit->text().toFloat();

	if (phaseNumber != 0)
	{
		if (SimulatorEngine->map->SetPhaseTime(phaseNumber, value))
		{
			ui->PhaseTimeSlider->setValue(int(value));
			ui->statusbar->showMessage(tr("Phase time set successfully"));
			ui->PhaseTimeLineEdit->setText(QString::number(value));
			return;
		}
	}
	ui->statusbar->showMessage(tr("Could not set phase time."));
}

void MainWindow::on_PhaseDelayLineEdit_editingFinished() {
	float value = ui->PhaseDelayLineEdit->text().toFloat();
	ui->PhaseDelaySlider->setValue(int(value));
	Settings::PhaseDelay = value;
	ui->PhaseDelayLineEdit->setText(QString::number(value));
	ui->statusbar->showMessage(tr("Phase delay changed."));
}

void MainWindow::on_ShowLaneBlockCheckBox_stateChanged(int arg1) {
	Settings::DrawLaneBlock = arg1;
}

void MainWindow::on_DrawTexturesCheckBox_stateChanged(int arg1) {
	Settings::DrawTextures = arg1;
	ui->MultiColorCheckBox->setEnabled(arg1);
}

void MainWindow::on_FollowSelectedCarButton_stateChanged(int arg1) {
	Settings::FollowSelectedVehicle = arg1;
}

void MainWindow::on_AbortButton_clicked() {
	SimulatorEngine->ClearMap();
	ui->AbortButton->setEnabled(false);
}

void MainWindow::on_SaveSimButton_clicked() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
	                                                "simulations.json",
	                                                tr("JSON Files (*.json"));
	SimulatorEngine->SaveSimulations(fileName.toStdString());
}

void MainWindow::on_LoadSimButton_clicked() {
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(tr("JSON Files (*.json)"));
	dialog.setViewMode(QFileDialog::Detail);

	QStringList fileNames;
	if (dialog.exec())
	{
		fileNames = dialog.selectedFiles();
		SimulatorEngine->LoadSimulations(fileNames.front().toStdString());
		reloadOptionData();
		reload_sim_table();
		resize_sim_table();
	}
}

void MainWindow::on_SimTable_clicked(const QModelIndex &index) {
	ui->RunDemoButton->setEnabled(true);
	ui->DeleteSimButton->setEnabled(true);
	selected_row_ = index.row();
}

void MainWindow::on_RunDemoButton_clicked() {
	int simNumber = model_->GetIdByRow(selected_row_);
	if (simNumber != 0)
	{
		SimulatorEngine->RunDemo(simNumber);
	} else
	{
		ui->statusbar->showMessage(tr("Could not run demo."));
	}

}

void MainWindow::on_DeleteSimButton_clicked() {
	int simNumber = model_->GetIdByRow(selected_row_);
	if (simNumber != 0)
	{
		if (SimulatorEngine->DeleteSimulation(simNumber))
		{
			QString s = "Simulation ";
			s.append(QString::number(simNumber));
			s.append(" has been deleted.");

			ui->statusbar->showMessage(s);
			reload_sim_table();
		} else
		{
			QString s = "Simulation ";
			s.append(QString::number(simNumber));
			s.append(" failed to be deleted.");
		}
	} else
	{
		ui->statusbar->showMessage(tr("Could not delete simulation."));
	}
}

void MainWindow::on_ShowMinimapCheckBox_stateChanged(int arg1) {
	Settings::DrawMinimap = arg1;
}

void MainWindow::on_TimedSimCheckBox_stateChanged(int arg1)
{
    if(arg1){
        ui->SimRunLabel->setText(tr("Seconds"));
    }
    else{
        ui->SimRunLabel->setText(tr("Vehicles"));
    }
}

void MainWindow::on_DensityColorCheckBox_stateChanged(int arg1)
{
    Settings::LaneDensityColorRamping = arg1;
}

void MainWindow::on_VehicleSpawnRateSlider_sliderMoved(int position)
{
    ui->VehicleSpawnRateTextBox->setText(QString::number(position));
    Settings::VehicleSpawnRate = position/1000.f;
}

void MainWindow::on_VehicleSpawnRateTextBox_editingFinished()
{
    float value = ui->VehicleSpawnRateTextBox->text().toFloat();
    ui->VehicleSpawnRateSlider->setValue(value);
    Settings::VehicleSpawnRate = value/1000.f;
}


void MainWindow::on_ShowSelectedPhaseLanesCheckBox_stateChanged(int arg1)
{
    Settings::ShowSelectedPhaseLanes = arg1;
    if(arg1)
    {
        reloadOptionData();
    }
    else
    {
        SimulatorEngine->map->UnselectAll();
    }
}

void MainWindow::on_AddCycleButton_clicked()
{
    int intersectionNumber = 0;
    if(ui->IntersectionNumberComboBox->isEnabled())
    {
         intersectionNumber = ui->IntersectionNumberComboBox->currentText().toInt();
    }

    if(SimulatorEngine->map->AddCycle(0, intersectionNumber) != nullptr)
    {
        reloadOptionData();
        ui->statusbar->showMessage(tr("Cycle successfully added."));
        return;
    }

    ui->statusbar->showMessage(tr("ERROR: Could not add cycle."));
}

void MainWindow::on_AssignToIntersectionCheckBox_stateChanged(int arg1)
{
    ui->IntersectionNumberComboBox->setEnabled(arg1);
}


void MainWindow::on_RemoveLaneFromPhaseButton_clicked()
{
    if(ui->AssignedLanesListView->selectedItems().count() == 1)
    {
        int laneNumber = ui->AssignedLanesListView->selectedItems().back()->text().mid(5).toInt();

        SimulatorEngine->map->UnassignLaneFromPhase(laneNumber);
        reloadOptionData();
        ui->statusbar->showMessage(tr("Selected lane unassigned from phase."));
        return;

    }
    ui->statusbar->showMessage(tr("Please select a lane from the list to unassign it from its phase."));
}
