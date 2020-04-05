//
// Created by Samuel Arbibe on 03/03/2020.
//

#include "Simulation.hpp"

int Simulation::SimulationCount = 0;
bool Simulation::SimRunning = false;
bool Simulation::DemoRunning = false;

Simulation::Simulation(int simulationNumber, int vehicleCount, float runningTime) {

	this->simulation_number_ = (simulationNumber != 0) ? simulationNumber : SimulationCount + 1;
	this->vehicle_count_ = vehicleCount;

	this->current_vehicle_count_ = 0;
	this->finished_ = false;
	this->running_ = false;

	this->is_timing_sim_ = bool(runningTime);
	this->running_time_ = runningTime;

	// start timer
	this->start_time_ = time(nullptr);
	this->end_time_ = 0;

	this->elapsed_time_ = 0;
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

		if ((is_timing_sim_ && elapsed_time_ >= running_time_) || (!is_timing_sim_ && current_vehicle_count_ == 0 && Vehicle::VehiclesToDeploy == 0))
		{
			running_ = false;
			SimRunning = false;

			// if already finished this is a demo
			if (finished_)
			{
				cout << "------------------------------------------------------------------" << endl;
				cout << "Demo of simulation " << simulation_number_ << " ended" << endl;
				cout << "------------------------------------------------------------------" << endl;

				DemoRunning = false;
			} else
			{

				// get simulation end time
				this->end_time_ = time(nullptr);

				if (is_timing_sim_)
				{
					vehicle_count_ = vehicle_count_ - current_vehicle_count_;
					elapsed_time_ = running_time_;
				}

				cout << "------------------------------------------------------------------" << endl;
				cout << "Simulation " << simulation_number_ << " ended at ";
				cout << ctime(&this->end_time_) << endl;
				cout << "Simulation Type :";
				if(is_timing_sim_)
				{
					cout << " Limited Time" << endl;
				} else
				{
					cout << " Vehicle Count" << endl;
				}
				cout << "Results:" << endl;
				cout << "   Vehicles Simulated: " << vehicle_count_ << endl;
				cout << "   Simulation Time: " << elapsed_time_ << " seconds" << endl;
				cout << "------------------------------------------------------------------" << endl;

				finished_ = true;
				result_ = float(vehicle_count_) / elapsed_time_;
			}

			// send a signal that simulation just finished.
			Vehicle::VehiclesToDeploy = 0;
			return true;
		}
	}
	return false;

}
