//
// Created by Samuel Arbibe on 06/04/2020.
//

#include "Set.hpp"

int Set::SetCount = 0;
bool Set::SetRunning = false;

Set::Set(int setNumber, int generationCount, int vehicleCount) {

	set_number_ = setNumber;
	generations_count_ = generationCount;
	generations_simulated_ = 0;
	vehicle_count_ = vehicleCount;
	running_simulation_ = nullptr;
	running_demo_ = nullptr;
	number_of_simulations_ = 0;
	progress_ = 0;
	running_ = false;
	finished_ = false;

	start_time_ = 0;
	end_time_ = 0;
}

Set::~Set() {

}

Simulation *Set::GetSimulation(int simulationNumber) {

	for (Simulation *s : simulations_)
	{
		if (s->GetSimulationNumber() == simulationNumber)
		{
			return s;
		}
	}
	return nullptr;
}

bool Set::Update(float elapsedTime) {

	if (running_)
	{
		//update set progress
		progress_ = float(generations_simulated_) / float(generations_count_);

		// if set is not finished
		if (generations_simulated_ < generations_count_)
		{
			// is there a simulation running ?
			if (running_simulation_ != nullptr)
			{
				running_simulation_->Update(elapsedTime);

				// check if simulation has finished
				if (running_simulation_->IsFinished())
				{
					last_simulation_result_ = running_simulation_->GetResult();

					running_simulation_ = nullptr;
					++generations_simulated_;
					return true;
				}
			} else
			{
				// start a new simulation and run it
				running_simulation_ = StartNewSimulation();
			}
		} else
		{
			running_ = false;
			finished_ = true;
			SetRunning = false;
			end_time_ = time(nullptr);
			return true;
		}
	}

	if (running_demo_ != nullptr)
	{
		running_demo_->Update(elapsedTime);
		if (running_demo_->IsFinished())
		{
			running_demo_ = nullptr;
		}
	}

	return false;
}

void Set::StopSet() {

	running_ = false;
	SetRunning = false;

	if (running_simulation_ != nullptr)
	{
		running_simulation_->StopSimulation();
		DeleteSimulation(running_simulation_->GetSimulationNumber());
		running_simulation_ = nullptr;
	}

	if (running_demo_ != nullptr)
	{
		running_demo_->StopDemo();
		running_demo_ = nullptr;
	}
}

bool Set::DeleteSimulation(int simulationNumber) {
	auto it = simulations_.begin();
	while (it != simulations_.end())
	{
		if ((*it)->GetSimulationNumber() == simulationNumber)
		{
			it = simulations_.erase(it);
			if ((*it)->IsFinished())
				generations_simulated_--;
			number_of_simulations_--;
			generations_count_--;

			delete (*it);

			return true;
		} else
		{
			it++;
		}
	}

	cout << "Could not delete simulation as it wasn't found. " << endl;
	return false;
}

Simulation *Set::StartNewSimulation() {

	// start a new simulation
	Simulation *s = new Simulation(0, set_number_, vehicle_count_);

	simulations_.push_back(s);
	number_of_simulations_++;

	s->Run();

	return s;
}

Simulation *Set::AddSimulation(int simulationNumber, int vehicleCount) {
	if (simulationNumber == 0)
	{
		simulationNumber = Simulation::SimulationCount + 1;
	}

	Simulation *s = new Simulation(simulationNumber, set_number_, vehicleCount);
	simulations_.push_back(s);

	number_of_simulations_++;
	Simulation::SimulationCount++;

	if (Settings::DrawAdded)
	{
		cout << "Simulation " << s->GetSimulationNumber() << " added." << endl;
	}

	return s;
}

bool Set::DemoSimulation(int simulationNumber) {
	if (!SetRunning)
	{
		Simulation *s = GetSimulation(simulationNumber);

		if (s != nullptr)
		{
			running_demo_ = s;
			s->Demo();
			return true;
		}
	}
	cout << "Cannot run demo as set is currently running." << endl;
	return false;
}

bool Set::StopDemo() {
	if (running_demo_ != nullptr)
	{
		running_demo_->StopDemo();
		running_demo_ = nullptr;
		return true;
	}
	return false;
}
