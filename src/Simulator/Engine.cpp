//
//  Engine.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Engine.hpp"



/// set up the map according to the selected presets
void Engine::OnInit()
{
    cout << "Setting Up Map..." << endl;
    map = new Map(0, Vector2i(this->width()/2, this->height()/2), Settings::DefaultMapWidth, Settings::DefaultMapWidth);

    cout << "Setting Up Camera..." << endl;
    m_viewPosition = Vector2f(0,0);
    m_viewTempPosition = Vector2f(0,0);
    SetView();
    SetMinimap(Settings::MinimapSize, Settings::MinimapMargin);
    this->setView(m_view);

    cout << "Setting up snap grid..." << endl;
    BuildGrid(Settings::GridRows, Settings::GridColumns);

    cout << "Setting up max speeds..." << endl;
    Settings::MaxSpeeds[VehicleTypeOptions::CAR] = Settings::ConvertVelocity(VelocityUnits::KMH, VelocityUnits::PXS, 100.f);
    Settings::MaxSpeeds[VehicleTypeOptions::TRUCK] = Settings::ConvertVelocity(VelocityUnits::KMH, VelocityUnits::PXS, 90.f);
    Settings::MaxSpeeds[VehicleTypeOptions::MOTORCYCLE] = Settings::ConvertVelocity(VelocityUnits::KMH, VelocityUnits::PXS, 110.f);

    Settings::MinDistanceFromNextCar = Settings::ConvertSize(M, PX, 5);
    Settings::MinDistanceFromStop = Settings::ConvertSize(M, PX, 2);

    map->AddIntersection(0, map->GetSize()/2.f);

    map->AddRoad(0, 1, DOWN, Settings::DefaultLaneLength);
    map->AddRoad(0, 1, RIGHT, Settings::DefaultLaneLength);

    map->AddLane(0, 1, false);
    map->AddLane(0, 2, true);

    map->GetLane(1)->SetIsBlocked(true);
}

void Engine::SetView()
{
    // view setup
    m_view.reset(sf::FloatRect(m_viewTempPosition.x,m_viewTempPosition.y, Settings::DefaultMapWidth,  Settings::DefaultMapHeight));
    m_view.zoom(Settings::Zoom);
    // update the shown area index rectangle
    updateShownArea();
}

void Engine::SetMinimap(float size, float margin)
{
    // minimap viewPort setup
    m_minimap.reset(sf::FloatRect(0, 0, Settings::DefaultMapWidth,  Settings::DefaultMapWidth));
    m_minimap.setSize(Vector2f(size, size));
    m_minimap.setViewport(sf::FloatRect(1.f - (size/this->width()) - (margin/this->height()), margin/this->height() , size/this->width(), size/this->height()));
    float zoomFactor = Settings::DefaultMapWidth / size;
    m_minimap.zoom(zoomFactor);
    float outlineThickness = 30;

    // background setup
    m_minimapBackground = RectangleShape(Vector2f(Settings::DefaultMapWidth - outlineThickness*2,
            Settings::DefaultMapWidth - outlineThickness*2));
    m_minimapBackground.setPosition(outlineThickness, outlineThickness);
    m_minimapBackground.setFillColor(Color(110, 110, 110, 220));
    m_minimapBackground.setOutlineColor(Color::Black);
    m_minimapBackground.setOutlineThickness(outlineThickness);

    m_shownArea = RectangleShape(Vector2f(0,0));
    m_shownArea.setOutlineColor(Color(255, 0, 0, 100));
    m_shownArea.setOutlineThickness(30.f);
    m_shownArea.setFillColor(Color::Transparent);
    updateShownArea();
}


void Engine::UpdateView(Vector2f posDelta, float newZoom)
{
    // m_viewPosition is position before dragging
    // m_viewTempPosition is the current view position
    m_viewTempPosition = m_viewPosition + posDelta;

    // TODO: enforce overflow
    /*
    Vector2f mapPosition = this->mapPixelToCoords(Vector2i(m_viewTempPosition), m_view);
    cout << m_viewTempPosition.x << ", " << m_viewTempPosition.y << endl;
    // enforce overflow blocking
    if(Settings::MapOverflow == false)
    {
        if(false)
        {
            m_viewTempPosition = m_viewPosition;
        }
    }
     */

    if(newZoom != 0)
    {
        Settings::Zoom = newZoom;
    }
    // set view
    SetView();
}

void Engine::updateShownArea()
{
    // calculate the actual position of the shown area
    Vector2f position = m_view.getCenter();

    // to calculate shown area dimensions,
    // we multiply the map size by the zoom
    // view_size = map_size * zoom
    Vector2f size = m_view.getSize();

    this->m_shownArea.setSize(size);
    this->m_shownArea.setOrigin(size/2.f);
    this->m_shownArea.setPosition(position);
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
    // convert it to units according to screen pixel to display ratio
    position *= float(Settings::SFMLRatio);
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
void Engine::input()
{
    // implementation of mouse dragging
    static bool dragging = false;
    static Vector2i startPos = Vector2i(0.0f, 0.0f);

    QPoint clickPoint = this->mapFromGlobal(QCursor::pos());
    Vector2i mousePos = Vector2i(clickPoint.x(), clickPoint.y());

    if (Mouse::isButtonPressed(sf::Mouse::Left) && this->getViewport(m_view).contains(mousePos))
    {
        if (!dragging)
            startPos = mousePos;
        dragging = true;
    }
    else
    {
        if(dragging)
        {
            m_viewPosition = m_viewTempPosition;
        }
        dragging = false;
    }

    if (dragging) {
        UpdateView(Vector2f(startPos.x - mousePos.x, startPos.y - mousePos.y));
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {

    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        queue<Lane*> * tempQueue = new queue<Lane*>();

        tempQueue->push(map->GetLane(1));
        tempQueue->push(map->GetLane(2));

        Vehicle::AddVehicle(tempQueue, this->map);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        //inter->GetLane(2)->SetIsBlocked(false);
    }
}

/// do the game cycle (input->update->draw)
void Engine::OnUpdate()
{
    input();
    update((myTimer.interval()/1000.f));
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

    if(Settings::DrawFps)cout << "FPS : " << 1.f/elapsedTime << endl;
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

    // Draw the shown area index
    this->draw(m_shownArea);
}
