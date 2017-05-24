#include "BxDF.h"

BxDF::BxDF()
{

}

BxDF::~BxDF()
{

}

RGBColor BxDF::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	return black;
}

RGBColor BxDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const
{
	return black;
}

RGBColor BxDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
	return black;
}
