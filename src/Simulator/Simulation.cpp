//
// Created by Samuel Arbibe on 03/03/2020.
//

#include "Simulation.hpp"

int Simulation::SimulationCount = 0;
bool Simulation::SimRunning = false;
bool Simulation::DemoRunning = false;


Simulation::Simulation(int simulationNumber, int vehicleCount) {

	this->simulation_number_ = (simulationNumber != 0) ? simulationNumber : SimulationCount + 1;
	this->vehicle_count_ = vehicleCount;

	this->current_vehicle_count_ = 0;
	this->finished_ = false;
	this->running_ = false;

	// start timer
	this->start_time_ = time(nullptr);

	this->elapsed_time_ = 0;
	++SimulationCount;
}
Simulation::~Simulation()
{
	if(Settings::DrawDelete) cout << "Simulation number " << simulation_number_ << " has been deleted. " << endl;
}

// function returns true if simulation has ended
bool Simulation::Update(float elapsedTime) {

	if(running_)
	{
		if(!finished_)
			elapsed_time_ += elapsedTime * Settings::Speed;

		current_vehicle_count_ = Vehicle::GetActiveVehicleCount();

		if(current_vehicle_count_ == 0)
		{
			running_ = false;
			SimRunning = false;

			// if already finished this is a demo
			if(finished_)
			{
				cout << "------------------------------------------------------------------" << endl;
				cout << "Demo of simulation " << simulation_number_ << " ended" << endl;
				cout << "------------------------------------------------------------------" << endl;

				DemoRunning = false;
			}
			else
			{

				// get simulation end time
				this->end_time_ = time(nullptr);

				cout << "------------------------------------------------------------------" << endl;
				cout << "Simulation " << simulation_number_ << " ended at ";
				cout << ctime(&this->end_time_) << endl;
				cout << "Results:" << endl;
				cout << "   Vehicles Simulated: " << vehicle_count_ << endl;
				cout << "   Simulation Time: " << elapsed_time_ << " seconds" << endl;
				cout << "------------------------------------------------------------------" << endl;

				finished_ = true;
			}

			// send a signal that simulation just finished.
			return true;
		}
	}
	return false;
}
