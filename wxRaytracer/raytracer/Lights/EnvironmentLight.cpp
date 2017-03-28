#include "EnvironmentLight.h"
#include "pre_define.h"
#include "Sampler.h"
#include "Emissive.h"

EnvironmentLight::EnvironmentLight() :
sampler_ptr(0),
material_ptr(0)
{

}

EnvironmentLight::~EnvironmentLight()
{
	SAFE_DELETE(sampler_ptr);
	SAFE_DELETE(material_ptr);
}

EnvironmentLight * EnvironmentLight::clone() const
{
	return 0;
}

void EnvironmentLight::set_sampler(Sampler *sampler)
{
	SAFE_DELETE(sampler_ptr);
	sampler_ptr = sampler;

	sampler_ptr->map_samples_to_hemisphere(1.0);
}

Vector3D EnvironmentLight::get_direction(ShadeRec& sr)
{
	w = sr.normal;
	v = Vector3D(0, 1, 0) ^ w;
	v.normalize();
	u = v ^ w;
	Point3D sp = sampler_ptr->sample_hemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;
	return wi;
}

RGBColor EnvironmentLight::L(ShadeRec& sr)
{
	return dynamic_cast<Emissive*>(material_ptr)->get_le();
}

bool EnvironmentLight::in_shadow(const Ray &ray, const ShadeRec &sr) const
{
	double t = 10000;
	int num_objects = sr.w.objects.size();
	double d = kEpsilon;

	for (int j = 0; j < num_objects; j++)
	if (sr.w.objects[j]->shadow_hit(ray, t) && t > d)
		return (true);

	return (false);
}

float EnvironmentLight::pdf(ShadeRec& sr) const
{
	return sr.normal * wi * invPI;
}

void EnvironmentLight::set_material(Material *mat)
{
	SAFE_DELETE(material_ptr);
	material_ptr = mat;
}
