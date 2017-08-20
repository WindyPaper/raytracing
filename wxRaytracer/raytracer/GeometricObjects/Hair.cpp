#include "Hair.h"
#include "Util.h"
#include "Maths.h"

Hair::Hair()
{
	hair_file.Initialize();
}

Hair::~Hair()
{

}

Hair* Hair::clone(void) const
{
	return NULL;
}

bool Hair::hit(const Ray& ray, double& t, ShadeRec *sr /*= 0*/) const
{
	//Point3D p[4] = { Point3D(0, 5, 10), Point3D(0, 5, 5), Point3D(0, 5, 0), Point3D(0, 5, -10) };
	//Point3D p[4] = { Point3D(0, 5, 20), Point3D(0, 5, 5), Point3D(0, 5, 0), Point3D(0, 5, -10) };
	Point3D p[4] = { Point3D(0, 5, -10), Point3D(0, 5, 0), Point3D(0, 5, 5), Point3D(0, 5, 10) };

	BezierCurve test_c(p);
	
	return test_c.hit(ray, t, sr);
}

bool Hair::shadow_hit(const Ray& ray, double& tmin) const
{
	return false;
}

void Hair::load_hair(const char *filename)
{
	int ret = hair_file.LoadFromFile(filename);

	switch (ret) {
	case CY_HAIR_FILE_ERROR_CANT_OPEN_FILE:
		printf("Error: Cannot open hair file!\n");
		return;
	case CY_HAIR_FILE_ERROR_CANT_READ_HEADER:
		printf("Error: Cannot read hair file header!\n");
		return;
	case CY_HAIR_FILE_ERROR_WRONG_SIGNATURE:
		printf("Error: File has wrong signature!\n");
		return;
	case CY_HAIR_FILE_ERROR_READING_SEGMENTS:
		printf("Error: Cannot read hair segments!\n");
		return;
	case CY_HAIR_FILE_ERROR_READING_POINTS:
		printf("Error: Cannot read hair points!\n");
		return;
	case CY_HAIR_FILE_ERROR_READING_COLORS:
		printf("Error: Cannot read hair colors!\n");
		return;
	case CY_HAIR_FILE_ERROR_READING_THICKNESS:
		printf("Error: Cannot read hair thickness!\n");
		return;
	case CY_HAIR_FILE_ERROR_READING_TRANSPARENCY:
		printf("Error: Cannot read hair transparency!\n");
		return;
	default:
		printf("Hair file \"%s\" loaded.\n", filename);
	}

	int hairCount = hair_file.GetHeader().hair_count;
	int pointCount = hair_file.GetHeader().point_count;
	unsigned short *segments = hair_file.GetSegmentsArray();
	printf("Number of hair strands = %d\n", hairCount);
	printf("Number of hair points = %d\n", pointCount);


}

BezierCurve::BezierCurve(const Point3D p[4]) :
	width(2.0f)
{
	for (int i = 0; i < 4; ++i)
	{
		this->p[i] = p[i];
	}
}

BezierCurve::~BezierCurve()
{

}

void BezierCurve::set_control_point(const Point3D p[4])
{
	for (int i = 0; i < 4; ++i)
	{
		this->p[i] = p[i];
	}
}

void BezierCurve::set_control_point(const Point3D &p0, const Point3D &p1, const Point3D &p2, const Point3D &p3)
{
	p[0] = p0;
	p[1] = p1;
	p[2] = p2;
	p[3] = p3;
}

bool BezierCurve::hit(const Ray &ray, double& t, ShadeRec *sr)
{
	BezierCurve project_c = project(ray);

	int max_d = max_depth();
	return recursive_hit(project_c, ray, t, sr, 0.0f, 1.0f, max_d);
}

bool BezierCurve::recursive_hit(const BezierCurve &c, const Ray &ray, double &t, ShadeRec *sr, float u0, float u1, int depth)
{
	BBox box = c.get_bbox();

	if (box.z0 > t || box.z1 < kEpsilon
		|| box.x0 >= width || box.x1 <= -width
		|| box.y0 >= width || box.y1 <= -width)
	{
		return false;
	}
	else if (depth <= 0)
	{
		Vector3D dir = c.p[3] - c.p[0];
		dir.normalize();

		Vector3D dp0 = c.dp(0.0f);
		if (dir * dp0 < 0.0f)
		{
			dp0 *= -1;
		}
		if (dp0 * (-c.p[0]) < 0)
		{
			return false;
		}
		Vector3D dpn = c.dp(1.0f);
		if (dir * dpn < 0.0f)
		{
			dpn *= -1;
		}
		if (dpn * c.p[3] < 0)
		{
			return false;
		}

		//compute w on the line segment
		float w = dir.x * dir.x + dir.y * dir.y;
		if (w < kEpsilon)
		{
			return false;
		}

		w = -(c.p[0].x * dir.x + c.p[0].y * dir.y) / w;
		w = clamp(w, 0.0f, 1.0f);

		Point3D cv = c.eval(w);
		if ((cv.x * cv.x + cv.y * cv.y) > (width *width) ||
			cv.z < kEpsilon)
		{
			return false;
		}

		float v = lerp(u0, u1, w); // global value
		Point3D gcv = this->eval(v);

		if (t < gcv.z)
		{
			return false;
		}

		t = gcv.z;
		sr->normal = Vector3D(0.0f, 0.0f, 1.0f);
		sr->hit_point = gcv;
		return true;
	}
	else //depth > 0
	{
		depth--;
		
		float um = (u0 + u1) / 2;

		Point3D default_v[4];
		BezierCurve cl(default_v), cr(default_v);
		c.split_bezier(cl, cr);

		return recursive_hit(cl, ray, t, sr, u0, um, depth) ||
			recursive_hit(cr, ray, t, sr, um, u1, depth);
	}
}

Point3D BezierCurve::project_point(const Point3D &o, const Vector3D &lx, const Vector3D &ly, const Vector3D &lz, const Point3D &q) const
{
	Vector3D p(q - o);
	return Point3D(lx * p, ly * p, lz * p);
}

BezierCurve BezierCurve::project(const Ray &ray) const
{	
	/*Matrix ray_pos_mat = Matrix();
	ray_pos_mat.m[0][3] = -ray.o.x;
	ray_pos_mat.m[1][3] = -ray.o.y;
	ray_pos_mat.m[2][3] = -ray.o.z;

	Matrix ray_rotate_mat = Matrix();
	float d = std::sqrt(ray.d.x * ray.d.x + ray.d.z * ray.d.z);

	if (d < kEpsilon)
	{
		printf("Error! Bezider curve hit d == 0!!\n");
	}
	ray_rotate_mat.m[0][0] = ray.d.z / d;
	ray_rotate_mat.m[1][1] = d;
	ray_rotate_mat.m[2][2] = ray.d.z;

	ray_rotate_mat.m[0][2] = -ray.d.x / d;

	ray_rotate_mat.m[1][0] = -ray.d.x * ray.d.y / d;
	ray_rotate_mat.m[1][2] = -ray.d.y * ray.d.z / d;

	ray_rotate_mat.m[2][0] = ray.d.x;
	ray_rotate_mat.m[2][1] = ray.d.y;

	Matrix mat = ray_rotate_mat * ray_pos_mat;

	Point3D t_p[4];
	for (int i = 0; i < 4; ++i)
	{
		t_p[i] = mat * p[i];
	}*/


	//From tungsten
	Point3D t_p[4];
	Vector3D lz(ray.d);
	float d = std::sqrt(lz.x*lz.x + lz.z*lz.z);
	Vector3D lx, ly;
	if (d == 0.0f) {
		lx = Vector3D(1.0f, 0.0f, 0.0f);
		ly = Vector3D(0.0f, 0.0f, -lz.y);
	}
	else {
		lx = Vector3D(lz.z / d, 0.0f, -lz.x / d);
		ly = Vector3D(lx.z*lz.y, d, -lz.y*lx.x);
	}

	for (int i = 0; i < 4; ++i)
	{
		t_p[i] = project_point(ray.o, lx, ly, lz, p[i]);
	}

	return BezierCurve(t_p);
}

void BezierCurve::split_bezier(BezierCurve &c0, BezierCurve &c1) const
{
	Point3D split_p[7];

	split_p[0] = p[0];
	split_p[1] = (p[0] + p[1]) / 2;
	split_p[2] = (p[0] + 2 * p[1] + p[2]) / 4;
	split_p[3] = (p[0] + 3 * p[1] + 3 * p[2] + p[3]) / 8;
	split_p[4] = (p[1] + 2 * p[2] + p[3]) / 4;
	split_p[5] = (p[2] + p[3]) / 2;
	split_p[6] = p[3];

	c0.set_control_point(split_p[0], split_p[1], split_p[2], split_p[3]);
	c1.set_control_point(split_p[3], split_p[4], split_p[5], split_p[6]);
}

Point3D BezierCurve::eval(float v) const
{
	Point3D a[3] = { lerp(p[0], p[1], v), lerp(p[1], p[2], v), lerp(p[2], p[3], v) };
	Point3D b[2] = { lerp(a[0], a[1], v), lerp(a[1], a[2], v) };

	return lerp(b[0], b[1], v);
}

Vector3D BezierCurve::dp(float v) const
{
	Point3D a[3] = { lerp(p[0], p[1], v), lerp(p[1], p[2], v), lerp(p[2], p[3], v) };
	Point3D b[2] = { lerp(a[0], a[1], v), lerp(a[1], a[2], v) };

	Vector3D tangent = b[1] - b[0];
	tangent.normalize();
	return tangent;
}

BBox BezierCurve::get_bbox() const
{
	BBox b1(p[0], p[1]);
	BBox b2(p[2], p[3]);
	
	return union_box(b1, b2);
}

int BezierCurve::max_depth() const
{
	int L0 = std::max(
		std::max(std::abs(p[0].x - p[1].x * 2 + p[2].x), std::abs(p[0].y - p[1].y * 2 + p[2].y)),
		std::max(std::abs(p[1].x - p[2].x * 2 + p[3].x), std::abs(p[1].y - p[2].y * 2 + p[3].y))
	);

	float epsilon = 1 / 20.0f * width;

	int n = 4;
	float r0 = std::log(std::sqrt(2.0) * n * (n - 1) * L0 / (8 * epsilon)) / std::log(4.0);

	r0 = clamp(r0, 3, 15);
	
	return (int)r0;
}

Point3D BezierCurve::blossom_bezier(float u0, float u1, float u2) const
{
	Point3D a[3] = { lerp(p[0], p[1], u0), lerp(p[1], p[2], u0), lerp(p[2], p[3], u0) };
	Point3D b[2] = { lerp(a[0], a[1], u1), lerp(a[1], a[2], u1) };

	return lerp(b[0], b[1], u2);
}
