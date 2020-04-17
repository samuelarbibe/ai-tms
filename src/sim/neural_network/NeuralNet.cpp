//
// Created by Samuel Arbibe on 08/04/2020.
//

#include "NeuralNet.hpp"

Net Net::NeuralNetwork = Net();

Net::Net(const vector<unsigned> &topology) {

	size_ = Vector2f(Settings::DefaultMapWidth, Settings::DefaultMapHeight);

	unsigned layerCount = topology.size();
	for (unsigned layerNum = 0; layerNum < layerCount; ++layerNum)
	{
		layers_.push_back(Layer());

		unsigned numOutputs =
			layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		// We have made a new Layer, fill it ith neurons
		unsigned neuronCount = topology[layerNum];
		for (unsigned neuronNum = 0; neuronNum < neuronCount;
		     ++neuronNum)
		{
			Vector2f position = calculate_neuron_position(layerNum,
			                                              layerCount,
			                                              neuronNum,
			                                              neuronCount);
			layers_.back().push_back(Neuron(numOutputs,
			                                neuronNum,
			                                position,
			                                size_.x / 25.f));
			cout << "Mad a Neuron!" << endl;
		}
	}

	create_weight_vertex_array();
	Update(0.f);
}

void Net::Save(const string dir) {

	json j;

	unsigned layerCount = layers_.size();

	for (unsigned layerNum = 0; layerNum < layerCount; ++layerNum)
	{
		unsigned neuronCount = layers_[layerNum].size();

		j["layers"].push_back(
			{
				{"ID", layerNum},
				{"neuron_count", neuronCount}
			}
		);

		for (unsigned neuronNum = 0; neuronNum < neuronCount; ++neuronNum)
		{
			vector<Connection>
				weights = layers_[layerNum][neuronNum].GetWeights();
			unsigned weightCount = weights.size();

			for (unsigned weightNum = 0; weightNum < weightCount; ++weightNum)
			{
				j["weights"].push_back(
					{
						{"delta_weight", weights[weightNum].deltaWeight},
						{"weight", weights[weightNum].weight}
					}
				);
			}
		}
	}

	// write to file
	ofstream o(dir);
	o << setw(4) << j << endl;
	o.close();

	cout << "Set saved to '" << dir << "' successfully." << endl;

}

void Net::Load(string dir) {
	try
	{
		json j;
		// open the given file, read it to a json variable
		ifstream i(dir);
		i >> j;

		vector<unsigned> topology;
		for (auto data : j["layers"])
		{
			topology.push_back(unsigned(data["neuron_count"]));
		}

		Net::NeuralNetwork = Net(topology);

		unsigned layerCount = Net::NeuralNetwork.layers_.size();

		for (unsigned layerNum = 0; layerNum < layerCount; ++layerNum)
		{
			unsigned neuronCount = Net::NeuralNetwork.layers_[layerNum].size();

			for (unsigned neuronNum = 0; neuronNum < neuronCount; ++neuronNum)
			{
				vector<Connection> weights;
				unsigned weightCount =
					Net::NeuralNetwork.layers_[layerNum][neuronNum].GetWeights()
						.size();

				for (unsigned weightNum = 0; weightNum < weightCount;
				     ++weightNum)
				{
					Connection con = Connection();
					con.deltaWeight = j["weights"][weightNum]["delta_weight"];
					con.weight = j["weights"][weightNum]["weight"];
					weights.push_back(con);
				}

				Net::NeuralNetwork.layers_[layerNum][neuronNum]
					.SetWeights(weights);
			}
		}
	}
	catch (const std::exception &e)
	{
		cout << "Could not load Neural Network from this directory." << endl;
		cout << e.what() << endl;
	}
}

void Net::create_weight_vertex_array() {

	unsigned layerCount = layers_.size();

	for (unsigned layerNum = 1; layerNum < layerCount; ++layerNum)
	{
		Layer &prevLayer = layers_[layerNum - 1];
		Layer &currLayer = layers_[layerNum];
		unsigned NeuronCount = layers_[layerNum].size();
		unsigned PrevNeuronCount = prevLayer.size();

		// previous layer
		for (unsigned i = 0; i < PrevNeuronCount; ++i)
		{
			Neuron *prevNeuron = &prevLayer[i];
			// current layer
			for (unsigned j = 0; j < NeuronCount; ++j)
			{
				Neuron *currNeuron = &currLayer[j];

				VertexArray line = VertexArray(Lines, 2);

				line[0].position = prevNeuron->GetPosition();
				line[1].position = currNeuron->GetPosition();

				weight_lines_.push_back(line);
			}
		}
	}
}

void Net::Update(float elapsedTime) {

	int weights_index = 0;
	for (int l = 0; l < layers_.size(); l++)
	{
		for (int n = 0; n < layers_[l].size(); n++)
		{
			layers_[l][n].Update(elapsedTime, &weight_lines_, &weights_index);
		}
	}
}

void Net::Draw(RenderWindow *window) {

	for (VertexArray va : weight_lines_)
	{
		window->draw(va);
	}

	for (Layer &l : layers_)
	{
		for (Neuron n : l)
		{
			n.Draw(window);
		}
	}
}

void Net::Reset() {

	for (int l = 0; l < layers_.size() - 1; l++)
	{
		Layer &layer = layers_[l];
		for (int n = 0; n < layer.size(); n++)
		{
			layer[n].Reset();
		}
	}
	Update(0.f);
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

Vector2f Net::calculate_neuron_position(unsigned layerNum,
                                        unsigned layerCount,
                                        unsigned neuronNum,
                                        unsigned neuronCount) {

	Vector2f pos;

	pos.x = size_.x / float(layerCount + 1) * float(layerNum + 1);
	pos.y = size_.y / float(neuronCount + 1) * float(neuronNum + 1);

	return pos;
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

