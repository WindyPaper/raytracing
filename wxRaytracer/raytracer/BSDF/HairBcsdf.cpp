#include "HairBcsdf.h"

HairBcsdf::HairBcsdf()
{
	//ior = 1.55f;
	//float roughness 
	beta_r = 0.2f;
	beta_tt = beta_r * 0.5f;
	beta_trt = beta_r * 2.0f;

	s = (0.265f * beta_r + 1.194f * (beta_r * beta_r) + 5.372f * std::pow(beta_r, 22));
}

HairBcsdf* HairBcsdf::clone(void) const
{
	return NULL;
}

RGBColor HairBcsdf::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const
{
	/*ior = 1.55f;
	v_dot_l = wo * wi;
	float sin_theta_i = sr.dpdu * wi;
	float sin_theta_o = sr.dpdu * wo;

	cos_theta_t = std::sqrt(1 - std::pow((1 / ior) * sin_theta_i, 2));
	cos_theta_d = std::cos(0.5 * std::abs(std::asin(sin_theta_o) - std::asin(sin_theta_i)));

	Vector3D wi_p = wi - sin_theta_i * sr.dpdu;
	Vector3D wo_p = wo - sin_theta_o * sr.dpdu;
	cos_phi = wi_p * wo_p * (1 / std::sqrt((wi_p * wi_p) * (wo_p * wo_p)));

	ior_prime = std::sqrt(ior * ior - 1 + std::pow(cos_theta_d, 2)) / cos_theta_d;

	float v[3] = { beta_r * beta_r, beta_tt * beta_tt, beta_trt * beta_trt };

	RGBColor s = RGBColor();
	for (int p = 0; p < 3; ++p)
	{
		s += M(v[p], sin_theta_i, sin_theta_o, std::sqrt(1.0f - sin_theta_i * sin_theta_i), std::sqrt(1.0f - sin_theta_o * sin_theta_o));
	}*/

	return RGBColor();
}

RGBColor HairBcsdf::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi)
{
	ior = 1.55f;
	float v_dot_l = wo * wi;
	float sin_theta_i = sr.dpdu * wi;
	float sin_theta_o = sr.dpdu * wo;

	float h = 2 * sr.v - 1.0f;

	float cos_theta_t = std::sqrt(1 - std::pow((1 / ior) * sin_theta_i, 2));
	float cos_theta_d = std::cos(0.5 * std::abs(std::asin(sin_theta_o) - std::asin(sin_theta_i)));

	Vector3D wi_p = wi - sin_theta_i * sr.dpdu;
	Vector3D wo_p = wo - sin_theta_o * sr.dpdu;
	float cos_phi = wi_p * wo_p * (1 / std::sqrt((wi_p * wi_p) * (wo_p * wo_p)));

	ior_prime = std::sqrt(ior * ior - 1 + std::pow(cos_theta_d, 2)) / cos_theta_d;

	float v[3] = { beta_r * beta_r, beta_tt * beta_tt, beta_trt * beta_trt };

	RGBColor s = RGBColor();
	for (int p = 0; p < 3; ++p)
	{
		//s += M(v[p], sin_theta_i, sin_theta_o, std::sqrt(1.0f - sin_theta_i * sin_theta_i), std::sqrt(1.0f - sin_theta_o * sin_theta_o)) * N(p, v[p]);
		s += M(v[p], sin_theta_i, sin_theta_o, std::sqrt(1.0f - sin_theta_i * sin_theta_i), std::sqrt(1.0f - sin_theta_o * sin_theta_o)) * N_h(p, v[p], h, v_dot_l, cos_theta_t, cos_theta_d, cos_phi);
	}

	return s;
}

RGBColor HairBcsdf::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const
{
	return RGBColor();
}

float HairBcsdf::g(float beta, float theta)
{
	return 1.0f / std::sqrt(2 * PI * beta * beta) * std::exp(-theta * theta / (2 * beta * beta));
}

float HairBcsdf::I0(float x)
{
	float frac = 1;
	float ret_i0 = 0.0f;
	for (int i = 0; i <= 10; ++i)
	{
		if (i != 0)
		{
			frac *= i;
		}
		ret_i0 += std::pow(x, 2 * i) / (std::pow(4, i) * (frac * frac));
	}

	return ret_i0;
}

float HairBcsdf::log_I0(float x)
{
	if (x > 12.0f)
		// More stable evaluation of log(I0(x))
		// See also https://publons.com/review/414383/
		return x + 0.5f*(std::log(1.0f / (2 * PI *x)) + 1.0f / (8.0f*x));
	else
		return std::log(I0(x));
}

float HairBcsdf::M(float v, float sin_thetai, float sin_thetao, float cos_thetai, float cos_thetao)
{
	float a = cos_thetai * cos_thetao / v;
	float b = sin_thetai * sin_thetao / v;

	float mp = 0.0f;
	if (v <= 0.1)
	{
		mp = std::exp(log_I0(a) - b - 1 / v + 0.6931f + std::log(1 / (2 * v)));
	}
	else
	{
		mp = std::exp(-b) * I0(a) / (std::sinh(1 / v) * 2 * v);
	}

	return mp;
}

float HairBcsdf::omega(int p, float h)
{
	float yi = std::asin(h);
	float yt = std::asin(h / ior_prime);

	return 2 * p * yt - 2 * yi + p * PI;
}

float HairBcsdf::gaussian_detector(float beta, float phi)
{
	float dp = 0;
	for (int k = -4; k <= 4; ++k)
	{
		dp += g(beta, phi - (2 * PI) * k);
	}

	return dp;
}

float HairBcsdf::logistic_function(float x, float s)
{
	x = std::abs(x);
	float e_val = std::exp(-x / s);
	return e_val / (s * (1 + e_val) * (1 + e_val));
}

float HairBcsdf::logistic_cdf(float x, float s)
{
	return 1.0f / (1.0 + std::exp(-x / s));
}

float HairBcsdf::trimmed_logistic(float x, float s, float a, float b)
{
	return logistic_function(x, s) / (logistic_cdf(b, s) - logistic_cdf(a, s));
}

float HairBcsdf::hair_F(float cos_theta)
{
	const float n = 1.55;
	float F0 = (1 - n) / (1 + n);
	F0 *= F0;
	return F0 + (1 - F0) * std::pow(1 - cos_theta, 5);
}

RGBColor HairBcsdf::attenuation(int p, float h, RGBColor ua, float v_dot_l, float cos_theta_t, float cos_theta_d)
{
	RGBColor A = 0;
	if (p == 0) //R
	{
		A = hair_F(std::sqrt(0.5 + 0.5 * v_dot_l));
	}
	else
	{
		//float ua = 0.2; //absorption
		RGBColor ua_prime = ua / cos_theta_t;
		float f = hair_F(cos_theta_d * std::sqrt(1 - h * h));

		//float yi = std::asin(h);
		float yt = std::asin(h / ior_prime);

		RGBColor T = 0;
		T.r = std::exp(-2 * ua_prime.r * (1 + std::cos(2 * yt)));
		T.g = std::exp(-2 * ua_prime.g * (1 + std::cos(2 * yt)));
		T.b = std::exp(-2 * ua_prime.b * (1 + std::cos(2 * yt)));

		A.r = std::pow(1 - f, 2) * std::pow(f, p - 1) * std::pow(T.r, p);
		A.g = std::pow(1 - f, 2) * std::pow(f, p - 1) * std::pow(T.g, p);
		A.b = std::pow(1 - f, 2) * std::pow(f, p - 1) * std::pow(T.b, p);
	}

	return A;
}

RGBColor HairBcsdf::N(int p, float beta, float v_dot_l, float cos_theta_t, float cos_theta_d, float cos_phi)
{
	RGBColor ua = RGBColor(0.4, 0.6, 0.1); //absorption
	float phi = std::acos(cos_phi);

	int subdiv_num = 16;
	float w = 2.0f / subdiv_num;
	RGBColor N = 0;
	for (int i = 0; i < subdiv_num; ++i)
	{
		float h = ((rand_float() + i) * w) - 1; //[-1, 1]

		N += attenuation(p, h, ua, v_dot_l, cos_theta_t, cos_theta_d) * gaussian_detector(beta, phi - omega(p, h));
	}

	N *= w;

	return N * 0.5f;
}

RGBColor HairBcsdf::N_h(int p, float beta, float h, float v_dot_l, float cos_theta_t, float cos_theta_d, float cos_phi)
{
	RGBColor ua = RGBColor(0.4, 0.6, 0.1); //absorption
	float phi = std::acos(cos_phi);

	RGBColor N = 0.0f;

	float dphi = phi - omega(p, h);
	while (dphi > PI) dphi -= 2 * PI;
	while (dphi < -PI) dphi += 2 * PI;

	N = attenuation(p, h, ua, v_dot_l, cos_theta_t, cos_theta_d) * trimmed_logistic(dphi, s, -PI, PI);;

	return N * 0.5f;
}
