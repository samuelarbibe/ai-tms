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
#include "Route.hpp"
#include "Phase.hpp"

using namespace sf;
using namespace std;

class Map
{

public:

    Map(int mapNumber, int width, int height);
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
    Phase * AddPhase(int phaseNumber, float cycleTime);
    Light * AddLight(int lightNumber, int phaseNumber, int parentRoadNumber);
    void AssignLaneToPhase(int phaseNumber, int laneNumber);
    void CyclePhase();

    vector<Intersection*> GetIntersectionByLaneNumber(int laneNumber);
    Intersection * GetIntersection(int intersectionNumber); 
    vector<Intersection*> * GetIntersections(){return &(intersections_);};

    Road * GetRoad(int roadNumber);
    Lane * GetLane(int laneNumber);
    Phase * GetPhase(int phaseNumber);
    vector<Route*> * GetRoutes(){return &routes_;}
    vector<Phase*> * GetPhases(){return &phases_;}

    set<QString> GetLaneIdList();
    set<QString> GetRoadIdList();
    set<QString> GetIntersectionIdList();

    int GetIntersectionCount(){return number_of_intersections_;}
    int GetRoadCount();
    int GetLaneCount();

    pair<ConnectionSides, ConnectionSides> AssignConnectionSides(Vector2f pos1, Vector2f pos2);

    // route manipulation

    void FindStartingLanes();
    bool AddRoute(int from, int to);
    bool RemoveRouteByLaneNumber(int laneNumber);
    Route * GetPossibleRoute(int from);
    Lane  * GetPossibleStartingLane();

    Lane * CheckSelection(Vector2f position);

    Lane * SelectedLane;

private:

    int map_number_;
    int number_of_intersections_;
    int number_of_phases;
    int width_;
    int height_;
    int current_phase_index_;

    vector<Route*> routes_;
    vector<Lane*> starting_lanes_;
    vector<Intersection*> intersections_;
    vector<Phase*> phases_;
};


#endif //SIMULATORSFML_MAP_HPP
