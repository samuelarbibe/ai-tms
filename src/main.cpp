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
	srand((time(nullptr)));
	vector<unsigned> topology;

	topology.push_back(2); // input neurons : max lane density, max queue length
	topology.push_back(3); // input neurons : max lane density, max queue length
	topology.push_back(2); // output neurons : priority points

	Net::NeuralNetwork = Net(topology, Vector2f(Settings::DefaultMapWidth, Settings::DefaultMapHeight));

    QApplication Application(argc, argv);

    auto * main = new MainWindow();
    main->show();

    return QApplication::exec();
}

