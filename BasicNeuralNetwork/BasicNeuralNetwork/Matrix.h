#pragma once

#include <vector>

class Matrix 
{
public:
	Matrix(int Rows, int Columns);

	Matrix(int Rows, int Columns, float InitialValue);

	int GetRows() const;

	int GetColumns() const;

	static Matrix Add(Matrix& MatrixA, float Value);

	static Matrix Add(Matrix& MatrixA, Matrix& MatrixB);

	static Matrix Multiply(Matrix& MatrixA, float Scale);

	static Matrix Multiply(Matrix& MatrixA, Matrix& MatrixB);

	// Transposes matrix
	void Transpose();

	static Matrix Transpose(Matrix& MatrixA);

	float& GetElement(int Row, int Column);

	// Randomizes values in the matrix in the given range
	void Randomize(float StartRange, float EndRange);

	// Applies function to all elements of the matrix
	void Map(float (*Func)(float));

private:

	int Rows;

	int Columns;

	std::vector<float> MatrixArray;
};