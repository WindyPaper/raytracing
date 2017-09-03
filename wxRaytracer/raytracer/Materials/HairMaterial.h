#pragma once

#include "Material.h"

class HairBcsdf;

class HairMaterial : public Material
{
public:
	HairMaterial();

	virtual Material*
		clone(void) const;

	virtual void
		set_ce(const float r, const float g, const float b);

	virtual RGBColor
		get_Le(ShadeRec& sr) const;

	virtual RGBColor
		area_light_shade(ShadeRec& sr);

	virtual RGBColor
		path_shade(ShadeRec& sr);

private:
	RGBColor color;
	HairBcsdf *hair_bcsdf;
};
