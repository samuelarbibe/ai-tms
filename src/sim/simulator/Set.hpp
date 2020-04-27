//
// Created by Samuel Arbibe on 06/04/2020.
//

#ifndef TMS_SRC_SIM_SIMULATOR_SET_HPP
#define TMS_SRC_SIM_SIMULATOR_SET_HPP

#include "Simulation.hpp"

using namespace std;
using namespace sf;

class Set
{
  public:

	explicit Set(int setNumber, int generationCount = 20, int vehicleCount = 1000);
	~Set();

	bool Update(float elapsedTime);
	void StopSet();
	void RunSet() {
		running_ = true;
		SetRunning = true;
		start_time_ = time(nullptr);
		Set::CurrentSet = set_number_;
	}
	bool DemoSimulation(int simulationNumber);
	Simulation *StartNewSimulation();
	Simulation *AddSimulation(int simulationNumber, int vehicleCount);
	bool DeleteSimulation(int simulationNumber);
	bool StopDemo();

	// set

	void SetStartTime(time_t time) { start_time_ = time; }
	void SetEndTime(time_t time) { end_time_ = time; }
	void SetFinished(bool finished) {
		finished_ = finished;
		running_ = false;
	}
	void SetGenerationsSimulated(int count) { generations_simulated_ = count; }
	void SetGenerationCount(int count) { generations_count_ = count; }
	void SetProgress(float progress) { progress_ = progress; }

	// get
	int GetSetNumber() const { return set_number_; }
	int GetNumberOfSimulations() const { return number_of_simulations_; }
	int GetGenerationsSimulated() const { return generations_simulated_; }
	int GetGenerationsCount() const { return generations_count_; }
	int GetVehicleCount() const { return vehicle_count_; }
	int GetRunning() { return running_; }
	float GetProgress() { return progress_; }
	time_t *GetStartTime() { return &start_time_; }
	time_t *GetEndTime() { return &end_time_; }
	bool IsFinished() { return finished_; }
	bool IsRunning() { return running_; }
	Simulation *GetSimulation(int simulationNumber);
	vector<Simulation *> *GetSimulations() { return &simulations_; }
	float GetLastSimulationResult() { return last_simulation_result_; }

	static int SetCount;
	static bool SetRunning;
	static int CurrentSet;

  private:

	// ID of this set
	int set_number_;
	// number of simulations in this set
	int number_of_simulations_;
	// number of simulations that have been done in this set
	int generations_simulated_;
	// number of simulation that have to be done in this set
	int generations_count_;
	// number of vehicles to run each simulation;
	int vehicle_count_;

	bool running_;

	bool finished_;
	// the generation progress.
	// calculated by gen_simulated_ / gen_count_
	float progress_;

	// The start time of this simulation
	time_t start_time_;
	// The end time of this simulation
	time_t end_time_;

	// simulations in this set
	vector<Simulation *> simulations_;
	// a pointer to the simulation that is currently being ran
	Simulation *running_simulation_;
	Simulation *running_demo_;

	float last_simulation_result_;
};

#endif //TMS_SRC_SIM_SIMULATOR_SET_HPP
