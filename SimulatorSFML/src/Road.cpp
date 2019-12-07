//
//  Road.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Road.hpp"

Road::Road(int roadNumber, int intersectionNumber, int connectionSide,  Vector2f startPosition, float length, float laneWidth, float direction)
{
    m_roadNumber         = roadNumber;
    m_intersectionNumber = intersectionNumber;
    m_connectionSide     = connectionSide;
    m_startPosition      = startPosition;
    m_length             = length;
    m_direction          = direction;
    m_laneWidth          = laneWidth;
    m_numberOfLanes      = 0;
    m_width              = m_numberOfLanes * laneWidth;
    
    // calculate end position:
    Vector2f lengthVec;
    
    Transform t;
    t.rotate(direction);
    
    lengthVec     = t.transformPoint(Vector2f(0.f, -1.f)) * length;
    
    m_endPosition = lengthVec + m_startPosition;
    
    // init rectangle shape
    this->setOrigin(m_width/2, 0.f);
    this->setPosition(m_startPosition);
    this->setOutlineColor(WhiteColor);
    this->setOutlineThickness(1.f);
    this->setFillColor(Color::Transparent);
    this->setRotation(m_direction + 180);
    this->setSize(Vector2f(m_width, m_length));
        

}

Road::~Road(){}

Lane * Road::AddLane(int laneNumber, bool isInRoadDirection)
{
    if(!laneNumber)
    {
        laneNumber = laneCount + 1;
    }
    
    if (isInRoadDirection) {
        m_lanes.push_back(Lane(laneNumber, m_roadNumber, m_startPosition, m_laneWidth, m_length, m_direction));
    }
    else
    {
        m_lanes.push_back(Lane(laneNumber, m_roadNumber, m_endPosition, m_laneWidth, m_length, (m_direction + 180.f)));
    }
    
    m_numberOfLanes++;
    laneCount++;
    
    // adjust road size
    m_width = m_numberOfLanes * m_laneWidth;
    this->setSize(Vector2f(m_width, m_length));
    this->setOrigin(m_width/2, 0.f);

    
    std::cout << "lane " << m_lanes[m_numberOfLanes - 1].GetLaneNumber() << " added to road " << m_roadNumber << std::endl;
    
    return &(m_lanes[m_numberOfLanes - 1]);
}

Lane * Road::GetLane(int laneNumber)
{
    for (int i = 0; i < m_numberOfLanes; i++)
    {
        if(m_lanes[i].GetLaneNumber() == laneNumber)
        {
            return &(m_lanes[i]);
        }
    }
    
    return nullptr;
}

void Road::reAssignLanePositions()
{
    
    Vector2f  firstLanePoint;
    Vector2f  firstLaneDifference;
    Vector2f  laneDifference;
    Vector2f  lengthVec;
    
    Transform t, x;
    
    t.rotate(m_direction+90);
    laneDifference = t.transformPoint(0.f, -1.f) * m_laneWidth;
    
    (m_numberOfLanes % 2) ?
    x.scale(m_numberOfLanes/2, m_numberOfLanes/2) :
    x.scale((m_numberOfLanes-1)/2 + 0.5, (m_numberOfLanes-1)/2 + 0.5);
        
    firstLaneDifference = x.transformPoint(laneDifference);
    
    firstLanePoint = m_startPosition - firstLaneDifference;
        
    for (int i = 0; i < m_numberOfLanes; i++) {
        
        Transform z, y;
        
        z.scale(i, i);
        
        int   tempLaneNumber = m_lanes[i].GetLaneNumber();
        float tempLaneDirection = m_lanes[i].GetDirection();
                
        //if lane is in road direction
        if(tempLaneDirection == m_direction)
        {
            // send calculated starting point
            m_lanes[i] = Lane(tempLaneNumber, m_roadNumber, firstLanePoint + z.transformPoint(laneDifference), m_laneWidth, m_length, m_direction);
        }
        else
        {
            // send starting point + length vector
            y.rotate(m_direction);
            lengthVec = y.transformPoint(Vector2f(0.f, -1.f)) * m_length;
            
            m_lanes[i] = Lane(tempLaneNumber, m_roadNumber, firstLanePoint + z.transformPoint(laneDifference) + lengthVec,
                           m_laneWidth, m_length, (m_direction + 180.f));
        }
    }
}

void Road::UpdateStartPosition(Vector2f position)
{
    m_startPosition = position;
    this->setOrigin(m_width/2, 0.f);
    this->setPosition(m_startPosition);
    
    reAssignLanePositions();
}

void Road::Draw(RenderWindow * window)
{
    for (int i = 0; i < m_numberOfLanes; i++) {
        m_lanes[i].Draw(window);
    }
    window->draw(*this);
}

