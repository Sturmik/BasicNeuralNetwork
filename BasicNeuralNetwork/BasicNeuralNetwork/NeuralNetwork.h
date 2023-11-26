#pragma once

#include "Matrix.h"

// Basic neural network with three layers
class NeuralNetwork
{
public:
	NeuralNetwork(int InputNodesNumber, int HiddenNodesNumber, int OutputNodesNumber, float LearningRate);

	void Train(std::vector<float> InputArray, std::vector<float> TargetArray);

	std::vector<float> FeedForward(std::vector<float> InputArray);

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