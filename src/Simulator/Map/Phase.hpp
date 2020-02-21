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
    Phase(int phaseNumber, float cycleTime);
    ~Phase();

    void Draw(RenderWindow * window);
    void Update(float elapsedTime);

    void ReloadPhase();
    void Open(){open_ = true;}
    void SetCycleTime(float cycleTime){cycle_time_ = cycleTime;}

    bool IsOpen(){return open_;}
    int GetCycleTime(){return cycle_time_;}
    int GetPhaseNumber(){return phase_number_;}
	vector<Light*> * GetLights(){return &lights_;}
	vector<Lane*> * GetAssignLanes(){return &lanes_;}

    Light * AddLight(int lightNumber, Road * parentRoad);
    void AddLane(Lane * lane);

    static int PhaseCount;

private:

    int phase_number_;
    int number_of_lights_;

    bool open_;

    float open_time_;
    float cycle_time_;

    LightState state_;

    vector<Light*> lights_;
    vector<Lane*> lanes_;
};


#endif //SIMULATORSFML_PHASE_HPP
