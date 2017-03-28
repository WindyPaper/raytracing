#include "NRooks.h"
#include "Point2D.h"
#include "Maths.h"

void NRooks::generate_samplers()
{
	Point2D point;
	for (int i = 0; i < num_sets; ++i)
	{
		for (int j = 0; j < num_samples; ++j)
		{
			point.x = (j + rand_float());
			point.y = (j + rand_float());
			samples.push_back(point);
		}
	}

	shuffle_x_coordinates();
	shuffle_y_coordinates();
}

void NRooks::shuffle_x_coordinates()
{
	//make pos random
	for (int i = 0; i < num_sets; ++i)
	{
		for (int j = 0; j < num_samples - 1; ++num_samples)
		{
			int random_pos = num_samples * i + rand_int() % num_samples;
			float temp_x = samples[num_samples * i + j].x;
			samples[num_samples * i + j + 1].x = samples[random_pos].x;
			samples[random_pos].x = temp_x;
		}
	}
}

void NRooks::shuffle_y_coordinates()
{
	//make pos random
	for (int i = 0; i < num_sets; ++i)
	{
		for (int j = 0; j < num_samples - 1; ++num_samples)
		{
			int random_pos = num_samples * i + rand_int() % num_samples;
			float temp_y = samples[num_samples * i + j].y;
			samples[num_samples * i + j + 1].y = samples[random_pos].y;
			samples[random_pos].y = temp_y;
		}
	}
}
