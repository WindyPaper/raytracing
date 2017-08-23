// 	Copyright (C) Mp77 2012
//	Original from Kevin Suffern 2000-2007
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "Ray.h"
#include <stdio.h>

// ---------------------------------------------------------------- default constructor

Ray::Ray (void)
	: 	o(0.0), 
		d(0.0, 0.0, 1.0),
		depth(0)
{}

// ---------------------------------------------------------------- constructor

Ray::Ray (const Point3D& origin, const Vector3D& dir)
	: 	o(origin), 
		d(dir),
		depth(0)
{}

// ---------------------------------------------------------------- copy constructor

Ray::Ray (const Ray& ray)
	: 	o(ray.o), 
		d(ray.d),
		depth(ray.depth)
{}

// ---------------------------------------------------------------- assignment operator

Ray& 
Ray::operator= (const Ray& rhs) {
	
	if (this == &rhs)
		return (*this);
		
	o = rhs.o; 
	d = rhs.d; 
	depth = rhs.depth;

	return (*this);	
}

// ---------------------------------------------------------------- destructor

Ray::~Ray (void) {}



Matrix look_at_mat(const Ray &ray, const Vector3D &up)
{
	const Point3D &pos = ray.o;
	Vector3D dir = ray.d;
	dir.normalize();
	Matrix cameraToWorld;
	// Initialize fourth column of viewing matrix
	cameraToWorld.m[0][3] = pos.x;
	cameraToWorld.m[1][3] = pos.y;
	cameraToWorld.m[2][3] = pos.z;
	cameraToWorld.m[3][3] = 1;

	// Initialize first three columns of viewing matrix
	//Vector3D dir = Normalize(look - pos);
	if ((normalize(up) ^ dir).length() == 0) {
		printf("up and dir are same!\n");
		return Matrix();
	}
	Vector3D left = normalize((normalize(up) ^ dir));
	//Vector3D newUp = (dir ^ left);
	Vector3D newUp = (left ^ dir);
	cameraToWorld.m[0][0] = left.x;
	cameraToWorld.m[1][0] = left.y;
	cameraToWorld.m[2][0] = left.z;
	cameraToWorld.m[3][0] = 0.;
	cameraToWorld.m[0][1] = newUp.x;
	cameraToWorld.m[1][1] = newUp.y;
	cameraToWorld.m[2][1] = newUp.z;
	cameraToWorld.m[3][1] = 0.;
	cameraToWorld.m[0][2] = dir.x;
	cameraToWorld.m[1][2] = dir.y;
	cameraToWorld.m[2][2] = dir.z;
	cameraToWorld.m[3][2] = 0.;

	return cameraToWorld.inverse();
}
