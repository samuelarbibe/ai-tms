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
    BuildGrid(Settings::GridRows, Settings::GridColumns);

    cout << "Setting up max Settings::Speeds..." << endl;
    Vehicle::SetMaxSpeed(VehicleTypeOptions::CAR, 100.f, 1.5f);
    Vehicle::SetMaxSpeed(VehicleTypeOptions::TRUCK, 80.f, 1.f);

    map = new Map(0, Vector2i(this->width()/2, this->height()/2), this->width(), this->height());

}

void Engine::BuildGrid(int rows, int cols)
{
    m_snapGrid.Lines.clear(); // clear the old lines list
    m_snapGrid.Rows = rows;
    m_snapGrid.Columns = cols;
    m_snapGrid.ColumnWidth = this->width()/m_snapGrid.Columns;
    m_snapGrid.RowHeight = this->height()/m_snapGrid.Rows;

    // create all vertical lines of the grid
    for (int i = 1; i < m_snapGrid.Columns; i++)
    {
        Vertex * line = new Vertex[2];

        line[0] = sf::Vertex(sf::Vector2f(i * m_snapGrid.ColumnWidth, 0));
        line[1] = sf::Vertex(sf::Vector2f(i * m_snapGrid.ColumnWidth, this->height()));

        m_snapGrid.Lines.push_back(line);
    }
    // create all horizontal lines of snap grid
    for (int i = 1; i < m_snapGrid.Rows; i++)
    {
        Vertex * line = new Vertex[2];

        line[0] = sf::Vertex(sf::Vector2f(0, i * m_snapGrid.RowHeight));
        line[1] = sf::Vertex(sf::Vector2f(this->width(), i * m_snapGrid.RowHeight));

        m_snapGrid.Lines.push_back(line);
    }
}

Vector2f Engine::DrawPoint(Vector2f position)
{
    Vector2f temp;
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

Vector2f Engine::GetSnappedPoint(Vector2f point)
{
    float x = 0, y = 0;

    if(int(point.x) % m_snapGrid.ColumnWidth > m_snapGrid.ColumnWidth/2)
    {
        x = int(ceil(point.x / m_snapGrid.ColumnWidth)) * m_snapGrid.ColumnWidth;
    }
    else
    {
        x = int(floor(point.x / m_snapGrid.ColumnWidth)) * m_snapGrid.ColumnWidth;
    }

    if(int(point.y) % m_snapGrid.RowHeight > m_snapGrid.RowHeight/2)
    {
        y = int(ceil(point.y / m_snapGrid.RowHeight)) * m_snapGrid.RowHeight;
    }
    else
    {
        y = int(floor(point.y / m_snapGrid.RowHeight)) * m_snapGrid.RowHeight;
    }

    return Vector2f(x,y);
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
    if(Settings::DrawFps)cout << "FPS : " << 1.f/myTimer.interval() * 1000 << endl;
    input();
    update((myTimer.interval()/1000.f) / float(Settings::Scale) * float(Settings::Speed));
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

    // Draw the grid
    if(this->m_showGrid == true)
    {
        for (Vertex *l : this->m_snapGrid.Lines) {
            this->draw(l, 2, Lines);
        }
    }
}
