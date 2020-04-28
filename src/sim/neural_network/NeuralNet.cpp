//
// Created by Samuel Arbibe on 08/04/2020.
//

#include "NeuralNet.hpp"

Net Net::BestNet = Net();
Net *Net::CurrentNet = nullptr;
const unsigned Net::PopulationSize = 10;
unsigned Net::GenerationCount = 0;
unsigned Net::CurrentNetIndex = 0;
float Net::HighScore = 0;
vector<Net> Net::Generation = vector<Net>();

////////////////////////////////////////////////////////////
/// \brief Overload of binary operator !=
///
/// Creates a new generation of neural nets, based on the
/// previous generation.
///
////////////////////////////////////////////////////////////
void Net::NextGeneration() {

	Net::CurrentNet = nullptr;

	// normalize the fitness of all the nets in this gen
	Net::NormalizeFitness(Net::Generation);
	// create a new generation of nets
	vector<Net> newGen = Net::Generate(Net::Generation);
	// copy the new gen to the Generation array
	Net::Generation = newGen;

	Net::CurrentNetIndex = 0;
	Net::GenerationCount++;
}

////////////////////////////////////////////////////////////
/// \brief
///
/// Create a new array of neural nets based on an old generation
///
/// \param oldGen (vector<Net>) - the previous gen of NN's
///
/// \return new array of nets
////////////////////////////////////////////////////////////
vector<Net> Net::Generate(const vector<Net> &oldGen) {
	vector<Net> newGen;
	for(unsigned i = 0; i < oldGen.size(); i++)
	{
		newGen.push_back(Net::PoolSelection(oldGen));
		newGen.back().mutate(0.2);
	}
	return newGen;
}

////////////////////////////////////////////////////////////
/// \brief
///
/// Normalizes the fitness of all the nets to a value
/// between 0 and 1
///
////////////////////////////////////////////////////////////
void Net::NormalizeFitness(vector<Net> &oldGen) {
	double sum = 0;
	for(unsigned i = 0; i < oldGen.size(); i++)
	{
		double score = pow(oldGen[i].score_, 2);
		oldGen[i].score_ = score;
		sum += score;
	}

	for(unsigned i = 0; i < oldGen.size(); i++)
	{
		oldGen[i].fitness_ = oldGen[i].score_ / sum;
	}
}

////////////////////////////////////////////////////////////
/// \brief
///
/// Create a copy of a net, selected from the old net.
/// The probability of a net being selected from the pool
/// is according to its fitness.
///
/// \param oldGen (vector<Net>) - The previous generation
///
/// \return a copied Net
////////////////////////////////////////////////////////////
Net Net::PoolSelection(const vector<Net> &oldGen) {
	unsigned index = 0;
	double r = rand() / double(RAND_MAX);

	while(r > 0)
	{
		r -= oldGen[index].fitness_;
		index++;
	}
	index--;
	return oldGen[index];
}

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
		}
	}

	create_weight_vertex_array();
	Update(0.f);
}

////////////////////////////////////////////////////////////
/// \brief
///
/// Mutates the weights in a neural net by a given mutation rate.
///
/// \param mutationRate (float) - the mutation range
////////////////////////////////////////////////////////////
void Net::mutate(float mutationRate)
{
	// for all layer but the output layer
	for(unsigned i = 0; i < layers_.size() - 1; i++)
	{
		Layer & layer = layers_[i];
		for(unsigned j = 0; j < layer.size(); j++)
		{
			layer[j].Mutate(mutationRate);
		}
	}
}

////////////////////////////////////////////////////////////
/// \brief
///
/// Saves the neural net in a JSON file.
///
/// \param dir (string) - the directory in which the file will be saved
///
////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////
/// \brief
///
/// Loads a given JSON file and creates a new NeuralNet
/// object with it, and sets it as 'bestNet'
///
/// \param dir (string) - the directory of the JSON file
////////////////////////////////////////////////////////////
void Net::Load(const string dir) {
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

		Net::BestNet = Net(topology);

		unsigned layerCount = Net::BestNet.layers_.size();

		for (unsigned layerNum = 0; layerNum < layerCount; ++layerNum)
		{
			unsigned neuronCount = Net::BestNet.layers_[layerNum].size();

			for (unsigned neuronNum = 0; neuronNum < neuronCount; ++neuronNum)
			{
				vector<Connection> weights;
				unsigned weightCount =
					Net::BestNet.layers_[layerNum][neuronNum].GetWeights()
						.size();

				for (unsigned weightNum = 0; weightNum < weightCount;
				     ++weightNum)
				{
					Connection con = Connection();
					con.deltaWeight = j["weights"][weightNum]["delta_weight"];
					con.weight = j["weights"][weightNum]["weight"];
					weights.push_back(con);
				}

				Net::BestNet.layers_[layerNum][neuronNum]
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

/// creates the visual element making the visual net
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

[[maybe_unused]] void Net::PrintNet() {

	for (unsigned l = 0; l < layers_.size(); l++)
	{
		for (unsigned n = 0; n < layers_[l].size(); n++)
		{
			cout << setprecision(6) << layers_[l][n].GetOutputValue() << " ";
		}
		cout << endl;
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

