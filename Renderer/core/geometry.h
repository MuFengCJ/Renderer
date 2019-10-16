#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <assert.h>
#include <algorithm>

const double GLH_EPSILON = 10e-6;
const double PI = 3.14159265358979323846;

inline bool isEqual(double a, double b, double tol = GLH_EPSILON)
{
	return std::fabs(a - b) < tol;
}

inline bool isZero(double a, double tol = GLH_EPSILON)
{
	return std::fabs(a - 0.0) < tol;
}

class Vector2d
{
public:
	double x, y;

	Vector2d(double _x = 0.0, double _y = 0.0) : x(_x), y(_y) {}
	Vector2d(const Vector2d& vec) : x(vec.x), y(vec.y) {}

	Vector2d &operator=(const Vector2d& vec)
	{
		x = vec.x;
		y = vec.y;
		return *this;
	}

	//get vector that has reverse direction
	Vector2d operator-() const { return Vector2d(-x, -y); } 
	Vector2d operator+(const Vector2d& vec) const { return Vector2d(x + vec.x, y + vec.y); }
	Vector2d operator-(const Vector2d& vec) const { return Vector2d(x - vec.x, y - vec.y); }
	Vector2d operator*(double t) const { return Vector2d(x * t, y * t); }
	Vector2d operator/(double t) const { return Vector2d(x / t, y / t); }

	Vector2d& operator+=(const Vector2d& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}
	Vector2d& operator-=(const Vector2d& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}
	Vector2d& operator*=(double t)
	{
		x *= t;
		y *= t;
		return *this;
	}
	Vector2d& operator/=(double t)
	{
		x /= t;
		y /= t;
		return *this;
	}

	friend Vector2d operator*(double t, const Vector2d& vec) { return Vector2d(t * vec.x, t * vec.y); }

	bool operator==(const Vector2d& vec) const { return isEqual(x, vec.x) && isEqual(y, vec.y); }
	bool operator!=(const Vector2d& vec) const { return !((*this) == vec); }

	//negate each element
	void negate()
	{
		x = -x;
		y = -y;
	}

 	//dot product
	double dot(const Vector2d& vec) const { return x * vec.x + y * vec.y; }
	//normalization of vector
	Vector2d& normalize()
	{
		*this = (*this) * 1.0 / sqrt(x * x + y * y);
		return *this;
	}
	//length of vector
	double length() const { return sqrt(x * x + y * y); } 
	//外法向量
	Vector2d normal() const { return Vector2d(-y, x); }

	//zero vector
	static Vector2d zero() { return Vector2d(0.0, 0.0); } 
};

class Vector3d
{
public:
		double x, y, z;

 	//declare constructor-initialize
	Vector3d(double _x = 0.0, double _y = 0.0, double _z = 0.0) : x(_x), y(_y), z(_z) {}
	//copy constructor-initialize
	Vector3d(const Vector3d& vec) : x(vec.x), y(vec.y), z(vec.z) {}	

	//assign operator overload
	Vector3d& operator=(const Vector3d& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	//get vector that has reverse direction
	Vector3d operator-() const { return Vector3d(-x, -y, -z); } 
	Vector3d operator+(const Vector3d& vec) const { return Vector3d(x + vec.x, y + vec.y, z + vec.z); }
	Vector3d operator-(const Vector3d& vec) const { return Vector3d(x - vec.x, y - vec.y, z - vec.z); }
	Vector3d operator*(double t) const { return Vector3d(x * t, y * t, z * t); }
	Vector3d operator/(double t) const { return Vector3d(x / t, y / t, z / t); }

	Vector3d& operator+=(const Vector3d& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}
	Vector3d& operator-=(const Vector3d& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}
	Vector3d& operator*=(double t)
	{
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}
	Vector3d& operator/=(double t)
	{
		x /= t;
		y /= t;
		z /= t;
		return *this;
	}

	friend Vector3d operator*(double t, const Vector3d& vec) { return Vector3d(t * vec.x, t * vec.y, t * vec.z); }

	bool operator==(const Vector3d& vec) const { return isEqual(x, vec.x) && isEqual(y, vec.y) && isEqual(z, vec.z); }
	bool operator!=(const Vector3d& vec) const { return !((*this) == vec); }

	//negate each element
	void negate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	//cross product
	Vector3d cross(const Vector3d& vec) const { return Vector3d(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x); } 
	//dot product
	double dot(const Vector3d& vec) const { return x * vec.x + y * vec.y + z * vec.z; }												
	//normalization of vector
	Vector3d& normalize()
	{
		*this = (*this) * 1.0 / sqrt(x * x + y * y + z * z);
		return *this;
	}
	//length of vector
	double length() const { return sqrt(x * x + y * y + z * z); } 
	//zero vector
	static Vector3d zero() { return Vector3d(0.0, 0.0, 0.0); } 
};

class Vector4d
{
public:
	double x, y, z, w;

	//declare constructor-initialize
	Vector4d(double _x = 0.0, double _y = 0.0, double _z = 0.0, double _w = 1.0) : x(_x), y(_y), z(_z), w(_w){}
	//copy constructor-initialize
	Vector4d(const Vector4d& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}

	//assign operator overload
	Vector4d& operator=(const Vector4d& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
		return *this;
	}

	//get vector that has reverse direction
	Vector4d operator-() const { return Vector4d(-x, -y, -z, -w); }
	Vector4d operator+(const Vector4d& vec) const { return Vector4d(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
	Vector4d operator-(const Vector4d& vec) const { return Vector4d(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	Vector4d operator*(double t) const { return Vector4d(x * t, y * t, z * t, w * t); }
	Vector4d operator/(double t) const { return Vector4d(x / t, y / t, z / t, w / t); }

	Vector4d& operator+=(const Vector4d& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;
		return *this;
	}
	Vector4d& operator-=(const Vector4d& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
		return *this;
	}
	Vector4d& operator*=(double t)
	{
		x *= t;
		y *= t;
		z *= t;
		w *= t;
		return *this;
	}
	Vector4d& operator/=(double t)
	{
		x /= t;
		y /= t;
		z /= t;
		w /= t;
		return *this;
	}

	friend Vector4d operator*(double t, const Vector4d& vec) { return Vector4d(t * vec.x, t * vec.y, t * vec.z, t*vec.w); }

	bool operator==(const Vector4d& vec) const { return isEqual(x, vec.x) && isEqual(y, vec.y) && isEqual(z, vec.z) && isEqual(w, vec.z); }
	bool operator!=(const Vector4d& vec) const { return !((*this) == vec); }

	//negate each element
	void negate()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}

	//dot product
	double dot(const Vector4d& vec) const { return x * vec.x + y * vec.y + z * vec.z + w * vec.w; }
	//normalization of vector
	Vector4d& normalize()
	{
		*this = (*this) * 1.0 / sqrt(x * x + y * y + z * z + w * w);
		return *this;
	}
	//length of vector
	double length() const { return sqrt(x * x + y * y + z * z + w * w); }
	//zero vector
	static Vector4d zero() { return Vector4d(0.0, 0.0, 0.0, 0.0); }
};


inline Vector3d toVec3d(const Vector2d &vec2d)
{
	return Vector3d(vec2d.x, vec2d.y, 1.0);
}

inline Vector3d toVec3d(const Vector4d &vec4d)
{
	return Vector3d(vec4d.x, vec4d.y, vec4d.z);
}

#endif