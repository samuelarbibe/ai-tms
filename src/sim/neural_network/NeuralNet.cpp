//
// Created by Samuel Arbibe on 08/04/2020.
//

#include "NeuralNet.hpp"

Net::Net(const vector<unsigned> &topology) {
	srand((time(nullptr)));

	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
	{
		layers_.push_back(Layer());

		unsigned numOutputs =
			layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		// We have made a new Layer, fill it ith neurons
		for (unsigned neuronNum = 0; neuronNum < topology[layerNum];
		     ++neuronNum)
		{
			layers_.back().push_back(Neuron(numOutputs, neuronNum));
			cout << "Mad a Neuron!" << endl;
		}
	}
}

double Net::recent_average_smoothing_factor_ =
	100.0; // Number of training samples to average over

void Net::GetResults(vector<double> &resultVals) const {
	resultVals.clear();

	for (unsigned n = 0; n < layers_.back().size(); ++n)
	{
		resultVals.push_back(layers_.back()[n].GetOutputValue());
	}
}

void Net::BackPropagate(const std::vector<double> &targetVals) {
	// Calculate overal net error (RMS of output neuron errors)

	Layer &outputLayer = layers_.back();
	error_ = 0.0;

	for (unsigned n = 0; n < outputLayer.size(); ++n)
	{
		double delta = targetVals[n] - outputLayer[n].GetOutputValue();
		error_ += delta * delta;
	}
	error_ /= outputLayer.size(); // get average error squared
	error_ = sqrt(error_); // RMS

	// Implement a recent average measurement:

	recent_average_error_ =
		(recent_average_error_ * recent_average_smoothing_factor_ + error_)
			/ (recent_average_smoothing_factor_ + 1.0);
	// Calculate output layer gradients

	for (unsigned n = 0; n < outputLayer.size(); ++n)
	{
		outputLayer[n].CalculateOutputGradients(targetVals[n]);
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

void Net::PrintNet() {

	for (unsigned l = 0; l < layers_.size(); l++)
	{
		for (unsigned n = 0; n < layers_[l].size(); n++)
		{
			cout << setprecision(6) << layers_[l][n].GetOutputValue() << " ";
		}
		cout << endl;
	}
}

void Net::SetActualResults(const double actualResult) {

	for (unsigned n = 0; n < layers_.back().size(); n++)
	{
		layers_.back()[n].SetOutputValue(actualResult);
	}
}

void Net::FeedForward(const vector<double> &inputVals) {
	// Check the num of inputVals euqal to neuronnum expect bias
	assert(inputVals.size() == layers_[0].size());

	// Assign {latch} the input values into the input neurons
	for (unsigned i = 0; i < inputVals.size(); ++i)
	{
		layers_[0][i].SetOutputValue(inputVals[i]);
	}

	// Forward propagate
	for (unsigned layerNum = 1; layerNum < layers_.size(); ++layerNum)
	{
		Layer &prevLayer = layers_[layerNum - 1];
		for (unsigned n = 0; n < layers_[layerNum].size(); ++n)
		{
			layers_[layerNum][n].FeedForward(prevLayer);
		}
	}
}

