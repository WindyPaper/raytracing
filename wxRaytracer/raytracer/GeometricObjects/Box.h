#ifndef _BOX_H_
#define _BOX_H_

#include "GeometricObject.h"

class Box : public GeometricObject
{
public:
	Box(Point3D min, Point3D max);
	~Box();

	virtual Box *clone(void) const;

	virtual bool hit(const Ray& ray, double& in_tmin, ShadeRec *sr = 0) const;

	virtual bool
		shadow_hit(const Ray& ray, double& tmin) const;

	Normal get_normal(const int face_hit) const;

private:
	Point3D min;
	Point3D max;
};

#endif