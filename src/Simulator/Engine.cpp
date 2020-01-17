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

/// set up the map according to the selected presets
void Engine::OnInit()
{
    m_snapToGrid = true;

    cout << "Setting up snap grid..." << endl;
    m_snapGrid = Grid();
    m_snapGrid.Rows = GRID_COLOUMNS;
    m_snapGrid.Coloumns = GRID_ROWS;
    m_snapGrid.ColoumnWidth = this->width()/m_snapGrid.Coloumns;
    m_snapGrid.RowHeight = this->height()/m_snapGrid.Rows;

    cout << "Setting up max speeds..." << endl;
    Vehicle::SetMaxSpeed(VehicleTypeOptions::CAR, 100.f, 1.5f);
    Vehicle::SetMaxSpeed(VehicleTypeOptions::TRUCK, 80.f, 1.f);

    cout << "Setting up weather conditions..." << endl;
    //Vehicle::SetWeatherCondition(WeatherCondition::DRY);

    map = new Map(0, Vector2i(this->width()/2, this->height()/2), this->width(), this->height());

}

Vector2i Engine::DrawPoint(Vector2i position)
{
    Vector2i temp;
    if(m_snapToGrid)
    {
        temp = GetSnappedPoint(position);
    }
    else
    {
        temp = position;
    }

    m_clickPoint = CircleShape(5.f);
    m_clickPoint.setOrigin(m_clickPoint.getRadius(), m_clickPoint.getRadius());
    m_clickPoint.setFillColor(Color::Red);
    m_clickPoint.setPosition(temp.x, temp.y);

    return temp;
}

Vector2i Engine::GetSnappedPoint(Vector2i point)
{
    int x, y;

    x = int(round(point.x / m_snapGrid.ColoumnWidth)) * m_snapGrid.ColoumnWidth;
    y = int(round(point.y / m_snapGrid.RowHeight)) * m_snapGrid.RowHeight;

    return Vector2i(x,y);
}

/// get use input, and make changes accordingly
void Engine::input(){
    
    /*
    if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        queue<Lane*> * tempQueue = new queue<Lane*>();

        tempQueue->push(map->GetLane(5));
        tempQueue->push(map->GetLane(2));
        tempQueue->push(map->GetLane(10));

        Vehicle::AddVehicle(tempQueue, this->map);
    }
    */
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        //inter->GetLane(2)->SetIsBlocked(true);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        //inter->GetLane(2)->SetIsBlocked(false);
    }
}

/// do the game cycle (input->update->draw)
void Engine::OnUpdate()
{
    if(DRAW_FPS)cout << "FPS : " << 1.f/myTimer.interval() * 1000 << endl;
    input();
    update((myTimer.interval()/1000.f) / float(SCALE) * float(SPEED));
    OnDraw();
}

/// update all the engine's objects
void Engine::update(float elapsedTime)
{
    map->Update(elapsedTime);

    for (Vehicle *v : Vehicle::ActiveVehicles)
    {
        v->Update(elapsedTime);
    }

    //clear all cars to be deleted
    Vehicle::ClearVehicles();

    OnDraw();
}

/// render the engine's objects
void Engine::OnDraw()
{
    // Clean out the last frame
    clear(BackgroundColor);
 
    // Draw the map
    this->map->Draw(this);

    // Draw all vehicles
    for(Vehicle * v : Vehicle::ActiveVehicles)
    {
        v->Draw(this);
    }

    // Draw the click index
    this->draw(this->m_clickPoint);
}
