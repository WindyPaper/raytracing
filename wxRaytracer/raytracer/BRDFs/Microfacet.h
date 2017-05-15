#pragma once

#include "BRDF.h"

struct IMFresnel
{
	virtual double val(Vector3D i, Vector3D o, Vector3D h, float in_ior, float out_ior) = 0;
};

struct SchlickApproximationFresnel : public IMFresnel
{
	virtual double val(Vector3D i, Vector3D o, Vector3D h, float in_ior, float out_ior);
};

struct IVisibleTerm
{
	virtual double val(Vector3D i, Vector3D o, Vector3D n, Vector3D h) = 0;
	virtual void set_roughness(float val) {}
};

struct BlinnGTerm : public IVisibleTerm
{
	virtual double val(Vector3D i, Vector3D o, Vector3D n, Vector3D h);
};

struct SchlickGTerm : public IVisibleTerm
{
	SchlickGTerm(float rough) :
		roughness(rough)
	{}
	virtual double val(Vector3D i, Vector3D o, Vector3D n, Vector3D h);

	virtual void set_roughness(float val)
	{
		roughness = val;
	}

	float roughness;

private:
	SchlickGTerm();
};

struct SmithGTerm : public IVisibleTerm
{
	SmithGTerm(float rough) :
		roughness(rough)
	{}

	virtual double val(Vector3D i, Vector3D o, Vector3D n, Vector3D h);

	//float G1(Vector3D v, Vector3D m, Vector3D n);

	virtual void set_roughness(float val)
	{
		roughness = val;
	}

	float roughness;

private:
	SmithGTerm();
};

struct IDistribution
{
	virtual double val(float roughness, Vector3D n, Vector3D h) = 0;
};

struct BeckmanDistribution : public IDistribution
{
	virtual double val(float roughness, Vector3D n, Vector3D h);
};

struct GGX : public IDistribution
{
	virtual double val(float roughness, Vector3D n, Vector3D h);
};

class Microfacet : public BRDF
{
public:
	enum DistributionType
	{
		GGX,
		WARD,		
		BECKMANNS,
	};

	Microfacet();
	Microfacet(DistributionType type);
	Microfacet(const Microfacet &obj);

	~Microfacet();

	Microfacet&
		operator= (const Microfacet& rhs);

	virtual Microfacet*
		clone(void) const;

	void set_ior(const float val);
	void set_cd(const RGBColor &color);
	void set_roughness(const float val);

	virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

	virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

protected:
	void init_default_param();

private:
	DistributionType d_type;

	IMFresnel *fresnel;
	IVisibleTerm *g_term;
	IDistribution *distribution;

	RGBColor cd;
	float ior;
	float roughness;
};
