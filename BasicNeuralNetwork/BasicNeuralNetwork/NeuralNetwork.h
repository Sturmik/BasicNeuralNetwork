#pragma once

#include "Matrix.h"

// Basic neural network with three layers
class NeuralNetwork
{
public:
	NeuralNetwork(int InputNodesNumber, int HiddenNodesNumber, int OutputNodesNumber, float LearningRate, float StartWeightFromRange, float StartWeightToRange);

	void Train(std::vector<float> InputArray, std::vector<float> TargetArray);

	std::vector<float> Predict(std::vector<float> InputArray);

	void SetLearningRate(float NewLearningRate);

private:

	// Matrix of weights between input and hidden layer
	Matrix WeightsIH;

	// Matrix of weights between hidden and output layer
	Matrix WeightsHO;

	// Matrix of bias weights between input and hidden layer 
	Matrix BiasIH;

	// Matrix of bias weights between hidden and output layer
	Matrix BiasHO;
	
	// Number of input nodes
	int InputNodesNumber;

	// Number of hidden nodes
	int HiddenNodesNumber;

	// Number of output nodes
	int OutputNodesNumber;

	// Learning rate
	float LearningRate;
};