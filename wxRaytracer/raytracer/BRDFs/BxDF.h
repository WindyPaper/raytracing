#pragma once

#include "RGBColor.h"
#include "Vector3D.h"
#include "ShadeRec.h"
#include "Sampler.h"
#include "Constants.h"

class BxDF
{
public:
	BxDF();
	virtual ~BxDF();

	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;

	virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;
};
