//
// Created by Samuel Arbibe on 03/03/2020.
//

#include "Simulation.hpp"

int Simulation::SimulationCount = 0;
bool Simulation::SimRunning = false;


Simulation::Simulation(int simulationNumber, int vehicleCount) {

	this->simulation_number_ = (simulationNumber != 0) ? simulationNumber : ++SimulationCount;

	this->vehicle_count_ = vehicleCount;

	this->current_vehicle_count_ = 0;
	this->finished_ = false;
	this->running_ = false;

	// start timer
	time_t now = time(nullptr);
	this->start_time_ = localtime(&now);

	this->elapsed_time_ = 0;
}
Simulation::~Simulation()
{
	if(Settings::DrawDelete) cout << "Simulation number " << simulation_number_ << " has been deleted. " << endl;
}

void Simulation::Update(float elapsedTime) {

	if(running_)
	{
		elapsed_time_ += elapsedTime * Settings::Speed;

		current_vehicle_count_ = Vehicle::GetActiveVehicleCount();

		if(current_vehicle_count_ == 0)
		{
			running_ = false;
			finished_ = false;

			// get simulation end time
			time_t now = time(nullptr);
			this->end_time_ = localtime(&now);

			SimRunning = false;

			cout << "------------------------------------------------------------------" << endl;
			cout << "Simulation "<< simulation_number_ << " ended at ";
			cout << 1 + end_time_->tm_hour << ":";
			cout << 1 + end_time_->tm_min << ":";
			cout << 1 + end_time_->tm_sec << endl;
			cout << "Results:" <<  endl;
			cout << "   Vehicles Simulated: " << vehicle_count_ << endl;
			cout << "   Simulation Time: " << elapsed_time_ << " seconds" <<  endl;
			cout << "------------------------------------------------------------------" << endl;
		}
	}
}
