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

    void on_UnitComboBox_currentIndexChanged(int index);

    void on_LaneWidthValueEdit_editingFinished();

    void on_ZoomSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    Engine * SimulatorEngine;
};

#endif // MAINWINDOW_H
