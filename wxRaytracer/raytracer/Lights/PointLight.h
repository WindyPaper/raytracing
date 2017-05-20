#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "Light.h"
#include "Vector3D.h"
#include "RGBColor.h"

class PointLight : public Light
{
public:
	PointLight();
	virtual ~PointLight();

	virtual Light* clone(void) const;

	virtual Vector3D get_direction(ShadeRec& sr);

	virtual RGBColor L(ShadeRec& sr);
	float G(const ShadeRec& sr) const;

	void set_position(const Point3D &pos);
	const Point3D &get_position() const;

	void set_radiance(float radiance);

	virtual bool in_shadow(const Ray &ray, const ShadeRec &sr) const;

private:
	float ls;
	RGBColor light_color;
	Point3D pos;
};

#endif