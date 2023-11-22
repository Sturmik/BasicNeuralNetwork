#include <iostream>
#include <vector>

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
		X = RandValueInRange(-1000, 1000);
		Y = RandValueInRange(-1000, 1000);

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

int main()
{
	srand(time(NULL));

	// Generate random points
	std::vector<Point> TargetPointArray(1000);

	// Initialize perceptron
	Perceptron Perceptron;

	int TrainingIterationCounter = 0;
	int ErrorCount = 1;
	// Train perceptron	
	while (ErrorCount > 0)
	{
		// Check perceptron
		ErrorCount = 0;
		for (int I = 0; I < TargetPointArray.size(); ++I)
		{
			if (Perceptron.Guess(TargetPointArray[I].GetPointsVector()) != TargetPointArray[I].GetLabel())
			{
				++ErrorCount;
			}
		}
		std::cout << "Training Iteration: " << TrainingIterationCounter++  << " | Error Count: " << ErrorCount << std::endl;
	
		// Train perceptron
		for (int I = 0; I < TargetPointArray.size(); ++I)
		{
			Perceptron.Train(TargetPointArray[I].GetPointsVector(), TargetPointArray[I].GetLabel());
		}
	}

	// Check perceptron
	ErrorCount = 0;
	for (int I = 0; I < TargetPointArray.size(); ++I)
	{
		if (Perceptron.Guess(TargetPointArray[I].GetPointsVector()) != TargetPointArray[I].GetLabel())
		{
			++ErrorCount;
		}
	}
	std::cout << "Check of perceptron | Error Count: " << ErrorCount << std::endl;
	
	// Test perceptron guess
	std::vector<float> Inputs = {-1, 0.5};

	float GuessValue = Perceptron.Guess(Inputs);

	std::cout << "Check guess must be equal (-1): " << GuessValue << std::endl;
}