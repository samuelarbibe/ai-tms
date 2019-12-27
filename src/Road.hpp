//
//  Road.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Road_hpp
#define Road_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "Lane.hpp"
#include "Flags.hpp"

using namespace std;

static int roadCount = 0;

class Road : public RectangleShape
{
    
public:
    
    Road(int roadNumber, int intersectionNumber, int connectionSide, Vector2f startPosition, float length, float laneWidth, float direction);
    Road(int roadNumber, int intersectionNumber1, int intersectionNumber2, int connectionSide1, int connectionSide2, Vector2f conPosition1, Vector2f conPosition2, float laneWidth, float direction);
    ~Road(){if(DRAW_DELETE)cout << "Road " << m_roadNumber << "deleted" << endl;};
    
    
    Lane * AddLane(int laneNumber, bool isInRoadDirection);
    Lane * GetLane(int laneNumber);

    // ge
    float  GetWidth(){return m_width;};
    int    GetRoadNumber(){return m_roadNumber;};
    int    GetIntersectionNumber(int index){return m_intersectionNumber[index];};
    int    GetConnectionSide(int index){return m_connectionSide[index];};
    bool   GetIsConnecting(){return m_isConnecting;};
    
    void   reAssignLanePositions();
    void   UpdateStartPosition(Vector2f position);
    void   UpdateEndPosition(Vector2f position);

    static float calculateDistance(Vector2f a, Vector2f b);

    void   Update(float elapsedTime);
    void   Draw(RenderWindow * window);
    
private:

    bool       m_isConnecting;
    int        m_roadNumber;
    int        m_numberOfLanes;
    int        m_intersectionNumber[2];
    int        m_connectionSide[2];
    
    Vector2f   m_startPosition;
    Vector2f   m_endPosition;
    
    float      m_direction;
    float      m_laneWidth;
    float      m_length;
    float      m_width;
    
    vector<Lane*> m_lanes;
        
};



#endif /* Road_hpp */
