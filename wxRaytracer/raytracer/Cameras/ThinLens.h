#ifndef _THIN_LENS_H_
#define _THIN_LENS_H_

#include "Point2D.h"
#include "Camera.h"

class Sampler;

class ThinLens : public Camera
{
public:
	ThinLens();
	virtual ~ThinLens();

	virtual Camera*
		clone(void) const;

/*
	ThinLens&
		operator= (const ThinLens& rhs);*/

	/*void
		set_view_distance(const float vpd);

	void
		set_zoom(const float zoom_factor);*/

	void set_sampler(Sampler *p_sampler);

	Vector3D
		get_direction(const Point2D& pixel_point, const Point2D &len_point) const;

	virtual void
		render_scene(const World& w);

private:
	float len_radius;
	float d;
	float f;
	float zoom;
	Sampler *sampler_ptr;
};

#endif