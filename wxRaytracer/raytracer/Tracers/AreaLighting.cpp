#include "AreaLighting.h"
#include "World.h"
#include "ShadeRec.h"
#include "Material.h"

AreaLighting::AreaLighting() :
Tracer()
{

}

AreaLighting::AreaLighting(World* _worldPtr) :
Tracer(_worldPtr)
{

}

AreaLighting::~AreaLighting()
{

}

RGBColor AreaLighting::trace_ray(const Ray& ray) const
{
	ShadeRec sr(world_ptr->hit_objects(ray));

	if (sr.hit_an_object)
	{
		sr.ray = ray;
		return sr.material_ptr->area_light_shade(sr);
	}
	else
	{
		return world_ptr->background_color;
	}
}

RGBColor AreaLighting::trace_ray(const Ray ray, double &t, const int depth) const
{
	ShadeRec sr(world_ptr->hit_objects(ray));

	if (sr.hit_an_object)
	{
		sr.ray = ray;
		return sr.material_ptr->area_light_shade(sr);
	}
	else
	{
		return world_ptr->background_color;
	}
}

RGBColor AreaLighting::trace_ray(const Ray ray, const int depth) const
{
	ShadeRec sr(world_ptr->hit_objects(ray));

	if (sr.hit_an_object)
	{
		sr.ray = ray;
		return sr.material_ptr->area_light_shade(sr);
	}
	else
	{
		return world_ptr->background_color;
	}
}
