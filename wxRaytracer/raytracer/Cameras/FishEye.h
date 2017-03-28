#ifndef _FISH_EYE_H_
#define _FISH_EYE_H_

#include "Point2D.h"
#include "World.h"
#include "Camera.h"

class FishEye : public Camera
{
public:
	FishEye();
	FishEye(float max_degree);
	virtual ~FishEye();

	virtual Camera*
		clone(void) const;

	/*
	ThinLens&
	operator= (const ThinLens& rhs);*/

	/*void
	set_view_distance(const float vpd);

	void
	set_zoom(const float zoom_factor);*/

	Vector3D
		get_direction(const Point2D& pixel_point, const int hres, const int vres, const float pixel_s, float &r_squared) const;

	virtual void
		render_scene(const World& w);

private:
	float psi_degree; //in dgree
};

#endif