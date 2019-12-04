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
private:
    
    int m_roadNumber;
    int m_numberOfLanes;
    Vector2f m_startPosition;
    Vector2f m_endPosition;
    float m_direction;
    float m_laneWidth;
    float m_width;
    float m_length;
    list<Lane> m_lanes;
        
public:
    Road();
    ~Road();
    
    void Init(int roadNumber, Vector2f startPosition, float length, float laneWidth, float direction);
    
    void AddLane(int laneNumber, bool isInRoadDirection);
    void RemoveLane(int laneNumber);
    
    void reAssignLanePositions();
    
    int GetRoadNumber(){return m_roadNumber;};
    
    void Draw(RenderWindow * window);
};

#endif /* Road_hpp */
