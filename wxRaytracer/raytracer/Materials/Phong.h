#ifndef _PHONG_H_
#define _PHONG_H_

#include "Material.h"

class Lambertian;
class GlossySpecular;

class Phong : public Material
{
public:
	Phong();
	virtual ~Phong();

	virtual Phong*
		clone(void) const;

	void
		set_ka(const float k);

	void
		set_kd(const float k);

	void set_ks(const float k);

	void set_exp(const float exp);

	void
		set_cd(const float r, const float g, const float b);

	virtual void
		set_ce(const float r, const float g, const float b);

	void set_cd(const RGBColor &color);

	virtual RGBColor
		get_Le(ShadeRec& sr) const;

	virtual RGBColor
		shade(ShadeRec& sr);

	virtual RGBColor area_light_shade(ShadeRec &sr);

protected:
	Lambertian *ambient_brdf;
	Lambertian *diffuse_brdf;
	GlossySpecular *specular_brdf;
};

#endif