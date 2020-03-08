#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "../Simulator/Engine.hpp"
#include "Widgets/SimModel.hpp"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
  
  public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	
    void reloadSimTable();
	void mouseDoubleClickEvent(QMouseEvent *event) override;
    void Update(float elapsedTime);
	void resizeEvent(QResizeEvent *event) override;

  protected:
	
	void showEvent(QShowEvent *ev) override;

  
  private slots:
	void on_SimulationFinished();

	void on_AddIntersectionButton_clicked();
	
	void on_AddConnectingRoadButton_clicked();
	
	void on_AddRoadButton_clicked();
	
	void on_AddLanePushButton_clicked();
	
	void on_SnapToGridCheckBox_stateChanged(int arg1);
	
	void on_ShowGridCheckBox_stateChanged(int arg1);
	
	void on_LaneWidthSlider_sliderMoved(int position);
	
	void on_DistanceUnitComboBox_currentIndexChanged(int index);
	
	void on_LaneWidthValueEdit_editingFinished();
	
	void on_ZoomSlider_valueChanged(int value);
	
	void on_CarMaxSpeed_editingFinished();
	
	void on_MotorcycleMaxSpeed_editingFinished();
	
	void on_TruckMaxSpeed_editingFinished();
	
	void on_VelocityUnitComboBox_currentIndexChanged(int index);
	
	void on_MultiColorCheckBox_stateChanged(int arg1);
	
	void on_DeleteButton_clicked();
	
	void reloadOptionData();
	
	void on_ResetButton_clicked();
	
	void on_LoadMapButton_clicked();
	
	void on_SaveMapButton_clicked();
	
	void on_ShowDataBoxesCheckBox_stateChanged(int arg1);
	
	void on_ShowRoutesCheckBox_stateChanged(int arg1);
	
	void on_FasterButton_clicked();
	
	void on_SlowerButton_clicked();
	
	void on_PauseButton_clicked();
	
	void on_RunSimulationButton_clicked();
	
	void on_AddRouteButton_clicked();
	
	void on_ReloadButton_clicked();
  
    void on_PhaseNumberComboBox_currentTextChanged(const QString &arg1);

    void on_AssignedLanesListView_itemClicked(QListWidgetItem *item);

    void on_AddPhaseButton_clicked();

    void on_AddLightButton_clicked();

    void on_PhaseTimeSlider_sliderMoved(int position);

    void on_PhaseDelaySlider_sliderMoved(int position);

    void on_OrangeLightDelaySlider_sliderMoved(int position);

    void on_AssignLaneButton_clicked();

    void on_PhaseTimeComboBox_currentTextChanged(const QString &arg1);

    void on_PhaseTimeLineEdit_editingFinished();

    void on_PhaseDelayLineEdit_editingFinished();

    void on_OrangeLightDelayLineEdit_editingFinished();

    void on_ShowLaneBlockCheckBox_stateChanged(int arg1);

    void on_DrawTexturesCheckBox_stateChanged(int arg1);

    void on_FollowSelectedCarButton_stateChanged(int arg1);

    void on_AbortButton_clicked();

    void on_SaveSimButton_clicked();

    void on_LoadSimButton_clicked();

    void on_SimTable_clicked(const QModelIndex &index);

    void on_RunDemoButton_clicked();

    void on_DeleteSimButton_clicked();

private:
	
	Ui::MainWindow *ui;
	Engine *SimulatorEngine;

	SimModel * model_;
    int selected_row_;

	void reloadLaneList();
};

#endif // MAINWINDOW_H
