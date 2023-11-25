#include "Matrix.h"
#include "Utility.h"

const unsigned int ConvertRowAndColumnToIndex(unsigned int Row, unsigned int Column, unsigned int TotalColumns)
{
	return (Row * TotalColumns) + Column;
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

Matrix Matrix::Add(Matrix& MatrixA, float Value)
{
	Matrix NewMatrix = MatrixA;
	for (int R = 0; R < NewMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < NewMatrix.GetColumns(); ++C)
		{
			NewMatrix.GetElement(R, C) += Value;
		}
	}
	return NewMatrix;
}

Matrix Matrix::Add(Matrix& MatrixA, Matrix& MatrixB)
{
	if (MatrixA.GetRows() != MatrixB.GetRows() || MatrixA.GetColumns() != MatrixB.GetColumns())
	{
		return Matrix(0, 0);
	}

	Matrix NewMatrix = MatrixA;

	for (int R = 0; R < NewMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < NewMatrix.GetColumns(); ++C)
		{
			NewMatrix.GetElement(R, C) *= MatrixB.GetElement(R, C);
		}
	}

	return NewMatrix;
}

Matrix Matrix::Multiply(Matrix& MatrixA, float Scale)
{
	Matrix NewMatrix = MatrixA;
	for (int R = 0; R < NewMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < NewMatrix.GetColumns(); ++C)
		{
			NewMatrix.GetElement(R, C) *= Scale;
		}
	}
	return NewMatrix;
}

Matrix Matrix::Multiply(Matrix& MatrixA, Matrix& MatrixB)
{
	if (MatrixA.GetColumns() != MatrixB.GetRows())
	{
		return Matrix(0, 0);
	}

	Matrix NewMatrix(MatrixA.GetRows(), MatrixB.GetColumns());

	for (int R = 0; R < NewMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < NewMatrix.GetColumns(); ++C)
		{
			for (int It = 0; It < MatrixA.GetColumns(); ++It)
			{
				NewMatrix.GetElement(R, C) += MatrixA.GetElement(R, It) * MatrixB.GetElement(It, C);
			}
		}
	}

	return NewMatrix;
}

void Matrix::Transpose()
{
	Matrix NewMatrix(Columns, Rows);

	for (int R = 0; R < NewMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < NewMatrix.GetColumns(); ++C)
		{
			NewMatrix.GetElement(R, C) = GetElement(C, R);
		}
	}

	*this = NewMatrix;
}

Matrix Matrix::Transpose(Matrix& MatrixA)
{
	Matrix NewMatrix(MatrixA.GetColumns(), MatrixA.GetRows());

	for (int R = 0; R < NewMatrix.GetRows(); ++R)
	{
		for (int C = 0; C < NewMatrix.GetColumns(); ++C)
		{
			NewMatrix.GetElement(R, C) = MatrixA.GetElement(C, R);
		}
	}

	return NewMatrix;
}

float& Matrix::GetElement(int Row, int Column)
{
	if (Row >= 0 && Row < Rows && Column >= 0 && Column < Columns)
	{
		return MatrixArray[ConvertRowAndColumnToIndex(Row, Column, Columns)];
	}
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
