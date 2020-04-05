//
// Created by Samuel Arbibe on 28/12/2019.
//

#include <QStringList>
#include "Map.hpp"

int Map::MapCount = 0;

bool compare_priority(Phase* first, Phase* second)
{
    return (first->GetPriorityScore() < second->GetPriorityScore());
}

template<typename T>
void pop_front(std::vector<T>& vec)
{
    assert(!vec.empty());
    vec.erase(vec.begin());
}

Map::Map(int mapNumber, int width, int height) {
	if (mapNumber == 0)
	{
		mapNumber = ++ MapCount;
	}
	map_number_ = mapNumber;
	width_ = width;
	height_ = height;
	SelectedLane = nullptr;
	current_phase_index_ = 0;
	number_of_phases = 0;
	number_of_intersections_ = 0;
}

Map::~Map() {
	for (Intersection *inter : intersections_)
	{
		delete inter;
	}

	for (Route *route : routes_)
	{
		delete route;
	}

	for (Phase *phase : phases_)
	{
		delete phase;
	}

	Route::RouteCount = 0;
	Lane::LaneCount = 0;
	Road::RoadCount = 0;
	Phase::PhaseCount = 0;
	Intersection::IntersectionCount = 0;

	if (Settings::DrawDelete)
		cout << "map " << map_number_ << " deleted" << endl;
}

/// add an intersection to the map
Intersection *Map::AddIntersection(int intersectionNumber, Vector2f position) {
	SelectedLane = nullptr;

	if (!intersectionNumber)
	{
		intersectionNumber = Intersection::IntersectionCount + 1;
	}

	intersections_.push_back(new Intersection(position, intersectionNumber));

	number_of_intersections_++;
	Intersection::IntersectionCount++;

	if (Settings::DrawAdded)
		std::cout << "Intersection " << intersectionNumber << " added" << endl;

	return intersections_[number_of_intersections_ - 1];
}

/// add a road to a specific intersection
Road *Map::AddRoad(int roadNumber, int intersectionNumber, int connectionSide, float length) {
	SelectedLane = nullptr;

	Intersection *temp = GetIntersection(intersectionNumber);
	Road *tempRoad = nullptr;

	if (temp)
	{
		tempRoad = temp->AddRoad(roadNumber, connectionSide, length);
	}

	this->ReloadMap();

	return tempRoad;
}

/// add a road to a specific road
Lane *Map::AddLane(int laneNumber, int roadNumber, bool isInRoadDirection) {
	SelectedLane = nullptr;

	Road *tempRoad = GetRoad(roadNumber);
	Intersection *temp = nullptr;
	Lane *tempLane = nullptr;

	if (tempRoad)
	{
		temp = GetIntersection(tempRoad->GetIntersectionNumber());
	}

	if (temp)
	{
		tempLane = temp->AddLane(laneNumber, roadNumber, isInRoadDirection);
	}

	this->ReloadMap();

	return tempLane;
}

/// add a road connecting between two intersections
Road *Map::AddConnectingRoad(int roadNumber, int intersectionNumber1, int intersectionNumber2) {
	SelectedLane = nullptr;

	Intersection *inter1 = GetIntersection(intersectionNumber1);
	Intersection *inter2 = GetIntersection(intersectionNumber2);

	if (inter1 == nullptr || inter2 == nullptr)
	{
		cerr << "one of the given intersections was not found..." << endl;
		return nullptr;
	}

	// if intersection do not align on one of the axis, return error
	if ((int(inter1->getPosition().x) != int(inter2->getPosition().x) &&
		int(inter1->getPosition().y) != int(inter2->getPosition().y))
		|| (int(inter1->getPosition().x) == int(inter2->getPosition().x) &&
			int(inter1->getPosition().y) == int(inter2->getPosition().y)))
	{
		cerr << "the intersections must align on one of the axis" << endl;
		return nullptr;
	}
	/*
	if(!reAssignIntersectionPositions(inter1, inter2 ,connectionSide1, connectionSide2))
	{
		cerr << "Connection Failed..." << endl;
		return nullptr;
	}
	*/
	pair<ConnectionSides, ConnectionSides> connections;
	connections = AssignConnectionSides(inter1->getPosition(), inter2->getPosition());

	Road *temp = inter1->AddConnectingRoad(roadNumber, connections.first, connections.second, inter2);

	this->ReloadMap();
	return temp;
}

/// add a possible route in this map
Route * Map::AddRoute(int from, int to) {
	Lane *fromLane = GetLane(from);
	Lane *toLane = GetLane(to);

	if (fromLane != nullptr && toLane != nullptr)
	{
		Route *r = new Route(fromLane, toLane);
		routes_.emplace_back(r);

		if(Settings::DrawAdded) cout << "Route added from " << r->FromLane->GetLaneNumber() << " to " << r->ToLane->GetLaneNumber() << endl;
		return r;
	} else
	{
		cout << "could not create possible route, as one of the roads was not found" << endl;
		return nullptr;
	}
}

Phase *Map::AddPhase(int phaseNumber, float cycleTime) {
	Phase *temp;

	if (phaseNumber == 0)
	{
		phaseNumber = Phase::PhaseCount + 1;
	}

	temp = new Phase(phaseNumber, cycleTime);

	phases_.push_back(temp);

	++number_of_phases;
	++Phase::PhaseCount;

	if(Settings::DrawAdded) cout << "phase " << phaseNumber << " added" << endl;

	return temp;
}

Light *Map::AddLight(int lightNumber, int phaseNumber, int parentRoadNumber) {
	Light *temp = nullptr;
	Phase *myPhase = GetPhase(phaseNumber);
	Road *parentRoad = GetRoad(parentRoadNumber);

	// assign position relative to parent road
	if (myPhase != nullptr && parentRoad != nullptr)
	{
		temp = myPhase->AddLight(lightNumber, parentRoad);
		if(Settings::DrawAdded) cout << "light " << temp->GetLightNumber() << " added to phase " << phaseNumber << endl;
		return temp;
	}

	cout << "could not add light as phase or parent road don't exist" << endl;
	return temp;
}

bool Map::SetPhaseTime(int phaseNumber, float phaseTime) {
	Phase *phase = GetPhase(phaseNumber);

	if (phase != nullptr)
	{
		phase->SetCycleTime(phaseTime);
		cout << "Phase time of phase " << phaseNumber << " successfully set." << endl;
		return true;
	}

	cout << "Could not set phase time as phase doesnt exist." << endl;
	return false;
}

bool Map::AssignLaneToPhase(int phaseNumber, int laneNumber) {
	Phase *temp = GetPhase(phaseNumber);
	Lane *lane = GetLane(laneNumber);

	if (temp != nullptr && lane != nullptr)
	{
		temp->AddLane(lane);
		if(Settings::DrawAdded) cout << "lane " << lane->GetLaneNumber()
		     << " added to phase " << temp->GetPhaseNumber() << endl;
		return true;
	}
	cout << "could not add lane to phase as phase or lane don't exist." << endl;
	return false;
}

bool Map::RemoveRouteByLaneNumber(int laneNumber) {
	Lane *laneToRemove = GetLane(laneNumber);

	if (laneToRemove != nullptr)
	{
		auto it = routes_.begin();

		// while there are cars to delete;
		while (it != routes_.end())
		{
			// if is to be deleted
			if ((*it)->FromLane->GetLaneNumber() == laneNumber ||
				(*it)->ToLane->GetLaneNumber() == laneNumber)
			{
				Route *temp = (*it);
				int routeNumber = temp->GetRouteNumber();
				it = routes_.erase(it);

				delete temp;

				if (Settings::DrawActive)
					cout << "route " << routeNumber << " deleted." << endl;
			} else
			{
				it++;
			}
		}

		return true;
	} else
	{
		//cout << "could not delete this route. the deleted lane could not be found." << endl;
		return false;
	}

}

bool Map::UnassignLaneFromPhase(int laneNumber){

	Lane * lane = GetLane(laneNumber);

	if(lane->GetPhaseNumber() != 0)
	{
		if (lane != nullptr)
		{
			Phase *phase = GetPhase(lane->GetPhaseNumber());

			if (phase->UnassignLane(lane))
			{
				return true;
			}

		} else
		{
			cout << "could not Unassign lane from phase, as lane does not belong to any phase" << endl;
		}
	}
	return false;
}

/// finds all starting lanes
void Map::FindStartingLanes() {
	// for a lane to be a starting lane, it has to be in a non-connecting road,
	// and it has to have isInRoadDirection = false;

	starting_lanes_.clear();

	for (Intersection *inter : intersections_)
	{
		for (Road *road  : *inter->GetRoads())
		{
			if (!road->GetIsConnecting())
			{
				for (Lane *lane : *road->GetLanes())
				{
					if (!lane->GetIsInRoadDirection())
					{
						starting_lanes_.push_back(lane);
					}
				}
			}
		}
	}
}

/// returns a possible starting lane
Lane *Map::GetPossibleStartingLane() {
	if (starting_lanes_.empty())
		return nullptr;
	int randomIndex = rand() % starting_lanes_.size();
	return starting_lanes_[randomIndex];
}

/// select all the routes
void Map::SelectRoutesByVehicle(list<Lane*> * instructionSet)
{
	Route * r = nullptr;
	std::list<Lane*>::const_iterator to = instructionSet->begin();
	std::list<Lane*>::const_iterator from = to;
	to++;
	for(; to != instructionSet->end(); ++to)
	{
		 r = GetRouteByStartEnd((*from)->GetLaneNumber(), (*to)->GetLaneNumber());
		 if(r != nullptr)
		 {
		 	selected_routes_.push_back(r);
		 	r->SetSelected(true);
		 }
		 from = to;
	}
}

/// randomly generate a track
list<Lane *> * Map::GenerateRandomTrack()
{
	// find a random starting point
	Lane *l = GetPossibleStartingLane();
	if (l == nullptr)
	{
		cout << "no starting lanes available." << endl;
		return nullptr;
	}
	// find a starting route from starting lane
	Route *r = GetPossibleRoute(l->GetLaneNumber());

	if (r == nullptr)
	{
		cout << "no routes available. please add them to the map" << endl;
		return nullptr;
	}

	// while new routes to append are available
	// new routes will be searched starting from the previous route end
	list<Lane *> *track = new list<Lane *>();
	Lane *lastLane = nullptr;

	while (r != nullptr)
	{
		track->push_back(r->FromLane);
		lastLane = r->ToLane;
		r = GetPossibleRoute(r->ToLane->GetLaneNumber());
	}
	if (lastLane != nullptr)
	{
		track->push_back(lastLane);
	}

	return track;
}

/// returns a possible route according to the given lane
Route *Map::GetPossibleRoute(int fromLane)
{
	Lane *myLane = GetLane(fromLane);
	vector<Route *> possibleRoutes;

	if (myLane != nullptr)
	{
		for (Route *r : routes_)
		{
			if (r->FromLane->GetLaneNumber() == myLane->GetLaneNumber())
			{
				possibleRoutes.push_back(r);
			}
		}
	}
	if (possibleRoutes.empty())
	{
		return nullptr;
	}
	int randomIndex = rand() % possibleRoutes.size();
	return possibleRoutes[randomIndex];
}

Route *Map::GetRouteByStartEnd(int from, int to)
{
	for(Route * r : routes_)
	{
		if(r->FromLane->GetLaneNumber() == from &&
			r->ToLane->GetLaneNumber() == to)
		{
			return r;
		}
	}
	return nullptr;
}

/// get intersection by intersectionNumber
Intersection *Map::GetIntersection(int intersectionNumber) {
	Intersection *temp;

	for (Intersection *inter : intersections_)
	{
		if ((temp = inter)->GetIntersectionNumber() == intersectionNumber)
		{
			return temp;
		}
	}

	cout << "error : intersection not found in map..." << endl;

	return nullptr;
}

/// get intersection by lane number
vector<Intersection *> Map::GetIntersectionByLaneNumber(int laneNumber) {
	Lane *l = this->GetLane(laneNumber);
	Road *r = this->GetRoad(l->GetRoadNumber());

	vector<Intersection *> retVal;

	if (r->GetIsConnecting())
	{
		retVal.push_back(this->GetIntersection(r->GetIntersectionNumber(1)));
	}
	retVal.push_back(this->GetIntersection(r->GetIntersectionNumber(0)));
	return retVal;
}

/// get road by roadNumber
Road *Map::GetRoad(int roadNumber) {
	Road *temp;

	for (Intersection *inter : intersections_)
	{
		if ((temp = inter->GetRoad(roadNumber)) != nullptr)
		{
			return temp;
		}
	}

	cout << "error : road not found in map..." << endl;

	return nullptr;
}

/// get lane by lane number
Lane *Map::GetLane(int laneNumber) {
	Lane *temp;

	for (Intersection *inter : intersections_)
	{
		if ((temp = inter->GetLane(laneNumber)) != nullptr)
		{
			return temp;
		}
	}

	cout << "error : lane not found in map..." << endl;

	return nullptr;
}

/// get phase by phase number
Phase *Map::GetPhase(int phaseNumber)
{
	Phase *temp;

	for (Phase *p : phases_)
	{
		if ((temp = p)->GetPhaseNumber() == phaseNumber)
		{
			return temp;
		}
	}

	cout << "error : phase not found in map..." << endl;

	return nullptr;
}

/// set 2 relative connection sides, by intersection positions
pair<ConnectionSides, ConnectionSides> Map::AssignConnectionSides(Vector2f pos1, Vector2f pos2) {
	ConnectionSides con1, con2;
	if (pos1.x > pos2.x)
	{
		con1 = ConnectionSides::LEFT;
		con2 = ConnectionSides::RIGHT;
	} else if (pos1.x < pos2.x)
	{
		con1 = ConnectionSides::RIGHT;
		con2 = ConnectionSides::LEFT;
	} else if (pos1.y > pos2.y)
	{
		con1 = ConnectionSides::UP;
		con2 = ConnectionSides::DOWN;
	} else if (pos1.y < pos2.y)
	{
		con1 = ConnectionSides::DOWN;
		con2 = ConnectionSides::UP;
	}

	pair<ConnectionSides, ConnectionSides> connections(con1, con2);
	return connections;
}

/// check if a road has been selected int this map
Lane *Map::CheckSelection(Vector2f position) {
	// for each intersection in map
	Lane *temp;
	for (Intersection *inter : intersections_)
	{
		// if selection found
		temp = inter->CheckSelection(position);
		if (temp != nullptr)
			return temp;
	}
	return nullptr;
}

/// Reload all intersection in this map
void Map::ReloadMap()
{

	// unselect all the selected lanes
	UnselectAll();

	for (Intersection *i : intersections_)
	{
		i->ReloadIntersection();
	}

	for (Phase *p : phases_)
	{
		p->ReloadPhase();
	}

	for (Route *r : routes_)
	{
		r->ReloadRoute();
	}

	FindStartingLanes();
}

/// update, for future use
void Map::Update(float elapsedTime) {
	for (Intersection *i : intersections_)
	{
		i->Update(elapsedTime);
	}

	for (Phase *p : phases_)
	{
		p->Update(elapsedTime);
	}

	CyclePhase();
}



/// cycle the phases by the phase array order.
// all the phases but the active one are constantly evaluated
// and sorted by the score they have been given by the NN
// sort(arr[0:-2])

// when phase is finished, it gets pop_front and push_back
// to advance and start the next phase in order.
void Map::CyclePhase() {
    // if current phase has ended
    if(phases_.size() > 1)
    {
        // when current phase is closed, advance to next phase and open it
        if (!phases_.back()->GetIsOpen())
        {
            Phase * backPhase = phases_[number_of_phases-1];
            phases_[number_of_phases-1] = phases_[number_of_phases-2];
            phases_[number_of_phases-2] = backPhase;

            phases_[number_of_phases-1]->Open();

            cout << "[ ";
            for(int i = 0; i < number_of_phases-1 ; i++)
            {
                cout << phases_[i]->GetPriorityScore() << " ";
            }
            cout << "[" << phases_[number_of_phases-1]->GetPriorityScore() << "]]" << endl;
        }
            // constantly sort the list by their priority score
        else
        {
            partial_sort(phases_.begin(), phases_.end() - 1, phases_.end() - 1, compare_priority);
        }
    }
}




void Map::SelectLanesByPhase(int phaseNumber)
{
	Phase * p = GetPhase(phaseNumber);

	if(p != nullptr)
	{
		UnselectAll();

		for(Lane * l : *p->GetAssignedLanes())
		{
			selected_lanes_.push_back(l);
			l->Select();
		}
	}
}

void Map::UnselectAll()
{
	// unselect selected lane
	if (SelectedLane != nullptr)
	{
		SelectedLane->Unselect();
		SelectedLane = nullptr;
	}

	for(Lane * l : selected_lanes_)
	{
		l->Unselect();
	}

	for(Route * r : selected_routes_)
	{
		r->SetSelected(false);
	}

	selected_lanes_.clear();
	selected_routes_.clear();
}

/// return road count in this map
int Map::GetRoadCount() {
	int sum = 0;

	for (Intersection *inter : intersections_)
	{
		sum += inter->GetRoadCount();
	}

	return sum;
}

/// return lane count int this map
int Map::GetLaneCount() {
	int sum = 0;

	for (Intersection *inter : intersections_)
	{
		sum += inter->GetLaneCount();
	}

	return sum;
}

/// return a vector of all the existing lanes
vector<Lane *> Map::GetLanes()
{
	vector<Lane*> lanes = vector<Lane*>();

	for(Intersection * inter : intersections_)
	{
		for(Road * road : *inter->GetRoads())
		{
			for(Lane * lane : *road->GetLanes())
			{
				lanes.push_back(lane);
			}
		}
	}

	return lanes;
}

/// delete a given lane in this map
bool Map::DeleteLane(int laneNumber) {

	vector<Intersection *> targetIntersections = GetIntersectionByLaneNumber(laneNumber);
	Lane * lane = GetLane(laneNumber);

	if (!targetIntersections.empty())
	{
		// delete all routes that go through this lane
		RemoveRouteByLaneNumber(laneNumber);

		// delete this lane from all phases it belongs to
		UnassignLaneFromPhase(laneNumber);
		// delete the given lane
		// if lane's road is connecting, send other intersection as well to handle deletion
		if (targetIntersections.size() > 1)
		{
			targetIntersections[0]->DeleteLane(laneNumber, targetIntersections[1]);
		} else
		{
			targetIntersections[0]->DeleteLane(laneNumber);
		}

		// if intersection has no roads left, delete it as well
		if (targetIntersections[0]->GetRoadCount() == 0)
		{
			auto it = find(intersections_.begin(), intersections_.end(), targetIntersections[0]);
			it = intersections_.erase(it);
			delete (*it);
			number_of_intersections_--;
		}

		// if road was connecting, check if the connected intersection needs to be deleted as well
		if (targetIntersections.size() > 1)
		{
			if (targetIntersections[1]->GetRoadCount() == 0)
			{
				auto it = find(intersections_.begin(), intersections_.end(), targetIntersections[1]);
				intersections_.erase(it);
				delete (*it);
				number_of_intersections_--;
			}
		}

		// set selected as nullptr
		this->SelectedLane = nullptr;

		// reload the map to display the changes
		this->ReloadMap();

		return true;
	}
	return false;
}

/// draw the map, and all of its belongings
void Map::Draw(RenderWindow *window) {
	// Draw all intersections
	for (Intersection *inter : intersections_)
	{
		inter->Draw(window);
	}

	// draw all routes

	for (auto &route : routes_)
	{
		route->Draw(window);
	}

	for (Phase *p : phases_)
	{
		p->Draw(window);
	}

}

/// return a list of all the lanes' id's
set<QString> Map::GetLaneIdList(int phaseNumber) {
	set<QString> idList = set<QString>();

	if (phaseNumber != 0)
	{
		Phase *p = GetPhase(phaseNumber);
		for (Lane *lane : *p->GetAssignedLanes())
		{
			idList.insert(QString::number(lane->GetLaneNumber()));
		}
	} else
	{
		for (Intersection *inter : intersections_)
		{
			for (Road *road : *inter->GetRoads())
			{
				for (Lane *lane : *road->GetLanes())
				{

					idList.insert(QString::number(lane->GetLaneNumber()));
				}
			}
		}
	}

	return idList;
}

/// return a list of all the roads' id's
set<QString> Map::GetRoadIdList() {
	set<QString> idList = set<QString>();
	for (Intersection *inter : intersections_)
	{
		for (Road *road : *inter->GetRoads())
		{
			idList.insert(QString::number(road->GetRoadNumber()));
		}
	}

	return idList;
}

/// return a list of all the intersections' id's
set<QString> Map::GetIntersectionIdList() {
	set<QString> idList = set<QString>();
	for (Intersection *inter : intersections_)
	{
		idList.insert(QString::number(inter->GetIntersectionNumber()));
	}

	return idList;
}

/// return a list of all the intersections' id's
set<QString> Map::GetPhaseIdList() {
	set<QString> idList = set<QString>();
	for (Phase *p : phases_)
	{
		idList.insert(QString::number(p->GetPhaseNumber()));
	}

	return idList;
}

/// return a list of all the intersections' id's
set<QString> Map::GetLightIdList() {
	set<QString> idList = set<QString>();
	for (Phase *p : phases_)
	{
		for (Light *l : *p->GetLights())
		{
			idList.insert(QString::number(l->GetLightNumber()));
		}
		idList.insert(QString::number(p->GetPhaseNumber()));
	}

	return idList;
}









