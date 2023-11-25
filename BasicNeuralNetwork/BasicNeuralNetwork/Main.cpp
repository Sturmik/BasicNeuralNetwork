#include <SFML/Graphics.hpp>

#include "NeuralNetwork.h"

#include <iostream>
#include <vector>

#include "Matrix.h"

void PrintMatrix(Matrix& Matrix)
{
	for (int R = 0; R < Matrix.GetRows(); ++R)
	{
		for (int C = 0; C < Matrix.GetColumns(); ++C)
		{
			std::cout << Matrix.GetElement(R,C) << " ";
		}
		std::cout << std::endl;
	}
}

int main()
{
	srand(time(NULL));

	NeuralNetwork ExampleNetwork(2, 2, 1);

	std::vector<float> InputArray = {1, 0};

	std::vector<float> Output = ExampleNetwork.FeedForward(InputArray);
	
	return 0;
}