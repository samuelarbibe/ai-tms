//
//  Engine.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Engine.hpp"

int Vehicle::VehicleCount = 0;
list<Vehicle*> Vehicle::ActiveVehicles;

Engine::Engine(int windowWidth, int windowHeight, const char * windowName)
{
    m_window.create(VideoMode(windowWidth, windowHeight), windowName);
    
    m_window.setFramerateLimit(1000);
    
    //FloatRect area(0, 0, resolution.x, resolution.y);
    m_window.setView(View(FloatRect(0, 0, 1000, 1000)));
    
    m_window.setActive();
            
    Vehicle::SetMaxSpeed(VehicleTypeOptions::CAR, 20000.f);
    Vehicle::SetMaxSpeed(VehicleTypeOptions::TRUCK, 20000.f);

    inter = new Intersection(Vector2f(windowWidth/2,windowHeight/2), 0, 0, 1);
    
    // add roads
    inter->AddRoad(0, 1, 400);
    inter->AddRoad(0, 2, 400);
    inter->AddRoad(0, 3, 400);
    inter->AddRoad(0, 4, 400);
    
    inter->AddLane(0, 1, false);
    inter->AddLane(0, 1, false);
    inter->AddLane(0, 1, true);
    inter->AddLane(0, 1, true);
    
    inter->AddLane(0, 2, false);
    inter->AddLane(0, 2, false);
    inter->AddLane(0, 2, true);
    inter->AddLane(0, 2, true);
    
    inter->AddLane(0, 3, false);
    inter->AddLane(0, 3, false);
    inter->AddLane(0, 3, true);
    inter->AddLane(0, 3, true);
    
    inter->AddLane(0, 4, false);
    inter->AddLane(0, 4, false);
    inter->AddLane(0, 4, true);
    inter->AddLane(0, 4, true);

    for (int i = 0; i < 10000; ++i)
    {
        Vehicle::AddVehicle(inter->GetLane(2), inter->GetLane(7), inter, VehicleTypeOptions::CAR);
    }

}

void Engine::Start(){
    
    Clock clock;
    int frameCount = 0;
    
    while (m_window.isOpen()) {
        
        // count elapsed frames for FPS calculation
        frameCount++;
        
        // the time it takes to do 1 frame iteration
        Time dt = clock.restart();
        
        float dtInSeconds = dt.asSeconds();
        
        // print out FPS
        if(frameCount % 20 == 0) std::cout << 1/dtInSeconds << std::endl;
        
        sf::Event event{};
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
    
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        //Vehicle::AddVehicle(inter->GetLane(2), inter->GetLane(7), inter, VehicleTypeOptions::CAR);
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        //Vehicle::AddVehicle(inter->GetLane(2), inter->GetLane(7), inter, VehicleTypeOptions::CAR);
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        inter->GetLane(2)->SetIsBlocked(true);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        inter->GetLane(2)->SetIsBlocked(false);
    }
}

void Engine::update(float dtInSeconds){
    
    inter->Update(dtInSeconds);

    for (Vehicle *v : Vehicle::ActiveVehicles)
    {
        v->Update(dtInSeconds);
    }
    
    //clear all cars to be deleted
    Vehicle::ClearVehicles();
}

void Engine::draw()
{
    // Clean out the last frame
    m_window.clear(BackgroundColor);
 
    // Draw the objects
    this->inter->Draw(&m_window);
    
    for(Vehicle * v : Vehicle::ActiveVehicles)
    {
        v->Draw(&m_window);
    }
     
    // Show everything that has been drawn
    m_window.display();
}
