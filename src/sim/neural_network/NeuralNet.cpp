//
// Created by Samuel Arbibe on 08/04/2020.
//

#include "NeuralNet.hpp"

float NeuralNet::recent_average_smoothing_factor = 100.f;

NeuralNet::NeuralNet(vector<int> topology) {
	unsigned numLayers = topology.size();

	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
	{
		layers_.push_back(Layer());
		// numOutputs of layer[i] is the numInputs of layer[i+1]
		// numOutputs of last layer is 0
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		// We have made a new Layer, now fill it ith neurons, and
		// add a bias neuron to the layer:
		for (unsigned neuronNum = 0; neuronNum < topology[layerNum]; ++neuronNum)
		{
			layers_.back().push_back(Neuron(numOutputs, neuronNum));
		}
	}
	recent_average_error_ = 0;
}

/// Feed values through the layers
void NeuralNet::FeedForward(vector<float> inputValues) {

	// Check the num of inputVals euqal to neuronnum expect bias
	assert(inputValues.size() == layers_[0].size());

	// Assign {latch} the input values into the input neurons
	for (int i = 0; i < inputValues.size(); ++i)
	{
		layers_[0][i].SetOutputValue(inputValues[i]);
	}

	// Forward propagate
	for (int layerNum = 1; layerNum < layers_.size(); ++layerNum)
	{
		Layer &prevLayer = layers_[layerNum - 1];
		for (int n = 0; n < layers_[layerNum].size() - 1; ++n)
		{
			layers_[layerNum][n].FeedForward(prevLayer);
		}
	}
}

/// back propogate through the layers
void NeuralNet::BackPropagate(vector<float> targetValues) {

	// Calculate overal net error (RMS of output neuron errors)

	Layer &outputLayer = layers_.back();
	error_ = 0.0;

	for (unsigned n = 0; n < outputLayer.size(); ++n)
	{
		double delta = targetValues[n] - outputLayer[n].GetOutputValue();
		error_ += delta * delta;
	}
	error_ /= outputLayer.size() - 1; // get average error squared
	error_ = sqrt(error_); // RMS

	// Implement a recent average measurement:

	recent_average_error_ =
		(recent_average_error_ * recent_average_smoothing_factor + error_)
			/ (recent_average_smoothing_factor + 1.0);
	// Calculate output layer gradients

	for (unsigned n = 0; n < outputLayer.size(); ++n)
	{
		outputLayer[n].CalculateOutputGradient(targetValues[n]);
	}
	// Calculate gradients on hidden layers

	for (unsigned layerNum = layers_.size() - 2; layerNum > 0; --layerNum)
	{
		Layer &hiddenLayer = layers_[layerNum];
		Layer &nextLayer = layers_[layerNum + 1];

		for (unsigned n = 0; n < hiddenLayer.size(); ++n)
		{
			hiddenLayer[n].CalculateHiddenGradients(nextLayer);
		}
	}

	// For all layers from outputs to first hidden layer,
	// update connection weights

	for (unsigned layerNum = layers_.size() - 1; layerNum > 0; --layerNum)
	{
		Layer &layer = layers_[layerNum];
		Layer &prevLayer = layers_[layerNum - 1];

		for (unsigned n = 0; n < layer.size(); ++n)
		{
			layer[n].UpdateInputWeights(prevLayer);
		}
	}
}

void NeuralNet::GetResults(vector<float> *resultValues) {

	resultValues->clear();

	for(unsigned n = 0; n < layers_.back().size(); ++n)
	{
		resultValues->push_back(layers_.back()[n].GetOutputValue());
	}
}


/// copy to each output neuron the actual result
void NeuralNet::SetActualResult(float actualResult) {

	actual_value_ = actualResult;

	Layer &outputLayer = layers_.back();

	for (int n = 0; n < GetOutputNeuronCount(); n++)
	{
		outputLayer[n].SetOutputValue(actual_value_);
	}
}