//
// Created by Samuel Arbibe on 20/02/2020.
//

#include "Phase.hpp"

int Phase::PhaseCount = 0;

Phase::Phase(int phaseNumber, int cycleNumber, float cycleTime) {
	number_of_lights_ = 0;
	phase_number_ = phaseNumber;
	cycle_number_ = cycleNumber;
	cycle_time_ = cycleTime;
	open_ = false;
	open_time_ = 0;
	state_ = RED;
	priority_ = phaseNumber;
}

Phase::~Phase() {
	for (Light *light : lights_)
	{
		delete light;
	}

	if (Settings::DrawDelete)
		cout << "Phase " << phase_number_ << " deleted." << endl;
}

/// add a light to this phase and attach it to a road
Light *Phase::AddLight(int lightNumber, Road *parentRoad) {
	if (lightNumber == 0)
	{
		lightNumber = Light::LightCount + 1;
	}

	Light *temp = new Light(lightNumber, phase_number_, parentRoad);
	lights_.push_back(temp);

	++Light::LightCount;
	++number_of_lights_;

	return temp;
}

/// assign a lane to this phase
void Phase::AddLane(Lane *lane) {
	lanes_.push_back(lane);
	lane->SetPhaseNumber(phase_number_);
}

/// reload all the lights in this phase
void Phase::ReloadPhase() {
	for (Light *l : lights_)
	{
		l->UpdatePosition();
	}
}

////////////////////////////////////////////////////////////
/// \brief
///
/// Unassign a given lane from this phase. delete it from the lanes_ array.
///
/// \param lane (Lane *) - a pointer to the lane to unassign
///
/// \return true if succesfull, else false
///
////////////////////////////////////////////////////////////
bool Phase::UnassignLane(Lane *lane) {

	if (lane != nullptr)
	{
		auto it = lanes_.begin();

		while (it != lanes_.end())
		{
			if ((*it)->GetLaneNumber() == lane->GetLaneNumber())
			{
				int laneNumber = lane->GetLaneNumber();
				it = lanes_.erase(it);

				if (Settings::DrawActive)
					cout << "lane " << laneNumber << " removed from phase "
					     << phase_number_ << endl;
				return true;
			} else
			{
				it++;
			}
		}
	}
	return false;
}

/// update
void Phase::Update(float elapsedTime) {
	if (open_)
	{
		open_time_ += elapsedTime * Settings::Speed;

		if (open_time_ > cycle_time_)
		{
			open_time_ = 0;
			open_ = false;
			state_ = RED;
		} else if (open_time_ < Settings::OrangeDelay)
		{
			state_ = ORANGE;
		} else
		{
			state_ = GREEN;
		}
	}

	for (Light *l : lights_)
	{
		l->SetState(state_);
		l->Update(elapsedTime);
	}

	for (Lane *l : lanes_)
	{
		if (state_ == GREEN)
		{
			l->SetIsBlocked(false);
		} else
		{
			l->SetIsBlocked(true);
		}
	}
}

////////////////////////////////////////////////////////////
/// \brief
///
/// Create input values for the neural network
///
/// \param inputValues (vector<double> &) - a refrence to the input array
///
////////////////////////////////////////////////////////////
void Phase::GetInputValues(vector<double> &inputValues) {
	inputValues[0] = GetMaxLaneDensity();
	inputValues[1] = GetMaxQueueLength();
}

/// return the largest lane queue length
float Phase::GetMaxQueueLength() {
	float max = 0;
	float temp = 0;

	for (Lane *l : lanes_)
	{
		if ((temp = l->GetQueueLength()) > max)
		{
			max = temp;
		}
	}

	return max / 2500.f;
}

/// return the largest lane density
float Phase::GetMaxLaneDensity() {
	float max = 0;
	float temp = 0;

	for (Lane *l : lanes_)
	{
		if ((temp = l->GetNormalizedDensity()) > max)
		{
			max = temp;
		}
	}

	return max;
}

/// draw
void Phase::Draw(RenderWindow *window) {
	for (Light *l : lights_)
	{
		l->Draw(window);
	}
}




