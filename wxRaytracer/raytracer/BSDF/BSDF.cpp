#include "BSDF.h"
#include "pre_define.h"
#include "Sampler.h"

BSDF::BSDF() :
	sampler_ptr(NULL)
{

}

BSDF::~BSDF()
{
	SAFE_DELETE(sampler_ptr);
}
