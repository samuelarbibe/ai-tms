//
// Created by Samuel Arbibe on 28/12/2019.
//

#ifndef SIMULATORSFML_MAP_HPP
#define SIMULATORSFML_MAP_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "../Flags.hpp"
#include "Intersection.hpp"

using namespace sf;
using namespace std;

class Map {

public:
    Map(int mapNumber, Vector2f position, int width, int height);
    ~Map(){if(DRAW_DELETE)cout << "Intersection " << m_mapNumber << "deleted" << endl;}

    bool   reAssignIntersectionPositions(Intersection * intersection1, Intersection * intersection2 ,int connectionSide1, int connectionSide2);
    void   assignAvaialablePosition();
    void   Update(float elapsedTime);
    void   Draw(RenderWindow *window);


    Intersection * AddIntersection(int intersectionNumber, Vector2f position, WeatherCondition weatherCondition = WeatherCondition::DRY);
    Road * AddRoad(int roadNumber, int intersectionNumber, int connectionSide, float length);
    Lane * AddLane(int laneNumber, int roadNumber, bool isInRoadDirection);
    Road * AddConnectingRoad(int roadNumber, int intersectionNumber1, int intersectionNumber2 ,int connectionSide1, int connectionSide2);
    Intersection * GetIntersectionByLaneNumber(int laneNumber);
    Intersection * GetIntersection(int intersectionNumber);
    Road * GetRoad(int roadNumber);
    Lane * GetLane(int laneNumber);

private:

    int m_mapNumber;
    int m_numberOfIntersections;
    int m_width;
    int m_height;

    Vector2f m_position;

    vector<Intersection*> m_intersections;

};


#endif //SIMULATORSFML_MAP_HPP
