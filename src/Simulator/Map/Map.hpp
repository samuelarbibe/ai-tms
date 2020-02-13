//
// Created by Samuel Arbibe on 28/12/2019.
//

#ifndef SIMULATORSFML_MAP_HPP
#define SIMULATORSFML_MAP_HPP

#include <iostream>
#include <list>
#include <set>

#include <SFML/Graphics.hpp>
#include <QString>

#include "../Settings.hpp"
#include "Intersection.hpp"

using namespace sf;
using namespace std;

struct Route{
    Lane * from;
    Lane * to;
};

class Map
{

public:

    Map(int mapNumber, Vector2i position, int width, int height);
    ~Map();

    void   Update(float elapsedTime);
    void   Draw(RenderWindow *window);
    bool   DeleteLane(int laneNumber);
    void   ReloadMap();

    Vector2f GetSize(){return Vector2f(width_, height_);}
    Intersection * AddIntersection(int intersectionNumber, Vector2f position);
    Road * AddRoad(int roadNumber, int intersectionNumber, int connectionSide, float length);
    Lane * AddLane(int laneNumber, int roadNumber, bool isInRoadDirection);
    Road * AddConnectingRoad(int roadNumber, int intersectionNumber1, int intersectionNumber2);
    Intersection * GetIntersectionByLaneNumber(int laneNumber);
    Intersection * GetIntersection(int intersectionNumber); 
    vector<Intersection*> * GetIntersections(){return &(intersections_);};
    Road * GetRoad(int roadNumber);
    Lane * GetLane(int laneNumber);
    set<QString> GetLaneIdList();
    set<QString> GetRoadIdList();
    set<QString> GetIntersectionIdList();
    int GetIntersectionCount(){return number_of_intersections_;}
    int GetRoadCount();
    int GetLaneCount();
    pair<ConnectionSides, ConnectionSides> AssignConnectionSides(Vector2f pos1, Vector2f pos2);
    Lane * SelectedLane;
    Lane * CheckSelection(Vector2f position);
    Route * GetRandomRoute();
    vector<Route*> * GetRoutes(){return &routes_;}
    bool AddRoute(int from, int to);
    Route * GetPossibleRoute(int from);

private:

    int map_number_;
    int number_of_intersections_;
    int width_;
    int height_;

    vector<Route*> routes_;

    Vector2i position_;

    vector<Intersection*> intersections_;
};


#endif //SIMULATORSFML_MAP_HPP
