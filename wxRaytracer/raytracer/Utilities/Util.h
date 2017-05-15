#pragma once

#include <algorithm>

template<typename T>
inline bool is_scalar_valid(T val)
{
	if (std::isnan(val) ||
		std::isinf(val))
	{
		//printf("The val is INF or NAN!\n");
		return false;
	}

	return true;
}

inline float fresnel_dielectric(float eta, const Vector3D N,
	const Vector3D I, Vector3D *R, Vector3D *T, bool *is_inside)
{
	float cos = N * I, neta;
	Vector3D Nn;
	// compute reflection
	*R = (2 * cos)* N - I;
	// check which side of the surface we are on
	if (cos > 0) {
		// we are on the outside of the surface, going in
		neta = 1 / eta;
		Nn = N;
		*is_inside = false;
	}
	else {
		// we are inside the surface, 
		cos = -cos;
		neta = eta;
		Nn = -N;
		*is_inside = true;
	}
	*R = (2 * cos)* Nn - I;
	float arg = 1 - (neta * neta *(1 - (cos * cos)));
	if (arg < 0) {
		*T = Vector3D(0.0f, 0.0f, 0.0f);
		return 1; // total internal reflection
	}
	else {
		float dnp = sqrtf(arg);
		float nK = (neta * cos) - dnp;
		*T = -(neta * I) + (nK * Nn);

		// compute Fresnel terms
		float cosTheta1 = cos; // N.R
		float cosTheta2 = -Nn * (*T);
		float pPara = (cosTheta1 - eta * cosTheta2) / (cosTheta1 + eta * cosTheta2);
		float pPerp = (eta * cosTheta1 - cosTheta2) / (eta * cosTheta1 + cosTheta2);
		return 0.5f * (pPara * pPara + pPerp * pPerp);
	}
}

inline Vector3D get_reflection(const Vector3D &i, const Vector3D &n)
{
	float ndotwo = std::max(0.0, i * n);
	return -i + 2.0 * n * ndotwo;
}

inline Vector3D get_refraction(const float eta_i, const float eta_o, const Vector3D &i, const Vector3D &n, bool *is_inside, bool *is_tir)
{
	float cos = n * i, neta;
	Vector3D Nn;
	// check which side of the surface we are on
	if (cos > 0) {
		// we are on the outside of the surface, going in
		neta = eta_i / eta_o;
		Nn = n;
		*is_inside = false;
	}
	else {
		// we are inside the surface, 
		cos = -cos;
		neta = eta_o / eta_i;
		Nn = -n;
		*is_inside = true;
	}
	float arg = 1 - (neta * neta *(1 - (cos * cos)));
	if (arg < 0) 
	{
		*is_tir = true;
		return Vector3D(0.0f, 0.0f, 0.0f); // total internal reflection
	}
	else 
	{
		*is_tir = false;
		float dnp = sqrtf(arg);
		float nK = (neta * cos) - dnp;
		return -(neta * i) + (nK * Nn);
	}
}
