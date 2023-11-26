#include <SFML/Graphics.hpp>

#include "NeuralNetwork.h"
#include "Utility.h"

#include <iostream>
#include <vector>

const int32_t WindowWidth = 800;
const int32_t WindowHeight = 800;

int main()
{
	srand(time(NULL));

	// Example of XOR solving network

	// Create network
	NeuralNetwork XORNetwork(2, 4, 1, 0.1, -0.3, 0.3);

	// Setup training data
	std::vector<std::vector<float>> InputArray = { {0, 0}, {1, 0}, {0, 1}, {1, 1} };
	std::vector<std::vector<float>> Targets = { {0}, {1}, {1}, {0} };

	// Render
	sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "BasicNeuralNetwork");

	while (Window.isOpen())
	{
		Window.clear();

		// Train network
		for (int I = 0; I < 1000; ++I)
		{
			int RandomIndex = rand() % InputArray.size();
			XORNetwork.Train(InputArray[RandomIndex], Targets[RandomIndex]);
		}

		int Resolution = 10;
		int Columns = WindowWidth / Resolution;
		int Rows = WindowHeight / Resolution;

		for (int I = 0; I < Columns; ++I)
		{
			for (int J = 0; J < Rows; ++J)
			{
				// Calculate X and Y position
				float X = (float)I / Columns;
				float Y = (float)J / Rows;
				// Form input coordinates
				std::vector<float> InputsCoordinates = { X, Y };
				// Get prediction
				float Prediction = XORNetwork.FeedForward(InputsCoordinates)[0];

				// Set up the rectangle
				sf::RectangleShape Rectangle;
				// Set the size of the rectangle
				Rectangle.setSize(sf::Vector2f(Resolution, Resolution));
				// Set position
				Rectangle.setPosition(J * Resolution, I * Resolution);
				// Set the color of the rectangle based on prediction
				float FillColor = 255 * Prediction;
				Rectangle.setFillColor(sf::Color(FillColor, FillColor, FillColor));
				// Draw rectangle
				Window.draw(Rectangle);
			}
		}

		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				XORNetwork = NeuralNetwork(2, 4, 1, 0.1, -0.3, 0.3);
			}
		}

		Window.display();
	}
	
	return 0;
}