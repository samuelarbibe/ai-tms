//
// Created by Samuel Arbibe on 08/04/2020.
//

#ifndef TMS_SRC_SIM_NN_NEURALNET_HPP
#define TMS_SRC_SIM_NN_NEURALNET_HPP

#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Neuron.hpp"

using namespace std;

class Net
{
  public:
	Net(){}
	Net(const vector<unsigned> &topology);
	void FeedForward(const vector<double> &inputVals);
	void BackPropagate(const vector<double> &targetVals);
	void PrintNet();
	void SetActualResults(const double actualResult);
	void GetResults(vector<double> &resultVals) const;
	double GetRecentAverageError(void) const { return recent_average_error_; }

  private:
	vector<Layer> layers_; //layers_[layerNum][neuronNum]
	double error_;
	double recent_average_error_;
	static double recent_average_smoothing_factor_;
};

#endif //TMS_SRC_SIM_NN_NEURALNET_HPP
