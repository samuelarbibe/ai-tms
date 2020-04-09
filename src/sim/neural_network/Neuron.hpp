//
// Created by Samuel Arbibe on 08/04/2020.
//

#ifndef TMS_SRC_SIM_NN_NEURON_HPP
#define TMS_SRC_SIM_NN_NEURON_HPP

#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

class Neuron;

typedef vector<Neuron> Layer;

struct Connection
{
	float weight;
	float delta_weight;
};

class Neuron
{
  public:

	Neuron(int outputsCount, int myIndex);
	~Neuron(){};

	void FeedForward(Layer & prevLayer);
	void CalculateOutputGradient(float targetValue);
	void CalculateHiddenGradients(Layer & nextLayer);
	void UpdateInputWeights(Layer & prevLayer);

	// get
	float GetOutputValue() {return output_value_;}

	// set
	void SetOutputValue(float value) { output_value_ = value;}

  private:
	// Training rate. [0.0...1.0]
	// Larger rate, more extreme learning
	static float eta;
	// Multiplier of previous weight change (Momentum);
	// if a change goes well, how much to push in that direction?
	static float alpha;
	static float transfer_function(float x);
	static float transfer_function_derivative(float x);
	static float randomize_weight() { return rand() / float(RAND_MAX);}

	float sum_dow(Layer & nextLayer);
	float output_value_;
	vector<Connection> output_weights_;
	// the Index of this neuron in its layer
	int my_index_;
	float gradient_;

};

#endif //TMS_SRC_SIM_NN_NEURON_HPP
