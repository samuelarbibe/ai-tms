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
#include <nlohmann/json.hpp>
#include "Settings.hpp"
#include "Vehicle.hpp"


using namespace sf;
using json = nlohmann::json;

class Simulation
{
  public:

	Simulation(int simulationNumber, int vehicleCount = 1000, float runningTime = 0);
	~Simulation();

	bool Update(float elapsedTime);
	void Run(bool demo = false){running_ = true; if(demo) DemoRunning = true; else SimRunning = true;}

	// get
	int GetSimulationNumber(){return simulation_number_;}
	int GetVehicleCount(){return vehicle_count_;}
	int GetCurrentVehicleCount(){return current_vehicle_count_;}
	int IsFinished(){return finished_;}
	int IsRunning(){return running_;}

	time_t * GetStartTime(){return &start_time_;}
	time_t * GetEndTime(){return &end_time_;}
	float GetElapsedTime(){return elapsed_time_;}

	// set
	void SetStartTime(time_t time){ start_time_ = time;}
	void SetEndTime(time_t time){ end_time_ = time;}
	void SetSimulationTime(float time){ elapsed_time_ = time;}
	void SetFinished(bool fin){ finished_ = fin; running_ = false;}

	static int SimulationCount;
	static bool SimRunning;
	static bool DemoRunning;


  private:

	int simulation_number_;
    int vehicle_count_;
    int current_vehicle_count_;
    bool finished_;
    bool running_;

    // a simulation can either be a timed simulation, or a vehicle-count simulation
    // TIMED-SIM : the sim will be ran for a given amount of time, and the result is the amount of cars that passed the simulation.
    // COUNT-SIM : the sim will send a given amount of cars, and the result is the time it took to run all of these vehicles.
    bool is_timing_sim_;
    float ran_vehicles_;

    time_t start_time_;
    time_t end_time_;
    float  elapsed_time_;
    float  running_time_;
};

#endif //SIMULATORSFML_SRC_SIMULATOR_SIMULATION_HPP
