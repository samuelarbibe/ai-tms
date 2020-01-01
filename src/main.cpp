//
//  main.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include <iostream>
#include <QApplication>
#include <QFrame>
#include "Simulator/Engine.hpp"

int main(int argc, char **argv)
{
    QApplication Application(argc, argv);

    // Create the main frame
    QFrame* MainFrame = new QFrame;
    MainFrame->setWindowTitle("Qt SFML");
    MainFrame->resize(1000, 1000);
    MainFrame->show();

    Engine * SimulatorEngine = new Engine(MainFrame, QPoint(0, 0), QSize(1000, 1000));
    SimulatorEngine->show();

    return Application.exec();
}

