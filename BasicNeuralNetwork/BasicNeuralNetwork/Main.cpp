#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

const uint32_t WindowWidth = 800;
const uint32_t WindowHeight = 800;

float RandValueInRange(float LowValue, float HighValue)
{
	return LowValue + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (HighValue - LowValue));
}

class Perceptron
{
public:

	// Constructor
	Perceptron() : Weights(2)
	{
		float LowValue = -1.0f;
		float HighValue = 1.0f;

		// Initialize the weights randomly
		for (int I = 0; I < Weights.size(); ++I)
		{
			Weights[I] = RandValueInRange(LowValue, HighValue);
		}
	}

	int Guess(std::vector<float> Inputs)
	{
		float Sum = 0; 
		for (int I = 0; I < Weights.size(); ++I)
		{
			Sum += Inputs[I] * Weights[I];
		}
		return Sign(Sum);
	}

	void Train(std::vector<float> Inputs, int Target)
	{
		int GuessValue = Guess(Inputs);
		int ErrorValue = Target - GuessValue;

		// Tune all the weights
		for (int I = 0; I < Weights.size(); ++I)
		{
			Weights[I] += ErrorValue * Inputs[I] * LR;
		}
	}

private:

	// The activation function
	float Sign(float Number)
	{
		return Number >= 0.0 ? 1.0 : -1.0;
	}

	std::vector<float> Weights;

	// Learning rate adjusts speed of learning
	float LR = 0.1f;
};

class Point
{
public:
	Point()
	{
		X = RandValueInRange(0, WindowWidth);
		Y = RandValueInRange(0, WindowHeight);

		if (X > Y)
		{
			Label = 1;
		}
		else
		{
			Label = -1;
		}
	}

	std::vector<float> GetPointsVector() const
	{
		return std::vector<float>() = {X, Y};
	}

	int GetLabel() const
	{
		return Label;
	}

private:

	float X;
	float Y;
	int Label;
};

// Returs number of errors
int TrainPerceptron(Perceptron& Perceptron, std::vector<Point> TargetPointArray)
{
	int ErrorCount = 0;
	// Train perceptron
	for (int I = 0; I < TargetPointArray.size(); ++I)
	{
		Perceptron.Train(TargetPointArray[I].GetPointsVector(), TargetPointArray[I].GetLabel());
	
		if (Perceptron.Guess(TargetPointArray[I].GetPointsVector()) != TargetPointArray[I].GetLabel())
		{
			++ErrorCount;
		}
	}

	return ErrorCount;
}

int main()
{
	srand(time(NULL));

	// Initialize perceptron
	Perceptron Perceptron;

	// Generate random points
	std::vector<Point> TargetPointArray(1000);

	// Render
	sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "BasicNeuralNetwork");

	// Point render
	sf::CircleShape PointRender(1.f);

	// Line render
	sf::Vertex LineRender[] =
	{
		sf::Vertex(sf::Vector2f(0, 0)),
		sf::Vertex(sf::Vector2f(WindowWidth, WindowHeight))
	};

	while (Window.isOpen())
	{
		Window.clear();

		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				std:: cout << "Number of errors during training: " << TrainPerceptron(Perceptron, TargetPointArray) << std::endl;
			}
		}

		// Draw points
		for (int I = 0; I < TargetPointArray.size(); ++I)
		{
			if (Perceptron.Guess(TargetPointArray[I].GetPointsVector()) == -1)
			{
				PointRender.setFillColor(sf::Color::Red);
			}
			else
			{
				PointRender.setFillColor(sf::Color::Green);
			}

			PointRender.setOutlineColor(sf::Color::White);
			PointRender.setPosition(sf::Vector2f(TargetPointArray[I].GetPointsVector()[0], TargetPointArray[I].GetPointsVector()[1]));
			Window.draw(PointRender);
		}

		Window.draw(LineRender, 2, sf::Lines);

		Window.display();
	}

	return 0;
}