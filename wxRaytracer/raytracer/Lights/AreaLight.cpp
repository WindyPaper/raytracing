#include "AreaLight.h"
#include "GeometricObject.h"
#include "Emissive.h"

AreaLight::AreaLight() :
object_ptr(0),
mat_ptr(0)
{

}

AreaLight* AreaLight::clone(void) const
{
	return 0;
}

AreaLight::~AreaLight(void)
{

}

Vector3D AreaLight::get_direction(ShadeRec& sr)
{
	Point3D sample_point = object_ptr->sample();
	light_normal = object_ptr->get_normal(sample_point);
	sr.light_wi = sample_point - sr.hit_point;
	sr.light_wi.normalize();

	sr.light_sample_point = sample_point;

	return sr.light_wi;
}

RGBColor AreaLight::L(ShadeRec& sr)
{
	float ndotd = -light_normal * sr.light_wi;
	if (ndotd > 0.0)
	{
		return dynamic_cast<Emissive*>(mat_ptr)->get_le();
	}
	else
	{
		return black;
	}
}

float AreaLight::G(const ShadeRec& sr) const
{
	float dot = -light_normal * sr.light_wi;
	float d2 = sr.light_sample_point.d_squared(sr.hit_point);
	//float d2 = 1.0f;

	return dot / d2;
}

float AreaLight::pdf(ShadeRec& sr) const
{
	return object_ptr->pdf(sr);
}

void AreaLight::set_object(GeometricObject *obj)
{
	object_ptr = obj;
}

void AreaLight::set_material(Material *mat)
{
	mat_ptr = mat;
}

bool AreaLight::in_shadow(const Ray &ray, const ShadeRec &sr) const
{
	double t;
	int num_objects = sr.w.objects.size();
	float ts = (sr.light_sample_point - ray.o) * ray.d;

	for (int j = 0; j < num_objects; j++)
	if (sr.w.objects[j]->shadow_hit(ray, t) && t < ts)
		return true;

	return false;
}




