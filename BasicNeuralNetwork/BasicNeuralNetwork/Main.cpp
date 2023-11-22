#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

const int32_t WindowWidth = 800;
const int32_t WindowHeight = 800;

float RandValueInRange(float LowValue, float HighValue)
{
	return LowValue + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (HighValue - LowValue));
}

// Maps number from one range into the other
float MapNumber(float NumberToMap, float CurrentMinRange, float CurrentMaxRange, float TargetMinRange, float TargetMaxRange)
{
	return TargetMinRange + (TargetMaxRange - TargetMinRange) * ((NumberToMap - CurrentMinRange) / (CurrentMaxRange - CurrentMinRange));
}

float LineFunction(float X)
{
	// y = mx + b
	return 0.9 * X + 0.1;
}

class Perceptron
{
public:

	// Constructor
	Perceptron(int NumberOfWeights)
	{
		Weights = std::vector<float>(NumberOfWeights);

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

	std::vector<float> GetWeights() const
	{
		return Weights;
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
		X = RandValueInRange(-1, 1);
		Y = RandValueInRange(-1, 1);
		Bias = 1;

		float LineY = LineFunction(X);
		if (Y > LineY)
		{
			Label = 1;
		}
		else
		{
			Label = -1;
		}
	}

	Point(float X, float Y) : X(X), Y(Y), Bias(1){}

	std::vector<float> GetDataVector() const
	{
		return std::vector<float>() = {X, Y, Bias};
	}

	int GetLabel() const
	{
		return Label;
	}

	float GetMappedPixelX() const
	{
		return MapNumber(X, -1, 1, 0, WindowWidth);
	}

	float GetMappedPixelY() const
	{
		return MapNumber(Y, -1, 1, WindowHeight, 0);
	}

	float GetBias() const
	{
		return Bias;
	}

private:

	float X;
	float Y;
	float Bias;
	int Label;
};

// Returs number of errors
void TrainPerceptron(Perceptron& Perceptron, std::vector<Point> TargetPointArray)
{
	// Train perceptron
	for (int I = 0; I < TargetPointArray.size(); ++I)
	{
		Perceptron.Train(TargetPointArray[I].GetDataVector(), TargetPointArray[I].GetLabel());
	}
}

float GuessYPerceptron(float XValue, Perceptron& Perceptron)
{
	std::vector<float> Weights = Perceptron.GetWeights();
	float M = Weights[1] / Weights[0];
	float Bias = Weights[2];
	// y = mx + b
	// Where 
	// x = XValue => XValue * Weights[0]
	// b = 1.0f => 1.0F * Weights[2]
	// Formula below is derivated from w0*x + w1*y + w2*b = 0
	// y => (-w0*x-w2*b)/w1
	return (-Weights[0] * XValue - Weights[2] * 1.0f) / Weights[1];
}

int main()
{
	srand(time(NULL));

	// Initialize perceptron
	Perceptron Perceptron(3);

	// Generate random points
	std::vector<Point> TargetPointArray(1000);

	// Render
	sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "BasicNeuralNetwork");

	// Point render
	sf::CircleShape PointRender(10.f);
	PointRender.setOrigin(sf::Vector2f(10.0f, 10.0f));
	PointRender.setOutlineThickness(1.0f);
	PointRender.setOutlineColor(sf::Color::White);

	// Target line render
	float TargetLineX1 = -1;
	float TargetLineY1 = LineFunction(TargetLineX1);
	float TargetLineX2 = 1;
	float TargetLineY2 = LineFunction(TargetLineX2);
	Point TargetLinePoint1 = { TargetLineX1,  TargetLineY1 };
	Point TargetLinePoint2 = { TargetLineX2,  TargetLineY2 };
	sf::Vertex TargetLineRender[] =
	{
		sf::Vertex(sf::Vector2f(TargetLinePoint1.GetMappedPixelX(), TargetLinePoint1.GetMappedPixelY())),
		sf::Vertex(sf::Vector2f(TargetLinePoint2.GetMappedPixelX(), TargetLinePoint2.GetMappedPixelY()))
	};
	TargetLineRender[0].color = sf::Color::Green;
	TargetLineRender[1].color = sf::Color::Green;

	// Perceptron Line render
	Point PerceptronLinePoint1 = { -1, GuessYPerceptron(-1,Perceptron) };
	Point PerceptronLinePoint2 = { 1, GuessYPerceptron(1,Perceptron) };
	sf::Vertex PerceptronLineRender[] =
	{
		sf::Vertex(sf::Vector2f(PerceptronLinePoint1.GetMappedPixelX(), PerceptronLinePoint1.GetMappedPixelY())),
		sf::Vertex(sf::Vector2f(PerceptronLinePoint2.GetMappedPixelX(), PerceptronLinePoint2.GetMappedPixelY()))
	};
	PerceptronLineRender[0].color = sf::Color::Red;
	PerceptronLineRender[1].color = sf::Color::Red;

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
				// Train perceptron
				TrainPerceptron(Perceptron, TargetPointArray);

				// Check errors
				int ErrorCount = 0;
				for (int I = 0; I < TargetPointArray.size(); ++I)
				{
					if (Perceptron.Guess(TargetPointArray[I].GetDataVector()) != TargetPointArray[I].GetLabel())
					{
						++ErrorCount;
					}
				}
				std::cout << "Number of errors during training: " << ErrorCount << std::endl;
			}
		}

		// Draw points
		for (int I = 0; I < TargetPointArray.size(); ++I)
		{
			if (Perceptron.Guess(TargetPointArray[I].GetDataVector()) == TargetPointArray[I].GetLabel())
			{
				PointRender.setFillColor(sf::Color::Green);
			}
			else
			{
				PointRender.setFillColor(sf::Color::Red);
			}

			// Map position of the point
			sf::Vector2f MappedPointPosition = { 
				TargetPointArray[I].GetMappedPixelX(),
				TargetPointArray[I].GetMappedPixelY()
			};

			PointRender.setPosition(MappedPointPosition);
			Window.draw(PointRender);
		}

		PerceptronLinePoint1 = { -1, GuessYPerceptron(-1,Perceptron) };
		PerceptronLinePoint2 = { 1, GuessYPerceptron(1,Perceptron) };
		PerceptronLineRender[0].position = sf::Vector2f(PerceptronLinePoint1.GetMappedPixelX(), PerceptronLinePoint1.GetMappedPixelY());
		PerceptronLineRender[1].position = sf::Vector2f(PerceptronLinePoint2.GetMappedPixelX(), PerceptronLinePoint2.GetMappedPixelY());
		 
		Window.draw(PerceptronLineRender, 2, sf::Lines);

		Window.draw(TargetLineRender, 2, sf::Lines);

		Window.display();
	}

	return 0;
}