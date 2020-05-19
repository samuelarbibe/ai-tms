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

	// input neurons
	topology.push_back(2);
	// hidden neurons
	topology.push_back(3);
	// output neurons
	topology.push_back(2);

	for(unsigned i = 0; i < Net::PopulationSize; i++)
	{
		Net::Generation.emplace_back(topology);
	}

	Net::CurrentNet = &(Net::Generation[Net::CurrentNetIndex]);

	QApplication Application(argc, argv);

	auto *main = new MainWindow();
	main->show();

	return QApplication::exec();
}
