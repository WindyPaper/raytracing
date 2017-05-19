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
	float wo_dot_n = wo * sr.normal;

	float eta = ior_i / ior_o;
	float eta_i = ior_i;
	float eta_o = ior_o;
	//Vector3D N = sr.normal;
	if (wi_dot_n < 0) //inside
	{
		eta = ior_o / ior_i;
		eta_i = ior_o;
		eta_o = ior_i;
	}

	if (wi_dot_n * wo_dot_n > 0.0) // internal reflection
	{
		Vector3D hr = signum(wi * sr.normal) * (wi + wo);

		float fl = fresnel->val(wi, wo, hr, eta_i, eta_o);
		float g = g_term->val(wi, wo, sr.normal, hr);
		double d = ndf->val(roughness, sr.normal, hr);

		float fr =fl * g * d / (4 * std::abs(wi_dot_n * wo_dot_n));
		if (!is_scalar_valid(fr))
		{
			//printf("MicracetBRDF f NAN !\n");
			fr = std::numeric_limits<float>::max();
		}
		return clamp(fr, 0, 1);
	}

	Vector3D h = -(wo + wi * eta);
	h.normalize();

	//h = h * signum(h * sr.normal);	

	float wi_dot_m = wi * h;
	float wo_dot_m = wo * h;	
	float f = fresnel->val(wi, wo, h, eta_i, eta_o);
	float g = g_term->val(wi, wo, sr.normal, h);
	double d = ndf->val(roughness, sr.normal, h);	

	float sq = (eta*wi_dot_m + wo_dot_m);
	float value = std::abs(wi_dot_m * wo_dot_m) * (1.0 - f) * g * d / (sq * sq * std::abs(wi_dot_n));

	if (!is_scalar_valid(value))
	{
		printf("MicracetBTDF f NAN !\n");	
	}
	
	return value;
}

RGBColor MicrofacetBTDF::sample_f(const ShadeRec& sr, const Vector3D& wi, Vector3D& wt, float &pdf) const
{
	Vector3D m = ndf->d_sample(roughness, sr.normal);

	if (m * sr.normal < -LOW_EPS)
	{
		printf("not possible! %f\n", m * sr.normal);
	}

	bool is_inside = false;
	bool is_tir = false;

	wt = get_refraction(ior_i, ior_o, wi, m, &is_inside, &is_tir);

	if (is_tir)
	{
		//printf("internal reflection! \n");
		wt = get_reflection(wi, -m);

		//Vector3D hr = signum(wi * sr.normal) * (wi + wt);
		float dwh_dwo = 1 / (4 * std::abs(wt * m));
		pdf = ndf->val(roughness, sr.normal, m) * std::abs(m * sr.normal) * dwh_dwo;

		return f(sr, wi, wt);
	}

	if (std::isnan(wt.x) || std::isnan(wt.y) || std::isnan(wt.z))
	{
		printf("the param of wt nan !\n");
		if (!is_scalar_valid(pdf))
		{
			printf("Invalid pdf %f\n", pdf);
		}
	}

	Vector3D h;
	float eta_i = ior_i;
	float eta_o = ior_o;
	
	if (is_inside)
	{		
		eta_i = ior_o;
		eta_o = ior_i;
	}
	float eta = eta_i / eta_o;
	h = -(eta_i * wi + eta_o * wt);
	Vector3D ht = h;
	h.normalize();
	float ht_2 = ht * ht;
	float d = ndf->val(roughness, sr.normal, h);
	float abs_nom = std::fabsf(sr.normal * m);
	float abs_oh = std::fabsf(wt * h);
	//pdf = d * abs_nom * (abs_oh * eta_o * eta_o) / ht_2;

	float wo_dot_m = wt * m;
	float wi_dot_m = wi * m;
	//printf("pm = %f, wo*m = %f, sqr(wim_wom) = %f \n", d * abs_nom, std::abs(wo_dot_m), sqr(eta*wi_dot_m + wo_dot_m));
	//pdf = d * abs_nom * std::abs(wo_dot_m) / sqr(eta*wi_dot_m + wo_dot_m);
	pdf = this->pdf(wi, wt, sr.normal);

	if (!is_scalar_valid(pdf))
	{
		printf("Invalid pdf %f\n", pdf);
	}
	/*if (pdf < LOW_EPS)
	{
		pdf = 1.0f;
	}
	else if (pdf > 1.0)
	{
		pdf = 1.0;
	}*/

	//printf("pdf = %f\n", pdf);
	return f(sr, wi, wt);
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

float MicrofacetBTDF::pdf(const Vector3D &wi, const Vector3D &wo, const Vector3D &n) const
{
	bool is_same_hsphere = ((wo * n) * (wi * n) > 0);
	if (is_same_hsphere)
	{
		Vector3D h = (wo + wi);
		h.normalize();
		float EoH = std::abs(wo * h);
		float HoN = std::abs(h * n);
		return ndf->val(roughness, n, h) * HoN / (4.0f * EoH);
	}

	float eta = (wo * n) > 0 ? (ior_o / ior_i) : (ior_i / ior_o);
	Vector3D wh = -(wo + wi * eta);
	wh.normalize();

	// Compute change of variables _dwh\_dwi_ for microfacet transmission
	float sqrtDenom = (wo * wh) + eta * (wi * wh);
	float dwh_dwi = eta * eta * std::abs(wi * wh) / (sqrtDenom * sqrtDenom);
	float HoN = std::abs(wh * n);
	return ndf->val(roughness, n, wh) * HoN * dwh_dwi;
}
