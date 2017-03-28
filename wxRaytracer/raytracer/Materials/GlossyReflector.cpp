// 	Copyright (C) Mp77 2012
//	Original from Kevin Suffern 2000-2007
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "GlossyReflector.h"
#include <assert.h>

// ---------------------------------------------------------------- default constructor

GlossyReflector::GlossyReflector (void)
	:	Phong(),
		ambient_brdf(new Lambertian),
		diffuse_brdf(new Lambertian),
		specular_brdf(new GlossySpecular),
		glossy_specular_brdf(new GlossySpecular)
{}



// ---------------------------------------------------------------- copy constructor

GlossyReflector::GlossyReflector(const GlossyReflector& m)
	: 	Phong(m)
{
	assert(false);
}


// ---------------------------------------------------------------- clone

GlossyReflector*
GlossyReflector::clone(void) const {
	return (new GlossyReflector(*this));
}	


// ---------------------------------------------------------------- assignment operator

GlossyReflector& 
GlossyReflector::operator= (const GlossyReflector& rhs) {
	assert(false);
	return (*this);
}


// ---------------------------------------------------------------- destructor

GlossyReflector::~GlossyReflector(void) {

	if (ambient_brdf) {
		delete ambient_brdf;
		ambient_brdf = NULL;
	}
	
	if (diffuse_brdf) {
		delete diffuse_brdf;
		diffuse_brdf = NULL;
	}

	if (specular_brdf) {
		delete specular_brdf;
		specular_brdf = NULL;
	}

	if (glossy_specular_brdf) {
		delete glossy_specular_brdf;
		glossy_specular_brdf = NULL;
	}
}


// ---------------------------------------------------------------- shade

RGBColor
GlossyReflector::shade(ShadeRec& sr) {
	/*Vector3D 	wo 			= -sr.ray.d;
	RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int 		num_lights	= sr.w.lights.size();
	
	for (int j = 0; j < num_lights; j++) {
		Vector3D wi = sr.w.lights[j]->get_direction(sr);    
		float ndotwi = sr.normal * wi;
	
		if (ndotwi > 0.0) 
			L += (	diffuse_brdf->f(sr, wo, wi) + 
					specular_brdf->f(sr, wo, wi)) * sr.w.lights[j]->L(sr) * ndotwi;
	}	
	return (L);*/
	
	Vector3D 	wo 			= -sr.ray.d;
	RGBColor 	L 			= ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
	int 		num_lights	= sr.w.lights.size();

	for (int j = 0; j < num_lights; j++) {
		Vector3D 	wi 		= sr.w.lights[j]->get_direction(sr);    
		float 		ndotwi 	= sr.normal * wi;

		if (ndotwi > 0.0) {
			bool in_shadow = false;

			if (sr.w.lights[j]->casts_shadows()) {
				Ray shadowRay(sr.hit_point, wi);
				in_shadow = sr.w.lights[j]->in_shadow(shadowRay, sr);
			}

			if (!in_shadow) 
				L += (	diffuse_brdf->f(sr, wo, wi) 
						  + specular_brdf->f(sr, wo, wi)) * sr.w.lights[j]->L(sr) * ndotwi;
		}
	}
	return (L);
}

void									
GlossyReflector::set_ce(const float r, const float g, const float b)
{
	//ce = RGBColor(r, g, b);
	return;
}

RGBColor
GlossyReflector::get_Le(ShadeRec& sr) const
{
	return RGBColor(1.0);
}
	
RGBColor
GlossyReflector::area_light_shade(ShadeRec& sr)			//this is for chapter 18 page one image ad-hoc
{
	RGBColor 	L(Phong::area_light_shade(sr));  // direct illumination
	Vector3D 	wo(-sr.ray.d);
	Vector3D 	wi;
	float		pdf;	
	RGBColor 	fr(glossy_specular_brdf->sample_f(sr, wo, wi, pdf)); 
	Ray 		reflected_ray(sr.hit_point, wi);
	
	L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf;
	
	return (L);
}
