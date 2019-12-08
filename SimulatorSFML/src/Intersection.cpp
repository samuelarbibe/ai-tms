//
//  Intersection.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 23/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Intersection.hpp"

Intersection::Intersection(Vector2f position, int width, int height, int intersectioNumber)
{
    m_intersectionNumber = intersectioNumber;
    m_position = position;
    m_width = width;
    m_height = height;
    //m_roads();
    
    m_numberOfRoads = 0;
    
    this->setOrigin(m_width/2, m_height/2);
    this->setPosition(m_position);
    this->setOutlineColor(WhiteColor);
    this->setFillColor(LaneColor);
    this->setOutlineThickness(1.f);
    this->setSize(Vector2f(m_width, m_height));
};

Intersection::~Intersection(){};


Road * Intersection::AddRoad(int roadNumber, int connectionSide, float length)
{
    
    if(!roadNumber)
    {
        roadNumber = roadCount + 1;
    }
    
    m_roads.push_back(new Road(roadNumber, m_intersectionNumber, connectionSide ,GetPositionByConnectionSide(connectionSide), length, LANE_WIDTH, (connectionSide-1)*90));
            
    m_numberOfRoads++;
    roadCount++;
    
    std::cout << "Road " << roadNumber << " added" << endl;
    
    return m_roads[m_numberOfRoads-1];
}

Lane * Intersection::GetLane(int laneNumber)
{
    Lane * temp;
    
    for (int i = 0; i < m_numberOfRoads; i++)
    {
        if ((temp = m_roads[i]->GetLane(laneNumber)) != nullptr)
        {
            return temp;
        }
    }
    
    cout << "error : lane not found in intersection..." << endl;
    
    return nullptr;
}

Road * Intersection::GetRoad(int roadNumber)
{
    for (int i = 0 ;i < m_numberOfRoads; i++)
    {
        if(m_roads[i]->GetRoadNumber() == roadNumber)
        {
            return m_roads[i];
        }
    }
    
    cout << "error: road not found in intersection..." << endl;
    
    return nullptr;
}

Road * Intersection::GetRoadByConnectionSide(int connectionSide)
{
    
    for (int i = 0 ;i < m_numberOfRoads; i++)
    {
        if(m_roads[i]->GetConnectionSide() == connectionSide)
        {
            return m_roads[i];
        }
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

Vehicle * Intersection::AddVehicle(int laneNumber, int destinationLaneNumber)
{
    Lane * origin = GetLane(laneNumber);
    Lane * dest = GetLane(destinationLaneNumber);
    
    return origin->AddVehicle(dest);
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
    for (int i = 0 ; i < m_numberOfRoads; i++)
    {
        m_roads[i]->UpdateStartPosition(GetPositionByConnectionSide(m_roads[i]->GetConnectionSide()));
    }
}

void Intersection::Update(float elapsedTime)
{
    for(Road * r : m_roads)
    {
        r->Update(elapsedTime);
    }
}

void Intersection::Draw(RenderWindow *window)
{
    (*window).draw(*this);
    
    for (int i = 0 ; i < m_numberOfRoads; i++) {
        m_roads[i]->Draw(window);
    }
}

