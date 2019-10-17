#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <algorithm>

template <typename T>
class Vector2
{
public:
	T x, y;

	//declare constructor-initialize
	Vector2() :x(0), y(0) {}
	Vector2(T _x, T _y) : x(_x), y(_y) {}
	//copy constructor-initialize
	Vector2(const Vector2<T>& vec) : x(vec.x), y(vec.y) {}

	Vector2<T> &operator=(const Vector2<T>& vec)
	{
		x = vec.x;
		y = vec.y;
		return *this;
	}

	//get vector that has reverse direction
	Vector2<T> operator-() const { return Vector2<T>(-x, -y); } 
	Vector2<T> operator+(const Vector2<T>& vec) const { return Vector2<T>(x + vec.x, y + vec.y); }
	Vector2<T> operator-(const Vector2<T>& vec) const { return Vector2<T>(x - vec.x, y - vec.y); }
	Vector2<T> operator*(float t) const { return Vector2<T>(x * t, y * t); }
	Vector2<T> operator/(float t) const { return Vector2<T>(x / t, y / t); }

	Vector2<T>& operator+=(const Vector2<T>& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}
	Vector2<T>& operator-=(const Vector2<T>& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}
	Vector2<T>& operator*=(float t)
	{
		x *= t;
		y *= t;
		return *this;
	}
	Vector2<T>& operator/=(float t)
	{
		x /= t;
		y /= t;
		return *this;
	}

	friend Vector2<T> operator*(float t, const Vector2<T>& vec) { return Vector2<T>(t * vec.x, t * vec.y); }


	//negate each element
	void negate()
	{
		x = -x;
		y = -y;
	}

 	//dot product
	T dot(const Vector2<T>& vec) const { return x * vec.x + y * vec.y; }
	//length of vector
	float length() const { return sqrt(x * x + y * y); }
	//normalization of vector
	Vector2<T>& normalize()
	{
		*this = (*this) * T(1) / length();
		return *this;
	}
	//外法向量
	Vector2<T> normal() const { return Vector2<T>(-y, x); }
};


template <typename T>
class Vector3
{
public:
	T x, y, z;

 	//declare constructor-initialize
	Vector3() :x(0), y(0), z(0) {}
	Vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	//copy constructor-initialize
	Vector3(const Vector3<T>& vec) : x(vec.x), y(vec.y), z(vec.z) {}	

	//assign operator overload
	Vector3<T>& operator=(const Vector3<T>& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	//get vector that has reverse direction
	Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); } 
	Vector3<T> operator+(const Vector3<T>& vec) const { return Vector3<T>(x + vec.x, y + vec.y, z + vec.z); }
	Vector3<T> operator-(const Vector3<T>& vec) const { return Vector3<T>(x - vec.x, y - vec.y, z - vec.z); }
	Vector3<T> operator*(float t) const { return Vector3<T>(x * t, y * t, z * t); }
	Vector3<T> operator/(float t) const { return Vector3<T>(x / t, y / t, z / t); }

	Vector3<T>& operator+=(const Vector3<T>& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}
	Vector3<T>& operator-=(const Vector3<T>& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}
	Vector3<T>& operator*=(float t)
	{
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}
	Vector3<T>& operator/=(float t)
	{
		x /= t;
		y /= t;
		z /= t;
		return *this;
	}

	friend Vector3<T> operator*(float t, const Vector3<T>& vec) { return Vector3<T>(t * vec.x, t * vec.y, t * vec.z); }

	//negate each element
	void negate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	//cross product
	Vector3<T> cross(const Vector3<T>& vec) const { return Vector3<T>(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x); }
	//dot product
	T dot(const Vector3<T>& vec) const { return x * vec.x + y * vec.y + z * vec.z; }
	//length of vector
	float length() const { return sqrt(x * x + y * y + z * z); }
	//normalization of vector
	Vector3<T>& normalize()
	{
		*this = (*this) * T(1) / length();
		return *this;
	}
};


template <typename T>
class Vector4
{
public:
	T x, y, z, w;

	//declare constructor-initialize
	Vector4() :x(0), y(0), z(0), w(1) {}
	Vector4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
	//copy constructor-initialize
	Vector4(const Vector4<T>& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}

	//assign operator overload
	Vector4<T>& operator=(const Vector4<T>& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
		return *this;
	}

	//get vector that has reverse direction
	Vector4<T> operator-() const { return Vector4<T>(-x, -y, -z, -w); }
	Vector4<T> operator+(const Vector4<T>& vec) const { return Vector4<T>(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	Vector4<T> operator-(const Vector4<T>& vec) const { return Vector4<T>(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	Vector4<T> operator*(float t) const { return Vector4<T>(x * t, y * t, z * t, w * t); }
	Vector4<T> operator/(float t) const { return Vector4<T>(x / t, y / t, z / t, w / t); }

	Vector4<T>& operator+=(const Vector4<T>& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;
		return *this;
	}
	Vector4<T>& operator-=(const Vector4<T>& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
		return *this;
	}
	Vector4<T>& operator*=(float t)
	{
		x *= t;
		y *= t;
		z *= t;
		w *= t;
		return *this;
	}
	Vector4<T>& operator/=(float t)
	{
		x /= t;
		y /= t;
		z /= t;
		w /= t;
		return *this;
	}

	friend Vector4<T> operator*(float t, const Vector4<T>& vec) { return Vector4<T>(t * vec.x, t * vec.y, t * vec.z, t*vec.w); }

	//negate each element
	void negate()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}

	//dot product
	T dot(const Vector4<T>& vec) const { return x * vec.x + y * vec.y + z * vec.z + w * vec.w; }
	//length of vector
	float length() const { return sqrt(x * x + y * y + z * z + w * w); }
	//normalization of vector
	Vector4<T>& normalize()
	{
		*this = (*this) * T(1) / sqrt(x * x + y * y + z * z + w * w);
		return *this;
	}
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector4<int> Vector4i;
typedef Vector4<float> Vector4f;

typedef Vector2f Point2d;
typedef Vector3f Point3d;

#endif