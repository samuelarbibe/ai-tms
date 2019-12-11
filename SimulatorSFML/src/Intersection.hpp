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

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "Road.hpp"

#define LANE_WIDTH 80

using namespace sf;
using namespace std;

class Intersection: public RectangleShape
{
    
public:
    
    Intersection(Vector2f position, int width, int height, int intersectioNumber);
    ~Intersection(){cout << "Intersection " << m_intersectionNumber << "deleted" << endl;};
    
    void   Init(Vector2f position, int width, int height, int intersectioNumber);
    void   reAssignRoadPositions();
    void   Update(float elapsedTime);
    void   Draw(RenderWindow *window);
    
    Lane * AddLane(int laneNumber, int roadNumber, bool isInRoadDirection);
    Road * GetRoad(int roadNumber);
    Lane * GetLane(int laneNumber);
    Road * GetRoadByConnectionSide(int connectionSide);
    
    Vector2f GetPositionByConnectionSide(int connectionSide);
    
    Road * AddRoad(int roadNumber, int connectionSide, float length);
    
    vector<Road*> * GetRoads(){return &(m_roads);};
    
private:
    
    int m_intersectionNumber;
    int m_numberOfRoads;
    int m_width;
    int m_height;
    
    Vector2f m_position;
    
    vector<Road*>  m_roads;
};

#endif /* Intersection_hpp */
