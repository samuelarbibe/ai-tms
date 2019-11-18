//
//  Simulator.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Simulator_hpp
#define Simulator_hpp

#include <stdio.h>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "inputManager.hpp"
#include "assetManager.hpp"
#include "stateLogic.hpp"


namespace Sim {

    struct SimData{ // struct containin all the needed data
        stateLogic logic;
        sf::RenderWindow window;
        assetManager assets;
        inputManager input;
    };

    typedef std::shared_ptr<SimData> SimDataRefrence; // a shared pointer to the data

    class Simulator{
        
    public:
        
        Simulator(int width, int height, std::string title);
        ~Simulator(){}
    
    private:
        
        const float dt = 1.0f / 60.0f;
        sf::Clock m_clock;
        
        SimDataRefrence m_data = std::make_shared<SimData>();
        
        void Run();
    };
}

#endif /* Simulator_hpp */
