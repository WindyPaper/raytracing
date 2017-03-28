#ifndef _NROOKS_H_
#define _NROOKS_H_

#include "Sampler.h"

class NRooks : public Sampler
{
public:
	virtual void generate_samplers();

	void shuffle_x_coordinates();
	void shuffle_y_coordinates();
};

#endif