#ifndef _SOLID_CYLINDER_H_
#define _SOLID_CYLINDER_H_

#include "GeometricObject.h"

class SolidCylinder : public GeometricObject
{
public:
	SolidCylinder(float bottom, float top, float radius);
	~SolidCylinder();

	virtual SolidCylinder *clone(void) const;

	virtual bool hit(const Ray& ray, double& tmin, ShadeRec *sr = 0) const;

	virtual bool
		shadow_hit(const Ray& ray, double& tmin) const;

private:
	float bottom;
	float top;
	float radius;

	BBox bbox;

	static const double kEpsilon;
};

#endif