#ifndef _HAIR_BCSDF_H_

#pragma once

#include "BRDF.h"

class HairBcsdf : public BRDF
{
public:
	HairBcsdf();

	virtual HairBcsdf*
		clone(void) const;

	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi);

	virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

protected:
	float g(float beta, float theta);
	float I0(float x);
	float log_I0(float x);
	float M(float v, float sin_thetai, float sin_thetao, float cos_thetai, float cos_thetao);

	float omega(int p, float h);
	float gaussian_detector(float beta, float phi);
	float hair_F(float cos_theta);
	float attenuation(int p, float h, float ua);
	float N(int p, float beta);

private:
	mutable float v_dot_l;
	mutable float ior;
	mutable float ior_prime;
	//float hair_n;
	mutable float cos_theta_t;
	mutable float cos_theta_d;
	mutable float cos_phi;

	mutable float beta_r;
	mutable float beta_tt;
	mutable float beta_trt;
};


#endif