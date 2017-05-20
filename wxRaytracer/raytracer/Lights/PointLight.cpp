#include "PointLight.h"
#include <assert.h>
#include "ShadeRec.h"
#include "World.h"

PointLight::PointLight() :
ls(1.0),
light_color(1.0)
{

}

PointLight::~PointLight()
{

}

Light* PointLight::clone(void) const
{
	assert(false);
	return 0;
}

Vector3D PointLight::get_direction(ShadeRec& sr)
{
	return (pos - sr.hit_point).hat();
}

RGBColor PointLight::L(ShadeRec& sr)
{
	return ls * light_color;
}

float PointLight::G(const ShadeRec& sr) const
{
	Vector3D dis_vec = pos - sr.hit_point;
	float dist = dis_vec.length();
	return 1.0 / dist;
}

void PointLight::set_position(const Point3D &pos)
{
	this->pos = pos;
}

const Point3D & PointLight::get_position() const
{
	return pos;
}

void PointLight::set_radiance(float radiance)
{
	ls = radiance;
}

bool PointLight::in_shadow(const Ray &ray, const ShadeRec &sr) const
{
	double t;
	int num_objects = sr.w.objects.size();
	float d = pos.distance(ray.o);

	for (int i = 0; i < num_objects; ++i)
	{
		if (sr.w.objects[i]->shadow_hit(ray, t) && (t < d))
		{
			return true;
		}
	}

	return false;
}
