//
//  Road.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Road.hpp"

int Road::RoadCount = 0;

/// ctor for a normal road
Road::Road(int roadNumber, int intersectionNumber, int connectionSide,  Vector2f startPosition, float length, float direction)
{
    m_isConnecting       = false;
    m_roadNumber         = roadNumber;
    m_intersectionNumber[0] = intersectionNumber;
    m_intersectionNumber[1] = intersectionNumber;
    m_connectionSide[0]     = connectionSide;
    m_startPosition      = startPosition;
    m_length             = length;
    m_direction          = direction;
    m_numberOfLanes      = 0;
    m_width              = 0;
    m_currentVehicleCount = 0;
    m_totalVehicleCount = 0;

    // calculate end position:
    Vector2f lengthVec;
    
    Transform t;
    t.rotate(direction + 180);
    
    lengthVec = t.transformPoint(Vector2f(0.f, -1.f)) * length;
    m_endPosition = m_startPosition - lengthVec;
    
    // init rectangle shape
    this->setOrigin(m_width/2, 0.f);
    this->setPosition(m_startPosition);
    this->setOutlineColor(WhiteColor);
    this->setOutlineThickness(1.f);
    this->setFillColor(Color::Transparent);
    this->setRotation(m_direction + 180);
    this->setSize(Vector2f(m_width, m_length));

    m_dataBox = new DataBox(m_endPosition);
    m_dataBox->AddData("ID", m_roadNumber);
    m_dataBox->AddData("Count", 0);
}

/// ctor for a connecting road
Road::Road(int roadNumber, int intersectionNumber1, int intersectionNumber2, int connectionSide1, int connectionSide2, Vector2f conPosition1, Vector2f conPosition2, float direction)
{
    m_isConnecting          = true;
    m_roadNumber            = roadNumber;
    m_intersectionNumber[0] = intersectionNumber1;
    m_intersectionNumber[1] = intersectionNumber2;
    m_connectionSide[0]     = connectionSide1;
    m_connectionSide[1]     = connectionSide2;
    m_startPosition      = conPosition1;
    m_endPosition        = conPosition2;
    m_direction          = direction;
    m_numberOfLanes      = 0;
    m_width              = 0;
    m_length             = calculateDistance(m_startPosition, m_endPosition);

    // calculate end position:

    // init rectangle shape
    this->setOrigin(m_width/2, 0.f);
    this->setPosition(m_startPosition);
    this->setOutlineColor(WhiteColor);
    this->setOutlineThickness(1.f);
    this->setFillColor(Color::Transparent);
    this->setRotation(m_direction + 180);
    this->setSize(Vector2f(m_width, m_length));

    m_dataBox = new DataBox(m_endPosition);
    m_dataBox->AddData("ID", m_roadNumber);
    m_dataBox->AddData("Count", 0);
}

Road::~Road()
{
    for(Lane * lane : m_lanes)
    {
        delete lane;
    }
    if(Settings::DrawDelete)cout << "Road " << m_roadNumber << " deleted" << endl;

    delete m_dataBox;
}

/// add a lane to a road
Lane * Road::AddLane(int laneNumber, bool isInRoadDirection)
{
    if(!laneNumber)
    {
        laneNumber = Lane::LaneCount + 1;
    }
    
    if (isInRoadDirection) {
        m_lanes.push_back(new Lane(laneNumber, m_roadNumber, m_intersectionNumber[isInRoadDirection], m_startPosition, m_length, m_direction, isInRoadDirection));
    }
    else
    {
        m_lanes.push_back(new Lane(laneNumber, m_roadNumber, m_intersectionNumber[isInRoadDirection], m_endPosition, m_length, (m_direction + 180.f), isInRoadDirection));
    }
    
    m_numberOfLanes++;
    Lane::LaneCount++;
    
    // adjust road size
    m_width = m_numberOfLanes * Settings::LaneWidth;
    this->setSize(Vector2f(m_width, m_length));
    this->setOrigin(m_width/2, 0.f);

    if(Settings::DrawAdded)std::cout << "lane " << m_lanes[m_numberOfLanes - 1]->GetLaneNumber() << " added to road " << m_roadNumber << std::endl;
    
    return m_lanes[m_numberOfLanes - 1];
}

/// get lane by laneNumber
Lane * Road::GetLane(int laneNumber)
{
    for (int i = 0; i < m_numberOfLanes; i++)
    {
        if(m_lanes[i]->GetLaneNumber() == laneNumber)
        {
            return m_lanes[i];
        }
    }
    
    return nullptr;
}

/// re-locate all lanes in road to align with the road
void Road::reAssignLanePositions()
{
    Vector2f  firstLanePoint;
    Vector2f  firstLaneDifference;
    Vector2f  laneDifference;
    Vector2f  lengthVec;

    this->m_width = Settings::LaneWidth * m_numberOfLanes;
    this->setSize(Vector2f(m_width, m_length));

    Transform t, x;
    
    t.rotate(m_direction+90);
    laneDifference = t.transformPoint(0.f, -1.f) * Settings::LaneWidth;
    
    (m_numberOfLanes % 2) ?
    x.scale(m_numberOfLanes/2, m_numberOfLanes/2) :
    x.scale((m_numberOfLanes-1)/2 + 0.5, (m_numberOfLanes-1)/2 + 0.5);
        
    firstLaneDifference = x.transformPoint(laneDifference);
    
    firstLanePoint = m_startPosition - firstLaneDifference;
        
    for (int i = 0; i < m_numberOfLanes; i++) {
        
        Transform z, y;
        
        z.scale(i, i);
        
        int   tempLaneNumber = m_lanes[i]->GetLaneNumber();
        float tempLaneDirection = m_lanes[i]->GetDirection();
        
        delete m_lanes[i];
                
        //if lane is in road direction
        if(tempLaneDirection == m_direction)
        {
            // send calculated starting point
            m_lanes[i] = new Lane(tempLaneNumber, m_roadNumber, m_intersectionNumber[1], firstLanePoint + z.transformPoint(laneDifference), m_length, m_direction, true);
        }
        else
        {
            // send starting point + length vector
            y.rotate(m_direction);
            lengthVec = y.transformPoint(Vector2f(0.f, -1.f)) * m_length;
            
            m_lanes[i] = new Lane(tempLaneNumber, m_roadNumber, m_intersectionNumber[0], firstLanePoint + z.transformPoint(laneDifference) + lengthVec,
                           m_length, (m_direction + 180.f), false);
        }
    }
}

/// update the road's start position
void Road::UpdateStartPosition(Vector2f position)
{
    m_startPosition = position;
    m_length = calculateDistance(m_endPosition, m_startPosition);

    this->setOrigin(m_width/2, 0.f);
    this->setPosition(m_startPosition);
    this->setSize(Vector2f(m_width, m_length));
    
    reAssignLanePositions();
}

/// update the road's end position
void Road::UpdateEndPosition(Vector2f position)
{
    m_endPosition = position;
    m_length = calculateDistance(m_endPosition, m_startPosition);
    this->setSize(Vector2f(m_width, m_length));

    reAssignLanePositions();
}

/// calculate distance between 2 vectors
float Road::calculateDistance(Vector2f a, Vector2f b)
{
    float xDist = abs(a.x - b.x);
    float yDist = abs(a.y - b.y);

    return sqrt(xDist*xDist + yDist*yDist);
}

Lane * Road::CheckSelection(Vector2f position)
{
    // for each intersection in map
    for(Lane * lane : m_lanes)
    {
        // if selection found
        if(lane->getGlobalBounds().contains(position))
        {
            return lane;
        }
    }
    return nullptr;
}

/// update, for future use
void Road::Update(float elapsedTime)
{
    m_currentVehicleCount = 0;
    m_totalVehicleCount = 0;
    for(Lane * l : m_lanes)
    {
        l->Update(elapsedTime);
        m_currentVehicleCount += l->GetCurrentVehicleCount();
        m_totalVehicleCount += l->GetTotalVehicleCount();
    }

    m_dataBox->SetData("Count", m_currentVehicleCount);
}

bool Road::DeleteLane(int laneNumber)
{
    Lane * targetLane = this->GetLane(laneNumber);

    // if lane was found
    if(targetLane != nullptr)
    {
        // remove the targetLane from the list by iterator
        auto it = find(m_lanes.begin(), m_lanes.end(), targetLane);
        m_lanes.erase(it);
        m_numberOfLanes --;
        return true;
    }
    return false;
}

/// draw the road and al of its lanes
void Road::Draw(RenderWindow * window)
{
    window->draw(*this);
    for (int i = 0; i < m_numberOfLanes; i++) {
        m_lanes[i]->Draw(window);
    }
    if(Settings::DrawRoadDataBoxes) m_dataBox->Draw(window);
}

