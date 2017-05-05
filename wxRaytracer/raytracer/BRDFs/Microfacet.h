#pragma once

#include "BRDF.h"

class Microfacet : public BRDF
{
public:
	enum DistributionType
	{
		GGX,
		WARD,		
		BECKMANNS,
	};

	Microfacet();
	Microfacet(const Microfacet &obj);

	Microfacet&
		operator= (const Microfacet& rhs);

	virtual Microfacet*
		clone(void) const;

	void set_ior(const float val);
	void set_cd(const RGBColor &color);
	void set_roughness(const float val);

	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

private:
	DistributionType d_type;

	RGBColor cd;
	float ior;
	float roughness;
};
