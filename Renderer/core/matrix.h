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
	vector<vector<double>> data;

public:
	//declare constructor
	Matrix(int row = DEFAULT_ALLOC, int col = DEFAULT_ALLOC) 
	{
		_rows = row;
		_cols = col;
		data = vector<vector<double>>(row, vector<double>(col, 0.0)); //(size, element);
	}

	//copy constructor
	Matrix(const Matrix& mat)
	{
		for (int i = 0; i < mat.row(); i++)
			for (int j = 0; j < mat.col(); j++)
				data[i][j] = mat.data[i][j];
	}
	// 以列向量方式存放，即矩阵 A(vec0, vec1, vec2, vec3)
	Matrix(const Vector4d& vec0, const Vector4d& vec1, const Vector4d& vec2, const Vector4d& vec3)
	{
		_rows = DEFAULT_ALLOC;
		_cols = DEFAULT_ALLOC;
		data = vector<vector<double>>(DEFAULT_ALLOC, vector<double>(DEFAULT_ALLOC, 0.0)); //(size, element);
		data[0][0] = vec0.x; data[0][1] = vec1.x; data[0][2] = vec2.x; data[0][3] = vec3.x;
		data[1][0] = vec0.y; data[1][1] = vec1.y; data[1][2] = vec2.y; data[1][3] = vec3.y;
		data[2][0] = vec0.z; data[2][1] = vec1.z; data[2][2] = vec2.z; data[2][3] = vec3.z;
		data[3][0] = vec0.w; data[3][1] = vec1.w; data[3][2] = vec2.w; data[3][3] = vec3.w;
	}

	//assign operator overload
	Matrix& operator =(const Matrix& mat) {
		assert(_rows == mat.row() && _cols == mat.col());
		for (int i = 0; i < _rows; i++) 
			for (int j = 0; j < _cols; j++) 
				data[i][j] = mat.data[i][j];

		return *this;
	}

	vector<double>& operator [](int i) { assert(i >= 0 && i < _rows); return data[i]; }

	Matrix operator *(const Matrix& mat) const
	{
		assert(_cols == mat.row()); //矩阵相乘得保证左行等于右列

		Matrix result(_rows, mat.col());
		for (int i = 0; i < _rows; i++){
			for (int j = 0; j < mat.col(); j++){
				result.data[i][j] = 0;
				for (int k = 0; k < _cols; k++)
					result.data[i][j] += data[i][k] * mat.data[k][j];
			}
		}
		return result;
	}

	Matrix operator *(double t) const 
	{ 
		Matrix result(_rows, _cols);
		for (int i = 0; i < _rows; i++) {
			for (int j = 0; j < _cols; j++) {
				result.data[i][j] = data[i][j] * t;
			}
		}
		return result;
	}

	friend Matrix operator *(double t, const Matrix& mat)
	{
		Matrix result(mat.row(), mat.col());
		for (int i = 0; i < mat.row(); i++) {
			for (int j = 0; j < mat.col(); j++)
				result.data[i][j] = mat.data[i][j] * t;
		}
		return result;
	}

	//对矩阵列分块，然后右乘一个列向量
	Vector4d operator *(const Vector4d& vec) const
	{
		Vector4d result;
		result.x = data[0][0] * vec.x + data[0][1] * vec.y + data[0][2] * vec.z + data[0][3] * vec.w;
		result.y = data[1][0] * vec.x + data[1][1] * vec.y + data[1][2] * vec.z + data[1][3] * vec.w;
		result.z = data[2][0] * vec.x + data[2][1] * vec.y + data[2][2] * vec.z + data[2][3] * vec.w;
		result.w = data[3][0] * vec.x + data[3][1] * vec.y + data[3][2] * vec.z + data[3][3] * vec.w;

		return result;
	}

	//return rows of matrix
	int row() const { return _rows; } 
	//return columns of matrix
	int col() const { return _cols; } 

	//transpose
	Matrix transpose() 
	{
		Matrix result;
		for (int i = 0; i < result.row(); i++)
			for (int j = 0; j < result.col(); j++)
				result.data[j][i] = data[i][j];

		return result;
	}

	//默认计算3阶方阵的行列式
	double det() const
	{
		double det = data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) - data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0])
			+ data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
		return det;
	}

	//默认求三阶可逆方针的逆矩阵
	bool inverse() 
	{
		double det = this->det();
		if (det == 0)
			return false;

		Matrix adjoint; //伴随矩阵
		adjoint.data[0][0] = data[1][1] * data[2][2] - data[1][2] * data[2][1];
		adjoint.data[1][0] = -(data[1][0] * data[2][2] - data[1][2] * data[2][0]);
		adjoint.data[2][0] = data[1][0] * data[2][1] - data[1][1] * data[2][0];

		adjoint.data[0][1] = -(data[0][1] * data[2][2] - data[0][2] * data[2][1]);
		adjoint.data[1][1] = data[0][0] * data[2][2] - data[0][2] * data[2][0];
		adjoint.data[2][1] = -(data[0][0] * data[2][1] - data[0][1] * data[2][0]);

		adjoint.data[0][2] = data[0][1] * data[1][2] - data[0][2] * data[1][1];
		adjoint.data[1][2] = -(data[0][0] * data[1][2] - data[0][2] * data[1][0]);
		adjoint.data[2][2] = data[0][0] * data[1][1] - data[0][1] * data[1][0];

		det = 1.0 / det;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				data[i][j] = adjoint.data[i][j] * det;

		return true;
	}

	//单位阵
	static Matrix identity(int n)
	{
		Matrix result(n, n);
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				result[i][j] = (i == j ? 1.0 : 0.0);
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
	static Matrix scaleMatrix(double xScale, double yScale, double zScale)
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
