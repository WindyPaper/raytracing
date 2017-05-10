#pragma once

#include "BTDF.h"

class MicrofacetBTDF : public BTDF
{
public:
	MicrofacetBTDF();
	virtual ~MicrofacetBTDF();

	virtual MicrofacetBTDF*
		clone(void);

	MicrofacetBTDF&
		operator= (const MicrofacetBTDF& rhs);

	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const;

	void set_ior_in(float val);
	void set_ior_out(float val);

private:
	float ior_i;
	float ior_o;
};
