//
//  Engine.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Engine.hpp"

Engine::Engine(){}

void Engine::Init(int windowWidth, int windowHeight, const char * windowName){
         
    m_window.create(VideoMode(windowWidth, windowHeight), windowName);
    
    m_window.setActive();
      
    this->car.Init(50, 0, 500, 500, "Cars/car_image2.png");
    
    this->inter.Init(10, 10, m_window.getSize().x, m_window.getSize().y, "Maps/map.txt", &m_window);
}

void Engine::Start(){
    
    Clock clock;
    int frameCount = 0;
    
    while (m_window.isOpen()) {
        frameCount++;
        
        Time dt = clock.restart();
        
        float dtInSeconds = dt.asSeconds();
        
        if(frameCount % 2000 == 0)std::cout << 1/dtInSeconds << std::endl;
        
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed){
                m_window.close();
            }
        }
        
        input();
        update(dtInSeconds);
        draw();
        
    }
}

void Engine::input(){
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        car.Move(0.f, 2.f);
        
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        car.Move(0.f, -2.f);
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        car.Move(2.f, 0.f);
    }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        car.Move(-2.f, 0.f);
    }
     
};

void Engine::update(float dtInSeconds){
    
    car.Update(dtInSeconds);
}

void Engine::draw()
{
    // Clean out the last frame
    m_window.clear(Color::White);
 
    // Draw the objects
    this->inter.Draw(&m_window);
    this->car.Draw(&m_window);
     
    // Show everything that has been drawn
    m_window.display();
}
