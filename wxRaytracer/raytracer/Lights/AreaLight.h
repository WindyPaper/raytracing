#ifndef _AREA_LIGHT_H_
#define _AREA_LIGHT_H_

#include "Normal.h"
#include "Light.h"

class GeometricObject;
class Material;

class AreaLight : public Light
{
public:
	AreaLight();

	virtual AreaLight*
		clone(void) const;

	virtual
		~AreaLight(void);

	virtual Vector3D
		get_direction(ShadeRec& sr);

	//RGBColor trace_ray(const Ray &ray, const int depth) const;
	virtual RGBColor
		L(ShadeRec& sr);

	virtual float
		G(const ShadeRec& sr) const;

	virtual float
		pdf(ShadeRec& sr) const;

	void set_object(GeometricObject *obj);

	void set_material(Material *mat);

	virtual bool in_shadow(const Ray &ray, const ShadeRec &sr) const;

private:
	GeometricObject *object_ptr;
	Material *mat_ptr;
	Point3D sample_point; // sample point on surface
	Normal light_normal;;
	Vector3D wi; // unit vector from hit point to sample point
};

#endif