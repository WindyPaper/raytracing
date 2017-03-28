#ifndef _AREA_LIGHTING_H_
#define _AREA_LIGHTING_H_

#include "Tracer.h"

class AreaLighting : public Tracer
{
public:
	AreaLighting();
	AreaLighting(World* _worldPtr);
	~AreaLighting();

	virtual RGBColor
		trace_ray(const Ray& ray) const;
};

#endif