#include "HairMaterial.h"
#include "Constants.h"
#include "HairBcsdf.h"

HairMaterial::HairMaterial() :
	hair_bcsdf(new HairBcsdf())
{
	
}

Material* HairMaterial::clone(void) const
{
	return NULL;
}

void HairMaterial::set_ce(const float r, const float g, const float b)
{
	//empty
}

RGBColor HairMaterial::get_Le(ShadeRec& sr) const
{
	return black;
}

RGBColor HairMaterial::area_light_shade(ShadeRec& sr)
{
	return black;
}

RGBColor HairMaterial::path_shade(ShadeRec& sr)
{
	RGBColor L = 0;
	//Vector3D wi;
	Vector3D wo = -sr.ray.d;

	//shadow ray
	RGBColor light_l = 0.0f;
	int light_sampler_num = 1;

	for (int sample_num = 0; sample_num < light_sampler_num; ++sample_num)
	{
		int num_lights = sr.w.lights.size();		
		for (int i = 0; i < num_lights; ++i)
		{
			Light *l = sr.w.lights[i];			

			Vector3D light_wi = l->get_direction(sr);
			float l_ndotwi = sr.normal * light_wi;

			//if (l_ndotwi > 0.0f)
			//{
				RGBColor f = hair_bcsdf->f(sr, wo, light_wi);

				light_l += f * l->L(sr) * l->G(sr) * 1 / l->pdf(sr);
			//}			
		}		
	}

	light_l /= light_sampler_num;

	return light_l;
	//return RGBColor(sr.u);
}
