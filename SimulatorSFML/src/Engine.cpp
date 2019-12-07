//
//  Engine.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Engine.hpp"


Engine::Engine(int windowWidth, int windowHeight, const char * windowName)
{
    m_window.create(VideoMode(windowWidth, windowHeight), windowName);
    
    m_window.setFramerateLimit(60);
    
    m_window.setActive();
    
    inter = new Intersection(Vector2f(windowWidth/2,windowHeight/2), 0, 0, 1);
    
    // add roads
    inter->AddRoad(0, 1, 400);
    inter->AddRoad(0, 2, 400);
    inter->AddRoad(0, 3, 400);
    inter->AddRoad(0, 4, 400);
    
    inter->AddLane(1, 1, false);
    inter->AddLane(2, 1, true);
    inter->AddLane(3, 2, false);
    inter->AddLane(4, 2, true);
    inter->AddLane(5, 3, false);
    inter->AddLane(6, 3, true);
    inter->AddLane(7, 4, false);
    inter->AddLane(8, 4, true);
    
    car = new MovableObject(50, 45, Vector2f(0.f, 0.f), "Cars/car_image2.png", 1, 3);
};

void Engine::Start(){
    
    Clock clock;
    int frameCount = 0;
    
    while (m_window.isOpen()) {
        
        // count elapsed frames for FPS calculation
        frameCount++;
        
        // the time it takes to do 1 frame itiration
        Time dt = clock.restart();
        
        float dtInSeconds = dt.asSeconds();
        
        // print out FPS
        if(frameCount % 20 == 0) std::cout << 1/dtInSeconds << std::endl;
        
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)){
                m_window.close();
            }
        }
        
        input();
        update(dtInSeconds);
        draw();
        
    }
}

void Engine::input(){
    
}

void Engine::update(float dtInSeconds){
    
    car->Update(dtInSeconds);
}

void Engine::draw()
{
    // Clean out the last frame
    m_window.clear(BackgroundColor);
 
    // Draw the objects
    this->inter->Draw(&m_window);
    this->car->Draw(&m_window);
     
    // Show everything that has been drawn
    m_window.display();
}
