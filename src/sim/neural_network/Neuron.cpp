//
// Created by Samuel Arbibe on 08/04/2020.
//

#include "Neuron.hpp"

double Neuron::eta = 0.1;
double Neuron::alpha = 0.3;

Neuron::Neuron(unsigned numOutputs, unsigned myIndex) {

	// create random output weights
	for(int c = 0; c < numOutputs ; c++)
	{
		// push a random into the output weights
		output_weights_.push_back(Connection());
		output_weights_.back().weight = randomize_weight();
	}

	my_index_ = myIndex;
}

/// sum up all the weights that go into this neuron
	// apply a transfer function
	// and set it as this neuron's output value
void Neuron::FeedForward(const Layer &prevLayer) {

	// the sum of outputs from previous layer
	// into this node
	float sum = 0.f;

	// for each neuron in previous layer
	for(int n = 0 ; n < prevLayer.size(); ++n)
	{
		sum += prevLayer[n].GetOutputValue() *
			prevLayer[n].output_weights_[my_index_].weight;
	}

	output_value_ = Neuron::transfer_function(sum);
}

/// calculate the gradient by multiplying the difference between
	// the target value and the calculated output value
	// by the transfer function's derivative
void Neuron::CalculateOutputGradients(double targetValue) {

	float delta = targetValue - output_value_;
	gradient_ = delta * Neuron::transfer_function_derivative(output_value_);
}

// TODO: find out what is DOW
void Neuron::CalculateHiddenGradients(const Layer &nextLayer) {

	float dow = sum_dow(nextLayer);
	gradient_ = dow * Neuron::transfer_function_derivative(output_value_);
}


/// calculate the weight delta from previous layer to this neuron
	// and apply weight changes
void Neuron::UpdateInputWeights(Layer &prevLayer) {

	for(int n = 0; n < prevLayer.size(); ++n)
	{
		Neuron &neuron = prevLayer[n];
		float oldDeltaWeight = neuron.output_weights_[my_index_].deltaWeight;

		float newDeltaWeight =
			// Individual input, magnified by the gradient and train rate:
			eta
				* neuron.GetOutputValue()
				* gradient_
				// Also add momentum = a fraction of the previous delta weight
				+ alpha
					* oldDeltaWeight;
		neuron.output_weights_[my_index_].deltaWeight = newDeltaWeight;
		neuron.output_weights_[my_index_].weight += newDeltaWeight;
	}
}

/// a simple transfer function
	// which outputs in range [-1.0 .. 1.0]
double Neuron::transfer_function(double x) {

	return  1/(1 + exp(-x));
}

/// an approximation of the derivative function of sigmoid
double Neuron::transfer_function_derivative(double x) {

	return x * (1 - x);
}

/// sums up the contributions of the errors at the nodes we feed
double Neuron::sum_dow(const  Layer &nextLayer) const  {

	float sum = 0;

	// for all neurons but the bias neuron
	for(int n = 0; n < nextLayer.size() - 1; n++)
	{
		sum += output_weights_[n].weight * nextLayer[n].gradient_;
	}

	return sum;
}

