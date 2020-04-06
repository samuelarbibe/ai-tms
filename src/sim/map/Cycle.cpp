//
// Created by Samuel Arbibe on 06/04/2020.
//

#include "Cycle.hpp"

int Cycle::CycleCount = 0;

bool compare_priority(Phase* first, Phase* second)
{
	return (first->GetPriorityScore() < second->GetPriorityScore());
}

template<typename T>
void pop_front(std::vector<T>& vec) {
	assert(!vec.empty());
	vec.erase(vec.begin());
}

Cycle::Cycle(int cycleNumber, Intersection * intersection)
{
	cycle_number_ = cycleNumber;
	intersection_ = intersection;
	number_of_phases_ = 0;
}

Cycle::~Cycle()
{
	for(Phase * p : phases_)
	{
		delete p;
	}

	cout << "Cycle number " << cycle_number_ << " has been deleted." << endl;
}

void Cycle::Update(float elapsedTime)
{
	for (Phase *p : phases_)
	{
		p->Update(elapsedTime);
	}

	cycle_phases();
}

void Cycle::ReloadCycle() {

	for(Phase * p : phases_)
	{
		p->ReloadPhase();
	}
}

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

	if(Settings::DrawAdded) cout << "phase " << phaseNumber << " added" << endl;

	return temp;
}

Phase * Cycle::GetPhase(int phaseNumber) {

	for(Phase * p : phases_)
	{
		if(p->GetPhaseNumber() == phaseNumber)
		{
			return p;
		}
	}

	return nullptr;
}

/// cycle the phases by the phase array order.
// all the phases but the active one are constantly evaluated
// and sorted by the score they have been given by the NN
// sort(arr[0:-2])

// when phase is finished, it gets pop_front and push_back
// to advance and start the next phase in order.
void Cycle::cycle_phases() {
	// if current phase has ended
	if(number_of_phases_ >= 2)
	{
		// when current phase is closed, advance to next phase and open it
		if (!phases_.back()->GetIsOpen())
		{
			Phase * backPhase = phases_[number_of_phases_-1];
			phases_[number_of_phases_-1] = phases_[number_of_phases_-2];
			phases_[number_of_phases_-2] = backPhase;

			phases_[number_of_phases_-1]->Open();

			/*
			cout << "[ ";
			for(int i = 0; i < number_of_phases_-1 ; i++)
			{
				cout << phases_[i]->GetPriorityScore() << " ";
			}
			cout << "[" << phases_[number_of_phases_-1]->GetPriorityScore() << "]]" << endl;
			*/
		 }
			// constantly sort the list by their priority score
		else
		{
			partial_sort(phases_.begin(), phases_.end() - 1, phases_.end() - 1, compare_priority);
		}
	}
}


void Cycle::Draw(RenderWindow *window) {

	for(Phase * p : phases_)
	{
		p->Draw(window);
	}
}