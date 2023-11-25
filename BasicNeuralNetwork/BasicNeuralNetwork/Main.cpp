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

float doubleval(float value)
{
	return value * 2;
}

int main()
{
	srand(time(NULL));

	Matrix ExampleMatrix(2, 2);

	ExampleMatrix.Randomize(-100, 100);

	PrintMatrix(ExampleMatrix); std::cout << std::endl;

	ExampleMatrix = Matrix::Multiply(ExampleMatrix, 2);

	PrintMatrix(ExampleMatrix); std::cout << std::endl;

	Matrix ExampleMatrix2(3, 2);
	ExampleMatrix2.Randomize(-100, 100);
	
	PrintMatrix(ExampleMatrix2); std::cout << std::endl;
	
	Matrix ExampleMatrix3(2, 3);
	ExampleMatrix3.Randomize(-100, 100);

	PrintMatrix(ExampleMatrix3); std::cout << std::endl;

	Matrix ResultMatrix = Matrix::Multiply(ExampleMatrix2, ExampleMatrix3);

	PrintMatrix(ResultMatrix); std::cout << std::endl;

	ResultMatrix.Transpose();

	PrintMatrix(ResultMatrix); std::cout << std::endl;

	ResultMatrix = Matrix::Transpose(ResultMatrix);

	PrintMatrix(ResultMatrix); std::cout << std::endl;

	ResultMatrix.Map(doubleval);

	PrintMatrix(ResultMatrix); std::cout << std::endl;

	return 0;
}