#ifndef _EMISSIVE_H_
#define _EMISSIVE_H_

#include "Material.h"

class Emissive : public Material
{
public:
	Emissive();
	~Emissive();

	virtual Emissive*
		clone(void) const;

	void set_ce(const float r, const float g, const float b);
	void set_ce(const RGBColor &color);
	RGBColor get_ce() const;

	RGBColor get_le() const;

	virtual RGBColor
		get_Le(ShadeRec& sr) const;

	void scale_radiance(const float scale);

	virtual RGBColor shade(ShadeRec& sr);

	virtual RGBColor area_light_shade(ShadeRec &sr);

private:
	float ls;
	RGBColor ce;
};

#endif