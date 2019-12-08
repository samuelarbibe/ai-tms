//
//  Road.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Road_hpp
#define Road_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "Lane.hpp"

using namespace std;

static int roadCount = 0;

class Road : public RectangleShape
{
    
public:
    
    Road(){};
    Road(int roadNumber, int intersectionNumber, int connectionSide, Vector2f startPosition, float length, float laneWidth, float direction);
    ~Road();
    
    
    Lane * AddLane(int laneNumber, bool isInRoadDirection);
    Lane * GetLane(int laneNumber);

    float  GetWidth(){return m_width;};
    
    int    GetRoadNumber(){return m_roadNumber;};
    int    GetIntersectionNumber(){return m_intersectionNumber;};
    int    GetConnectionSide(){return m_connectionSide;};
    
    void   reAssignLanePositions();
    void   UpdateStartPosition(Vector2f position);
    
    void   Update(float elapsedTime);
    void   Draw(RenderWindow * window);
    
private:
    
    int        m_roadNumber;
    int        m_numberOfLanes;
    int        m_intersectionNumber;
    int        m_connectionSide;
    
    Vector2f   m_startPosition;
    Vector2f   m_endPosition;
    
    float      m_direction;
    float      m_laneWidth;
    float      m_length;
    float      m_width;
    
    vector<Lane*> m_lanes;
        
};



#endif /* Road_hpp */
