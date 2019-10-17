#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <assert.h>

#include "geometry.h"

using std::vector;

const int DEFAULT_ALLOC = 4; //默认为4阶矩阵

class Matrix
{
private:
	int _rows, _cols;
	vector<vector<float>> data;

public:
	//declare constructor
	Matrix(int row = DEFAULT_ALLOC, int col = DEFAULT_ALLOC);
	//copy constructor
	Matrix(const Matrix& mat);
	// 以列向量方式存放，即矩阵 A(vec0, vec1, vec2, vec3)
	Matrix(const Vector4f& vec0, const Vector4f& vec1, const Vector4f& vec2, const Vector4f& vec3);

	//assign operator overload
	Matrix& operator =(const Matrix& mat);

	vector<float>& operator [](int i) { assert(i >= 0 && i < _rows); return data[i]; }

	Matrix operator *(const Matrix& mat) const;
	Matrix operator *(float t) const;
	friend Matrix operator *(float t, const Matrix& mat);
	//对矩阵列分块，然后右乘一个列向量
	Vector4f operator *(const Vector4f& vec) const;

	//return rows of matrix
	int row() const { return _rows; } 
	//return columns of matrix
	int col() const { return _cols; } 

	//transpose
	Matrix transpose();
	//默认计算3阶方阵的行列式
	float det() const;
	//默认求三阶可逆方针的逆矩阵
	bool inverse();

	//单位阵
	static Matrix identity(int n)
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
	static Matrix zeroMatrix(int n)
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
	static Matrix scaleMatrix(float xScale, float yScale, float zScale)
	{
		Matrix result = identity(DEFAULT_ALLOC);
		result[0][0] = xScale;
		result[1][1] = yScale;
		result[2][2] = zScale;
		return result;
	}

	//Rotate Transformation Matrix
	static Matrix rotateMatrix()
	{

	}

	static Matrix translateMatrix()
	{

	}
};

#endif
