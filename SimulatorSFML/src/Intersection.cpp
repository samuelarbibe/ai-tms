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
    this->setSize(Vector2f(m_width, m_height));
}

RoadConnection * Intersection::AddRoadConnection(int roadNumber, int connectionSide, float length)
{
    RoadConnection temp;
    
    if(!roadNumber)
    {
        roadNumber = roadCount + 1;
    }
    
    temp.road = new Road();
    
    temp.connectionSide = connectionSide;
    
    temp.connectionPosition = GetPositionByConnectionSide(connectionSide);
    
    temp.road->Init(roadNumber, temp.connectionPosition, length, LANE_WIDTH, (connectionSide-1)*90);
    
    m_roadConnetions.push_back(temp);
    
    m_numberOfConnections++;
    roadCount++;
    
    return &(m_roadConnetions.back());
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

Lane * Intersection::AddLane(int laneNumber, int roadNumber, bool isInRoadDirection)
{
    Road * temp = GetRoad(roadNumber);
    Lane * l;
    
    l = temp->AddLane(laneNumber, isInRoadDirection);
    
    // update intersection dimentions
    Road * r1 = GetRoadByConnectionSide(1);
    Road * r2 = GetRoadByConnectionSide(2);
    Road * r3 = GetRoadByConnectionSide(3);
    Road * r4 = GetRoadByConnectionSide(4);
    
    
    // TODO: improve this shitty code
    m_width = r1->GetWidth();
    
    if(r3 != nullptr && r3->GetWidth() > m_width)
    {
        m_width = r3->GetWidth();
    }
    
    if(r2 != nullptr && r2->GetWidth() > m_height)
    {
        m_height = r2->GetWidth();
    }
    
    if(r4 != nullptr && r4->GetWidth() > m_height)
    {
        m_height = r4->GetWidth();
    }
    
    this->setSize(Vector2f(m_width, m_height));
    this->setOrigin(m_width/2, m_height/2);
    
    reAssignRoadPositions();
    
    return l;
}

Vector2f Intersection::GetPositionByConnectionSide(int connectionSide)
{
    switch (connectionSide) {
        case 1:
            return Vector2f(m_position.x, m_position.y - m_height/2);
            
            break;
        case 2:
            return Vector2f(m_position.x + m_width/2, m_position.y);
            
            break;
        case 3:
            return Vector2f(m_position.x, m_position.y + m_height/2);
            
            break;
        case 4:
            return Vector2f(m_position.x - m_width/2, m_position.y);
            
            break;
        default:
            return m_position;
            break;
    }
}

void Intersection::reAssignRoadPositions()
{
    for (RoadConnection r : m_roadConnetions)
    {
        if(r.road != nullptr)
        {
            r.road->UpdateStartPosition(GetPositionByConnectionSide(r.connectionSide));
        }
    }
}

void Intersection::Draw(RenderWindow *window)
{
    (*window).draw(*this);
    
    for (RoadConnection r : m_roadConnetions) {
        r.road->Draw(window);
    }
}

