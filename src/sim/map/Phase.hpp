//
// Created by Samuel Arbibe on 20/02/2020.
//

#ifndef SIMULATORSFML_PHASE_HPP
#define SIMULATORSFML_PHASE_HPP

#include <vector>
#include "Light.hpp"

using namespace sf;
using namespace std;

class Phase
{
public:
    Phase(int phaseNumber, int cycleNumber, float cycleTime);
    ~Phase();

    void Draw(RenderWindow * window);
    void Update(float elapsedTime);
    void ReloadPhase();

    // add entities
    Light * AddLight(int lightNumber, Road * parentRoad);
    void AddLane(Lane * lane);

    // get
    int   GetPhaseNumber(){return phase_number_;}
    int   GetCycleNumber(){return cycle_number_;}
    bool  GetIsOpen(){return open_;}
    float GetCycleTime(){return cycle_time_;}
	vector<Light*> * GetLights(){return &lights_;}
	vector<Lane*> *  GetAssignedLanes(){return &lanes_;}
	void GetInputValues(vector<double> & inputValues);
	float GetMaxQueueLength();
	float GetMaxLaneDensity();
	float GetPriorityScore() {return priority_;}

    // set
    void Open(){open_ = true;}
    void SetCycleTime(float cycleTime){cycle_time_ = cycleTime;}
	bool UnassignLane(Lane * lane);
    void SetPhasePriority(float points) { priority_ = points;}

	// The total count of all the phases created this session
    static int PhaseCount;

private:

	// ID of this phase
    int phase_number_;
	// Id of the cycle it belongs to
	int cycle_number_;
	// Number of lights that represent this phase
    int number_of_lights_;
    // Is this phase active and open
    bool open_;

    // The time that the phase has been open over
    float open_time_;
    // Cycle time of this phase
    float cycle_time_;
    // The points given to a phase according to its need to be open
    float priority_;

    // the current state of this phase
    LightState state_;

    vector<Light*> lights_;
    vector<Lane*> lanes_;

};


#endif //SIMULATORSFML_PHASE_HPP
