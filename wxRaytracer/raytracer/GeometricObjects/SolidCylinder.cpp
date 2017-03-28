#include "SolidCylinder.h"
#include <math.h>

const double SolidCylinder::kEpsilon = 0.001;

SolidCylinder::SolidCylinder(float bottom, float top, float radius) :
bottom(bottom),
top(top),
radius(radius)
{

}

SolidCylinder::~SolidCylinder()
{

}

SolidCylinder * SolidCylinder::clone(void) const
{
	return 0;
}

bool SolidCylinder::hit(const Ray& ray, double& tmin, ShadeRec *sr) const
{
	const Vector3D &start = ray.o;
	const Vector3D &dir = ray.d;

	float a = dir.x * dir.x + dir.z * dir.z;
	float b = 2 * start.x * dir.x + 2 * start.z * dir.z;
	float c = start.x * start.x + start.z * start.z - radius * radius;

	float t0, t1;
	float b24ac = b * b - 4 * a * c;
	if (b24ac < kEpsilon)
	{
		return false;
	}

	float sqb24ac = std::sqrtf(b24ac);

	t0 = (-b + sqb24ac) / (2 * a);
	t1 = (-b - sqb24ac) / (2 * a);

	if (t0 > t1)
	{
		float tmp = t0;
		t0 = t1;
		t1 = tmp;
	}

	float y0 = start.y + t0 * dir.y;
	float y1 = start.y + t1 * dir.y;

	if (y0 < bottom)
	{
		if (y1 < bottom)
		{
			return false;
		}
		else
		{
			float th = (bottom - start.y) / dir.y;
			if (th <= kEpsilon) //保证起点在圆柱外面
			{
				return false;
			}

			Vector3D hit_pos = start + dir * th;
			if (sr)
			{
				sr->hit_point = Point3D(hit_pos.x, hit_pos.y, hit_pos.z);
				sr->normal = Normal(0, -1, 0);
			}
			return true;
		}
	}
	else if (y0 >= bottom && y0 <= top)
	{
		if (t0 <= kEpsilon) //保证起点在圆柱外面
		{
			return false;
		}

		Vector3D hit_pos = start + dir * t0;
		if (sr)
		{
			sr->hit_point = Point3D(hit_pos.x, hit_pos.y, hit_pos.z);
			sr->normal = Normal(hit_pos.x, 0, hit_pos.z);
			sr->normal.normalize();
		}
		return true;
	}
	else
	{
		if (y1 > top)
		{
			return false;
		}
		
		float th = (top - start.y) / dir.y;
		if (th <= kEpsilon)
		{
			return false;
		}

		Vector3D hit_pos = start + dir * th;
		if (sr)
		{
			sr->hit_point = Point3D(hit_pos.x, hit_pos.y, hit_pos.z);
			sr->normal = Normal(0, 1, 0);
			sr->normal.normalize();
		}
		return true;
	}

	return false;
}

bool SolidCylinder::shadow_hit(const Ray& ray, double& tmin) const
{
	return hit(ray, tmin);
}
