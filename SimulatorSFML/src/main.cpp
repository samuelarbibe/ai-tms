//
//  main.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include <iostream>
#include "Engine.hpp"

int main(int argc, const char * argv[]) {
    
    Engine * engine;
    
    engine = new Engine(1000, 1000, "Simulator");
    
    engine->Start();
    
    return EXIT_SUCCESS;
}
