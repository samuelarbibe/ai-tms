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

int main(int argc, char **argv) {
	srand((time(nullptr)));
	vector<unsigned> topology;

	// input neurons : max lane density, max queue length
	topology.push_back(2);
	// hidden neurons
	topology.push_back(3);
	// output neurons : priority points, phase time
	topology.push_back(2);

	Net::NeuralNetwork = Net(topology);

	QApplication Application(argc, argv);

	auto *main = new MainWindow();
	main->show();

	return QApplication::exec();
}

