#include "NeuralNetwork.h"

#include "Matrix.h"

float Sigmoid(float X)
{
	return 1 / (1 + exp(-X));
}

NeuralNetwork::NeuralNetwork(int InputNodesNumber, int HiddenNodesNumber, int OutputNodesNumber) : InputNodesNumber(InputNodesNumber), HiddenNodesNumber(HiddenNodesNumber), OutputNodesNumber(OutputNodesNumber)
{
	// Create matrix of weights between input and hidden layer
	WeightsIH = Matrix(HiddenNodesNumber, InputNodesNumber);
	// Create matrix of weights between hidden and output layer
	WeightsHO = Matrix(OutputNodesNumber, HiddenNodesNumber);
	// Randomize weights
	WeightsIH.Randomize(-1, 1);
	WeightsHO.Randomize(-1, 1);

	// Create matrix of bias weights between input and hidden layer
	BiasIH = Matrix(HiddenNodesNumber, 1);
	// Create matrix of bias weights between hidden and output layer
	BiasHO = Matrix(OutputNodesNumber, 1);

	// Randomize biases
	BiasIH.Randomize(-1, 1);
	BiasHO.Randomize(-1, 1);
}

std::vector<float> NeuralNetwork::FeedForward(std::vector<float> InputArray)
{
	// Generating hidden's outputs

	// Fill input matrix
	Matrix Input = Matrix::FromArray(InputArray);
	// Multiplying matrices of weights and layer = W * I
	Matrix Hidden = Matrix::Multiply(WeightsIH, Input);
	// Adding biases = W * I + B
	Hidden = Matrix::Add(Hidden, BiasIH);
	// Apply activation function = Sigmoid(W * I + B)
	Hidden.Map(Sigmoid);

	// Generating output's ouput

	// Multiplying matrices of weights and layer = W * I
	Matrix Output = Matrix::Multiply(WeightsHO, Hidden);
	// Adding biases = W * I + B
	Output = Matrix::Add(Output, BiasHO);
	// Apply activation function = Sigmoid(W * I + B)
	Output.Map(Sigmoid);

	return Output.ToArray();
}
