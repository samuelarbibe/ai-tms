//
// Created by Samuel Arbibe on 28/12/2019.
//

#ifndef SIMULATORSFML_MAP_HPP
#define SIMULATORSFML_MAP_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "../Settings.hpp"
#include "Intersection.hpp"

using namespace sf;
using namespace std;

class Map {

public:
    Map(int mapNumber, Vector2i position, int width, int height);
    ~Map(){if(Settings::DrawDelete)cout << "Intersection " << m_mapNumber << "deleted" << endl;}

    //bool   reAssignIntersectionPositions(Intersection * intersection1, Intersection * intersection2 ,int connectionSide1 = 0, int connectionSide2 = 0);

    void   assignAvaialablePosition();
    void   Update(float elapsedTime);
    void   Draw(RenderWindow *window);
    void   ReloadMap();


    Vector2f GetSize(){return Vector2f(m_width, m_height);}
    Intersection * AddIntersection(int intersectionNumber, Vector2f position, WeatherCondition weatherCondition = WeatherCondition::DRY);
    Road * AddRoad(int roadNumber, int intersectionNumber, int connectionSide, float length);
    Lane * AddLane(int laneNumber, int roadNumber, bool isInRoadDirection);
    Road * AddConnectingRoad(int roadNumber, int intersectionNumber1, int intersectionNumber2);
    Intersection * GetIntersectionByLaneNumber(int laneNumber);
    Intersection * GetIntersection(int intersectionNumber); 
    Road * GetRoad(int roadNumber);
    Lane * GetLane(int laneNumber);
    pair<ConnectionSides, ConnectionSides> AssignConnectionSides(Vector2f pos1, Vector2f pos2);

private:

    int m_mapNumber;
    int m_numberOfIntersections;
    int m_width;
    int m_height;

    Vector2i m_position;

    vector<Intersection*> m_intersections;

};


#endif //SIMULATORSFML_MAP_HPP
