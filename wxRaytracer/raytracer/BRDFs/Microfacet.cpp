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
	fresnel = new DielectricFresnel();
	g_term = new SmithGTerm(roughness);
	distribution = new ::GGX();
	//distribution = new BeckmanDistribution();
}

Microfacet::~Microfacet()
{
	SAFE_DELETE(fresnel);
	SAFE_DELETE(g_term);
	SAFE_DELETE(distribution);
}

void Microfacet::init_default_param()
{
	fresnel = new DielectricFresnel();
	g_term = new SmithGTerm(roughness);
	distribution = new ::GGX();
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
	roughness = std::max(val, ROUGHNESS_EPS);

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
	if (!is_scalar_valid(d))
	{
		d = 100000;//std::numeric_limits<double>::max()/2;
	}

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
	Vector3D m = distribution->d_sample(roughness, sr.normal);
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
	float dwh_dwo = 1 / (4 * std::abs(wo * h));
	pdf = distribution->val(roughness, sr.normal, h) * std::abs(m * sr.normal) * dwh_dwo;

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
	float cosi = o * h;
	float eta = out_ior / in_ior;

	if (cosi < 0.0f)
	{
		eta = 1 / (eta);
		cosi = -cosi;
	}

	float Rn = (1.0f - eta) / (1.0f + eta);
	float R0 = Rn * Rn;
	float F1 = 1.0f - cosi;
	float F2 = F1 * F1;
	float F5 = F2 * F2 * F1;

	return clamp(R0 + (1.0f - R0) * F5, 0.0f, 1.0f);
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

Vector3D BeckmanDistribution::d_sample(float roughness, const Vector3D &n)
{
	Vector3D w = n;
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
	
	return m;
}

double GGX::val(float roughness, Vector3D n, Vector3D h)
{
	if (h * n <= 0.0f)
		return 0.0f;
	float alphaSq = roughness*roughness;
	float cosThetaSq = (h * n) * (h * n);
	return alphaSq / (PI * std::pow((cosThetaSq * (alphaSq - 1) + 1), 2));
}

Vector3D GGX::d_sample(float roughness, const Vector3D &n)
{
	Vector3D w = n;
	Vector3D v = Vector3D(0.000034, 1.0, 0.00071) ^ w;
	v.normalize();
	Vector3D u = v ^ w;

	float rand0_1 = rand_float();
	float theta = std::atan(roughness * std::sqrt(rand0_1/(1.0 - rand0_1)));
	float phi = rand_float(0, 2 * PI);

	Vector3D m = Vector3D(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta));
	m = m.x * u + m.y * v + m.z * w; // to world space
	m.normalize();

	return m;
}

double SmithGTerm::val(Vector3D i, Vector3D o, Vector3D n, Vector3D h)
{
	return G1(i, h, n) * G1(o, h, n);
}

float SmithGTerm::G1(Vector3D v, Vector3D m, Vector3D n)
{
	int visible = v * m / (v * n) > 0.0f ? 1 : 0;
	if (visible == 0)
	{
		return 0;
	}

	float tan_o_2 = 1 / ((v * n) * (v * n)) - 1;
	float rough_2 = roughness * roughness;

	float val = visible * 2 / (1 + std::sqrt(1 + rough_2 * tan_o_2));
	return val;
}

double DielectricFresnel::val(Vector3D i, Vector3D o, Vector3D h, float in_ior, float out_ior)
{
	float cosi = o * h;
	float eta = out_ior / in_ior;

	if (cosi < 0.0f)
	{
		eta = 1 / (eta);
		cosi = -cosi;
	}

	float g = eta * eta - 1.0f + cosi * cosi;
	if (g > 0.0f)
	{
		g = sqrtf(g);
		float A = (g - cosi) / (g + cosi);
		float B = (cosi * (g + cosi) - 1.0f) / (cosi * (g - cosi) + 1.0f);
		return 0.5f * A * A * (1.0f + B * B);
	}

	return 1.0f;
}
