#pragma once

class NeuralNetwork
{
public:
	NeuralNetwork(int InputNodesNumber, int HiddenNodesNumber, int OutputNodesNumber);

private:
	int InputNodes;

	int HiddenNodes;

	int OutputNodes;
};