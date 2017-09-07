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
	float logistic_function(float x, float s);
	float logistic_cdf(float x, float s);
	float trimmed_logistic(float x, float s, float a, float b);
	float hair_F(float cos_theta);
	RGBColor attenuation(int p, float h, RGBColor ua, float v_dot_l, float cos_theta_t, float cos_theta_d);
	RGBColor N(int p, float beta, float v_dot_l, float cos_theta_t, float cos_theta_d, float cos_phi);
	RGBColor N_h(int p, float beta, float h, float v_dot_l, float cos_theta_t, float cos_theta_d, float cos_phi);

private:
	float s; //logistic function

	float ior;
	float ior_prime;
	//float hair_n;
	/*mutable float v_dot_l;
	mutable float cos_theta_t;
	mutable float cos_theta_d;
	mutable float cos_phi;*/

	float beta_r;
	float beta_tt;
	float beta_trt;
};


#endif