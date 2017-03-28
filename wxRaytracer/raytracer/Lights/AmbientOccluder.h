#ifndef _AMBIENT_OCCLUDER_H_
#define _AMBIENT_OCCLUDER_H_

#include "Light.h"
#include "ShadeRec.h"

class Sampler;

class AmbientOccluder : public Light
{
public:
	AmbientOccluder();
	AmbientOccluder(const AmbientOccluder& a);

	virtual Light* clone(void) const;

	//Ambient& operator= (const Ambient& rhs);

	virtual ~AmbientOccluder(void);

	void scale_radiance(const float b);

	void set_color(const float c);

	void set_color(const RGBColor& c);

	void set_color(const float r, const float g, const float b);

	virtual Vector3D get_direction(ShadeRec& s);

	virtual RGBColor L(ShadeRec& s);

	void set_sampler(Sampler *p_sampler);

	void set_min_amount(const float val);

	bool in_shadow(const Ray &ray, const ShadeRec &sr);


private:
	float		ls;
	RGBColor	color;
	Sampler *sampler_ptr;
	Vector3D u, v, w;
	RGBColor min_amount;

};

#endif