#pragma once

class Sampler;

class BSDF
{
public:
	BSDF();
	virtual ~BSDF();

protected:
	Sampler* sampler_ptr;		// for indirect illumination
};
