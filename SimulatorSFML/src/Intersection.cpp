//
//  Intersection.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 23/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Intersection.hpp"

Intersection::Intersection(){};

Intersection::~Intersection(){};

void Intersection::Init(Vector2f position, int width, int height, int intersectioNumber)
{
    m_intersectionNumber = intersectioNumber;
    m_position = position;
    m_width = width;
    m_height = height;
    
    m_numberOfConnections = 0;
    
    this->setOrigin(m_width/2, m_height/2);
    this->setPosition(m_position);
    this->setOutlineColor(WhiteColor);
    this->setFillColor(LaneColor);
    this->setOutlineThickness(1.f);
    this->setSize(Vector2f(m_width, m_width));
}

void Intersection::AddRoadConnection(int roadNumber, int connectionSide, float length)
{
    RoadConnection temp;
    
    temp.road = new Road();
    
    Transform t;
    
    t.rotate((connectionSide-1)*90);
    Vector2f baseVec(0.f, -1.f);
    
    t.scale(m_height/2, m_width/2);
    
    if(roadNumber == 0)
    {
        roadNumber = roadCount + 1;
    }
    
    temp.connectionSide = connectionSide;
    temp.connectionPosition = t.transformPoint(baseVec);
    temp.road->Init(roadNumber, m_position + temp.connectionPosition, length, LANE_WIDTH, (connectionSide-1)*90);
        
    m_roadConnetions.push_back(temp);
    
    m_numberOfConnections++;
    roadCount++;
}

Road * Intersection::GetRoad(int roadNumber)
{
    list<RoadConnection>::iterator iterator = m_roadConnetions.begin();
    
    for (int i = 0; i < m_numberOfConnections; i++)
    {
        if(iterator->road->GetRoadNumber() == roadNumber)
        {
            return iterator->road;
        }
        iterator++;
    }
    
    cout << "error: road not found in intersection..." << endl;
    
    return nullptr;
}

Road * Intersection::GetRoadByConnectionSide(int connectionSide)
{
    list<RoadConnection>::iterator iterator = m_roadConnetions.begin();
    
    for (int i = 0; i < m_numberOfConnections; i++)
    {
        if(iterator->connectionSide == connectionSide)
        {
            return iterator->road;
        }
        iterator++;
    }
    
    cout << "error: intersection connection unused or unexisting" << endl;
    
    return nullptr;
}

/*
void Intersection::CreateMapRender(const char * fileDir)
{
    
    sf::Texture texture;

    texture.create(this->m_window->getSize().x, this->m_window->getSize().y);
    texture.update(*(this->m_window));
    
    // screenshot map and save it to file
    if (texture.copyToImage().saveToFile(fileDir))
    {
        std::cout << "new map render saved to " << fileDir << std::endl;
        this->m_renderDir = strdup(fileDir);
    }
    else
    {
        std::cout << "could not create new map render to" << fileDir << std::endl;
    }
     
}
*/
void Intersection::Draw(RenderWindow *window)
{
    (*window).draw(*this);
    
    for (RoadConnection r : m_roadConnetions) {
        r.road->Draw(window);
    }
}

