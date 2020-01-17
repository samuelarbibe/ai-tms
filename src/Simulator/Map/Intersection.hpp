#pragma once
//
//  Intersection.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 23/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Intersection_hpp
#define Intersection_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "Road.hpp"
#include "../Flags.hpp"

using namespace sf;
using namespace std;

typedef enum {DRY = 8, MOIST = 7 ,RAIN = 5, HEAVY_RAIN = 4, SNOW = 3, ICE = 1} WeatherCondition; // the friction coefficient, defining the cars breaking distance
typedef enum {UP = 1, RIGHT, DOWN, LEFT} ConnectionSides;



class Intersection: public RectangleShape
{
    
public:
    
    Intersection(Vector2f position, int intersectionNumber, WeatherCondition weatherCondition = WeatherCondition::DRY);
    ~Intersection(){if(DRAW_DELETE)cout << "Intersection " << m_intersectionNumber << "deleted" << endl;}

    void   reAssignIntersectionPosition(Vector2f position);
    void   reAssignRoadPositions();
    void   Update(float elapsedTime);
    void   Draw(RenderWindow *window);

    void   SetWeatherCondition(WeatherCondition weatherCondition){m_weatherCondition = weatherCondition;}
    WeatherCondition   GetWeatherCondition(){return m_weatherCondition;}

    int GetIntersectionNumber(){return m_intersectionNumber;}
    Lane * AddLane(int laneNumber, int roadNumber, bool isInRoadDirection);
    Road * AddConnectingRoad(int roadNumber, int connectionSide1, int connectionSide2, Intersection * connectedIntersection);
    Road * GetRoad(int roadNumber);
    Lane * GetLane(int laneNumber);
    Road * GetRoadByConnectionSide(int connectionSide);
    
    Vector2f GetPositionByConnectionSide(int connectionSide);
    
    Road * AddRoad(int roadNumber, int connectionSide, float length);
    
    vector<Road*> * GetRoads(){return &(m_roads);}

    static int IntersectionCount;
    
private:
    
    int m_intersectionNumber;
    int m_numberOfRoads;
    int m_width;
    int m_height;

    WeatherCondition m_weatherCondition;
    
    Vector2f m_position;
    
    vector<Road*> m_roads;


};

#endif /* Intersection_hpp */
