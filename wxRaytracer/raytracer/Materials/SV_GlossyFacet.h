#pragma once

#include "Material.h"

class Microfacet;
class MicrofacetBTDF;

class SV_GlossyFacet : public Material
{
public:
	SV_GlossyFacet();
	SV_GlossyFacet(const SV_GlossyFacet &rhs);

	virtual ~SV_GlossyFacet();

	Material &operator=(const SV_GlossyFacet &rhs);

	virtual SV_GlossyFacet*
		clone(void) const;

	virtual void
		set_ce(const float r, const float g, const float b);

	virtual RGBColor
		get_Le(ShadeRec& sr) const;

	virtual RGBColor
		area_light_shade(ShadeRec& sr);

	void set_cd(const RGBColor &color);
	void set_roughness(const float val);
	void set_ior(const float val);

	virtual RGBColor
		path_shade(ShadeRec& sr);

	virtual void
		set_sampler(Sampler* sPtr);

private:
	Microfacet *_p_glossy_brdf;
	MicrofacetBTDF *_p_glossy_btdf;
};
