#ifndef _ENVIRONMENT_LIGHT_H_
#define _ENVIRONMENT_LIGHT_H_

#include "Light.h"

class Sampler;
class Material;

class EnvironmentLight : public Light
{
public:
	EnvironmentLight();
	virtual ~EnvironmentLight();

	virtual EnvironmentLight *clone() const;

	void set_sampler(Sampler *sampler);
	virtual Vector3D get_direction(ShadeRec& sr);
	virtual RGBColor L(ShadeRec& sr);
	bool in_shadow(const Ray &ray, const ShadeRec &sr) const;
	virtual float pdf(ShadeRec& sr) const;
	void set_material(Material *mat);

private:
	Sampler *sampler_ptr;
	Material *material_ptr;
	Vector3D u, v, w;
	Vector3D wi;
};

#endif