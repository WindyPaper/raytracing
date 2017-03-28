#include "AmbientOccluder.h"
#include "pre_define.h"
#include "Sampler.h"
#include "World.h"
#include <assert.h>

AmbientOccluder::AmbientOccluder() :
u(0),
v(0),
w(0),
sampler_ptr(0),
min_amount(0),
ls(1.0),
color(1.0)
{

}

AmbientOccluder::AmbientOccluder(const AmbientOccluder& a)
{
	assert(false);
}



Light* AmbientOccluder::clone(void) const
{
	return 0;
}

AmbientOccluder::~AmbientOccluder(void)
{
	SAFE_DELETE(sampler_ptr);
}

void AmbientOccluder::scale_radiance(const float b)
{
	ls = b;
}

void AmbientOccluder::set_color(const float c)
{
	color = c;
}

void AmbientOccluder::set_color(const RGBColor& c)
{
	color = c;
}

void AmbientOccluder::set_color(const float r, const float g, const float b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

Vector3D AmbientOccluder::get_direction(ShadeRec& s)
{
	Point3D sp = sampler_ptr->sample_hemisphere();
	return (sp.x * u + sp.y * v + sp.z * w);
	//return Vector3D((sp.x * u).x, (sp.y * v).y, (sp.z * w).z);
}

RGBColor AmbientOccluder::L(ShadeRec& s)
{
	w = s.normal;
	v = w ^ Vector3D(0.00001, 1.0, 0.00001);
	v.normalize();
	u = v ^ w;
	u.normalize();

	Ray shadow_ray;
	shadow_ray.o = s.hit_point;
	shadow_ray.d = get_direction(s);
	shadow_ray.d.normalize();
	if (in_shadow(shadow_ray, s))
	{
		return min_amount * ls * color;
	}
	else
	{
		return ls * color;
	}

}

void AmbientOccluder::set_sampler(Sampler *p_sampler)
{
	SAFE_DELETE(sampler_ptr);
	sampler_ptr = p_sampler;
	sampler_ptr->map_samples_to_hemisphere(1.0);
}

void AmbientOccluder::set_min_amount(const float val)
{
	min_amount = val;
}

bool AmbientOccluder::in_shadow(const Ray &ray, const ShadeRec &sr)
{
	double t;
	int num_objs = sr.w.objects.size();

	for (int i = 0; i < num_objs; ++i)
	{
		if (sr.w.objects[i]->shadow_hit(ray, t))
		{
			return true;
		}
	}

	return false;
}
