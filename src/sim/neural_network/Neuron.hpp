//
// Created by Samuel Arbibe on 08/04/2020.
//

#ifndef TMS_SRC_SIM_NN_NEURON_HPP
#define TMS_SRC_SIM_NN_NEURON_HPP

#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <iomanip>
#include <SFML/Graphics.hpp>


using namespace std;
using namespace sf;

class Neuron;

struct Connection
{
	double weight;
	double deltaWeight;
};

typedef vector<Neuron> Layer;

// ****************** class Neuron ******************

class Neuron
{
  public:
	Neuron(unsigned numOutputs, unsigned myIndex, Vector2f position, float radius);

	void Draw(RenderWindow * window);
	void Update(float elapsedTime, vector<VertexArray> * weight_lines_, int * firstWeightIndex);
	void Reset();

	vector<Connection> GetWeights();
	void SetWeights(vector<Connection> weights);

	void SetOutputValue(double val) { output_value_ = val; }
	double GetOutputValue() const { return output_value_; }
	void FeedForward(const Layer &prevLayer);
	void CalculateOutputGradients(double targetVals);
	void CalculateHiddenGradients(const Layer &nextLayer);
	void UpdateInputWeights(Layer &prevLayer);
	Vector2f GetPosition(){return circle_->getPosition();}
	void Mutate(float mutationRate);

  private:
	// [0.0...1.0] net training rate
	static double eta;
	// [0.0...n] multiplier of last weight change [momentum]
	static double alpha;
	static double transfer_function(double x);
	static double transfer_function_derivative(double x);
	// randomWeight: 0 - 1
	static double randomize_weight() { return random() / double(RAND_MAX); }
	double sum_dow(const Layer &nextLayer) const;
	double output_value_;
	vector<Connection> output_weights_;
	unsigned my_index_;
	double gradient_;
	CircleShape * circle_;
};

#endif //TMS_SRC_SIM_NN_NEURON_HPP
