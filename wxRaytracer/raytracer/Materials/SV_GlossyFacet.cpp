#include "SV_GlossyFacet.h"
#include "pre_define.h"
#include "Microfacet.h"

SV_GlossyFacet::SV_GlossyFacet() :
	Material(),
	_p_glossy_brdf(new Microfacet())
{

}

SV_GlossyFacet::SV_GlossyFacet(const SV_GlossyFacet &rhs) :
	Material(rhs),
	_p_glossy_brdf(rhs._p_glossy_brdf->clone())
{

}

SV_GlossyFacet::~SV_GlossyFacet()
{
	SAFE_DELETE(_p_glossy_brdf);
}

Material & SV_GlossyFacet::operator=(const SV_GlossyFacet &rhs)
{
	if (this == &rhs)
		return (*this);

	Material::operator=(rhs);

	SAFE_DELETE(_p_glossy_brdf);
	_p_glossy_brdf = rhs._p_glossy_brdf->clone();

	return (*this);
}

SV_GlossyFacet* SV_GlossyFacet::clone(void) const
{
	return new SV_GlossyFacet(*this);
}

void SV_GlossyFacet::set_ce(const float r, const float g, const float b)
{

}

RGBColor SV_GlossyFacet::get_Le(ShadeRec& sr) const
{
	return RGBColor(0.0);
}

RGBColor SV_GlossyFacet::area_light_shade(ShadeRec& sr)
{
	return RGBColor(0.0);
}

void SV_GlossyFacet::set_cd(const RGBColor &color)
{
	_p_glossy_brdf->set_cd(color);
}

void SV_GlossyFacet::set_roughness(const float val)
{
	_p_glossy_brdf->set_roughness(val);
}

void SV_GlossyFacet::set_reflectance(const float val)
{
	_p_glossy_brdf->set_reflect_val(val);
}

RGBColor SV_GlossyFacet::path_shade(ShadeRec& sr)
{
	Vector3D wi;
	Vector3D wo = -sr.ray.d;
	float pdf;
	RGBColor f = _p_glossy_brdf->sample_f(sr, wo, wi, pdf);
	float ndotwi = sr.normal * wi;
	Ray reflected_ray(sr.hit_point, wi);

	//shadow ray
	RGBColor light_l = 0.0f;
	int light_sampler_num = 16;

	for (int sample_num = 0; sample_num < light_sampler_num; ++sample_num)
	{
		int num_lights = sr.w.lights.size();
		for (int i = 0; i < num_lights; ++i)
		{
			Light *l = sr.w.lights[i];

			Vector3D light_wi = l->get_direction(sr);
			float l_ndotwi = sr.normal * light_wi;
			if (l_ndotwi > 0.0f)
			{
				bool is_in_shadow = false;
				if (l->casts_shadows())
				{
					Ray shadow_ray(sr.hit_point, light_wi);
					is_in_shadow = l->in_shadow(shadow_ray, sr);
				}

				if (is_in_shadow == false)
				{
					light_l += l->L(sr) * l->G(sr) * l_ndotwi / l->pdf(sr);
				}
			}
		}
	}
	light_l /= light_sampler_num;

	return (f * (sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) + light_l) * ndotwi / pdf);
}

void SV_GlossyFacet::set_sampler(Sampler* sPtr)
{
	_p_glossy_brdf->set_sampler(sPtr);
}