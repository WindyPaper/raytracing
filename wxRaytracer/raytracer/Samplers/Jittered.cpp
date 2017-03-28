#include "Jittered.h"
#include "Maths.h"
#include <math.h>

void Jittered::generate_samplers()
{
	int n = (int)std::sqrt(num_samples);

	for (int i = 0; i < num_sets; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			for (int k = 0; k < n; ++k)
			{
				Point2D point((k + rand_float()) / n, (j + rand_float()) / n);
				samples.push_back(point);
			}
		}
	}
}
