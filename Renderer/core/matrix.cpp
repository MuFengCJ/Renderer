#include "matrix.h"

//declare constructor
Matrix::Matrix(int row, int col)
{
	rows_ = row;
	cols_ = col;
	data_ = vector<vector<float>>(row, vector<float>(col, 0.0f)); //(size, element);
}

//copy constructor
Matrix::Matrix(const Matrix& mat)
{
	for (int i = 0; i < mat.row(); i++) {
		for (int j = 0; j < mat.col(); j++)
			data_[i][j] = mat.data_[i][j];
	}
}

Matrix::Matrix(const Vector4f& vec0, const Vector4f& vec1, const Vector4f& vec2, const Vector4f& vec3)
{
	rows_ = Dimension;
	cols_ = Dimension;
	data_ = vector<vector<float>>(Dimension, vector<float>(Dimension, 0.0)); //(size, element);
	data_[0][0] = vec0.x; data_[0][1] = vec1.x; data_[0][2] = vec2.x; data_[0][3] = vec3.x;
	data_[1][0] = vec0.y; data_[1][1] = vec1.y; data_[1][2] = vec2.y; data_[1][3] = vec3.y;
	data_[2][0] = vec0.z; data_[2][1] = vec1.z; data_[2][2] = vec2.z; data_[2][3] = vec3.z;
	data_[3][0] = vec0.w; data_[3][1] = vec1.w; data_[3][2] = vec2.w; data_[3][3] = vec3.w;
}

//assign operator overload
Matrix& Matrix::operator =(const Matrix& mat)
{
	assert(rows_ == mat.row() && cols_ == mat.col());
	for (int i = 0; i < rows_; i++)
		for (int j = 0; j < cols_; j++)
			data_[i][j] = mat.data_[i][j];

	return *this;
}

Matrix Matrix::operator *(const Matrix& mat) const
{
	assert(cols_ == mat.row());

	Matrix result(rows_, mat.col());
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < mat.col(); j++) {
			result.data_[i][j] = 0;
			for (int k = 0; k < cols_; k++)
				result.data_[i][j] += data_[i][k] * mat.data_[k][j];
		}
	}
	return result;
}

Matrix Matrix::operator *(float t) const
{
	Matrix result(rows_, cols_);
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			result.data_[i][j] = data_[i][j] * t;
		}
	}
	return result;
}

Matrix operator *(float t, const Matrix& mat)
{
	Matrix result(mat.row(), mat.col());
	for (int i = 0; i < mat.row(); i++) {
		for (int j = 0; j < mat.col(); j++)
			result.data_[i][j] = mat.data_[i][j] * t;
	}
	return result;
}

Vector4f Matrix::operator *(const Vector4f& vec) const
{
	Vector4f result;
	result.x = data_[0][0] * vec.x + data_[0][1] * vec.y + data_[0][2] * vec.z + data_[0][3] * vec.w;
	result.y = data_[1][0] * vec.x + data_[1][1] * vec.y + data_[1][2] * vec.z + data_[1][3] * vec.w;
	result.z = data_[2][0] * vec.x + data_[2][1] * vec.y + data_[2][2] * vec.z + data_[2][3] * vec.w;
	result.w = data_[3][0] * vec.x + data_[3][1] * vec.y + data_[3][2] * vec.z + data_[3][3] * vec.w;

	return result;
}

Matrix Matrix::Transpose()
{
	Matrix result;
	for (int i = 0; i < result.row(); i++)
		for (int j = 0; j < result.col(); j++)
			result.data_[j][i] = data_[i][j];

	return result;
}

float Matrix::Det() const
{
	float det = data_[0][0] * (data_[1][1] * data_[2][2] - data_[1][2] * data_[2][1]) - data_[0][1] * (data_[1][0] * data_[2][2] - data_[1][2] * data_[2][0])
		+ data_[0][2] * (data_[1][0] * data_[2][1] - data_[1][1] * data_[2][0]);
	return det;
}

bool Matrix::Inverse()
{
	float det = this->Det();
	if (det == 0)
		return false;

	Matrix adjoint; //adjoint matrix
	adjoint.data_[0][0] = data_[1][1] * data_[2][2] - data_[1][2] * data_[2][1];
	adjoint.data_[1][0] = -(data_[1][0] * data_[2][2] - data_[1][2] * data_[2][0]);
	adjoint.data_[2][0] = data_[1][0] * data_[2][1] - data_[1][1] * data_[2][0];

	adjoint.data_[0][1] = -(data_[0][1] * data_[2][2] - data_[0][2] * data_[2][1]);
	adjoint.data_[1][1] = data_[0][0] * data_[2][2] - data_[0][2] * data_[2][0];
	adjoint.data_[2][1] = -(data_[0][0] * data_[2][1] - data_[0][1] * data_[2][0]);

	adjoint.data_[0][2] = data_[0][1] * data_[1][2] - data_[0][2] * data_[1][1];
	adjoint.data_[1][2] = -(data_[0][0] * data_[1][2] - data_[0][2] * data_[1][0]);
	adjoint.data_[2][2] = data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0];

	det = 1.0f / det;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			data_[i][j] = adjoint.data_[i][j] * det;

	return true;
}