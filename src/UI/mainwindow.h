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

private slots:
    void on_AddIntersectionButton_clicked();

    void on_AddConnectingRoadButton_clicked();

    void on_AddRoadButton_clicked();

private:
    Ui::MainWindow *ui;
    Engine * SimulatorEngine;
};

#endif // MAINWINDOW_H
