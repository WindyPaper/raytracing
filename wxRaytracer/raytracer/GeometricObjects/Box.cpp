#include "Box.h"
#include <algorithm>

Box::Box(Point3D min, Point3D max) :
min(min),
max(max)
{

}

Box::~Box()
{

}

Box * Box::clone(void) const
{
	return 0;
}

bool Box::hit(const Ray& ray, double& in_tmin, ShadeRec *sr) const
{
	double ox = ray.o.x; double oy = ray.o.y; double oz = ray.o.z;
	double dx = ray.d.x; double dy = ray.d.y; double dz = ray.d.z;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (min.x - ox) * a;
		tx_max = (max.x - ox) * a;
	}
	else {
		tx_min = (max.x - ox) * a;
		tx_max = (min.x - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (min.y - oy) * b;
		ty_max = (max.y - oy) * b;
	}
	else {
		ty_min = (max.y - oy) * b;
		ty_max = (min.y - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (min.z - oz) * c;
		tz_max = (max.z - oz) * c;
	}
	else {
		tz_min = (max.z - oz) * c;
		tz_max = (min.z - oz) * c;
	}

	double t0, t1;

	// this is the same as Listing 19.1 down to the statement float t0, t1;

	int face_in, face_out;

	// find largest entering t value

	if (tx_min > ty_min) {
		t0 = tx_min;
		face_in = (a >= 0.0) ? 0 : 3;
	}
	else {
		t0 = ty_min;
		face_in = (b >= 0.0) ? 1 : 4;
	}

	if (tz_min > t0) {
		t0 = tz_min;
		face_in = (c >= 0.0) ? 2 : 5;
	}

	// find smallest exiting t value

	if (tx_max < ty_max) {
		t1 = tx_max;
		face_out = (a >= 0.0) ? 3 : 0;
	}
	else {
		t1 = ty_max;
		face_out = (b >= 0.0) ? 4 : 1;
	}

	if (tz_max < t1) {
		t1 = tz_max;
		face_out = (c >= 0.0) ? 5 : 2;
	}

	if (t0 < t1 && t1 > kEpsilon) {  // condition for a hit
		if (t0 > kEpsilon) {
			in_tmin = t0;  			// ray hits outside surface
			if (sr)
			{
				sr->normal = get_normal(face_in);
			}
		}
		else {
			in_tmin = t1;				// ray hits inside surface
			if (sr)
			{
				sr->normal = get_normal(face_out);
			}
		}
		
		if (sr)
		{
			sr->local_hit_point = ray.o + in_tmin * ray.d;
		}
		return (true);
	}
	else
		return (false);
}

Normal
Box::get_normal(const int face_hit) const {
	switch (face_hit) {
	case 0:	return (Normal(-1, 0, 0));	// -x face
	case 1:	return (Normal(0, -1, 0));	// -y face
	case 2:	return (Normal(0, 0, -1));	// -z face
	case 3:	return (Normal(1, 0, 0));	// +x face
	case 4:	return (Normal(0, 1, 0));	// +y face
	case 5:	return (Normal(0, 0, 1));	// +z face
	}
}

bool Box::shadow_hit(const Ray& ray, double& tmin) const
{
	//return hit(ray, tmin);
	double ox = ray.o.x; double oy = ray.o.y; double oz = ray.o.z;
	double dx = ray.d.x; double dy = ray.d.y; double dz = ray.d.z;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (min.x - ox) * a;
		tx_max = (max.x - ox) * a;
	}
	else {
		tx_min = (max.x - ox) * a;
		tx_max = (min.x - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (min.y - oy) * b;
		ty_max = (max.y - oy) * b;
	}
	else {
		ty_min = (max.y - oy) * b;
		ty_max = (min.y - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (min.z - oz) * c;
		tz_max = (max.z - oz) * c;
	}
	else {
		tz_min = (max.z - oz) * c;
		tz_max = (min.z - oz) * c;
	}

	double t0, t1;

	// this is the same as Listing 19.1 down to the statement float t0, t1;

	int face_in, face_out;

	// find largest entering t value

	if (tx_min > ty_min) {
		t0 = tx_min;
		face_in = (a >= 0.0) ? 0 : 3;
	}
	else {
		t0 = ty_min;
		face_in = (b >= 0.0) ? 1 : 4;
	}

	if (tz_min > t0) {
		t0 = tz_min;
		face_in = (c >= 0.0) ? 2 : 5;
	}

	// find smallest exiting t value

	if (tx_max < ty_max) {
		t1 = tx_max;
		face_out = (a >= 0.0) ? 3 : 0;
	}
	else {
		t1 = ty_max;
		face_out = (b >= 0.0) ? 4 : 1;
	}

	if (tz_max < t1) {
		t1 = tz_max;
		face_out = (c >= 0.0) ? 5 : 2;
	}

	if (t0 < t1 && t1 > kEpsilon) {  // condition for a hit
		if (t0 > kEpsilon) {
			tmin = t0;  			// ray hits outside surface
		}
		else {
			tmin = t1;				// ray hits inside surface
		}
		return (true);
	}
	else
		return (false);
}
