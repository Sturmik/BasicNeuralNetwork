#include "Matrix.h"
#include "Utility.h"

const unsigned int ConvertRowAndColumnToIndex(unsigned int Row, unsigned int Column, unsigned int TotalColumns)
{
	return (Row * TotalColumns) + Column;
}

Matrix::Matrix() : Matrix(0, 0)
{
}

Matrix::Matrix(int Rows, int Columns) : Rows(Rows), Columns(Columns)
{
	MatrixArray = std::vector<float>(Rows * Columns, 0);
}

Matrix::Matrix(int Rows, int Columns, float InitialValue) : Rows(Rows), Columns(Columns)
{
	MatrixArray = std::vector<float>(Rows * Columns, InitialValue);
}

int Matrix::GetRows() const
{
	return Rows;
}

int Matrix::GetColumns() const
{
	return Columns;
}

std::vector<float> Matrix::ToArray() const
{
	return MatrixArray;
}

Matrix Matrix::FromArray(std::vector<float> Array)
{
	Matrix ResultMatrix(Array.size(), 1);
	ResultMatrix.MatrixArray = Array;
	return ResultMatrix;
}

Matrix Matrix::Add(Matrix MatrixA, float Value)
{
	Matrix ResultMatrix = MatrixA;
	for (int R = 0; R < ResultMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < ResultMatrix.GetColumns(); ++C)
		{
			ResultMatrix.GetElement(R, C) += Value;
		}
	}
	return ResultMatrix;
}

Matrix Matrix::Add(Matrix MatrixA, Matrix MatrixB)
{
	if (MatrixA.GetRows() != MatrixB.GetRows() || MatrixA.GetColumns() != MatrixB.GetColumns())
	{
		return Matrix(0, 0);
	}

	Matrix ResultMatrix = MatrixA;

	for (int R = 0; R < ResultMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < ResultMatrix.GetColumns(); ++C)
		{
			ResultMatrix.GetElement(R, C) += MatrixB.GetElement(R, C);
		}
	}

	return ResultMatrix;
}

Matrix Matrix::Subtract(Matrix MatrixA, float Value)
{
	Matrix ResultMatrix = MatrixA;

	for (int R = 0; R < ResultMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < ResultMatrix.GetColumns(); ++C)
		{
			ResultMatrix.GetElement(R, C) -= Value;
		}
	}

	return ResultMatrix;
}

Matrix Matrix::Subtract(Matrix MatrixA, Matrix MatrixB)
{
	if (MatrixA.GetRows() != MatrixB.GetRows() || MatrixA.GetColumns() != MatrixB.GetColumns())
	{
		return Matrix(0, 0);
	}

	Matrix ResultMatrix = MatrixA;

	for (int R = 0; R < ResultMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < ResultMatrix.GetColumns(); ++C)
		{
			ResultMatrix.GetElement(R, C) -= MatrixB.GetElement(R, C);
		}
	}

	return ResultMatrix;
}

Matrix Matrix::Multiply(Matrix MatrixA, float Scale)
{
	Matrix ResultMatrix = MatrixA;
	for (int R = 0; R < ResultMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < ResultMatrix.GetColumns(); ++C)
		{
			ResultMatrix.GetElement(R, C) *= Scale;
		}
	}
	return ResultMatrix;
}

Matrix Matrix::Multiply(Matrix MatrixA, Matrix MatrixB)
{
	if (MatrixA.GetColumns() != MatrixB.GetRows())
	{
		return Matrix(0, 0);
	}

	Matrix ResultMatrix(MatrixA.GetRows(), MatrixB.GetColumns());

	for (int R = 0; R < ResultMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < ResultMatrix.GetColumns(); ++C)
		{
			for (int It = 0; It < MatrixA.GetColumns(); ++It)
			{
				ResultMatrix.GetElement(R, C) += MatrixA.GetElement(R, It) * MatrixB.GetElement(It, C);
			}
		}
	}

	return ResultMatrix;
}

Matrix Matrix::ElementWiseMultiplication(Matrix MatrixA, Matrix MatrixB)
{
	if (MatrixA.GetRows() != MatrixB.GetRows() || MatrixA.GetColumns() != MatrixB.GetColumns())
	{
		return Matrix(0, 0);
	}

	Matrix ResultMatrix = MatrixA;

	for (int R = 0; R < ResultMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < ResultMatrix.GetColumns(); ++C)
		{
			ResultMatrix.GetElement(R, C) *= MatrixB.GetElement(R, C);
		}
	}

	return ResultMatrix;
}

void Matrix::Transpose()
{
	Matrix ResultMatrix(Columns, Rows);

	for (int R = 0; R < ResultMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < ResultMatrix.GetColumns(); ++C)
		{
			ResultMatrix.GetElement(R, C) = GetElement(C, R);
		}
	}

	*this = ResultMatrix;
}

Matrix Matrix::Transpose(Matrix MatrixA)
{
	Matrix ResultMatrix(MatrixA.GetColumns(), MatrixA.GetRows());

	for (int R = 0; R < ResultMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < ResultMatrix.GetColumns(); ++C)
		{
			ResultMatrix.GetElement(R, C) = MatrixA.GetElement(C, R);
		}
	}

	return ResultMatrix;
}

float& Matrix::GetElement(int Row, int Column)
{
	return MatrixArray[ConvertRowAndColumnToIndex(Row, Column, Columns)];
}

void Matrix::Randomize(float StartRange, float EndRange)
{
	if (StartRange > EndRange)
	{
		std::swap(StartRange, EndRange);
	}

	for (int R = 0; R < Rows; ++R)
	{
		for (int C = 0; C < Columns; ++C)
		{
			GetElement(R, C) = RandValueInRange(StartRange, EndRange);
		}
	}
}

void Matrix::Map(float(*Func)(float))
{
	for (int R = 0; R < GetRows(); ++R)
	{
		for (int C = 0; C < GetColumns(); ++C)
		{
			GetElement(R, C) = Func(GetElement(R, C));
		}
	}
}

Matrix Matrix::Map(Matrix MatrixA, float(*Func)(float))
{
	Matrix ResultMatrix = MatrixA;

	for (int R = 0; R < ResultMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < ResultMatrix.GetColumns(); ++C)
		{
			ResultMatrix.GetElement(R, C) = Func(ResultMatrix.GetElement(R, C));
		}
	}

	return ResultMatrix;
}
