//
// Created by Samuel Arbibe on 08/04/2020.
//

#ifndef TMS_SRC_SIM_NN_NEURALNET_HPP
#define TMS_SRC_SIM_NN_NEURALNET_HPP

#include <cstdlib>
#include <cmath>
#include <vector>
#include "Neuron.hpp"

using namespace std;

class NeuralNet
{
  public:

	NeuralNet(){};
	NeuralNet(vector<int> topology);
	~NeuralNet(){};

	void FeedForward(vector<float> inputValues);
	void BackPropagate(vector<float> targetValues);
	//void BackPropagate();
	void GetResults(vector<float> * resultValues);
	void SetActualResult(float actualResult);
	//void SetTargetResult(float targetResult){ target_value_ = targetResult;}
	float GetActualResult() { return actual_value_;}
	float GetRecentAverageError(){return recent_average_error_;}
	int GetOutputNeuronCount() { return layers_.back().size();}
	int GetInputNeuronCount() { return layers_.front().size();}

  private:

	static float recent_average_smoothing_factor;

	float error_;
	float actual_value_;
	float target_value_;

	vector<Layer> layers_;

	float recent_average_error_;
};

#endif //TMS_SRC_SIM_NN_NEURALNET_HPP
