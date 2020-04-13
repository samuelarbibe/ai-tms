//
// Created by Samuel Arbibe on 03/03/2020.
//

#ifndef SIMULATORSFML_SRC_SIMULATOR_SIMULATION_HPP
#define SIMULATORSFML_SRC_SIMULATOR_SIMULATION_HPP

#include <iostream>
#include <fstream>
#include <list>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <QtWidgets>

#include "../../../public/json.hpp"
#include "Settings.hpp"
#include "Vehicle.hpp"

using namespace sf;
using json = nlohmann::json;

class Simulation
{
  public:

	Simulation(int simulationNumber, int setNumber, int vehicleCount = 1000);
	~Simulation();

	bool Update(float elapsedTime);
	void Run() {
		running_ = true;
		SimRunning = true;
		start_time_ = time(nullptr);
		Vehicle::VehiclesToDeploy = vehicle_count_;
	}
	void Demo() {
		running_ = true;
		DemoRunning = true;
		Vehicle::VehiclesToDeploy = vehicle_count_;
	}
	void PrintSimulationLog();
	void StopDemo() {
		finished_ = true;
		DemoRunning = false;
	}
	void StopSimulation() {
		finished_ = false;
		finished_ = true;
		SimRunning = false;
	}

	// get
	int GetSimulationNumber() { return simulation_number_; }
	int GetVehicleCount() { return vehicle_count_; }
	int GetSetNumber() { return set_number_; }
	float GetResult() { return result_; }
	int GetCurrentVehicleCount() { return current_vehicle_count_; }
	int IsFinished() { return finished_; }
	int IsRunning() { return running_; }

	time_t *GetStartTime() { return &start_time_; }
	time_t *GetEndTime() { return &end_time_; }
	float GetElapsedTime() { return elapsed_time_; }

	// set
	void SetStartTime(time_t time) { start_time_ = time; }
	void SetEndTime(time_t time) { end_time_ = time; }
	void SetSimulationTime(float time) { elapsed_time_ = time; }
	void SetFinished(bool fin) {
		finished_ = fin;
		running_ = false;
	}

	// The total count of all the simulations created this session
	static int SimulationCount;
	// Is a simulation currently active
	static bool SimRunning;
	// Is a demo of a simulation currently active
	static bool DemoRunning;

  private:

	// ID of this simulation
	int simulation_number_;
	// the set number of this simulation
	int set_number_;
	// Vehicles created in this simulation
	int vehicle_count_;
	// The Vehicles left in this simulation
	int current_vehicle_count_;
	// Is this simulation finished
	bool finished_;
	// Is this simulation active and running
	bool running_;
	// The start time of this simulation
	time_t start_time_;
	// The end time of this simulation
	time_t end_time_;
	// Time elapsed since simulation has begun
	float elapsed_time_;
	// The result of the simulation
	// vehicles per second
	float result_;

};

#endif //SIMULATORSFML_SRC_SIMULATOR_SIMULATION_HPP
