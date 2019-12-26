//
//  Lane.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 03/12/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Lane_hpp
#define Lane_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "Flags.hpp"

using namespace std;
using namespace sf;

const Color LaneColor(45, 45, 45);
const Color WhiteColor(230, 230, 230);
const Color BackgroundColor(150, 150, 150);

static int laneCount = 0;

class Lane : public RectangleShape
{
    
public:
    
    Lane(int laneNumber, int roadNumber, Vector2f startPosition, float width, float length, float direction);
    ~Lane(){if(DRAW_DELETE)cout << "Lane " << m_laneNumber << " deleted" << endl;};
    
    void  Update(float elapsedTime);

    // Get
    bool  GetIsBlocked() {return m_isBlocked;};
    float GetDirection() {return m_direction;};
    int   GetLaneNumber(){return m_laneNumber;};
    int   GetCurrentVehicleCount(){return m_currentVehicleCount;};
    int   GetTotalVehicleCount(){return m_totalVehicleCount;};
    int GetLastCar(){return m_lastEnteredCarNumber;};
    int GetRoadNumber(){return m_roadNumber;};
    Vector2f GetStartPosition(){return m_startPosition;};
    Vector2f GetEndPosition(){return m_endPosition;};

    // Set
    void  AddVehicleCount(){m_currentVehicleCount++; m_totalVehicleCount++;};
    void  RemoveVehicleCount(){m_currentVehicleCount--;};
    void  SetIsBlocked(bool blocked) {m_isBlocked = blocked; if(DRAW_ACTIVE)cout << "Lane " << m_laneNumber << " blocked : "<< blocked << endl;};
    void  SetLastCar(int lastCar){m_lastEnteredCarNumber = lastCar;};

    
    void  Draw(RenderWindow * window);
    
private:
    
    bool     m_isBlocked;
    int      m_roadNumber;
    int      m_laneNumber;
    int      m_currentVehicleCount;
    int      m_totalVehicleCount;
    
    int      m_lastEnteredCarNumber;
    
    Vector2f m_startPosition;
    Vector2f m_endPosition;
    
    float    m_direction;
    float    m_width;
    float    m_length;
    
    void     m_createArrowShape(Transform t);
        
    ConvexShape m_arrowShape;
};



#endif /* Lane_hpp */
