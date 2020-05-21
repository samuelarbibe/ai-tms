//
//  Engine.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Engine.hpp"

Engine::Engine(QWidget *Parent) : QSFMLCanvas(Parent,
                                              1000 / Settings::Interval,
                                              1000 / Settings::Fps) {

	cout << "Setting Up map..." << endl;
	map = new Map(0, Settings::DefaultMapWidth, Settings::DefaultMapHeight);

	cout << "Setting Up Camera..." << endl;
	snap_to_grid_ = true;
	view_pos_ = Vector2f(0, 0);
	temp_view_pos_ = Vector2f(0, 0);
	number_of_sets_ = 0;
	set_view();
	set_minimap(Vector2f(Settings::MinimapWidth, Settings::MinimapHeight),
	            Settings::MinimapMargin);
	set_visual_net(Vector2f(Settings::VisualNetWidth,
	                        Settings::VisualNetHeight),
	               Settings::VisualNetMargin);
	this->setView(view_);

	cout << "Setting up snap grid..." << endl;
	BuildGrid(Settings::GridRows, Settings::GridColumns);
}

/// set up the map according to the selected presets
void Engine::on_init() {

	map->AddIntersection(0, map->GetSize() / 2.f);

	map->AddRoad(0, 1, UP, Settings::DefaultLaneLength);
	map->AddRoad(0, 1, RIGHT, Settings::DefaultLaneLength);
	map->AddRoad(0, 1, DOWN, Settings::DefaultLaneLength);
	map->AddRoad(0, 1, LEFT, Settings::DefaultLaneLength);

	map->AddLane(0, 1, false);
	map->AddLane(0, 1, false);
	map->AddLane(0, 1, true);
	map->AddLane(0, 1, true);
	map->AddLane(0, 2, false);
	map->AddLane(0, 2, false);
	map->AddLane(0, 2, true);
	map->AddLane(0, 2, true);
	map->AddLane(0, 3, false);
	map->AddLane(0, 3, false);
	map->AddLane(0, 3, true);
	map->AddLane(0, 3, true);
	map->AddLane(0, 4, false);
	map->AddLane(0, 4, false);
	map->AddLane(0, 4, true);
	map->AddLane(0, 4, true);

	map->AddRoute(1, 16);
	map->AddRoute(1, 12);
	map->AddRoute(2, 7);
	map->AddRoute(5, 4);
	map->AddRoute(5, 16);
	map->AddRoute(6, 11);
	map->AddRoute(9, 8);
	map->AddRoute(9, 4);
	map->AddRoute(10, 15);
	map->AddRoute(13, 12);
	map->AddRoute(13, 8);
	map->AddRoute(14, 3);

	map->AddCycle(0, 1);

	map->AddPhase(0, 1, 20);
	map->AddPhase(0, 1, 20);
	map->AddPhase(0, 1, 20);
	map->AddPhase(0, 1, 20);

	map->AssignLaneToPhase(1, 1);
	map->AssignLaneToPhase(1, 9);
	map->AssignLaneToPhase(2, 2);
	map->AssignLaneToPhase(2, 10);
	map->AssignLaneToPhase(3, 5);
	map->AssignLaneToPhase(3, 13);
	map->AssignLaneToPhase(4, 6);
	map->AssignLaneToPhase(4, 14);

}

/// resize the sfml window
void Engine::ResizeFrame(QSize size) {
	resize(size);
	setSize(sf::Vector2u(size.width(), size.height()));

	set_view();
	//set_minimap(Settings::MinimapSize, Settings::MinimapMargin);
	this->setView(view_);
}

/// delete a simulation
bool Engine::DeleteSimulation(int simulationNumber) {

	Simulation *s = GetSimulation(simulationNumber);
	if (s != nullptr)
	{
		Set *set = GetSet(s->GetSetNumber());

		if (set != nullptr)
		{
			return set->DeleteSimulation(simulationNumber);
		}
	}
	cout << "Could not delete simulation as it wasnt found. " << endl;
	return false;
}

/// deletes the current set. The current set is either:
// the set that is currently running
// the last set that ran
bool Engine::DeleteCurrentSet() {
	auto it = sets_.begin();
	while (it != sets_.end())
	{
		if ((*it)->GetSetNumber() == Set::CurrentSet)
		{
			Set::CurrentSet = 0;
			(*it)->StopSet();
			it = sets_.erase(it);
			number_of_sets_--;

			delete (*it);

			return true;
		} else
		{
			it++;
		}
	}

	return false;
}

/// re-run a simualtion by sim-number, without calculating it as a simulation
bool Engine::RunDemo(int simulationNumber) {

	if (!Simulation::DemoRunning && !Set::SetRunning)
	{
		Simulation *s = GetSimulation(simulationNumber);
		if (s != nullptr)
		{
			Set *set = GetSet(s->GetSetNumber());

			if (set != nullptr)
			{
				return set->DemoSimulation(simulationNumber);
			}
		}
		cout << "Could not demo simulation as it wasn't found. " << endl;
	}
	cout << "Cannot run demo as a set is currently running." << endl;
	return false;
}

/// Trains the neural network for a set amount of generation.
// at the end of a training, it saves all the data in a simulation file.
bool Engine::RunSet(int vehicleCount, int generations) {
	if (!Set::SetRunning)
	{
		ClearMap();

		if (Settings::ResetNeuralNet)
		{
			cout << "Creating a new neural network..." << endl;
			if (Net::CurrentNet != nullptr)
				Net::CurrentNet->Reset();
		}

		Set *s = AddSet(0, vehicleCount, generations);

		s->RunSet();

		cout << "Set number " << s->GetSetNumber() << " has started running"
		     << endl;
		return true;
	} else
	{
		cout << "Cannot run set as another set is already running." << endl;
		return false;
	}
}

/// set the viewport for the camera
void Engine::set_view() {
	// view setup

	view_.reset(sf::FloatRect(temp_view_pos_.x,
	                          temp_view_pos_.y,
	                          Settings::DefaultMapWidth,
	                          Settings::DefaultMapHeight));
	view_.zoom(Settings::Zoom);
	// update the shown area index rectangle
	update_shown_area();
}

/// build the minimap
void Engine::set_minimap(Vector2f size, float margin) {
	// minimap viewPort setup
	minimap_.reset(sf::FloatRect(0,
	                             0,
	                             Settings::DefaultMapWidth,
	                             Settings::DefaultMapHeight));

	minimap_.setViewport(sf::FloatRect(
		1 - size.x - margin,
		margin,
		size.x,
		size.y
	));

	float outlineThickness = Settings::DefaultMapWidth / 150.f;

	// background setup
	minimap_bg_ =
		RectangleShape(Vector2f(
			Settings::DefaultMapWidth - outlineThickness * 2,
			Settings::DefaultMapHeight - outlineThickness * 2));
	minimap_bg_.setPosition(outlineThickness, outlineThickness);
	minimap_bg_.setFillColor(Color(110, 110, 110, 220));
	minimap_bg_.setOutlineColor(Color::Black);
	minimap_bg_.setOutlineThickness(outlineThickness);

	shown_area_index_ = RectangleShape(Vector2f(0, 0));
	shown_area_index_.setOutlineColor(Color(255, 0, 0, 100));
	shown_area_index_.setOutlineThickness(outlineThickness);
	shown_area_index_.setFillColor(Color::Transparent);
	update_shown_area();
}

/// build the visual net
void Engine::set_visual_net(Vector2f size, float margin) {
	// visual_net viewPort setup


	visual_net_.reset(sf::FloatRect(0,
	                                0,
	                                Settings::DefaultMapWidth,
	                                Settings::DefaultMapHeight));
	visual_net_.setViewport(sf::FloatRect(
		margin,
		margin,
		size.x,
		size.y));

	/*
	float zoomFactor = Settings::DefaultMapWidth / size;
	visual_net_.zoom(zoomFactor);
	 */

	float outlineThickness = Settings::DefaultMapWidth / 200.f;

	// background setup
	visual_net_bg_ =
		RectangleShape(Vector2f(
			Settings::DefaultMapWidth - outlineThickness * 2.f,
			Settings::DefaultMapHeight - outlineThickness * 2.f));
	visual_net_bg_.setPosition(outlineThickness, outlineThickness);
	visual_net_bg_.setFillColor(Color(110, 110, 110, 220));
	visual_net_bg_.setOutlineColor(Color::Black);
	visual_net_bg_.setOutlineThickness(outlineThickness);

}

/// update camera after movement, and enforce overflow
void Engine::UpdateView(Vector2f posDelta, float newZoom) {
	// view_pos_ is position before dragging
	// t_view_pos_ is the current view position
	temp_view_pos_ =
		view_pos_ + posDelta * Settings::Zoom * Settings::DragFactor;

	// enforce overflow blocking
	if (!Settings::MapOverflow && map != nullptr)
	{
		if (abs(temp_view_pos_.x)
			> map->GetSize().x / 2 - shown_area_index_.getSize().x / 2)
		{
			temp_view_pos_.x =
				(map->GetSize().x / 2 - shown_area_index_.getSize().x / 2)
					* temp_view_pos_.x / abs(temp_view_pos_.x);
		}
		if (abs(temp_view_pos_.y)
			> map->GetSize().y / 2 - shown_area_index_.getSize().y / 2)
		{
			temp_view_pos_.y =
				(map->GetSize().y / 2 - shown_area_index_.getSize().y / 2)
					* temp_view_pos_.y / abs(temp_view_pos_.y);
		}
	}

	if (newZoom != 0)
	{
		Settings::Zoom = newZoom;
	}
	// set view
	set_view();
}

/// update the shown area index in minimap
void Engine::update_shown_area() {
	// calculate the actual position of the shown area
	Vector2f position = view_.getCenter();

	// to calculate shown area dimensions,
	// we multiply the map size by the zoom
	// view_size = map_size * zoom
	Vector2f size = view_.getSize();

	this->shown_area_index_.setSize(size);
	this->shown_area_index_.setOrigin(size / 2.f);
	this->shown_area_index_.setPosition(position);
}

/// build the snap grid
void Engine::BuildGrid(int rows, int cols) {

	snap_grid_.Lines.clear(); // clear the old lines list
	snap_grid_.Rows = rows;
	snap_grid_.Columns = cols;
	snap_grid_.ColumnWidth = int(map->GetSize().x) / snap_grid_.Columns;
	snap_grid_.RowHeight = int(map->GetSize().y) / snap_grid_.Rows;

	// create all vertical lines of the grid
	for (int i = 1; i < snap_grid_.Columns; i++)
	{
		Vertex *line = new Vertex[2];

		line[0] = sf::Vertex(sf::Vector2f(i * snap_grid_.ColumnWidth, 0));
		line[1] = sf::Vertex(sf::Vector2f(i * snap_grid_.ColumnWidth,
		                                  map->GetSize().y));

		snap_grid_.Lines.push_back(line);
	}
	// create all horizontal lines of snap grid
	for (int i = 1; i < snap_grid_.Rows; i++)
	{
		Vertex *line;
		line = new Vertex[2];

		line[0] = sf::Vertex(sf::Vector2f(0, i * snap_grid_.RowHeight));
		line[1] = sf::Vertex(sf::Vector2f(map->GetSize().x,
		                                  i * snap_grid_.RowHeight));

		snap_grid_.Lines.push_back(line);
	}
}

////////////////////////////////////////////////////////////
/// \brief
///
/// Draws a point in the clicked position on the canvas. if the snap_to_grid option is enabled,
/// it will snap the point to the grid.
///
/// \param position (Vector2f) - the click point
///
/// \return the snapped point if option enabled, else the point
///
////////////////////////////////////////////////////////////
Vector2f Engine::DrawPoint(Vector2f position) {
	// convert it to units according to screen pixel to display ratio
	position *= float(Settings::SFMLRatio);
	// convert it to world coordinates
	position = this->mapPixelToCoords(Vector2i(position), view_);

	// check if a lane exists in the choosen location
	check_selection(position);

	Vector2f temp;
	if (snap_to_grid_)
	{
		temp = GetSnappedPoint(position);
	} else
	{
		temp = position;
	}

	click_point_ = CircleShape(snap_grid_.ColumnWidth / 5);
	click_point_.setOrigin(click_point_.getRadius(), click_point_.getRadius());
	click_point_.setFillColor(Color::Red);
	click_point_.setPosition(temp.x, temp.y);

	return temp;
}


/// get simualtion by simulation number
Simulation *Engine::GetSimulation(int simulationNumber) {
	Simulation *temp = nullptr;

	for (Set *s : sets_)
	{
		if ((temp = s->GetSimulation(simulationNumber)) != nullptr)
		{
			return temp;
		}
	}

	return nullptr;
}

/// get set by set number
Set *Engine::GetSet(int setNumber) {
	for (Set *s: sets_)
	{
		if (s->GetSetNumber() == setNumber)
		{
			return s;
		}
	}

	return nullptr;
}

/// generate a grid-snapped point with a given point
Vector2f Engine::GetSnappedPoint(Vector2f point) {
	float x = 0, y = 0;

	if (int(point.x) % snap_grid_.ColumnWidth > snap_grid_.ColumnWidth / 2)
	{
		x = ceil(point.x / snap_grid_.ColumnWidth)
			* snap_grid_.ColumnWidth;
	} else
	{
		x = floor(point.x / snap_grid_.ColumnWidth)
			* snap_grid_.ColumnWidth;
	}

	if (int(point.y) % snap_grid_.RowHeight > snap_grid_.RowHeight / 2)
	{
		y = ceil(point.y / snap_grid_.RowHeight) * snap_grid_.RowHeight;
	} else
	{
		y = floor(point.y / snap_grid_.RowHeight) * snap_grid_.RowHeight;
	}

	return Vector2f(x, y);
}

/// check if a road was selected
void Engine::check_selection(Vector2f position) {
	// unselect current selection
	map->UnselectAll();

	if (Vehicle::SelectedVehicle != nullptr)
	{
		Vehicle::SelectedVehicle->Unselect();
	}
	Vehicle::SelectedVehicle = nullptr;

	// only check for lane selection if vehicle hasnt been selected
	if (Vehicle::CheckSelection(position) == nullptr)
	{
		Lane *temp = map->CheckSelection(position);

		if (temp != nullptr)
		{

			map->SelectedLane = temp;
			map->SelectedLane->Select();
		}
	} else // if vehicle has been selected, select its routes as well
	{
		list<Lane *> *ptr = Vehicle::SelectedVehicle->GetInstructionSet();
		Lane *currentLane;
		if ((currentLane = Vehicle::SelectedVehicle->GetCurrentLane())
			!= nullptr)
		{
			ptr->push_front(currentLane);
			map->SelectRoutesByVehicle(ptr);
			ptr->pop_front();
		}
	}
}

/// get use input, and make changes accordingly
void Engine::input() {
	// implementation of mouse dragging
	static bool dragging = false;
	static Vector2i startPos = Vector2i(0.0f, 0.0f);

	QPoint clickPoint = this->mapFromGlobal(QCursor::pos());
	Vector2i mousePos = Vector2i(clickPoint.x(), clickPoint.y());

	if (Mouse::isButtonPressed(sf::Mouse::Left)
		&& this->getViewport(view_).contains(mousePos))
	{
		if (!dragging)
			startPos = mousePos;
		dragging = true;
	} else
	{
		if (dragging)
		{
			view_pos_ = temp_view_pos_;
		}
		dragging = false;
	}

	if (dragging)
	{
		UpdateView(Vector2f(startPos.x - mousePos.x, startPos.y - mousePos.y));
	}
}

/// add a new set into the sim engine
Set *Engine::AddSet(int setNumber, int vehicleCount, int generations) {
	if (setNumber == 0)
	{
		setNumber = Set::SetCount + 1;
	}

	Set *set = new Set(setNumber, generations, vehicleCount);
	sets_.push_back(set);

	Set::SetCount++;
	number_of_sets_++;

	if (Settings::DrawAdded)
	{
		cout << "Set number " << setNumber << " added." << endl;
	}

	return set;
}

/// build a map using instructions from a given json file
void Engine::LoadMap(const string &loadDirectory) {
	// first, delete the old map.
	ResetMap();

	try
	{
		json j;
		// open the given file, read it to a json variable
		ifstream i(loadDirectory);
		i >> j;

		// build intersections
		for (auto data : j["intersections"])
		{
			map->AddIntersection(data["id"],
			                     Vector2f(data["position"][0],
			                              data["position"][1]));
		}

		// build connecting roads
		for (auto data : j["connecting_roads"])
		{
			map->AddConnectingRoad(data["id"],
			                       data["intersection_number"][0],
			                       data["intersection_number"][1]);
		}

		// build roads
		for (auto data : j["roads"])
		{
			map->AddRoad(data["id"],
			             data["intersection_number"],
			             data["connection_side"],
			             Settings::DefaultLaneLength);
		}

		for (auto data : j["lanes"])
		{
			map->AddLane(data["id"],
			             data["road_number"],
			             data["is_in_road_direction"]);
		}

		for (auto data : j["routes"])
		{
			map->AddRoute(data["from"], data["to"]);
		}

		for (auto data : j["cycles"])
		{
			int interId = data["attached_intersection_id"];
			map->AddCycle(data["id"], interId);
		}

		for (auto data : j["phases"])
		{
			map->AddPhase(data["id"], data["cycle_id"], data["cycle_time"]);
		}

		for (auto data : j["assigned_lanes"])
		{
			map->AssignLaneToPhase(data["phase_number"], data["lane_number"]);
		}

		for (auto data : j["lights"])
		{
			map->AddLight(data["id"],
			              data["phase_number"],
			              data["parent_lane_number"]);
		}

		cout << "map has been successfully loaded from '" << loadDirectory
		     << "'. "
		     << endl;
	}
	catch (const std::exception &e)
	{
		cout << "Could not load map from this directory." << endl;
		cout << e.what() << endl;
	}
}

/// save the current map to a json file
void Engine::SaveMap(const string &saveDirectory) {
	// first save intersections, then save connecting roads, then save roads, then save lanes
	json j;

	for (Intersection *inter : *map->GetIntersections())
	{
		j["intersections"].push_back(
			{
				{"id", inter->GetIntersectionNumber()},
				{"position", {inter->getPosition().x, inter->getPosition().y}}
			});
		for (Road *road : *inter->GetRoads())
		{
			// check if road is a connecting road
			if (!road->GetIsConnecting())
			{
				j["roads"].push_back(
					{
						{"id", road->GetRoadNumber()},
						{"intersection_number", road->GetIntersectionNumber()},
						{"connection_side", road->GetConnectionSide()}
					});

				for (Lane *lane : *road->GetLanes())
				{
					j["lanes"].push_back(
						{
							{"id", lane->GetLaneNumber()},
							{"road_number", lane->GetRoadNumber()},
							{"is_in_road_direction",
							 lane->GetIsInRoadDirection()}
						});
				}
			} else
			{
				// only save the connecting road once for the connected intersection
				if (inter->GetIntersectionNumber()
					== road->GetIntersectionNumber(0))
				{
					j["connecting_roads"].push_back(
						{
							{"id", road->GetRoadNumber()},
							{"intersection_number",
							 {road->GetIntersectionNumber(
								 0), road->GetIntersectionNumber(1)}}
						});

					for (Lane *lane : *road->GetLanes())
					{
						j["lanes"].push_back(
							{
								{"id", lane->GetLaneNumber()},
								{"road_number", lane->GetRoadNumber()},
								{"is_in_road_direction",
								 lane->GetIsInRoadDirection()}
							});
					}
				}
			}
		}
	}

	for (Route *route : *map->GetRoutes())
	{
		j["routes"].push_back(
			{
				{"from", route->FromLane->GetLaneNumber()},
				{"to", route->ToLane->GetLaneNumber()}
			}
		);
	}

	for (Cycle *cycle : *map->GetCycles())
	{
		j["cycles"].push_back(
			{
				{"id", cycle->GetCycleNumber()},
				{"attached_intersection_id",
				 (cycle->GetIntersection() != nullptr) ? cycle
					 ->GetIntersection()
					 ->GetIntersectionNumber() : 0}
			}
		);
		for (Phase *phase : *cycle->GetPhases())
		{
			j["phases"].push_back(
				{
					{"cycle_id", phase->GetCycleNumber()},
					{"id", phase->GetPhaseNumber()},
					{"cycle_time", phase->GetCycleTime()}
				}
			);

			for (Lane *lane : *phase->GetAssignedLanes())
			{
				j["assigned_lanes"].push_back(
					{
						{"phase_number", phase->GetPhaseNumber()},
						{"lane_number", lane->GetLaneNumber()}
					}
				);
			}

			for (Light *light : *phase->GetLights())
			{
				j["lights"].push_back(
					{
						{"id", light->GetLightNumber()},
						{"phase_number", light->GetPhaseNumber()},
						{"parent_lane_number",
						 light->GetParentLane()->GetLaneNumber()}
					}
				);
			}
		}
	}

	// write to file
	ofstream o(saveDirectory);
	o << setw(4) << j << endl;
	o.close();

	cout << "map saved to '" << saveDirectory << "' successfully." << endl;
}

/// save the neural net in a given directory in JSON file
void Engine::SaveNet(const string &saveDirectory) {
	if (Net::CurrentNet != nullptr)
		Net::BestNet.Save(saveDirectory);
}

/// load a neural network from a given JSON file
void Engine::LoadNet(const string &saveDirectory) {
	Net::Load(saveDirectory);
}

/// save the recent simulations to a file
void Engine::SaveSets(const string &saveDirectory) {

	json j;
	for (Set *set : sets_)
	{
		j["sets"].push_back(
			{
				{"id", set->GetSetNumber()},
				{"generation_simulated", set->GetGenerationsSimulated()},
				{"generation_count", set->GetGenerationsCount()},
				{"vehicle_count", set->GetVehicleCount()},
				{"start_time", static_cast<long int>(*set->GetStartTime())},
				{"end_time", static_cast<long int>(*set->GetEndTime())},
				{"progress", set->GetProgress()},
				{"finished", set->IsFinished()}

			}
		);

		for (Simulation *sim : *set->GetSimulations())
		{
			j["simulations"].push_back(
				{
					{"id", sim->GetSimulationNumber()},
					{"set_id", sim->GetSetNumber()},
					{"vehicle_count", sim->GetVehicleCount()},
					{"start_time", static_cast<long int>(*sim->GetStartTime())},
					{"end_time", static_cast<long int>(*sim->GetEndTime())},
					{"simulated_time", sim->GetElapsedTime()},
					{"result", sim->GetResult()},

				}
			);
		}
	}

	// write to file
	ofstream o(saveDirectory);
	o << setw(4) << j << endl;
	o.close();

	cout << "Set saved to '" << saveDirectory << "' successfully." << endl;
}

/// load simulations from a file
void Engine::LoadSets(const string &loadDirectory) {

	try
	{
		json j;
		// open the given file, read it to a json variable
		ifstream i(loadDirectory);
		i >> j;

		Set *s;
		for (auto data : j["sets"])
		{
			s = new Set(data["id"],
			            data["generation_count"],
			            data["vehicle_count"]);
			s->SetStartTime(time_t(data["start_time"]));
			s->SetEndTime(time_t(data["end_time"]));
			s->SetGenerationsSimulated(data["generation_simulated"]);
			s->SetProgress(data["progress"]);
			s->SetFinished(data["finished"]);
			sets_.push_back(s);
		}

		// build intersections
		Simulation *sim;
		for (auto data : j["simulations"])
		{
			s = GetSet(data["set_id"]);
			sim = s->AddSimulation(data["id"], data["vehicle_count"]);
			sim->SetStartTime(time_t(data["start_time"]));
			sim->SetEndTime(time_t(data["end_time"]));
			sim->SetSimulationTime(data["simulated_time"]);
			sim->SetFinished(true);
		}

		if (!sets_.empty())
		{
			cout << "sets have been successfully loaded from '" << loadDirectory
			     << "'. "
			     << endl;
		} else
		{
			throw std::exception();
		}
	}
	catch (const std::exception &e)
	{
		cout << "Could not load simulations from this directory." << endl;
		cout << e.what() << endl;
	}
}

/// reset the whole map, delete everything
void Engine::ResetMap() {

	ClearMap();

	cout << "Resetting the Neural Network..." << endl;
	if (Net::CurrentNet != nullptr)
		Net::CurrentNet->Reset();

	cout << "Resetting map..." << endl;
	delete map;
	map = new Map(0, Settings::DefaultMapWidth, Settings::DefaultMapWidth);

	cout << "======================= map has been reset ======================="
	     << endl;
}

/// stop the current simulation, and clear all vehicles
void Engine::ClearMap() {

	// clear all lanes;
	for (Lane *l : *map->GetLanes())
	{
		l->ClearLane();
	}

	for (Set *s : sets_)
	{
		s->StopSet();
	}

	cout << "Deleting Vehicles..." << endl;
	Vehicle::DeleteAllVehicles();

	cout << "====================== map has been cleared ======================"
	     << endl;
}

/// do the game cycle (input->update)
/// draw and display are seperate for different fps
/// this allows running logic cycle in high rate -> better accuracy
// and running draw cycle in low rate -> better performance
void Engine::logic_cycle() {

	input();
	update((float(logic_timer_.interval()) / 1000.f));
}

/// do the rest of the game cycle independently
void Engine::draw_cycle() {
	render();
	display();
}

/// update all the engine's objects
void Engine::update(float elapsedTime) {

	map->Update(elapsedTime);

	// deploy vehicles if needed
	if (Vehicle::VehiclesToDeploy > 0)
	{
		add_vehicles_with_delay(elapsedTime);
	}

	for (Vehicle *v : Vehicle::ActiveVehicles)
	{
		v->Update(elapsedTime);
	}

	//clear all cars to be deleted
	Vehicle::ClearVehicles();

	if (Settings::DrawFps)
		cout << "FPS : " << 1000.f / elapsedTime << endl;


	// follow the selected car
	if (Settings::FollowSelectedVehicle && Vehicle::SelectedVehicle != nullptr)
	{
		view_pos_ = Vehicle::SelectedVehicle->getPosition()
			- Vector2f(map->GetSize().x / 2, map->GetSize().y / 2);
		temp_view_pos_ = view_pos_;
		set_view();
	}

	for (Set *s : sets_)
	{
		// when an update on a set returns true
		// it means that a simulation has finished
		if (s->Update(elapsedTime))
		{
			if (s->IsFinished())
			{
				SetFinished();
			} else
			{
				SimulationFinished();

				// set the new score as result
				float result = s->GetLastSimulationResult();

				if (Settings::RunBestNet)
				{
					Net::BestNet.Update(elapsedTime);
				} else
				{

					Net::CurrentNet->SetScore(result);
					Net::CurrentNet->Update(elapsedTime);

					if (result > Net::HighScore)
					{
						Net::HighScore = result;
						Net::BestNet = *(Net::CurrentNet);
					}

					Net::CurrentNetIndex++;

					// check if generation is done
					if (Net::CurrentNetIndex == Net::PopulationSize)
					{
						// if is, create a new generation
						Net::NextGeneration();
					}

					Net::CurrentNet = &(Net::Generation[Net::CurrentNetIndex]);

					if (Settings::DrawNnProgression)
					{
						cout << "Gen no. " << Net::GenerationCount + 1
						     << " Net no. " << Net::CurrentNetIndex << "/"
						     << Net::PopulationSize
						     << " High Score: " << Net::HighScore << endl;
					}
				}
			}
		}
	}
}

/// deploy vehicles in a time controlled manner
void Engine::add_vehicles_with_delay(float elapsedTime) {
	static float totalElapsedTime = 0;

	totalElapsedTime += elapsedTime * Settings::Speed;

	if (totalElapsedTime > (Settings::VehicleSpawnRate))
	{
		AddVehicleRandomly();
		totalElapsedTime -= Settings::VehicleSpawnRate;
		Vehicle::VehiclesToDeploy--;
	}
}

/// add a vehicle at a random track
bool Engine::AddVehicleRandomly() {

	list<Lane *> *track = map->GenerateRandomTrack();

	if (track != nullptr && !track->empty())
	{

		int randomIndex = 0;

		if (Settings::MultiTypeVehicle)
			randomIndex = random() % 4;

		return (Vehicle::AddVehicle(track,
		                            this->map,
		                            static_cast<VehicleTypeOptions>(randomIndex))
			!= nullptr);
	} else
	{
		cout << "Could not add a new vehicle as tracks could not be generated."
		     << endl;
		return false;
	}
}

/// render the engine's objects
void Engine::render() {
	// Clean out the last frame
	clear(BackgroundColor);

	// Draw the map
	this->map->Draw(this);

	// Draw all vehicles
	for (Vehicle *v : Vehicle::ActiveVehicles)
	{
		// only draw active vehicles; stacked vehicles wont be rendered
		if (v->GetIsActive())
		{
			v->Draw(this);
		}
	}

	// Draw the click index
	if (Settings::DrawClickPoint)
		this->draw(this->click_point_);

	// Draw the grid
	if (Settings::DrawGrid)
	{
		for (Vertex *l : this->snap_grid_.Lines)
		{
			this->draw(l, 2, Lines);
		}
	}

	// draw the minimap
	if (Settings::DrawMinimap)
	{
		this->setView(minimap_); // switch to minimap for rendering
		render_minimap(); // render minimap
	}

	// draw the visual net
	if (Settings::DrawVisualNet)
	{
		this->setView(visual_net_); // switch to minimap for rendering
		render_visual_net(); // render minimap
	}

	this->setView(view_); // switch back to main view
}

/// drawing the minimap is drawing everything but the vehicles and the grid, on a smaller scale
void Engine::render_minimap() {
	// Draw the minimap's background
	this->draw(minimap_bg_);

	// Draw the map
	this->map->Draw(this);

	// Draw the click index
	if (Settings::DrawClickPoint)
		this->draw(this->click_point_);

	// Draw the shown area index
	this->draw(shown_area_index_);
}

/// render the visual net
void Engine::render_visual_net() {

	this->draw(visual_net_bg_);

	if (Settings::RunBestNet)
	{
		Net::BestNet.Draw(this);
	} else if (Net::CurrentNet != nullptr)
	{
		Net::CurrentNet->Draw(this);
	}
}