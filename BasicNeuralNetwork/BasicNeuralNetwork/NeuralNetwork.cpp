#include "NeuralNetwork.h"

float Sigmoid(float X)
{
	return 1 / (1 + exp(-X));
}

// We assume, that derivative has already calculate y of Sigmoid(X)
float dSigmoid(float Y)
{
	return Y * (1 - Y);
}

NeuralNetwork::NeuralNetwork(int InputNodesNumber, int HiddenNodesNumber, int OutputNodesNumber, float LearningRate, float StartWeightFromRange, float StartWeightToRange)
: InputNodesNumber(InputNodesNumber), HiddenNodesNumber(HiddenNodesNumber), OutputNodesNumber(OutputNodesNumber), LearningRate(LearningRate)
{
	// Create matrix of weights between input and hidden layer
	WeightsIH = Matrix(HiddenNodesNumber, InputNodesNumber);
	// Create matrix of weights between hidden and output layer
	WeightsHO = Matrix(OutputNodesNumber, HiddenNodesNumber);
	// Randomize weights
	WeightsIH.Randomize(StartWeightFromRange, StartWeightToRange);
	WeightsHO.Randomize(StartWeightFromRange, StartWeightToRange);

	// Create matrix of bias weights between input and hidden layer
	BiasIH = Matrix(HiddenNodesNumber, 1);
	// Create matrix of bias weights between hidden and output layer
	BiasHO = Matrix(OutputNodesNumber, 1);

	// Randomize biases
	BiasIH.Randomize(StartWeightFromRange, StartWeightToRange);
	BiasHO.Randomize(StartWeightFromRange, StartWeightToRange);
}

void NeuralNetwork::Train(std::vector<float> InputArray, std::vector<float> TargetArray)
{
	// ################################################################################

	// Guess the value

	// Get the outputs
	
	// Fill input matrix
	Matrix Inputs = Matrix::FromArray(InputArray);
	// Multiplying matrices of weights and layer = W * I
	Matrix Hiddens = Matrix::Multiply(WeightsIH, Inputs);
	// Adding biases = W * I + B
	Hiddens = Matrix::Add(Hiddens, BiasIH);
	// Apply activation function = Sigmoid(W * I + B)
	Hiddens.Map(Sigmoid);

	// Generating output's ouput

	// Multiplying matrices of weights and layer = W * I
	Matrix Outputs = Matrix::Multiply(WeightsHO, Hiddens);
	// Adding biases = W * I + B
	Outputs = Matrix::Add(Outputs, BiasHO);
	// Apply activation function = Sigmoid(W * I + B)
	Outputs.Map(Sigmoid);

	// ################################################################################

	// Analyze result and train neural network

	// Prepare matrix representations of arrays
	Matrix MatrixTargets = Matrix::FromArray(TargetArray);

	// Calculate the error, adjust weights and biases

	// Example of formula is given below
	// Weight formula = LearningRate * Error o (Sigmoid(LayerResultElement) * (1 - Sigmoid(LayerResultElement)) * PreviousLayerInput 
	// Bias formula = LearningRate * Error o PreviousLayerInput

	// o - hadammard multiplication (element wise)
	// * - matrix multiplication

	// Those formulas are based on gradient descent
	// y = m * x + b
	// delta m = lr * error * x  => Weight formula
	// delat b = lr * error      => Bias formula
	// , where
	// error = Error * (Sigmoid(LayerResult) * (1 - Sigmoid(LayerResult))
	// x = PreviousLayerInput
	// lr = our given value

	// ################################################################################
	
	// Calculate hidden - output layer

	// OutputErrors = TargetArray - Outputs
	Matrix OutputErrors = Matrix::Subtract(MatrixTargets, Outputs);

	// Calculate gradient

	// Outputs - (1 - Outputs)
	Matrix OutputGradients = Matrix::Map(Outputs, dSigmoid);

	// OutputErrors o Outputs
	OutputGradients = Matrix::ElementWiseMultiplication(OutputErrors, OutputGradients);

	// LearningRate * Outputs
	OutputGradients = Matrix::Multiply(OutputGradients, LearningRate);

	// Calculate deltas
	
	// Get transposed hidden layer results
	Matrix Hiddens_T = Matrix::Transpose(Hiddens);

	// Calculate new values for delta by multiplying gradient with hidden
	Matrix Weights_HO_Deltas = Matrix::Multiply(OutputGradients, Hiddens_T);

	// Adjust output weights
	WeightsHO = Matrix::Add(WeightsHO, Weights_HO_Deltas);

	// Adjust biases
	BiasHO = Matrix::Add(BiasHO, OutputGradients);

	// ################################################################################

	// Calculate input - hidden layer

	// In order to get hidden layer error, we need to get transposed matrix of weights
	Matrix WeightsHO_T = Matrix::Transpose(WeightsHO);

	// HiddenErrors = WeightsHO_T * OutputErrors
	Matrix HiddenErrors = Matrix::Multiply(WeightsHO_T, OutputErrors);

	// Calculate gradient

	// Outputs - (1 - Outputs)
	Matrix HiddenGradients = Matrix::Map(Hiddens, dSigmoid);

	// OutputErrors * Outputs
	HiddenGradients = Matrix::ElementWiseMultiplication(HiddenErrors, HiddenGradients);

	// LearningRate * Outputs
	HiddenGradients = Matrix::Multiply(HiddenGradients, LearningRate);

	// Calculate deltas

	// Get transposed hidden layer results
	Matrix Inputs_T = Matrix::Transpose(Inputs);

	// Calculate new values for delta by multiplying gradient with hidden
	Matrix Weights_IH_Deltas = Matrix::Multiply(HiddenGradients, Inputs_T);

	// Adjust output weights
	WeightsIH = Matrix::Add(WeightsIH, Weights_IH_Deltas);

	// Adjust biases
	BiasIH = Matrix::Add(BiasIH, HiddenGradients);
}

std::vector<float> NeuralNetwork::Predict(std::vector<float> InputArray)
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

void NeuralNetwork::SetLearningRate(float NewLearningRate)
{
	LearningRate = NewLearningRate;
}
