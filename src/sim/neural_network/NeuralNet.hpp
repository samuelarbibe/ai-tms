//
// Created by Samuel Arbibe on 08/04/2020.
//

#ifndef TMS_SRC_SIM_NN_NEURALNET_HPP
#define TMS_SRC_SIM_NN_NEURALNET_HPP

#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Neuron.hpp"
#include "../simulator/Settings.hpp"
#include "../../../public/json.hpp"

using namespace std;
using namespace sf;
using json = nlohmann::json;

class Net
{
  public:
	Net(){}
	Net(const vector<unsigned> &topology);

	void Draw(RenderWindow * window);
	void Update(float elapsedTime);

	void Reset();

	void FeedForward(const vector<double> &inputVals);
	[[maybe_unused]] [[maybe_unused]] void PrintNet();
	void SetScore(double score){ score_ = score;};
	void GetResults(vector<double> &resultVals) const;
	void Save(const string dir);
	static void Load(const string dir);

	static void NextGeneration();
	static void NormalizeFitness(vector<Net> &oldGen);
	static Net PoolSelection(const vector<Net> &oldGen);
	static vector<Net> Generate(const vector<Net> &oldGen);

	static vector<Net> Generation;
	static unsigned CurrentNetIndex;
	static unsigned GenerationCount;
	static Net * CurrentNet;
	static Net BestNet;
	static float HighScore;
	static const unsigned PopulationSize;

  private:
	vector<Layer> layers_; //layers_[layerNum][neuronNum]
	double fitness_;
	double score_;
	double error_;
	double recent_average_error_;

	Vector2f calculate_neuron_position(unsigned layerNum, unsigned layerCount,
	                                   unsigned neuronNum, unsigned neuronCount);

	void mutate(float mutationRate);

	void create_weight_vertex_array();
	vector<VertexArray> weight_lines_;
	Vector2f size_;
};

#endif //TMS_SRC_SIM_NN_NEURALNET_HPP
