#pragma once

#define MAX_SURFACE_BXDF_NUM 8

class Sampler;
class BxDF;

class BSDF
{
public:
	BSDF();
	virtual ~BSDF();

	void add(BxDF *bxdf);

protected:
	Sampler* sampler_ptr;		// for indirect illumination

	BxDF *bxdfs[MAX_SURFACE_BXDF_NUM];
};
