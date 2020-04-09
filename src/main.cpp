//
//  main.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QFrame>
#include "ui/mainwindow.h"

int main(int argc, char **argv)
{
	/*
	vector<int> topology;

	topology.push_back(2); // input neurons : max lane density, max queue length
	topology.push_back(1); // output neurons : priority points

	Settings::NeuralNetwork = NeuralNet(topology);
*/
    QApplication Application(argc, argv);

    auto * main = new MainWindow();
    main->show();

    return QApplication::exec();
}

