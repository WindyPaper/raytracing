#include "ThinLens.h"
#include <assert.h>
#include "Constants.h" 
#include "Point3D.h"
#include "Vector3D.h"
#include "Pinhole.h"
#include "Sampler.h"
#include <math.h>

ThinLens::ThinLens() :
Camera(),
len_radius(20),
d(500),
f(700),
zoom(1.0),
sampler_ptr(0)
{

}

ThinLens::~ThinLens()
{
	if (sampler_ptr)
	{
		delete sampler_ptr;
		sampler_ptr = 0;
	}
}

Camera* ThinLens::clone(void) const
{
	assert(false);
	return 0;
}

void ThinLens::set_sampler(Sampler *p_sampler)
{
	if (sampler_ptr)
	{
		delete sampler_ptr;
		sampler_ptr = 0;
	}
	sampler_ptr = p_sampler;
	sampler_ptr->map_samples_to_unit_disk();
}

Vector3D ThinLens::get_direction(const Point2D& pixel_point, const Point2D &len_point) const
{
	Point2D p;
	p.x = pixel_point.x * f / d;
	p.y = pixel_point.y * f / d;

	Vector3D dir = (p.x - len_point.x) * u + (p.y - len_point.y) * v - f * w;
	dir.normalize();

	return dir;
}

void ThinLens::render_scene(const World& w)
{
	RGBColor	L;
	ViewPlane	vp(w.vp);
	Ray			ray;
	int 		depth = 0;
	
	Point2D sp; //random sample point in [0, 1] x [0, 1]
	Point2D pp;	//sample point on a pixel
	Point2D dp; //sample point on unit disk
	Point2D lp; //sample point on lens

	vp.s /= zoom;
	//ray.o = eye;

	for (int r = 0; r < vp.vres; r++)			// up
	for (int c = 0; c < vp.hres; c++) {		// across 					
		L = black;

		for (int n = 0; n < vp.num_samples; ++n) {
			sp = vp.sampler_ptr->sample_unit_square();
			pp.x = vp.s * (c - 0.5 * vp.hres + sp.x);
			pp.y = vp.s * (r - 0.5 * vp.vres + sp.y);
			
			dp = sampler_ptr->sample_unit_disk();
			lp = dp * len_radius;
			ray.o = eye + lp.x * u + lp.y * v;
			ray.d = get_direction(pp, lp);
			L += w.tracer_ptr->trace_ray(ray);
		}

		L /= vp.num_samples;
		L *= exposure_time;
		w.display_pixel(r, c, L);
	}
}
