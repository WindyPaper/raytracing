#include "BSDF.h"
#include "pre_define.h"
#include "Sampler.h"

BSDF::BSDF() :
	sampler_ptr(NULL)
{
	memset(bxdfs, 0, sizeof(bxdfs));
}

BSDF::~BSDF()
{
	SAFE_DELETE(sampler_ptr);
}

void BSDF::add(BxDF *bxdf)
{

}
