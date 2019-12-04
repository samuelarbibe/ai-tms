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

#define LANE_WIDTH 75

using namespace sf;
using namespace std;

typedef struct RoadConnection
{
    int connectionSide;
    Vector2f connectionPosition;
    Road * road;
    
}RoadConnection;

class Intersection: public RectangleShape
{
private:
    
    int m_intersectionNumber;
    
    list<RoadConnection> m_roadConnetions;
    int m_numberOfConnections;
    
    Vector2f m_position;
    int m_width;
    int m_height;
 
public:
    Intersection();
    ~Intersection();
    
    void Init(Vector2f position, int width, int height, int intersectioNumber);
    
    void AddRoadConnection(int roadNumber, int connectionSide, float length);
    void RemoveRoadConnection(int roadNumber);
    
    Road * GetRoad(int roadNumber);
    Road * GetRoadByConnectionSide(int connectionSide);
    
    void Draw(RenderWindow *window);
};

#endif /* Intersection_hpp */
