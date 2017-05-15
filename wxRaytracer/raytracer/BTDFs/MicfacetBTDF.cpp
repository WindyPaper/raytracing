#include "MicfacetBTDF.h"
#include "pre_define.h"
#include "Util.h"

MicrofacetBTDF::MicrofacetBTDF() :
	ior_i(1.0f),
	ior_o(1.5f),
	roughness(0.5)
{
	fresnel = new SchlickApproximationFresnel();
	g_term = new SmithGTerm(roughness);
	ndf = new GGX();
}

MicrofacetBTDF::~MicrofacetBTDF()
{
	SAFE_DELETE(fresnel);
	SAFE_DELETE(g_term);
	SAFE_DELETE(ndf);
}

MicrofacetBTDF* MicrofacetBTDF::clone(void)
{
	return new MicrofacetBTDF(*this);
}

RGBColor MicrofacetBTDF::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	//if (sr.normal * wo < 0.0 ||
	//	sr.normal * wi >= 0.0)
	//{
	//	return 0; //vectors on same side-- not possible
	//}

	//if ((wi * sr.normal) * (wo * sr.normal) > 0)
	//{
	//	return 0;
	//}

	float wi_dot_n = wi * sr.normal;
	float eta = ior_i / ior_o;
	float eta_i = ior_i;
	float eta_o = ior_o;
	Vector3D N = sr.normal;
	if (wi_dot_n < 0) //inside
	{
		eta = ior_o / ior_i;
		eta_i = ior_o;
		eta_o = ior_i;
		//N = -N;
		//h = -h;
	}
	Vector3D h = -(wo + wi * eta);
	Vector3D ht = h;
	h.normalize();

	/*h = h * signum(h * sr.normal);

	if (h * N < 0.0)
	{
		printf("h * n = %f\n", h * sr.normal);
	}*/	

	/*double term1 = std::abs(wi * h) * std::abs(wo * h) / (std::abs(wi * sr.normal) * std::abs(wo * sr.normal));
	double term2 = ior_o * ior_o * (1 - fresnel->val(wi, wo, h, ior_i, ior_o)) *  g_term->val(wi, wo, sr.normal, h) * ndf->val(roughness, sr.normal, h) / \
		std::pow(ior_i * (wi * h) + ior_o * (wo * h), 2);*/

	//float fs = std::abs(wiDotM*woDotM)*(1.0f - F)*G*D/(sqr(eta*wiDotM + woDotM)*std::abs(wiDotN));

	float wi_dot_m = wi * h;
	float wo_dot_m = wo * h;
	////float wi_dot_n = wi * sr.normal;
	//float temp = eta * wi_dot_m + wo_dot_m;
	float f = fresnel->val(wi, wo, h, eta_i, eta_o);
	float g = g_term->val(wi, wo, N, h);
	double d = ndf->val(roughness, N, h);
	//printf("d = %lf\n", d);
	//float ret_f_val = std::abs(wi_dot_m * wo_dot_m) * (1.0f - f) * g * d / std::sqrt(eta * wi_dot_m + wo_dot_m) * std::abs(wi_dot_n);

	//float invHt2 = 1 / (ht * ht);
	//float ret_f_val = (fabsf(wi_dot_m * wo_dot_m) * (eta * eta) * (g * d) * invHt2) / (wo * sr.normal);

	//from paper
	/*float term1 = std::abs(wi * h) * std::abs(wo * h) / (std::abs(wi * N) * std::abs(wo * N));
	float t = eta * wi_dot_m + wo_dot_m;
	float term2 = (1 - f) * g * d / (t * t);
	float ret_f_val = term1 * term2;*/

	if (d < LOW_EPS)
	{
		return 0;
	}

	/*float sqrtDenom = wi * h + 1/eta * (wo * h);
	float value = ((1/eta) * (1/eta)
		* (wi * h) * (wo * h)) /
		((wi * sr.normal)* sqrtDenom * sqrtDenom);*/

	float sq = (eta*wi_dot_m + wo_dot_m);
	float fs = std::abs(wi_dot_m*wo_dot_m)*g*d / (sq * sq * std::abs(wi * N));
	float value = fs;
	//printf("term1 * term2 = %f\n", term1 * term2);

	//printf("val = %f\n", value);
	if (!is_scalar_valid(value))
	{
		printf("MicracetBTDF f NAN !\n");	
	}

	if (value < 0.0)
	{
		value = 0;
	}
	else if (value > 1.0)
	{
		value = 1.0;
	}
	//printf("value = %f\n", value);
	return value;
	//return 0.5;
}

RGBColor MicrofacetBTDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt, float &pdf) const
{
	//if (wo * sr.normal < 0.0)
	//{
	//	//inside, return
	//	return 1.0;
	//}

	Vector3D w = sr.normal;
	Vector3D v = Vector3D(0.000034, 1.0, 0.00071) ^ w;
	v.normalize();
	Vector3D u = v ^ w;

	double roughness_2 = roughness * roughness;
	float rand0_1 = rand_float();
	float theta = std::atan(std::sqrt((-roughness_2 * std::log(1 - rand0_1))));
	float phi = rand_float(0, 2 * PI);
	Vector3D m = Vector3D(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta));
	m = m.x * u + m.y * v + m.z * w; // to world space
	m.normalize();

	if (m * sr.normal < -LOW_EPS)
	{
		printf("not possible! %f\n", m * sr.normal);
	}

	bool is_inside = false;
	bool is_tir = false;
	wt = get_refraction(ior_i, ior_o, wo, m, &is_inside, &is_tir);

	if (is_tir)
	{
		//printf("internal reflection! \n");
		wt = get_reflection(wo, -m);

		pdf = ndf->val(roughness, sr.normal, m) * std::abs(m * sr.normal);

		return f(sr, wo, wt);
	}

	if (std::isnan(wt.x) || std::isnan(wt.y) || std::isnan(wt.z))
	{
		printf("the param of wt nan !\n");
	}

	Vector3D h;
	float eta = ior_i / ior_o;
	bool is_enter = wo * sr.normal > 0.0 ? true : false;
	if (is_enter)
	{
		h = -(ior_o * wo + ior_i * wt);
	}
	else
	{
		h = -(ior_i * wo + ior_o * wt);
		eta = ior_o / ior_i;
	}
	Vector3D ht = h;
	h.normalize();
	float ht_2 = ht * ht;
	float d = ndf->val(roughness, sr.normal, h);
	float abs_nom = std::fabsf(sr.normal * m);
	float abs_wtom = std::fabsf(wt * m);
	pdf = d * abs_nom * (abs_wtom * eta * eta) / ht_2;
	if (!is_scalar_valid(pdf))
	{
		printf("Invalid pdf %f\n", pdf);
	}
	if (pdf < LOW_EPS)
	{
		pdf = 1.0f;
	}
	return f(sr, wo, wt);
}

void MicrofacetBTDF::set_ior_in(float val)
{
	ior_i = val;
}

void MicrofacetBTDF::set_ior_out(float val)
{
	ior_o = val;
}

void MicrofacetBTDF::set_roughness(float val)
{
	roughness = std::max(val, LOW_EPS);

	g_term->set_roughness(roughness);
}

MicrofacetBTDF& MicrofacetBTDF::operator=(const MicrofacetBTDF& rhs)
{
	ior_i = rhs.ior_i;
	ior_o = rhs.ior_o;

	return *this;
}
