//
// Created by Samuel Arbibe on 28/12/2019.
//

#ifndef SIMULATORSFML_MAP_HPP
#define SIMULATORSFML_MAP_HPP

#include <iostream>
#include <list>
#include <queue>
#include <algorithm>
#include <set>

#include <SFML/Graphics.hpp>
#include <QString>

#include "src/sim/simulator/Settings.hpp"
#include "Intersection.hpp"
#include "Route.hpp"
#include "Cycle.hpp"

using namespace sf;
using namespace std;

bool compare_priority(Phase* first, Phase* second);

class Map
{

  public:

	Map(int mapNumber, int width, int height);
	~Map();

	void Update(float elapsedTime);
	void Draw(RenderWindow *window);
	bool DeleteLane(int laneNumber);
	void ReloadMap();
	void CyclePhase();

	// entity adding
	Intersection *AddIntersection(int intersectionNumber, Vector2f position);
	Road *  AddRoad(int roadNumber, int intersectionNumber, int connectionSide, float length);
	Road *  AddConnectingRoad(int roadNumber, int intersectionNumber1, int intersectionNumber2);
	Lane *  AddLane(int laneNumber, int roadNumber, bool isInRoadDirection);
	Cycle * AddCycle(int cycleNumber, int intersectionNumber = 0);
	Phase * AddPhase(int phaseNumber, int cycleNumber, float cycleTime);
	Light * AddLight(int lightNumber, int phaseNumber, int parentRoadNumber);
	Route * AddRoute(int from, int to);

	// get
	Vector2f GetSize() { return Vector2f(width_, height_); }
	Road *   GetRoad(int roadNumber);
	Lane *   GetLane(int laneNumber);
	Cycle *  GetCycle(int cycleNumber);
	Phase *  GetPhase(int phaseNumber);


	set<QString> GetLaneIdList(int phaseNumber = 0);
	set<QString> GetRoadIdList();
	set<QString> GetIntersectionIdList();
	set<QString> GetPhaseIdList();
	set<QString> GetCycleIdList();
	set<QString> GetLightIdList();
	int GetIntersectionCount() { return number_of_intersections_; }
	int GetRoadCount();
	int GetLaneCount();
	vector<Route *> *GetRoutes() { return &routes_; }
	vector<Cycle *> *GetCycles() { return &cycles_; }
	vector<Phase *> *GetPhases();
	vector<Lane  *> *GetLanes();
	vector<Light *> *GetLights();
	Intersection *GetIntersection(int intersectionNumber);
	vector<Intersection *>  GetIntersectionByLaneNumber(int laneNumber);
	vector<Intersection *> *GetIntersections() { return &(intersections_); };
	Route *GetPossibleRoute(int from);
	Route *GetRouteByStartEnd(int from, int to);
	Lane * GetPossibleStartingLane();
	list<Lane *> * GenerateRandomTrack();

	// set
	bool SetPhaseTime(int phaseNumber, float phaseTime);
	bool AssignLaneToPhase(int phaseNumber, int laneNumber);
	bool UnassignLaneFromPhase(int laneNumber);
	void SelectLanesByPhase(int phaseNumber);
	void UnselectAll();
	void FindStartingLanes();
	bool RemoveRouteByLaneNumber(int laneNumber);
	void SelectRoutesByVehicle(list<Lane*> * instructionSet);
	void UnselectRoutes();

	pair<ConnectionSides, ConnectionSides> AssignConnectionSides(Vector2f pos1, Vector2f pos2);

	Lane *CheckSelection(Vector2f position);
	Lane *SelectedLane;

	// The total count of all the maps created this session
	static int MapCount;


  private:

    // comp function for phase sorting


	// ID of this map
	int map_number_;
	// Number of intersection that belong to this
	int number_of_intersections_;
	// number of cycles that belong to this
	int number_of_cycles_;
	int width_;
	int height_;
	// ID of the current active phase
	int current_phase_index_;

	vector<Route *> routes_;
	vector<Lane *> starting_lanes_;
	vector<Intersection *> intersections_;
    vector<Cycle *> cycles_;

	vector<Lane *> selected_lanes_;
	vector<Route *> selected_routes_;
};

#endif //SIMULATORSFML_MAP_HPP
