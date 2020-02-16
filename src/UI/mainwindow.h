#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "../Simulator/Engine.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mouseDoubleClickEvent(QMouseEvent *event) override;


private slots:
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

private:
    Ui::MainWindow *ui;
    Engine * SimulatorEngine;
};

#endif // MAINWINDOW_H
