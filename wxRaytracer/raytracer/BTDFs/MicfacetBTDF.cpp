#include "MicfacetBTDF.h"

MicrofacetBTDF::MicrofacetBTDF() :
	ior_i(1.0f),
	ior_o(1.5f)
{

}

MicrofacetBTDF::~MicrofacetBTDF()
{

}

MicrofacetBTDF* MicrofacetBTDF::clone(void)
{
	return new MicrofacetBTDF(*this);
}

RGBColor MicrofacetBTDF::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	return 0;
}

RGBColor MicrofacetBTDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const
{
	return 0;
}

void MicrofacetBTDF::set_ior_in(float val)
{
	ior_i = val;
}

void MicrofacetBTDF::set_ior_out(float val)
{
	ior_o = val;
}

MicrofacetBTDF& MicrofacetBTDF::operator=(const MicrofacetBTDF& rhs)
{
	ior_i = rhs.ior_i;
	ior_o = rhs.ior_o;

	return *this;
}
