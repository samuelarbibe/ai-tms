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
#include "UI/mainwindow.h"

int main(int argc, char **argv)
{
    QApplication Application(argc, argv);

    MainWindow * main = new MainWindow;
    main->show();

    return Application.exec();
}

