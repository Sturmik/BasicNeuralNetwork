#include "ExampleProjects.h"

#include <SFML/Graphics.hpp>

#include "NeuralNetwork.h"
#include "Utility.h"

#include <iostream>
#include <vector>
#include <fstream>

void XORProblemSolvingExample()
{
	const int32_t WindowWidth = 800;
	const int32_t WindowHeight = 800;

	srand(time(NULL));

	// Example of XOR solving network

	// Create network
	NeuralNetwork XORNetwork(2, 4, 1, 0.1, -0.3, 0.3);

	// Setup training data
	std::vector<std::vector<float>> InputArray = { {0, 0}, {1, 0}, {0, 1}, {1, 1} };
	std::vector<std::vector<float>> Targets = { {0}, {1}, {1}, {0} };

	// Render
	sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "XORProblemSolvingExample");

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
				float Prediction = XORNetwork.Predict(InputsCoordinates)[0];

				// Set up the rectangle
				sf::RectangleShape Rectangle;
				// Set the size of the rectangle
				Rectangle.setSize(sf::Vector2f(Resolution, Resolution));
				// Set position
				Rectangle.setPosition(J * Resolution, I * Resolution);
				// Set the BackgroundColor of the rectangle based on prediction
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
}

sf::Color GetRandomColor()
{
	return sf::Color(rand() % 255, rand() % 255, rand() % 255);
}

// Returns suitable foreground BackgroundColor of text for the given background with RGB BackgroundColor scheme
sf::Color ColorPredictor(NeuralNetwork& ColorPredictorNetwork, float R, float G, float B)
{
	// Prepare inputs
	std::vector<float> Inputs = { R / 255, G / 255, B / 255 };
	// Get prediction from network
	std::vector<float> Outputs = ColorPredictorNetwork.Predict(Inputs);
	std::cout << "Black color percentage: " << Outputs[0] << std::endl;
	std::cout << "White color percentage: " << Outputs[1] << std::endl << std::endl;
	// We consider that outputs correlate with colors in next way:
	// [0] - Black
	// [1] - White
	if (Outputs[0] > Outputs[1])
	{
		return sf::Color::Black;
	}
	else
	{
		return sf::Color::White;
	}
}

std::vector<float> TrainColor(sf::Color BackgroundColor)
{
	// Convert RGB to HSL
	float R = BackgroundColor.r / 255.0f;
	float G = BackgroundColor.g / 255.0f;
	float B = BackgroundColor.b / 255.0f;

	float MaxVal = std::max({ R, G, B });
	float MinVal = std::min({ R, G, B });

	float Luminance = (MaxVal + MinVal) / 2.0f;

	if (Luminance > 0.5)
	{
		// Black BackgroundColor
		return std::vector<float>() = {1.0f, 0.0f};
	}
	else
	{
		// White BackgroundColor
		return std::vector<float>() = {0.0f, 1.0f};
	}
}

void ColorPredictorExample()
{
	const int32_t WindowWidth = 800;
	const int32_t WindowHeight = 400;

	srand(time(NULL));

	// Example of BackgroundColor predictor solving network

	// Create network
	NeuralNetwork ColorPredictorNetwork(3, 3, 2, 0.1, -1, 1);

	// Train network
	for (int I = 0; I < 10000; ++I)
	{
		sf::Color RandomColor = GetRandomColor();
		std::vector<float> Inputs = { (float)RandomColor.r, (float)RandomColor.g,(float)RandomColor.b };
		ColorPredictorNetwork.Train(Inputs, TrainColor(RandomColor));
	}

	// Render window and its BackgroundColor
	sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "ColorPredictorExample");
	sf::Color WindowColor = GetRandomColor();

	// Load a LabelFont
	sf::Font LabelFont;
	if (!LabelFont.loadFromFile("Resources\\Fonts\\Montserrat-Light.ttf"))
	{
		std::cout << "Font loading failed!" << std::endl;
		return;
	}

	// Create a TestText object
	sf::Text TestText;

	// Set the LabelFont
	TestText.setFont(LabelFont);

	// Set the string to display
	TestText.setString("Test Color!");

	// Set the character size
	TestText.setCharacterSize(34);

	// Set origin
	sf::FloatRect FloatRectTestText = TestText.getLocalBounds();
	TestText.setOrigin(FloatRectTestText.width / 2, FloatRectTestText.height / 2);

	// Define the MiddleLine properties
	sf::VertexArray MiddleLine(sf::Lines, 2);
	// Starting point (middle top)
	MiddleLine[0].position = sf::Vector2f(Window.getSize().x / 2, 0);
	// Ending point (middle bottom)
	MiddleLine[1].position = sf::Vector2f(Window.getSize().x / 2, Window.getSize().y);
	MiddleLine[0].BackgroundColor = sf::Color::Black;
	MiddleLine[1].BackgroundColor = sf::Color::Black;

	// Create a ColorPickerCircle shape BackgroundColor picker
	sf::CircleShape ColorPickerCircle(25.0f);

	// Set the origin to the center of the ColorPickerCircle
	ColorPickerCircle.setOrigin(ColorPickerCircle.getRadius(), ColorPickerCircle.getRadius());

	// Position the ColorPickerCircle in the center of the Window
	ColorPickerCircle.setPosition(Window.getSize().x / 2.0f, Window.getSize().y / 2.0f);

	// Set the fill BackgroundColor
	ColorPickerCircle.setFillColor(sf::Color::White);

	// Color chosen by network
	sf::Color NetworkChosenColor = ColorPredictor(ColorPredictorNetwork, (float)WindowColor.r, (float)WindowColor.g, (float)WindowColor.b);

	while (Window.isOpen())
	{
		Window.clear(WindowColor);

		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				// Send target for network training

				// Prepare inputs as window BackgroundColor
				std::vector<float> Inputs = { (float)WindowColor.r, (float)WindowColor.g,(float)WindowColor.b };
				// Black BackgroundColor for text is better
				if (sf::Mouse::getPosition(Window).x < WindowWidth / 2)
				{
					// Create vector for targets, which will train neural network
					std::vector<float> Targets = { 1.0f, 0.0f };
					ColorPredictorNetwork.Train(Inputs, Targets);
				}
				// White BackgroundColor for text is better
				else
				{
					// Create vector for targets, which will train neural network
					std::vector<float> Targets = { 0.0f, 1.0f };
					ColorPredictorNetwork.Train(Inputs, Targets);
				}

				// Update background BackgroundColor
				WindowColor = GetRandomColor();
				NetworkChosenColor = ColorPredictor(ColorPredictorNetwork, WindowColor.r, WindowColor.g, WindowColor.b);
			}
			if (event.type == sf::Event::KeyPressed)
			{
				// Update background BackgroundColor
				WindowColor = GetRandomColor();
				NetworkChosenColor = ColorPredictor(ColorPredictorNetwork, WindowColor.r, WindowColor.g, WindowColor.b);
			}
		}

		// Setup text on the left side
		TestText.setPosition(WindowWidth / 4, WindowHeight / 2);
		TestText.setFillColor(sf::Color::Black);
		Window.draw(TestText);

		// Setup text on the right side
		TestText.setPosition(WindowWidth - (WindowWidth / 4), WindowHeight / 2);
		TestText.setFillColor(sf::Color::White);
		Window.draw(TestText);

		// Draw the middle line
		Window.draw(MiddleLine);

		// Draw circle upon BackgroundColor, which was chosen by network
		if (NetworkChosenColor == sf::Color::Black)
		{
			ColorPickerCircle.setPosition(WindowWidth / 4, WindowHeight / 4);
		}
		else if (NetworkChosenColor == sf::Color::White)
		{
			ColorPickerCircle.setPosition(WindowWidth - (WindowWidth / 4), WindowHeight / 4);
		}
		Window.draw(ColorPickerCircle);

		Window.display();
	}
}
