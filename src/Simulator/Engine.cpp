//
//  Engine.cpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#include "Engine.hpp"

Engine::Engine(QWidget *Parent) : QSFMLCanvas(Parent, 1000.f / Settings::Interval, 1000.f / Settings::Fps) {

	cout << "Setting Up Map..." << endl;
	map = new Map(0, Settings::DefaultMapWidth, Settings::DefaultMapHeight);
 
	cout << "Setting Up Camera..." << endl;
	snap_to_grid_ = true;
	view_pos_ = Vector2f(0, 0);
	temp_view_pos_ = Vector2f(0, 0);
	SetView();
	SetMinimap(Settings::MinimapSize, Settings::MinimapMargin);
	this->setView(view_);

	number_of_simulations_ = 0;

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

	map->AddPhase(0, 20);
	map->AddPhase(0, 20);
	map->AddPhase(0, 20);
	map->AddPhase(0, 20);

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

	SetView();
	SetMinimap(Settings::MinimapSize, Settings::MinimapMargin);
	this->setView(view_);
}

/// delete a simulation
bool Engine::DeleteSimulation(int simulationNumber) {

	Simulation *s = GetSimulation(simulationNumber);

	if (s != nullptr)
	{
		// remove the targetLane from the list by iterator
		auto it = simulations_.begin();
		while (it != simulations_.end())
		{
			if ((*it)->GetSimulationNumber() == simulationNumber)
			{
				it = simulations_.erase(it);
				delete s;
				number_of_simulations_--;
				return true;
			} else
			{
				it++;
			}
		}
	}

	cout << "Could not delete simulation as it wasnt found. " << endl;
	return false;
}

/// run a simualtion on the given vehicle count
void Engine::RunSimulation(int vehicleCount, float runningTime) {

	if (!Simulation::SimRunning)
	{
		// if demo is running, stop it
		if (Simulation::DemoRunning)
		{
			ClearMap();
		}

		cout << "Running Simulation on this map..." << endl;

		if(runningTime != 0)
			cout << "Timing set at " << runningTime << " seconds..." << endl;
		else
			cout << "Sending " << vehicleCount << " vehicles..." << endl;

		Vehicle::VehiclesToDeploy = vehicleCount;

		// start a new simulation
		Simulation *s = new Simulation(0, vehicleCount, runningTime);


		simulations_.push_back(s);
		number_of_simulations_ = simulations_.size();

		s->Run();

		cout << "------------------------------------------------------------------" << endl;
		cout << "Simulation " << s->GetSimulationNumber() << " started at ";
		cout << ctime(s->GetStartTime());
		cout << "------------------------------------------------------------------" << endl;
	} else
	{
		cout << "Another simulation is running, please wait for it to finish." << endl;
	}
}

/// re-run a simualtion by sim-number, without calculating it as a simulation
void Engine::RunDemo(int simulationNumber) {
	demo_simulation_ = GetSimulation(simulationNumber);

	if (demo_simulation_ != nullptr)
	{
		if (!Simulation::SimRunning)
		{
			// if demo is running, stop it
			if (Simulation::DemoRunning)
			{
				ClearMap();
			}

			int vehicleCount = demo_simulation_->GetVehicleCount();

			Vehicle::VehiclesToDeploy = vehicleCount;

			demo_simulation_->Run(true);

			cout << "------------------------------------------------------------------" << endl;
			cout << "Demo of simulation " << demo_simulation_->GetSimulationNumber() << " started" << endl;
			cout << "------------------------------------------------------------------" << endl;
		} else
		{
			cout << "A simulation is already running. Abort the current simulation to run a demo." << endl;
		}
	} else
	{
		cout << "Could not demo simualtion as it wasnt found." << endl;
	}
}

/// set the viewport for the camera
void Engine::SetView() {
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
void Engine::SetMinimap(float size, float margin) {
	// minimap viewPort setup
	minimap_.reset(sf::FloatRect(0, 0, Settings::DefaultMapWidth, Settings::DefaultMapHeight));
	minimap_.setSize(Vector2f(size, size));
	minimap_.setViewport(sf::FloatRect(
		1.f - (size / this->width()) - (margin / this->width()),
		margin / this->height(),
		size / this->width(),
		size / this->height()));
	float zoomFactor = Settings::DefaultMapWidth / size;
	minimap_.zoom(zoomFactor);
	float outlineThickness = 30;

	// background setup
	minimap_bg_ = RectangleShape(Vector2f(Settings::DefaultMapWidth - outlineThickness * 2,
	                                      Settings::DefaultMapHeight - outlineThickness * 2));
	minimap_bg_.setPosition(outlineThickness, outlineThickness);
	minimap_bg_.setFillColor(Color(110, 110, 110, 220));
	minimap_bg_.setOutlineColor(Color::Black);
	minimap_bg_.setOutlineThickness(outlineThickness);

	shown_area_index_ = RectangleShape(Vector2f(0, 0));
	shown_area_index_.setOutlineColor(Color(255, 0, 0, 100));
	shown_area_index_.setOutlineThickness(30.f);
	shown_area_index_.setFillColor(Color::Transparent);
	update_shown_area();
}

/// update camera after movement, and enforce overflow
void Engine::UpdateView(Vector2f posDelta, float newZoom) {
	// view_pos_ is position before dragging
	// t_view_pos_ is the current view position
	temp_view_pos_ = view_pos_ + posDelta;

	// enforce overflow blocking
	if (!Settings::MapOverflow && map != nullptr)
	{
		if (abs(temp_view_pos_.x) > map->GetSize().x / 2 - shown_area_index_.getSize().x / 2)
		{
			temp_view_pos_.x =
				(map->GetSize().x / 2 - shown_area_index_.getSize().x / 2) * temp_view_pos_.x / abs(temp_view_pos_.x);
		}
		if (abs(temp_view_pos_.y) > map->GetSize().y / 2 - shown_area_index_.getSize().y / 2)
		{
			temp_view_pos_.y =
				(map->GetSize().y / 2 - shown_area_index_.getSize().y / 2) * temp_view_pos_.y / abs(temp_view_pos_.y);
		}
	}

	if (newZoom != 0)
	{
		Settings::Zoom = newZoom;
	}
	// set view
	SetView();
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
	snap_grid_.ColumnWidth = map->GetSize().x / snap_grid_.Columns;
	snap_grid_.RowHeight = map->GetSize().y / snap_grid_.Rows;

	// create all vertical lines of the grid
	for (int i = 1; i < snap_grid_.Columns; i++)
	{
		Vertex *line = new Vertex[2];

		line[0] = sf::Vertex(sf::Vector2f(i * snap_grid_.ColumnWidth, 0));
		line[1] = sf::Vertex(sf::Vector2f(i * snap_grid_.ColumnWidth, map->GetSize().y));

		snap_grid_.Lines.push_back(line);
	}
	// create all horizontal lines of snap grid
	for (int i = 1; i < snap_grid_.Rows; i++)
	{
		Vertex *line = new Vertex[2];

		line[0] = sf::Vertex(sf::Vector2f(0, i * snap_grid_.RowHeight));
		line[1] = sf::Vertex(sf::Vector2f(map->GetSize().x, i * snap_grid_.RowHeight));

		snap_grid_.Lines.push_back(line);
	}
}

/// display a given point on the map
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

	click_point_ = CircleShape(snap_grid_.ColumnWidth / 5.f);
	click_point_.setOrigin(click_point_.getRadius(), click_point_.getRadius());
	click_point_.setFillColor(Color::Red);
	click_point_.setPosition(temp.x, temp.y);

	return temp;
}

vector<Simulation *> *Engine::GetSimulations()
{
	if(Settings::DrawSimTable)
	{

		VariadicTable<int, string, string, float, int> vt({"ID", "Start Time", "End Time", "Simulated Time", "Vehicle Count"});

		for(Simulation * s : simulations_)
		{
			vt.addRow(s->GetSimulationNumber(), ctime(s->GetStartTime()), ctime(s->GetEndTime()), s->GetElapsedTime(), s->GetVehicleCount());
		}

		vt.print(std::cout);
	}

	return &simulations_;
}


/// get simualtion by simulation number
Simulation *Engine::GetSimulation(int simulationNumber) {
	for (Simulation *s : simulations_)
	{
		if (s->GetSimulationNumber() == simulationNumber)
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
		x = int(ceil(point.x / snap_grid_.ColumnWidth)) * snap_grid_.ColumnWidth;
	} else
	{
		x = int(floor(point.x / snap_grid_.ColumnWidth)) * snap_grid_.ColumnWidth;
	}

	if (int(point.y) % snap_grid_.RowHeight > snap_grid_.RowHeight / 2)
	{
		y = int(ceil(point.y / snap_grid_.RowHeight)) * snap_grid_.RowHeight;
	} else
	{
		y = int(floor(point.y / snap_grid_.RowHeight)) * snap_grid_.RowHeight;
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
		if ((currentLane = Vehicle::SelectedVehicle->GetCurrentLane()) != nullptr)
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

	if (Mouse::isButtonPressed(sf::Mouse::Left) && this->getViewport(view_).contains(mousePos))
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

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		map->GetLane(1)->SetIsBlocked(true);
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		map->GetLane(1)->SetIsBlocked(false);
	}
}

/// build a map using instructions from a given json file
void Engine::LoadMap(const string loadDirectory) {
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
			map->AddIntersection(data["id"], Vector2f(data["position"][0], data["position"][1]));
		}

		// build connecting roads
		for (auto data : j["connecting_roads"])
		{
			map->AddConnectingRoad(data["id"], data["intersection_number"][0], data["intersection_number"][1]);
		}

		// build roads
		for (auto data : j["roads"])
		{
			map->AddRoad(data["id"], data["intersection_number"], data["connection_side"], Settings::DefaultLaneLength);
		}

		for (auto data : j["lanes"])
		{
			map->AddLane(data["id"], data["road_number"], data["is_in_road_direction"]);
		}

		for (auto data : j["routes"])
		{
			map->AddRoute(data["from"], data["to"]);
		}

		for (auto data : j["phases"])
		{
			map->AddPhase(data["id"], data["cycle_time"]);
		}

		for (auto data : j["assigned_lanes"])
		{
			map->AssignLaneToPhase(data["phase_number"], data["lane_number"]);
		}

		for (auto data : j["lights"])
		{
			map->AddLight(data["id"], data["phase_number"], data["parent_road_number"]);
		}

		cout << "Map has been successfully loaded from '" << loadDirectory << "'. " << endl;
	}
	catch (const std::exception &e)
	{
		cout << "Could not load map from this directory." << endl;
		cout << e.what() << endl;
	}
}

/// save the current map to a json file
void Engine::SaveMap(const string saveDirectory) {
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
							{"is_in_road_direction", lane->GetIsInRoadDirection()}
						});
				}
			} else
			{
				// only save the connecting road once for the connected intersection
				if (inter->GetIntersectionNumber() == road->GetIntersectionNumber(0))
				{
					j["connecting_roads"].push_back(
						{
							{"id", road->GetRoadNumber()},
							{"intersection_number", {road->GetIntersectionNumber(
								0), road->GetIntersectionNumber(1)}}
						});

					for (Lane *lane : *road->GetLanes())
					{
						j["lanes"].push_back(
							{
								{"id", lane->GetLaneNumber()},
								{"road_number", lane->GetRoadNumber()},
								{"is_in_road_direction", lane->GetIsInRoadDirection()}
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

	for (Phase *phase : *map->GetPhases())
	{
		j["phases"].push_back(
			{
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
					{"parent_road_number", light->GetParentRoad()->GetRoadNumber()}
				}
			);
		}
	}

	// write to file
	ofstream o(saveDirectory);
	o << setw(4) << j << endl;
	o.close();

	cout << "Map saved to '" << saveDirectory << "' successfully." << endl;
}

/// save the recent simulations to a file
void Engine::SaveSimulations(string saveDirectory) {

	json j;
	for (Simulation *sim : simulations_)
	{
		j["simulations"].push_back(
			{
				{"id", sim->GetSimulationNumber()},
				{"vehicle_count", sim->GetVehicleCount()},
				{"start_time", static_cast<long int>(*sim->GetStartTime())},
				{"end_time", static_cast<long int>(*sim->GetEndTime())},
				{"simulated_time", sim->GetElapsedTime()}
			}
		);
	}

	// write to file
	ofstream o(saveDirectory);
	o << setw(4) << j << endl;
	o.close();

	cout << "Simulation saved to '" << saveDirectory << "' successfully." << endl;
}

/// load simulations from a file
void Engine::LoadSimulations(string loadDirectory) {

	try
	{
		json j;
		// open the given file, read it to a json variable
		ifstream i(loadDirectory);
		i >> j;

		Simulation *s;
		// build intersections
		for (auto data : j["simulations"])
		{
			s = new Simulation(data["id"], data["vehicle_count"]);
			s->SetStartTime(time_t(data["start_time"]));
			s->SetEndTime(time_t(data["end_time"]));
			s->SetSimulationTime(data["simulated_time"]);
			s->SetFinished(true);
			simulations_.push_back(s);
		}

		if (simulations_.size() > 0)
		{
			cout << "simulations have been successfully loaded from '" << loadDirectory << "'. " << endl;
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

	cout << "Saving Simulations..." << endl;
	//SaveSimulations("../../");

	cout << "Deleting Vehicles..." << endl;
	Vehicle::DeleteAllVehicles();

	cout << "Resetting Map..." << endl;
	delete map;
	map = new Map(0, Settings::DefaultMapWidth, Settings::DefaultMapWidth);

	cout << "======================= Map has been reset =======================" << endl;
}

/// stop the current simulation, and clear all vehicles
void Engine::ClearMap() {

	// clear all lanes;
	for(Lane * l : map->GetLanes())
	{
		l->ClearLane();
	}

	if (Simulation::DemoRunning)
	{
		cout << "Stopping demo of simulation " << demo_simulation_->GetSimulationNumber() << endl;
		demo_simulation_->SetFinished(true);
		demo_simulation_ = nullptr;
		Simulation::DemoRunning = false;
		Vehicle::DeleteAllVehicles();

	} else if (Simulation::SimRunning)
	{
		cout << "Stopping running simulations..." << endl;
		for (auto it = simulations_.begin(); it != simulations_.end();)
		{
			if ((*it)->IsRunning())
			{
				Simulation *temp = (*it);
				it = simulations_.erase(it);

				Simulation::SimRunning = false;

				cout << "Simulation number " << temp->GetSimulationNumber() << " stopped and deleted." << endl;

				delete temp;

			} else
			{
				it++;
			}
		}

		cout << "Deleting Vehicles..." << endl;
		Vehicle::DeleteAllVehicles();

		cout << "====================== Map has been cleared ======================" << endl;
	} else
	{
		cout << "No simulations are currently active" << endl;
	}
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
void Engine::draw_cycle()
{
	render();
	display();
}

/// update all the engine's objects
void Engine::update(float elapsedTime) {

	map->Update(elapsedTime);

	// deploy vehicles if needed
	if(Vehicle::VehiclesToDeploy > 0)
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
		view_pos_ = Vehicle::SelectedVehicle->GetPosition() - Vector2f(map->GetSize().x / 2, map->GetSize().y / 2);
		temp_view_pos_ = view_pos_;
		SetView();
	}

	for (Simulation *s : simulations_)
	{
		if (s->IsRunning())
		{
			// update, and check if simulation has ended
			if (s->Update(elapsedTime))
			{
				// send a signal that simulation has ended
				Vehicle::DeleteAllVehicles();
				SimulationFinished();
			}
		}
	}
}

/// deploy vehicles in a time controlled manner
void Engine::add_vehicles_with_delay(float elapsedTime)
{
	static float totalElapsedTime = 0;

	totalElapsedTime += elapsedTime;

	if(totalElapsedTime > (Settings::VehicleSpawnRate / Settings::Speed))
	{
		AddVehicleRandomly();
		totalElapsedTime -= Settings::VehicleSpawnRate / Settings::Speed;
		Vehicle::VehiclesToDeploy --;
	}
}

/// add a vehicle at a random track
bool Engine::AddVehicleRandomly() {
	// find a random starting point
	Lane *l = map->GetPossibleStartingLane();
	if (l == nullptr)
	{
		cout << "no starting lanes available." << endl;
		return false;
	}
	// find a starting route from starting lane
	Route *r = map->GetPossibleRoute(l->GetLaneNumber());

	if (r == nullptr)
	{
		cout << "no routes available. please add them to the map" << endl;
		return false;
	}

	// while new routes to append are available
	// new routes will be searched starting from the previous route end
	list<Lane *> *tempQueue = new list<Lane *>();
	Lane *lastLane = nullptr;

	while (r != nullptr)
	{
		tempQueue->push_back(r->FromLane);
		lastLane = r->ToLane;
		r = map->GetPossibleRoute(r->ToLane->GetLaneNumber());
	}
	if(lastLane != nullptr)
	{
		tempQueue->push_back(lastLane);
	}

	int randomIndex = 0;

	if(Settings::MultiTypeVehicle)
		randomIndex = rand() % 4;

	return (Vehicle::AddVehicle(tempQueue, this->map, static_cast<VehicleTypeOptions>(randomIndex)) != nullptr);
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






