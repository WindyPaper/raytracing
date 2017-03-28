#include "Emissive.h"
#include <assert.h>

Emissive::Emissive()
{

}

Emissive::~Emissive()
{

}

Emissive* Emissive::clone(void) const
{
	return 0;
}

void Emissive::set_ce(const float r, const float g, const float b)
{
	ce.r = r;
	ce.g = g;
	ce.b = b;
}

void Emissive::set_ce(const RGBColor &color)
{
	ce = color;
}

RGBColor Emissive::get_ce() const
{
	return ce;
}

RGBColor Emissive::get_le() const
{
	return ls * ce;
}

RGBColor Emissive::get_Le(ShadeRec& sr) const
{
	return ls * ce;
}

void Emissive::scale_radiance(const float scale)
{
	ls = scale;
}

RGBColor Emissive::shade(ShadeRec& sr)
{
	assert(false);
	return black;
}

RGBColor Emissive::area_light_shade(ShadeRec &sr)
{
	if (-sr.normal * sr.ray.d > 0.0)
	{
		return ls * ce;
	}
	else
	{
		return black;
	}
}
