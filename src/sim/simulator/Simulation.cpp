//
// Created by Samuel Arbibe on 03/03/2020.
//

#include "Simulation.hpp"

int Simulation::SimulationCount = 0;
bool Simulation::SimRunning = false;
bool Simulation::DemoRunning = false;

Simulation::Simulation(int simulationNumber, int setNumber, int vehicleCount) {

	simulation_number_ = (simulationNumber != 0) ? simulationNumber : SimulationCount + 1;
	vehicle_count_ = vehicleCount;

	current_vehicle_count_ = 0;
	finished_ = false;
	running_ = false;
	set_number_ = setNumber;

	// start timer
	start_time_ = 0;
	end_time_ = 0;
	score_ = 0;

	elapsed_time_ = 0;
	++SimulationCount;
}

Simulation::~Simulation() {
	if (Settings::DrawDelete)
		cout << "Simulation number " << simulation_number_ << " has been deleted. " << endl;
}

// function returns true if simulation has ended
bool Simulation::Update(float elapsedTime) {

	if (running_)
	{
		if (!finished_)
			elapsed_time_ += elapsedTime * Settings::Speed;

		current_vehicle_count_ = Vehicle::GetActiveVehicleCount();

		if (current_vehicle_count_ == 0 && Vehicle::VehiclesToDeploy == 0)
		{
			running_ = false;
			finished_ = true;
			Simulation::SimRunning = false;
			Simulation::DemoRunning = false;

			// get simulation end time
			end_time_ = time(nullptr);

			score_ = float(vehicle_count_) / elapsed_time_;

			if (Settings::PrintSimulationLog)
			{
				PrintSimulationLog();
			}

			return true;
		}
	}
	return false;

}

void Simulation::PrintSimulationLog() {

	cout << "------------------------------------------------------------------" << endl;
	cout << "Set " << set_number_ << endl;
	cout << "Simulation " << simulation_number_ << " ended at ";
	cout << ctime(&this->end_time_) << endl;
	cout << "Simulation Type :";
	cout << " Vehicle Count" << endl;
	cout << "Results:" << endl;
	cout << "   Vehicles Simulated: " << vehicle_count_ << endl;
	cout << "   Simulation Time: " << elapsed_time_ << " seconds" << endl;
	cout << "   SCORE: " << score_ << endl;
	cout << "------------------------------------------------------------------" << endl;

}

