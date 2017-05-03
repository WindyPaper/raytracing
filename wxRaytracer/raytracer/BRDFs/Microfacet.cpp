#include "Microfacet.h"
#include <algorithm>

Microfacet::Microfacet() :
	BRDF(),
	refletance(1.0f),
	roughness(0.0f)
{

}

Microfacet::Microfacet(const Microfacet &obj) :
	BRDF(obj),
	refletance(obj.refletance),
	roughness(obj.roughness),
	cd(obj.cd)
{

}

Microfacet* Microfacet::clone(void) const
{
	return new Microfacet(*this);
}

void Microfacet::set_reflect_val(const float val)
{
	refletance = val;
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
	double fresnel = refletance + (1.0f - refletance) * std::powf((1.0 - (h * wi)), 5);

	//Blinn  type
	double g_reach_view = 1.0;
	double g_block_after_reflect = 2 * (sr.normal * h) * (sr.normal * wo) / (wo * h);
	double g_block_before_reach = 2 * (sr.normal * h) * (sr.normal * wi) / (wo * h);
	double g_term = std::min({ g_reach_view, g_block_after_reflect, g_block_before_reach });

	//Beckmanns
	double n_dot_h = sr.normal * h;
	double n_dot_h_2 = n_dot_h * n_dot_h;
	double roughness_2 = roughness * roughness;
	double exp_pow = (n_dot_h_2 - 1) / (roughness_2 * n_dot_h_2);
	double d = 1.0f / (PI * roughness_2 * n_dot_h_2 * n_dot_h_2) * std::exp(exp_pow);

	double f = fresnel * g_term * d / (4 * (sr.normal * wi) * (sr.normal * wo));
	return f * cd;
}

RGBColor Microfacet::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
	Vector3D w = sr.normal;
	Vector3D v = Vector3D(0.000034, 1.0, 0.00071) ^ w;
	v.normalize();
	Vector3D u = v ^ w;

	Point3D sp = sampler_ptr->sample_hemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;
	wi.normalize();

	pdf = 1 / (2 * PI);
	return f(sr, wo, wi);
}

Microfacet& Microfacet::operator=(const Microfacet& rhs)
{
	BRDF::operator=(rhs);

	cd = rhs.cd;
	refletance = rhs.refletance;

	return (*this);
}
