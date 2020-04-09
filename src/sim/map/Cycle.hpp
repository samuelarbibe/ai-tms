//
// Created by Samuel Arbibe on 06/04/2020.
//

#ifndef TMS_SRC_SIM_MAP_CYCLE_HPP
#define TMS_SRC_SIM_MAP_CYCLE_HPP

#include <algorithm>
#include <cstdint>

#include "Phase.hpp"
#include "Intersection.hpp"


using namespace std;
using namespace sf;

class Cycle
{
  public:

	Cycle(int cycleNumber, Intersection * intersection = nullptr);
	~Cycle();

	void Update(float elapsedTime);
	void Draw(RenderWindow * window);
	void ReloadCycle();

	Phase * AddPhase(int phaseNumber, float cycleTime);

	int GetCycleNumber(){ return cycle_number_;}
	Phase * GetPhase(int phaseNumber);
	vector<Phase *> *GetPhases() { return &phases_; }
	Intersection * GetIntersection(){ return intersection_;}

	static int CycleCount;

  private:

	void cycle_phases();
	void calculate_priority();

	int cycle_number_;
	int number_of_phases_;

	vector<Phase *> phases_;
	Intersection * intersection_;

	/*
	vector<float> neural_net_results_;
	vector<float> output_values_;
	 */
	vector<float> input_values_;
};

#endif //TMS_SRC_SIM_MAP_CYCLE_HPP
