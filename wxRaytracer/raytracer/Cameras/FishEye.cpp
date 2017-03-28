#include "FishEye.h"
#include <assert.h>
#include "Maths.h"
#include "RGBColor.h"
#include "ViewPlane.h"
#include "Sampler.h"


FishEye::FishEye() :
psi_degree(0)
{

}

FishEye::FishEye(float max_degree)
{
	psi_degree = max_degree;
}

FishEye::~FishEye()
{

}

Camera* FishEye::clone(void) const
{
	assert(false);
	return 0;
}

Vector3D FishEye::get_direction(const Point2D& pixel_point, const int hres, const int vres, const float pixel_s, float &r_squared) const
{
	Point2D pn(2.0 / (pixel_s * hres) * pixel_point.x, 2.0 / (pixel_s * vres) * pixel_point.y);
	r_squared = pn.x * pn.x + pn.y * pn.y;

	if (r_squared <= 1.0)
	{
		float r = std::sqrt(r_squared);
		float psi = r * psi_degree;
		float sin_psi = std::sin(to_radian(psi));		
		float cos_psi = std::cos(to_radian(psi));
		float sin_alpha = pn.y / r;
		float cos_alpha = pn.x / r;
		Vector3D dir = sin_psi * cos_alpha * u + sin_psi * sin_alpha * v - cos_psi * w;
		return dir;
	}
	return Vector3D(0.0);
}

void FishEye::render_scene(const World& w)
{
	RGBColor L;
	ViewPlane vp(w.vp);
	int hres = vp.hres;
	int vres = vp.vres;
	float s = vp.s;
	Ray ray;
	int depth = 0;
	Point2D sp;
	Point2D pp;
	float r_squared;

	//w.open_window()
	ray.o = eye;

	for (int i = 0; i < vres; ++i)
	{
		for (int j = 0; j < hres; ++j)
		{
			L = black;
			for (int s_num = 0; s_num < vp.num_samples; ++s_num)
			{
				sp = vp.sampler_ptr->sample_unit_square();
				pp.x = s * (j - 0.5 * hres + sp.x);
				pp.y = s * (i - 0.5 * vres + sp.y);
				ray.d = get_direction(pp, hres, vres, s, r_squared);

				if (r_squared <= 1.0)
				{
					L += w.tracer_ptr->trace_ray(ray);
				}
			}

			L /= vp.num_samples;
			L *= exposure_time;
			w.display_pixel(i, j, L);
		}
	}
}
