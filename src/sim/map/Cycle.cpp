//
// Created by Samuel Arbibe on 06/04/2020.
//

#include "Cycle.hpp"

int Cycle::CycleCount = 0;

/// a compare function to copmare phases priority
bool compare_priority(Phase *first, Phase *second) {
	return (first->GetPriorityScore() < second->GetPriorityScore());
}

Cycle::Cycle(int cycleNumber, Intersection *intersection) {
	cycle_number_ = cycleNumber;
	intersection_ = intersection;
	number_of_phases_ = 0;

	input_values_ = vector<double>(2, 0);
	output_values_ = vector<double>(1, 0);
}

Cycle::~Cycle() {
	for (Phase *p : phases_)
	{
		delete p;
	}

	cout << "Cycle number " << cycle_number_ << " has been deleted." << endl;
}

/// update function
void Cycle::Update(float elapsedTime) {
	for (Phase *p : phases_)
	{
		p->Update(elapsedTime);
	}

	cycle_phases();
}

/// reload all phases in this cycle
void Cycle::ReloadCycle() {

	for (Phase *p : phases_)
	{
		p->ReloadPhase();
	}
}

/// add a phase to this cycle
Phase *Cycle::AddPhase(int phaseNumber, float cycleTime) {

	Phase *temp;

	if (phaseNumber == 0)
	{
		phaseNumber = Phase::PhaseCount + 1;
	}

	temp = new Phase(phaseNumber, this->cycle_number_, cycleTime);

	phases_.push_back(temp);

	++number_of_phases_;
	++Phase::PhaseCount;

	if (Settings::DrawAdded)
		cout << "phase " << phaseNumber << " added" << endl;

	return temp;
}

/// get a phase by phase ID
/*
Phase *Cycle::GetPhase(int phaseNumber) {

	for (Phase *p : phases_)
	{
		if (p->GetPhaseNumber() == phaseNumber)
		{
			return p;
		}
	}

	return nullptr;
}
 */

/// calculate the phases priority using neural network
////////////////////////////////////////////////////////////
/// \brief
///
/// Calculates the priority of each lane using the neural
/// network.
/// Runs the data of each phase as input through the NN,
/// and sets its priority from the NN outputs.
///
////////////////////////////////////////////////////////////
void Cycle::calculate_priority() {

	for (int p = 0; p < number_of_phases_ - 1; p++)
	{
		// get input values
		phases_[p]->GetInputValues(input_values_);

		if (input_values_[0] > 0)
		{
			if(Settings::RunBestNet)
			{
				Net::BestNet.FeedForward(input_values_);
				Net::BestNet.GetResults(output_values_);
			}
			else
			{
				Net::CurrentNet->FeedForward(input_values_);
				Net::CurrentNet->GetResults(output_values_);
			}

			phases_[p]->SetPhasePriority(output_values_[0]);
			phases_[p]->SetCycleTime(clamp(
				float(output_values_[1]) * Settings::MaxCycleTime,
				Settings::MinCycleTime,
				Settings::MaxCycleTime));
		} else
		{
			phases_[p]->SetCycleTime(Settings::MinCycleTime);
			phases_[p]->SetPhasePriority(0);
		}
	}
}

/// cycle the phases by the phase array order.
////////////////////////////////////////////////////////////
/// \brief
///
/// all the phases but the active one are constantly evaluated
/// and sorted by the score they have been given by the NN
/// when phase is finished, it gets swapped with the previous one in the array
/// to advance and start the next phase in order.
///
////////////////////////////////////////////////////////////

void Cycle::cycle_phases() {
	// if cycle has a minimum of 2 phases
	if (number_of_phases_ >= 2)
	{
		// when current phase is closed, advance to next phase and open it
		if (!phases_.back()->GetIsOpen())
		{
			Phase *backPhase = phases_[number_of_phases_ - 1];
			phases_[number_of_phases_ - 1] = phases_[number_of_phases_ - 2];
			phases_[number_of_phases_ - 2] = backPhase;

			phases_[number_of_phases_ - 1]->Open();

			//Net::NeuralNetwork.printNet();
		}
			// constantly sort the list by their priority score
		else
		{
			// calculate the priority of each phase
			calculate_priority();
			// sort(arr[0:-2])
			partial_sort(phases_.begin(),
			             phases_.end() - 1,
			             phases_.end() - 1,
			             compare_priority);
		}
	}
}

/// draw function
void Cycle::Draw(RenderWindow *window) {

	for (Phase *p : phases_)
	{
		p->Draw(window);
	}
}