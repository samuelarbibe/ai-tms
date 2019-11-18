//
//  main.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024

#include <iostream>
#include "Simulator.hpp"

int main(int argc, const char * argv[]) {
    
    Sim::Simulator(SCREEN_WIDTH, SCREEN_HEIGHT, "SFML");
    
    return EXIT_SUCCESS;
}
