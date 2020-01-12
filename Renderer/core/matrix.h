#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <assert.h>

#include "geometry.h"

using std::vector;

//default as 4x4 Matrix
const int Dimension = 4;

class Matrix
{
private:
	int rows_, cols_;
	vector<vector<float>> data_;

public:
	//declare constructor
	Matrix(int row = Dimension, int col = Dimension);
	//copy constructor
	Matrix(const Matrix& mat);
	//stored as column vector，eg. A(vec0, vec1, vec2, vec3)
	Matrix(const Vector4f& vec0, const Vector4f& vec1, const Vector4f& vec2, const Vector4f& vec3);

	//assign operator overload
	Matrix& operator =(const Matrix& mat);

	vector<float>& operator [](int i) { assert(i >= 0 && i < rows_); return data_[i]; }

	Matrix operator *(const Matrix& mat) const;
	Matrix operator *(float t) const;
	friend Matrix operator *(float t, const Matrix& mat);
	//right multiply Matrix by column vector
	Vector4f operator *(const Vector4f& vec) const;

	//return rows of matrix
	int row() const { return rows_; } 
	//return columns of matrix
	int col() const { return cols_; } 

	//transpose
	Matrix Transpose();
	//determinant
	float Det() const;
	//inverse
	bool Inverse();

	//identity Matrix
	static Matrix Identity(int n)
	{
		Matrix result(n, n);
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				result[i][j] = (i == j ? 1.0f : 0.0f);
			}
		}
		return result;
	}

	//zero Matrix
	static Matrix ZeroMatrix(int n)
	{
		Matrix result(n, n);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				result[i][j] = 0.0;
			}
		}
		return result;
	}

	//Scale Transformation Matrix
	static Matrix ScaleMatrix(float xScale, float yScale, float zScale)
	{
		Matrix result = Identity(Dimension);
		result[0][0] = xScale;
		result[1][1] = yScale;
		result[2][2] = zScale;
		return result;
	}

	//Rotate Transformation Matrix
	static Matrix RotateMatrix()
	{

	}

	static Matrix TranslateMatrix()
	{

	}
};

#endif
