// This file contains the definition of the class Vector3D

#include <math.h>
#include <algorithm>

#include "Maths.h"
#include "Vector3D.h"
#include "Normal.h"
#include "Point3D.h"

// ---------------------------------------------------------- default constructor

Vector3D::Vector3D(void)
	 : x(0.0), y(0.0), z(0.0)							
{}

// ---------------------------------------------------------- constructor

Vector3D::Vector3D(double a)
	 : x(a), y(a), z(a)							
{}

// ---------------------------------------------------------- constructor

Vector3D::Vector3D(double _x, double _y, double _z)	 
	: x(_x), y(_y), z(_z)
{}

// ---------------------------------------------------------- copy constructor

Vector3D::Vector3D(const Vector3D& vector)
	: x(vector.x), y(vector.y), z(vector.z)
{}


// ---------------------------------------------------------- constructor
// constructs a vector from a normal

Vector3D::Vector3D(const Normal& n)	 
	: x(n.x), y(n.y), z(n.z)
{}

// ---------------------------------------------------------- constructor
// constructs a vector from a point
// this is used in the ConcaveHemisphere hit functions

Vector3D::Vector3D(const Point3D& p)	 
	: x(p.x), y(p.y), z(p.z)
{}


// ---------------------------------------------------------- destructor

Vector3D::~Vector3D (void) 							
{}



// ---------------------------------------------------------- assignment operator

Vector3D& 
Vector3D::operator= (const Vector3D& rhs) {
	if (this == &rhs)
		return (*this);

	x = rhs.x; y = rhs.y; z = rhs.z;

	return (*this);
}


// ----------------------------------------------------------- assignment operator
// assign a Normal to a vector

Vector3D& 
Vector3D::operator= (const Normal& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;
	return (*this);
}


// ---------------------------------------------------------- assignment operator 
// assign a point to a vector

Vector3D& 												
Vector3D::operator= (const Point3D& rhs) {
	x = rhs.x; y = rhs.y; z = rhs.z;
	return (*this);
}


// ----------------------------------------------------------  length
// length of the vector

double													
Vector3D::length(void) {
	return (sqrt(x * x + y * y + z * z));
}


Vector3D Vector3D::operator*=(const float a)
{
	x *= a;
	y *= a;
	z *= a;

	return (*this);
}

// ----------------------------------------------------------  normalize
// converts the vector to a unit vector

void 													
Vector3D::normalize(void) {	
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}


// ----------------------------------------------------------  hat
// converts the vector to a unit vector and returns the vector

Vector3D& 													
Vector3D::hat(void) {	
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
	return (*this);
} 


// non-member function

// ----------------------------------------------------------  operator* 
// multiplication by a matrix on the left

Vector3D
operator* (const Matrix& mat, const Vector3D& v) {
	return (Point3D(mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z,
		mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z,
		mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z));
}

Matrix rotate_to_matrix(float theta, const Vector3D &axis)
{
	Vector3D a = axis;
	a.normalize();
	float sinTheta = std::sin(to_radian(theta));
	float cosTheta = std::cos(to_radian(theta));
	Matrix mat;
	// Compute rotation of first basis vector
	mat.m[0][0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
	mat.m[0][1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
	mat.m[0][2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;
	mat.m[0][3] = 0;

	// Compute rotations of second and third basis vectors
	mat.m[1][0] = a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
	mat.m[1][1] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
	mat.m[1][2] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;
	mat.m[1][3] = 0;

	mat.m[2][0] = a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
	mat.m[2][1] = a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
	mat.m[2][2] = a.z * a.z + (1 - a.z * a.z) * cosTheta;
	mat.m[2][3] = 0;
	return mat;
}

Vector3D normalize(const Vector3D &v)
{
	double length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return Vector3D(v.x / length, v.y / length, v.z / length);
}
