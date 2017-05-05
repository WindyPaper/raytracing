#include "Microfacet.h"
#include <algorithm>

Microfacet::Microfacet() :
	BRDF(),
	ior(1.5f),
	roughness(0.0f)
{

}

Microfacet::Microfacet(const Microfacet &obj) :
	BRDF(obj),
	ior(obj.ior),
	roughness(obj.roughness),
	cd(obj.cd)
{

}

Microfacet* Microfacet::clone(void) const
{
	return new Microfacet(*this);
}

void Microfacet::set_ior(const float val)
{
	ior = val;
}

void Microfacet::set_cd(const RGBColor &color)
{
	cd = color;
}

void Microfacet::set_roughness(const float val)
{
	roughness = val;
}

RGBColor Microfacet::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	//Schlick approximation
	Vector3D h(wo + wi);
	h.normalize();
	double f0 = abs((1.0 - ior) / (1.0 + ior));
	f0 = f0 * f0;
	double fresnel = f0 + (1.0f - f0) * std::powf((1.0 - (h * wi)), 5);

	//Blinn  type
	double g_reach_view = 1.0;
	double g_block_after_reflect = 2 * (sr.normal * h) * (sr.normal * wo) / (wo * h);
	double g_block_before_reach = 2 * (sr.normal * h) * (sr.normal * wi) / (wo * h);
	double g_term = std::min({ g_reach_view, g_block_after_reflect, g_block_before_reach });

	//Beckmanns
	double n_dot_h = std::max(0.0, sr.normal * h);
	double n_dot_h_2 = n_dot_h * n_dot_h;
	double roughness_2 = roughness * roughness;
	double exp_pow = (n_dot_h_2 - 1) / (roughness_2 * n_dot_h_2);
	double d = 1.0f / (PI * roughness_2 * n_dot_h_2 * n_dot_h_2) * std::exp(exp_pow);

	double f = fresnel * g_term * d / (4 * std::max(0.0, (sr.normal * wi)) * std::max(0.0, (sr.normal * wo)));

	if (std::isnan(f))
	{
		printf("Microfacet f is NAN!\n");
	}
	return f * cd;
}

RGBColor Microfacet::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
	Vector3D w = sr.normal;
	Vector3D v = Vector3D(0.000034, 1.0, 0.00071) ^ w;
	v.normalize();
	Vector3D u = v ^ w;

	double roughness_2 = roughness * roughness;
	float rand0_1 = rand_float();
	float theta = std::atan(std::sqrt((-roughness_2 * std::log(1 - rand0_1))));
	float phi = rand_float(0, 2 * PI);
	wi = Vector3D(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta));
	wi = wi.x * u + wi.y * v + wi.z * w;
	wi.normalize();
	if (std::isnan(wi.x) || std::isnan(wi.y) || std::isnan(wi.z))
	{
		printf("the param of wi nan !\n");
	}

	//Beckmanns pdf
	Vector3D h(wo + wi);
	h.normalize();
	double n_dot_h = std::max(0.0, sr.normal * h);
	double n_dot_h_2 = n_dot_h * n_dot_h;
	double exp_pow = (n_dot_h_2 - 1) / (roughness_2 * n_dot_h_2);
	pdf = 2 * std::sinf(theta) / (roughness_2 * std::powf(std::cos(theta), 3)) * std::expf(exp_pow);

	if (std::isinf(pdf))
	{
		pdf = 0.000001;
	}

	return f(sr, wo, wi);
}

Microfacet& Microfacet::operator=(const Microfacet& rhs)
{
	BRDF::operator=(rhs);

	cd = rhs.cd;
	ior = rhs.ior;

	return (*this);
}
