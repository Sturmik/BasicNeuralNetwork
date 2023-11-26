#include <SFML/Graphics.hpp>

#include "NeuralNetwork.h"

#include <iostream>
#include <vector>

int main()
{
	srand(time(NULL));

	// Example of XOR solving network

	// Set network with two inputs, two hidden nodes, one ouput and learning rate
	NeuralNetwork ExampleNetwork(2, 2, 1, 0.1);

	// Setup training data
	std::vector<std::vector<float>> InputArray = { {0, 0}, {1, 0}, {0, 1}, {1, 1} };
	std::vector<std::vector<float>> Targets = { {0}, {1}, {1}, {0} };

	// Train network
	for (int LearningIterations = 0; LearningIterations < 50000; ++LearningIterations) 
	{
		for (int I = 0; I < InputArray.size(); ++I)
		{
			int RandomIndex = rand() % InputArray.size();
			ExampleNetwork.Train(InputArray[RandomIndex], Targets[RandomIndex]);
		}
	}

	std::cout << "Testing XOR neural network" << std::endl;
	std::cout << "0 XOR 0 = " << ExampleNetwork.FeedForward(InputArray[0])[0] << std::endl; // Close to 0
	std::cout << "1 XOR 0 = " << ExampleNetwork.FeedForward(InputArray[1])[0] << std::endl; // Close to 1
	std::cout << "0 XOR 1 = " << ExampleNetwork.FeedForward(InputArray[2])[0] << std::endl; // Close to 1
	std::cout << "1 XOR 1 = " << ExampleNetwork.FeedForward(InputArray[3])[0] << std::endl; // Close to 0
	
	return 0;
}