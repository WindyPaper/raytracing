#include "Microfacet.h"
#include "Util.h"
#include "pre_define.h"

Microfacet::Microfacet() :
	BRDF(),
	ior(1.5f),
	roughness(0.0f)
{
	init_default_param();
}

Microfacet::Microfacet(const Microfacet &obj) :
	BRDF(obj),
	ior(obj.ior),
	roughness(obj.roughness),
	cd(obj.cd)
{
	init_default_param();
	set_roughness(roughness);
}

Microfacet::Microfacet(DistributionType type) :
	BRDF(),
	ior(1.5f),
	roughness(0.0f)
{
	fresnel = new SchlickApproximationFresnel();
	g_term = new SchlickGTerm(roughness);
	distribution = new BeckmanDistribution();
}

Microfacet::~Microfacet()
{
	SAFE_DELETE(fresnel);
	SAFE_DELETE(g_term);
	SAFE_DELETE(distribution);
}

void Microfacet::init_default_param()
{
	fresnel = new SchlickApproximationFresnel();
	g_term = new SchlickGTerm(roughness);
	distribution = new BeckmanDistribution();
}

Microfacet* Microfacet::clone(void) const
{
	return new Microfacet(*this);
}

void Microfacet::set_ior(const float val)
{
	ior = val;
}

void Microfacet::set_cd(const RGBColor &color)
{
	cd = color;
}

void Microfacet::set_roughness(const float val)
{
	roughness = std::max(val, LOW_EPS);

	SchlickGTerm *s_gterm = dynamic_cast<SchlickGTerm*>(g_term);
	if (s_gterm)
	{
		s_gterm->roughness = roughness;
	}
}

RGBColor Microfacet::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	if (sr.normal * wo < 0.0 ||
		sr.normal * wi < 0.0)
	{
		return 0;
	}

	int sign_val = wo * sr.normal >= 0 ? 1 : -1;
	Vector3D h(sign_val * (wo + wi));
	h.normalize();
	
	double fresnel_val = fresnel->val(wi, wo, h, 1.0, ior);
	double g = g_term->val(wi, wo, sr.normal, h);
	double d = distribution->val(roughness, sr.normal, h);

	double f = fresnel_val * g * d / (4 * std::max(0.0, (sr.normal * wi)) * std::max(0.0, (sr.normal * wo)));
	
	if (!is_scalar_valid(f))
	{
		printf("Microfacet f is NAN!\n");
		f = 0.0;
	}
	return f * cd;
}

RGBColor Microfacet::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
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
	float ndotwo = std::max(0.0, wo * m);
	wi = -wo + 2.0 * m * ndotwo;
	wi.normalize();
	if (std::isnan(wi.x) || std::isnan(wi.y) || std::isnan(wi.z))
	{
		printf("the param of wi nan !\n");
	}

	//Beckmanns pdf
	int sign_val = wo * sr.normal >= 0 ? 1 : -1;
	Vector3D h(sign_val * (wo + wi));
	h.normalize();	
	pdf = distribution->val(roughness, sr.normal, h) * std::abs(m * sr.normal);

	if (std::isinf(pdf) ||
		std::isnan(pdf))
	{
		pdf = 1.0;
	}

	return f(sr, wo, wi);
}

Microfacet& Microfacet::operator=(const Microfacet& rhs)
{
	BRDF::operator=(rhs);

	cd = rhs.cd;
	ior = rhs.ior;

	return (*this);
}

double SchlickApproximationFresnel::val(Vector3D i, Vector3D o, Vector3D h, float in_ior, float out_ior)
{
	double f0 = abs((in_ior - out_ior) / (in_ior + out_ior));
	f0 = f0 * f0;
	double fresnel = f0 + (1.0f - f0) * std::powf((1.0 - (h * i)), 5);

	return fresnel;
}

double BlinnGTerm::val(Vector3D i, Vector3D o, Vector3D n, Vector3D h)
{
	double g_reach_view = 1.0;
	double g_block_after_reflect = 2 * (n * h) * (n * o) / (o * h);
	double g_block_before_reach = 2 * (n * h) * (n * i) / (o * h);
	double g_term = std::min({ g_reach_view, g_block_after_reflect, g_block_before_reach });
	return g_term;
}

double SchlickGTerm::val(Vector3D i, Vector3D o, Vector3D n, Vector3D h)
{
	/*double a = 1 / (roughness * std::tan(std::acos(o * n)));
	double px = (o * h / (o * n)) > 0.000 ? 1.0 : 0.0;
	double g_term = 1.0;
	if (a < 1.6)
	{
		g_term = px * (3.535 * a + 2.181 * a * a) / (1 + 2.276 * a + 2.577 * a * a);
	}

	return g_term;*/
	float k = roughness * roughness * 0.5f;
	float Vis_SchlickV = std::abs(o * n) * (1 - k) + k;
	float Vis_SchlickL = std::abs(i * n) * (1 - k) + k;
	return 0.25f / (Vis_SchlickV * Vis_SchlickL);
}

double BeckmanDistribution::val(float roughness, Vector3D n, Vector3D h)
{
	double n_dot_h = std::max(0.0, n * h);
	if (n_dot_h < LOW_EPS)
	{
		return 0;
	}
	double n_dot_h_2 = n_dot_h * n_dot_h;
	double roughness_2 = roughness * roughness;
	double exp_pow = (n_dot_h_2 - 1) / (roughness_2 * n_dot_h_2);
	double d = 1.0f / (PI * roughness_2 * n_dot_h_2 * n_dot_h_2) * std::exp(exp_pow);

	return d;
}

double GGX::val(float roughness, Vector3D n, Vector3D h)
{
	/*int m_dot_n = n * h > 0.0f ? 1 : 0;
	if (m_dot_n == 0)
	{
		return 0;
	}

	float cos_m_4 = std::powf(n * h, 4);
	float tan_m_2 = 1 / ((n * h) * (n * h)) - 1;
	float rough_2 = roughness * roughness;
	float d = rough_2 * m_dot_n / (PI * cos_m_4 * std::powf(rough_2 + tan_m_2, 2));
	return d;*/

	float m = roughness * roughness;
	float m_dot_n = n * h;
	if (m_dot_n < LOW_EPS)
	{
		return 0;
	}
	float d = (m - 1.0f) * m_dot_n * m_dot_n + 1.0f;
	if (d < LOW_EPS)
	{
		return 0;
	}
	return m / (PI*d*d);
}

double SmithGTerm::val(Vector3D i, Vector3D o, Vector3D n, Vector3D h)
{
	int visible = o * h / (o * n) > 0.0f ? 1 : 0;
	if (visible == 0)
	{
		return 0;
	}

	float tan_o_2 = 1 / (o * h * o * h) - 1;
	float rough_2 = roughness * roughness;

	float v = visible * 2 / (1 + std::sqrt(1 + rough_2 * tan_o_2));
	return v;
}
