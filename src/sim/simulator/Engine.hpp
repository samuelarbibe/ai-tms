//
//  Engine.hpp
//  SimulatorSFML
//
//  Created by Samuel Arbibe on 22/11/2019.
//  Copyright © 2019 Samuel Arbibe. All rights reserved.
//

#ifndef Engine_hpp
#define Engine_hpp

#include <iostream>
#include <fstream>
#include <ctime>
#include <list>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <QtWidgets>
#include <nlohmann/json.hpp>

#include "src/sim/map/Map.hpp"
#include "Vehicle.hpp"
#include "Settings.hpp"
#include "src/ui/widgets/QsfmlCanvas.hpp"
#include "public/VariadicTable.h"
#include "src/sim/map/Route.hpp"
#include "Set.hpp"


using namespace sf;
using json = nlohmann::json;

struct Grid
{
	int Columns;
	int Rows;
	int ColumnWidth;
	int RowHeight;
	list<Vertex *> Lines;
};

class Engine : public QSFMLCanvas
{

  Q_OBJECT
  public:

	Engine(QWidget *Parent);
	~Engine() {};

	bool RunSet(int vehicleCount = 1000, int generations = 10);
	bool RunDemo(int simulationNumber);
	Set * AddSet(int setNumber, int vehicleCount, int generations);


	// get
	Vector2f GetSnappedPoint(Vector2f point);
	Vector2f DrawPoint(Vector2f position);
	vector<Simulation *> *GetSimulations();
	Simulation *GetSimulation(int simulationNumber);
	Set * GetSet(int setNumber);

	// set
	void SetSnapToGrid(bool snapToGrid) { this->snap_to_grid_ = snapToGrid; }
	void SetView();
	void BuildGrid(int rows, int cols);

	void UpdateView(Vector2f posDelta = Vector2f(0, 0), float zoom = 0);
	void SaveMap(string saveDirectory);
	void LoadMap(string loadDirectory);
	void SaveSets(string saveDirectory);
	void LoadSets(string loadDirectory);
	void ResetMap();
    void ClearMap();
	bool AddVehicleRandomly();
	void ResizeFrame(QSize size);
	bool DeleteSimulation(int simulationNumber);

	Map *map;

  signals:

	void SimulationFinished();
	void SetFinished();

  private:

	void on_init() override ;

	void logic_cycle() override;
	void draw_cycle() override;
	void render();
	void input();


	void render_minimap();
	void update_shown_area();
	void update(float elapsedTime);
	void add_vehicles_with_delay(float elapsedTime);
	void check_selection(Vector2f position);
	void set_minimap(float size, float margin);

	// Snap click points to set grid
	bool snap_to_grid_;
	Grid snap_grid_;

	// The simulator camera
	View view_;
	// The minimap camera
	View minimap_;

	// The actual view position
	Vector2f view_pos_;
	// The current view position while dragging mouse
	Vector2f temp_view_pos_;

	RectangleShape minimap_bg_;
	RectangleShape shown_area_index_;
	CircleShape click_point_;


	int number_of_sets_;
	// an array of simulation sets
	vector<Set*> sets_;
};

#endif /* Engine_hpp */