#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

    void mousePressEvent(QMouseEvent *event) override;


private slots:
    void on_AddIntersectionButton_clicked();

    void on_AddConnectingRoadButton_clicked();

    void on_AddRoadButton_clicked();

    void on_AddLanePushButton_clicked();

    void on_SnapToGridCheckBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    Engine * SimulatorEngine;
};

#endif // MAINWINDOW_H
