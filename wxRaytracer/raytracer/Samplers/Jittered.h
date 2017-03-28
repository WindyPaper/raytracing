#ifndef _JITTERED_H_
#define _JITTERED_H_

#include "Sampler.h"

class Jittered : public Sampler
{
public:
	virtual void generate_samplers();
};

#endif