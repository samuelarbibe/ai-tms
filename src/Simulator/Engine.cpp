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
    cout << "Setting Up Camera..." << endl;
    SetView(Settings::Zoom, Vector2f(0,0));

    map = new Map(0, Vector2i(this->width()/2, this->height()/2), Settings::DefaultMapWidth, Settings::DefaultMapWidth);

    cout << "Setting up snap grid..." << endl;
    BuildGrid(Settings::GridRows, Settings::GridColumns);

    cout << "Setting up max speeds..." << endl;
    Vehicle::SetMaxSpeed(VehicleTypeOptions::CAR, 100.f, 1.5f);
    Vehicle::SetMaxSpeed(VehicleTypeOptions::TRUCK, 80.f, 1.f);
}

void Engine::SetView(float zoom, Vector2f pos)
{
    // normal view setup
    m_view.reset(sf::FloatRect(pos.x,pos.y, Settings::DefaultMapWidth,  Settings::DefaultMapHeight));
    m_view.zoom(zoom);

    // minimap viewPort setup
    float size = Settings::MinimapSize;
    float margin = Settings::MinimapMargin;
    m_minimap.reset(sf::FloatRect(0, 0, Settings::DefaultMapWidth,  Settings::DefaultMapWidth));
    m_minimap.setSize(Vector2f(size, size));
    m_minimap.setViewport(sf::FloatRect(1.f - (size/this->width()) - (margin/this->height()), margin/this->height() , size/this->width(), size/this->height()));
    float zoomFactor = Settings::DefaultMapWidth / size;
    m_minimap.zoom(zoomFactor);
    float outlineThickness = 30;

    // background setup
    m_minimapBackground = RectangleShape(Vector2f(Settings::DefaultMapWidth - outlineThickness*2, Settings::DefaultMapWidth - outlineThickness*2));
    m_minimapBackground.setPosition(outlineThickness, outlineThickness);
    m_minimapBackground.setFillColor(Color(110, 110, 110, 220));
    m_minimapBackground.setOutlineColor(Color::Black);
    m_minimapBackground.setOutlineThickness(outlineThickness);

    this->setView(m_view);
}

void Engine::BuildGrid(int rows, int cols)
{
    m_snapGrid.Lines.clear(); // clear the old lines list
    m_snapGrid.Rows = rows;
    m_snapGrid.Columns = cols;
    m_snapGrid.ColumnWidth = map->GetSize().x/m_snapGrid.Columns;
    m_snapGrid.RowHeight = map->GetSize().y/m_snapGrid.Rows;

    // create all vertical lines of the grid
    for (int i = 1; i < m_snapGrid.Columns; i++)
    {
        Vertex * line = new Vertex[2];

        line[0] = sf::Vertex(sf::Vector2f(i * m_snapGrid.ColumnWidth, 0));
        line[1] = sf::Vertex(sf::Vector2f(i * m_snapGrid.ColumnWidth, map->GetSize().y));

        m_snapGrid.Lines.push_back(line);
    }
    // create all horizontal lines of snap grid
    for (int i = 1; i < m_snapGrid.Rows; i++)
    {
        Vertex * line = new Vertex[2];

        line[0] = sf::Vertex(sf::Vector2f(0, i * m_snapGrid.RowHeight));
        line[1] = sf::Vertex(sf::Vector2f(map->GetSize().x, i * m_snapGrid.RowHeight));

        m_snapGrid.Lines.push_back(line);
    }
}

Vector2f Engine::DrawPoint(Vector2f position)
{
     position = position * float(Settings::SFMLRatio);
    // convert it to world coordinates
    position = this->mapPixelToCoords(Vector2i(position), m_view);

    Vector2f temp;
    if(m_snapToGrid)
    {
        temp = GetSnappedPoint(position);
    }
    else
    {
        temp = position;
    }

    m_clickPoint = CircleShape(m_snapGrid.ColumnWidth/3.f);
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
    if(this->m_showGrid)
    {
        for (Vertex *l : this->m_snapGrid.Lines) {
            this->draw(l, 2, Lines);
        }
    }

    // draw the minimap
    this->setView(m_minimap); // switch to minimap for rendering
    DrawMinimap(); // render minimap
    this->setView(m_view); // switch back to main view
}

/// drawing the minimap is drawing everything but the vehicles and the grid, on a smaller scale
void Engine::DrawMinimap()
{
    // Draw the minimap's background
    this->draw(m_minimapBackground);

    // Draw the map
    this->map->Draw(this);

    // Draw the click index
    this->draw(this->m_clickPoint);
}
