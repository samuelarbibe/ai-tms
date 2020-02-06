//
// Created by Samuel Arbibe on 28/12/2019.
//

#ifndef SIMULATORSFML_MAP_HPP
#define SIMULATORSFML_MAP_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include <QString>
#include "../Settings.hpp"
#include "Intersection.hpp"

using namespace sf;
using namespace std;

class Map {

public:

    Map(int mapNumber, Vector2i position, int width, int height);
    ~Map();

    void   Update(float elapsedTime);
    void   Draw(RenderWindow *window);
    bool   DeleteLane(int laneNumber);
    void   ReloadMap();


    Vector2f GetSize(){return Vector2f(m_width, m_height);}
    Intersection * AddIntersection(int intersectionNumber, Vector2f position, WeatherCondition weatherCondition = WeatherCondition::DRY);
    Road * AddRoad(int roadNumber, int intersectionNumber, int connectionSide, float length);
    Lane * AddLane(int laneNumber, int roadNumber, bool isInRoadDirection);
    Road * AddConnectingRoad(int roadNumber, int intersectionNumber1, int intersectionNumber2);
    Intersection * GetIntersectionByLaneNumber(int laneNumber);
    Intersection * GetIntersection(int intersectionNumber); 
    vector<Intersection*> * GetIntersections(){return &(m_intersections);};
    Road * GetRoad(int roadNumber);
    Lane * GetLane(int laneNumber);
    QStringList GetLaneIdList();
    QStringList GetRoadIdList();
    QStringList GetIntersectionIdList();
    int GetIntersectionCount(){return m_numberOfIntersections;};
    int GetRoadCount();
    int GetLaneCount();
    pair<ConnectionSides, ConnectionSides> AssignConnectionSides(Vector2f pos1, Vector2f pos2);
    Lane * SelectedLane;
    Lane * CheckSelection(Vector2f position);

private:

    int m_mapNumber;
    int m_numberOfIntersections;
    int m_width;
    int m_height;

    Vector2i m_position;

    vector<Intersection*> m_intersections;
};


#endif //SIMULATORSFML_MAP_HPP
