#include "MicfacetBTDF.h"
#include "pre_define.h"
#include "Util.h"

MicrofacetBTDF::MicrofacetBTDF() :
	ior_i(1.0f),
	ior_o(1.5f),
	roughness(0.5)
{
	fresnel = new SchlickApproximationFresnel();
	g_term = new SchlickGTerm(roughness);
	ndf = new BeckmanDistribution();
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
	//	sr.normal * wi < 0.0)
	//{
	//	return 0;
	//}

	Vector3D h = -(ior_o * wo + ior_i * wi);
	h.normalize();

	double term1 = std::abs(wi * h) * std::abs(wo * h) / (std::abs(wi * sr.normal) * std::abs(wo * sr.normal));
	double term2 = ior_o * ior_o * (1 - fresnel->val(wi, wo, h, ior_i, ior_o)) *  g_term->val(wi, wo, sr.normal, h) * ndf->val(roughness, sr.normal, h) / \
		std::pow(ior_i * (wi * h) + ior_o * (wo * h), 2);

	
	//printf("term1 * term2 = %f\n", term1 * term2);

	if (!is_scalar_valid(term1) ||
		!is_scalar_valid(term2))
	{
		printf("MicracetBTDF term NAN !\n");
	}

	return term1 * term2;
}

RGBColor MicrofacetBTDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const
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
	float c = wo * m;
	float ior_ratio = ior_i / ior_o;
	int sign_i_dot_n = wo * sr.normal >= 0.0 ? 1 : -1;
	wt = (c * ior_ratio - sign_i_dot_n * std::sqrt(1 + ior_ratio * (c * c - 1))) * m - ior_ratio * wo;
	wt.normalize();
	if (std::isnan(wt.x) || std::isnan(wt.y) || std::isnan(wt.z))
	{
		printf("the param of wt nan !\n");
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
	roughness = val;

	SchlickGTerm *p = dynamic_cast<SchlickGTerm*>(g_term);
	if (p)
	{
		p->roughness = val;
	}
}

MicrofacetBTDF& MicrofacetBTDF::operator=(const MicrofacetBTDF& rhs)
{
	ior_i = rhs.ior_i;
	ior_o = rhs.ior_o;

	return *this;
}
