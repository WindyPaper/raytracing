#include "Matte.h"
#include <algorithm>

// ---------------------------------------------------------------- default constructor

Matte::Matte (void)
	:	Material(),
		ambient_brdf(new Lambertian),
		diffuse_brdf(new Lambertian)
{}



// ---------------------------------------------------------------- copy constructor

Matte::Matte(const Matte& m)
	: 	Material(m)
{
	if(m.ambient_brdf)
		ambient_brdf = (Lambertian*)m.ambient_brdf->clone(); 
	else  ambient_brdf = NULL;
	
	if(m.diffuse_brdf)
		diffuse_brdf = (Lambertian*)m.diffuse_brdf->clone(); 
	else  diffuse_brdf = NULL;
}


// ---------------------------------------------------------------- clone

Material*										
Matte::clone(void) const {
	return (new Matte(*this));
}	


// ---------------------------------------------------------------- assignment operator

Matte& 
Matte::operator= (const Matte& rhs) {
	if (this == &rhs)
		return (*this);
		
	Material::operator=(rhs);
	
	if (ambient_brdf) {
		delete ambient_brdf;
		ambient_brdf = NULL;
	}

	if (rhs.ambient_brdf)
		ambient_brdf = (Lambertian*)rhs.ambient_brdf->clone();
		
	if (diffuse_brdf) {
		delete diffuse_brdf;
		diffuse_brdf = NULL;
	}

	if (rhs.diffuse_brdf)
		diffuse_brdf = (Lambertian*)rhs.diffuse_brdf->clone();

	return (*this);
}


// ---------------------------------------------------------------- destructor

Matte::~Matte(void) {

	if (ambient_brdf) {
		delete ambient_brdf;
		ambient_brdf = NULL;
	}
	
	if (diffuse_brdf) {
		delete diffuse_brdf;
		diffuse_brdf = NULL;
	}
}

void Matte::set_ce(const float r, const float g, const float b)
{
	//empty
}

RGBColor Matte::get_Le(ShadeRec& sr) const
{
	return RGBColor(1.0);
}

// ---------------------------------------------------------------- shade

RGBColor
Matte::shade(ShadeRec& sr) {
	Vector3D 	wo 			= -sr.ray.d;
	RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int 		num_lights	= sr.w.lights.size();
	
	for (int j = 0; j < num_lights; j++) {
		Vector3D wi = sr.w.lights[j]->get_direction(sr);    
		float ndotwi = sr.normal * wi;

		if (ndotwi > 0.0)
		{
			bool in_shadow = false;
			if (sr.w.lights[j]->casts_shadows())
			{
				Ray shadow_ray(sr.hit_point, wi);
				in_shadow = sr.w.lights[j]->in_shadow(shadow_ray, sr);
			}

			if (!in_shadow)
			{
				L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * ndotwi;
			}
		}
	}
	
	return (L);
}

RGBColor Matte::area_light_shade(ShadeRec &sr)
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


RGBColor Matte::path_shade(ShadeRec& sr)
{
	Vector3D wi;
	Vector3D wo = -sr.ray.d;
	float pdf;
	RGBColor f = diffuse_brdf->sample_f(sr, wo, wi, pdf);
	float ndotwi = std::max(0.0, sr.normal * wi);
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

	if (pdf < 0.0000000001)
	{
		pdf = 0.0000000001;
	}

	return (f * (sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) + light_l) * ndotwi / pdf);
}

void Matte::set_sampler(Sampler* sPtr)
{
	diffuse_brdf->set_sampler(sPtr);
}
