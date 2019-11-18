//
//  Simulator.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 18/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Simulator.hpp"

namespace Sim {

    Simulator::Simulator(int width, int height, std::string title){
        
        m_data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);
        
        
        this->Run();
    }

    void Simulator::Run(){
        float newTime, frameTime, interpelation;
        
        float currentTime = this->m_clock.getElapsedTime().asSeconds(); // get the amount of time elapsed since clock started
        
        float accumulator = 0.0f;
        
        while (this->m_data->window.isOpen()) {
            this->m_data->logic.ProcessStateChanges();
            
            newTime = this->m_clock.getElapsedTime().asSeconds(); // get the amount of time elapsed since clock started AGAIN
            
            frameTime = newTime - currentTime; // calculate frame time;
            
            if(frameTime > 0.25f){ // limit frame time
                frameTime = 0.25f;
            }
            
            currentTime = newTime;
            accumulator += frameTime;
            
            while (accumulator >= dt) {
                this->m_data->logic.GetActiveState()->HandleInput();
                this->m_data->logic.GetActiveState()->Update(dt);
                
                accumulator -= dt;
            }
            
            interpelation = accumulator / dt;
            this->m_data->logic.GetActiveState()->Draw(interpelation);
        }
    }
}
