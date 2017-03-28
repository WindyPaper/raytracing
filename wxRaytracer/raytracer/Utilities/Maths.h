#ifndef __MATHS__
#define __MATHS__

#include <math.h>
#include <stdlib.h>

#define PI 3.141592653

#define INV_RAND_MAX (1.0 / (float)RAND_MAX)

inline double
max(double x0, double x1);

inline double
max(double x0, double x1)
{
	return((x0 > x1) ? x0 : x1);
}

/*
inline float rand_float()
{
	int random_int = std::rand() % 10000;
	return random_int / 1000.0;
}*/

// ----------------------------------------------------------------- min

inline double
min(double x0, double x1) {
	return ((x0 < x1) ? x0 : x1);
}

inline int rand_int()
{
	return rand();
}

inline float rand_float(void)
{
	return((float)rand_int() * INV_RAND_MAX);
}

inline float rand_float(int l, float h)
{
	return (rand_float() * (h - l) + l);
}

inline int rand_int(int l, int h) 
{
	return ((int)(rand_float(0, h - l + 1) + l));
}

inline float to_radian(float degree)
{
	return (degree * PI) / 180.0;
}

inline float to_degree(float radian)
{
	return (radian * 180.0) / PI;
}

static inline double clamp(double value, double low, double high)
{
	return value < low ? low : (value > high ? high : value);
}

#endif
