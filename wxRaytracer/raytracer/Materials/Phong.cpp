#include "Phong.h"
#include "pre_define.h"
#include <assert.h>
#include "Lambertian.h"
#include "GlossySpecular.h"

Phong::Phong() :
ambient_brdf(0),
diffuse_brdf(0),
specular_brdf(0)
{
	ambient_brdf = new Lambertian();
	diffuse_brdf = new Lambertian();
	specular_brdf = new GlossySpecular();
}

Phong::~Phong()
{
	SAFE_DELETE(ambient_brdf);
	SAFE_DELETE(diffuse_brdf);
	SAFE_DELETE(specular_brdf);
}

Phong* Phong::clone(void) const
{
	assert(false);
	return 0;
}

void Phong::set_ka(const float k)
{
	ambient_brdf->set_ka(k);
}

void Phong::set_kd(const float k)
{
	diffuse_brdf->set_kd(k);
}

void Phong::set_ks(const float k)
{
	specular_brdf->set_ks(k);
}

void Phong::set_exp(const float exp)
{
	specular_brdf->set_exp(exp);
}

void Phong::set_cd(const float r, const float g, const float b)
{
	ambient_brdf->set_cd(r, g, b);
	diffuse_brdf->set_cd(r, g, b);
	specular_brdf->set_cs(r, g, b);
}

void Phong::set_cd(const RGBColor &color)
{
	ambient_brdf->set_cd(color);
	diffuse_brdf->set_cd(color);
	specular_brdf->set_cs(color);
}

void Phong::set_ce(const float r, const float g, const float b)
{
	//empty
}

RGBColor Phong::get_Le(ShadeRec& sr) const
{
	return RGBColor(1.0);
}

RGBColor Phong::shade(ShadeRec& sr)
{
	RGBColor L;
	Vector3D wo = -sr.ray.d;
	wo.normalize();
	L = ambient_brdf->f(sr, wo, wo) * sr.w.ambient_ptr->L(sr);
	int num_lights = sr.w.lights.size();

	for (int j = 0; j < num_lights; j++)
	{
		Light *light = sr.w.lights[j];
		Vector3D wi = light->get_direction(sr);
		wi.normalize();
		float ndotwi = sr.normal * wi;

		if (ndotwi > 0)
		{
			bool in_shadow = false;

			if (light->casts_shadows())
			{
				Ray shadow_ray(sr.hit_point, wi);
				in_shadow = light->in_shadow(shadow_ray, sr);
			}
			
			if (!in_shadow)
			{
				L += diffuse_brdf->f(sr, wo, wo) * light->L(sr) * ndotwi + \
					specular_brdf->f(sr, wo, wi) * light->L(sr) * ndotwi;
			}
		}
	}

	return L;
}

RGBColor Phong::area_light_shade(ShadeRec &sr)
{
	Vector3D wo = -sr.ray.d;
	RGBColor L = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);

	int num_lights = sr.w.lights.size();
	for (int i = 0; i < num_lights; ++i)
	{
		Vector3D wi = sr.w.lights[i]->get_direction(sr);
		float ndotwi = sr.normal * wi;

		if (ndotwi > 0.0)
		{
			bool in_shadow = false;
			if (sr.w.lights[i]->casts_shadows())
			{
				Ray shadow_ray(sr.hit_point, wi);
				in_shadow = sr.w.lights[i]->in_shadow(shadow_ray, sr);
			}

			if (!in_shadow)
			{
				L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[i]->L(sr) * sr.w.lights[i]->G(sr) * ndotwi / sr.w.lights[i]->pdf(sr);
			}
		}
	}
	return L;
}
