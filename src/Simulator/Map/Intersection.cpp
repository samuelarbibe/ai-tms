//
//  Intersection.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 23/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Intersection.hpp"

Intersection::Intersection(Vector2f position, int intersectionNumber, WeatherCondition weatherCondition)
{
    m_intersectionNumber = intersectionNumber;
    m_position = position;
    m_width = 0;
    m_height = 0;
    m_weatherCondition = weatherCondition;

    m_numberOfRoads = 0;
    
    this->setOrigin(m_width/2, m_height/2);
    this->setPosition(m_position);
    this->setOutlineColor(WhiteColor);
    this->setFillColor(LaneColor);
    this->setOutlineThickness(1.f);
    this->setSize(Vector2f(m_width, m_height));
};

Road * Intersection::AddRoad(int roadNumber, int connectionSide, float length)
{
    if(!roadNumber)
    {
        roadNumber = roadCount + 1;
    }
    
    m_roads.push_back(new Road(roadNumber, m_intersectionNumber, connectionSide ,GetPositionByConnectionSide(connectionSide), length, LANE_WIDTH, (connectionSide-1)*90));
            
    m_numberOfRoads++;
    roadCount++;
    
    if(DRAW_ADDED)std::cout << "Road " << roadNumber << " added" << endl;
    
    return m_roads[m_numberOfRoads-1];
}

Road * Intersection::AddConnectingRoad(int roadNumber, int connectionSide1, int connectionSide2, Intersection * connectedIntersection)
{
    if(!roadNumber)
    {
        roadNumber = roadCount + 1;
    }

    m_roads.push_back(new Road(roadNumber, this->m_intersectionNumber, connectedIntersection->m_intersectionNumber, connectionSide1, connectionSide2 ,
            this->GetPositionByConnectionSide(connectionSide1) ,connectedIntersection->GetPositionByConnectionSide(connectionSide2), LANE_WIDTH, (connectionSide1-1)*90));

    connectedIntersection->m_roads.push_back(m_roads[m_numberOfRoads]);
    connectedIntersection->m_numberOfRoads++;

    m_numberOfRoads++;
    roadCount++;

    if(DRAW_ADDED)std::cout << "Road " << roadNumber << " added" << endl;

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

    return nullptr;
}

Road * Intersection::GetRoad(int roadNumber)
{
    Road * temp;
    for (int i = 0 ;i < m_numberOfRoads; i++)
    {
        temp = m_roads[i];
        if((temp->GetRoadNumber()) == roadNumber)
        {
            return temp;
        }
    }

    return nullptr;
}

Road * Intersection::GetRoadByConnectionSide(int connectionSide)
{
    
    for (int i = 0 ;i < m_numberOfRoads; i++)
    {
        if(m_roads[i]->GetConnectionSide(0) == connectionSide)
        {
            return m_roads[i];
        }
        else if(m_roads[i]->GetIsConnecting() && m_roads[i]->GetConnectionSide(1) == connectionSide)
        {
            return m_roads[i];
        }
    }

    return nullptr;
}



Lane * Intersection::AddLane(int laneNumber, int roadNumber, bool isInRoadDirection)
{
    Road * temp = GetRoad(roadNumber);
    Lane * l;
    
    l = temp->AddLane(laneNumber, isInRoadDirection);
    
    // update intersection dimensions
    Road * r1 = GetRoadByConnectionSide(1);
    Road * r2 = GetRoadByConnectionSide(2);
    Road * r3 = GetRoadByConnectionSide(3);
    Road * r4 = GetRoadByConnectionSide(4);
    
    
    m_width = r1->GetWidth();
    if(r2 != nullptr)m_height = r2->GetWidth();
    
    if(r3 != nullptr && r3->GetWidth() > m_width)
    {
        m_width = r3->GetWidth();
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
        case UP:
            return Vector2f(m_position.x, m_position.y - m_height/2);
            
            break;
        case RIGHT:
            return Vector2f(m_position.x + m_width/2, m_position.y);
            
            break;
        case DOWN:
            return Vector2f(m_position.x, m_position.y + m_height/2);
            
            break;
        case LEFT:
            return Vector2f(m_position.x - m_width/2, m_position.y);
            
            break;
        default:
            return m_position;
            break;
    }
}

void Intersection::reAssignIntersectionPosition(Vector2f position)
{
    m_position = position;
    this->setPosition(position);
    reAssignRoadPositions();
}

void Intersection::reAssignRoadPositions()
{
    for (int i = 0 ; i < m_numberOfRoads; i++)
    {
        // if road starts from this intersection, adjust start position
        if(m_roads[i]->GetIntersectionNumber(0) == this->m_intersectionNumber)
        {
            m_roads[i]->UpdateStartPosition(GetPositionByConnectionSide(m_roads[i]->GetConnectionSide(0)));
        }
        else
        {
            if(m_roads[i]->GetIsConnecting() && m_roads[i]->GetIntersectionNumber(1) == this->m_intersectionNumber) {
                // if road connects this intersection to another
                m_roads[i]->UpdateEndPosition(GetPositionByConnectionSide(m_roads[i]->GetConnectionSide(1)));
            }
        }
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

    for (int i = 0 ; i < m_numberOfRoads; i++)
    {
        m_roads[i]->Draw(window);
    }
}


