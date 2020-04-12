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
#include "../simulator/Settings.hpp"

using namespace std;
using namespace sf;

class Net
{
  public:
	Net(){}
	Net(const vector<unsigned> &topology, Vector2f size);

	void Draw(RenderWindow * window);
	void Update(float elapsedTime);

	void Reset();

	void FeedForward(const vector<double> &inputVals);
	void BackPropagate(const vector<double> &targetVals);
	void PrintNet();
	void SetActualResults(const double actualResult);
	void GetResults(vector<double> &resultVals) const;
	double GetRecentAverageError(void) const { return recent_average_error_; }

	static Net NeuralNetwork;

  private:
	vector<Layer> layers_; //layers_[layerNum][neuronNum]
	double error_;
	double recent_average_error_;
	static double recent_average_smoothing_factor_;

	Vector2f calculate_neuron_position(unsigned layerNum, unsigned layerCount,
	                                   unsigned neuronNum, unsigned neuronCount);

	void create_weight_vertex_array();
	vector<VertexArray> weight_lines_;
	Vector2f size_;
};

#endif //TMS_SRC_SIM_NN_NEURALNET_HPP
