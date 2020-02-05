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
#include "../Settings.hpp"

using namespace std;
using namespace sf;

const Color LaneColor(45, 45, 45);
const Color WhiteColor(230, 230, 230);
const Color BackgroundColor(150, 150, 150);


class Lane : public RectangleShape
{
    
public:
    
    Lane(int laneNumber, int roadNumber, int intersectionNumber, Vector2f startPosition, float length, float direction);
    ~Lane();
    
    void Update(float elapsedTime);

    // Get
    bool  GetIsBlocked() {return m_isBlocked;};
    float GetDirection() {return m_direction;};
    int   GetLaneNumber(){return m_laneNumber;};
    int   GetIntersectionNumber(){return m_intersectionNumber;};
    int   GetCurrentVehicleCount(){return m_currentVehicleCount;};
    int   GetTotalVehicleCount(){return m_totalVehicleCount;};
    int GetLastCar(){return m_lastEnteredCarNumber;};
    int GetRoadNumber(){return m_roadNumber;};
    Vector2f GetStartPosition(){return m_startPosition;};
    Vector2f GetEndPosition(){return m_endPosition;};

    // Set
    void  Select();
    void  Unselect();
    void  AddVehicleCount(){m_currentVehicleCount++; m_totalVehicleCount++;};
    void  RemoveVehicleCount(){m_currentVehicleCount--;};
    void  SetIsBlocked(bool blocked) {m_isBlocked = blocked; if(Settings::DrawActive)cout << "Lane " << m_laneNumber << " blocked: " << blocked << endl;};
    void  SetLastCar(int lastCar){m_lastEnteredCarNumber = lastCar;};
    
    void  Draw(RenderWindow * window);

    static int LaneCount;
    
private:
    
    bool     m_isBlocked;
    int      m_intersectionNumber;
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
