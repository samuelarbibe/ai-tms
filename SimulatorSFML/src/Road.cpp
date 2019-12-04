//
//  Road.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Road.hpp"

Road::Road(){}
Road::~Road(){}

void Road::Init(int roadNumber, Vector2f startPosition, float length, float laneWidth, float direction)
{
    m_roadNumber = roadNumber;
    m_startPosition = startPosition;
    m_length = length;
    m_direction = direction;
    m_laneWidth = laneWidth;
    m_numberOfLanes = 0;
    
    // calculate end position:
    Vector2f lengthVec;
    
    Transform t;
    t.rotate(direction);
    
    lengthVec = t.transformPoint(Vector2f(0.f, -1.f)) * length;
    
    m_endPosition = lengthVec + m_startPosition;
    
    // init rectangle shape
    this->setOrigin(m_width/2, 0.f);
    this->setPosition(m_startPosition);
    this->setOutlineColor(WhiteColor);
    this->setOutlineThickness(10.f);
    this->setRotation(m_direction + 180);
    this->setSize(Vector2f(m_width, m_length));
        
    //cout << startPosition.x << "," << startPosition.y << endl;
    //cout << m_endPosition.x << "," << m_endPosition.y << endl;
}

void Road::AddLane(int laneNumber, bool isInRoadDirection)
{
    Lane temp;
    
    if(!laneNumber)
    {
        laneNumber = ++laneCount;
    }
    
    if (isInRoadDirection) {
        temp.Init(m_roadNumber, (m_numberOfLanes + 1), m_startPosition, m_laneWidth, m_length, m_direction);
    }
    else
    {
        temp.Init(m_roadNumber, (m_numberOfLanes + 1), m_endPosition, m_laneWidth, m_length, (m_direction + 180.f));
    }
    
    m_lanes.push_back(temp);
    m_numberOfLanes++;
    laneCount++;
    
    reAssignLanePositions();
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
    
    list<Lane>::iterator iterator = m_lanes.begin();
    
    for (int i = 0; i < m_numberOfLanes; i++) {
        
        Transform z, y;
        
        z.scale(i, i);
        
        //if lane is in road direction
        if(iterator->GetDirection() == m_direction)
        {
            // send calculated starting point
            iterator->Init(m_roadNumber, iterator->GetLaneNumber(), firstLanePoint + z.transformPoint(laneDifference), m_laneWidth, m_length, m_direction);
        }
        else
        {
            // send starting point + length vector
            y.rotate(m_direction);
            lengthVec = y.transformPoint(Vector2f(0.f, -1.f)) * m_length;
            
            iterator->Init(m_roadNumber, iterator->GetLaneNumber(), firstLanePoint + z.transformPoint(laneDifference) + lengthVec,
                           m_laneWidth, m_length, (m_direction + 180.f));
        }
                
        iterator++;
    }
}

void Road::Draw(RenderWindow * window)
{
    for (Lane i : m_lanes) {
        i.Draw(window);
    }
}
