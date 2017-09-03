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
	//Point3D p[4] = { Point3D(0, 5, 10), Point3D(5, 5, 5), Point3D(5, 5, 0), Point3D(0, 5, -10) };
	//Point3D p[4] = { Point3D(0, 5, -10), Point3D(0, 5, -4), Point3D(0, 5, 3), Point3D(0, 5, 10) };
	Point3D p[4] = { Point3D(0, 5, -10), Point3D(5, 5, -4), Point3D(-5, 5, 3), Point3D(0, 5, 10) };

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
	width(3.0f)
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
	Matrix object_to_ray_mat;
	BezierCurve project_c = project(ray, object_to_ray_mat);
	//BezierCurve project_c = *this;

	int max_d = max_depth();
	return recursive_hit(project_c, ray, t, sr, 0.0f, 1.0f, max_d, object_to_ray_mat);
}

bool BezierCurve::recursive_hit(const BezierCurve &c, const Ray &ray, double &t, ShadeRec *sr, float u0, float u1, int depth, const Matrix& object_to_ray_mat)
{
	BBox box = c.get_bbox();
	//printf("min x = %f, max x = %f\n", box.x0, box.x1);

	if (
		box.x0 >= width || box.x1 <= -width
		|| box.y0 >= width || box.y1 <= -width)
	{
		return false;
	}
	else if (depth <= 0)
	{
		// Test sample point against tangent perpendicular at curve start
		float edge =
			(c.p[1].y - c.p[0].y) * -c.p[0].y + c.p[0].x * (c.p[0].x - c.p[1].x);
		if (edge < 0) return false;

		// Test sample point against tangent perpendicular at curve end
		edge = (c.p[2].y - c.p[3].y) * -c.p[3].y + c.p[3].x * (c.p[3].x - c.p[2].x);
		if (edge < 0) return false;

		//compute w on the line segment
		Vector3D dir = c.p[3] - c.p[0];
		float w = dir.x * dir.x + dir.y * dir.y;
		if (w < kEpsilon)
		{
			return false;
		}

		w = -(c.p[0].x * dir.x + c.p[0].y * dir.y) / w;
		//printf("w = %f\n", w);
		w = clamp(w, 0.0f, 1.0f);

		Point3D cv = c.eval(w);
		float cv_dist2 = cv.x * cv.x + cv.y * cv.y;
		if (cv_dist2 > (width *width) ||
			cv.z < kEpsilon)
		{
			return false;
		}

		float gu = lerp(u0, u1, w); // global value
		//Point3D gcv = this->eval(gu);

		if (t < cv.z)
		{
			return false;
		}

		Vector3D dpcdw = c.dp(w);

		// Compute $v$ coordinate of curve intersection point
		float cv_dist = std::sqrt(cv_dist2);
		float edgeFunc = dpcdw.x * -cv.y + cv.x * dpcdw.y;
		float v = (edgeFunc > 0) ? 0.5f + cv_dist / width
			: 0.5f - cv_dist / width;

		Vector3D dpdu, dpdv;
		dpdu = this->dp(gu);

		Vector3D dpdu_plane = object_to_ray_mat * dpdu;
		Vector3D dpdv_plane = Vector3D(-dpdu_plane.y, dpdu_plane.x, 0);
		dpdv_plane.normalize();
		dpdv_plane *= width;

		float theta = lerp(-90, 90, v);
		Matrix rotate_mat = rotate_to_matrix(theta, dpdu_plane);
		dpdv_plane = rotate_mat * dpdv_plane;

		//dpdv plane to global
		dpdv = object_to_ray_mat.inverse() * dpdv_plane;

		//dpdu.normalize();
		//dpdv.normalize();
		//float test_dot = dpdu * dpdv;

		t = cv.z;
		if (t < 0.0f)
		{
			printf("Error!, t < 0 in Hair Intersection!\n");
		}
		sr->normal = dpdu ^ dpdv;
		sr->dpdu = normalize(dpdu);
		sr->dpdv = normalize(dpdv);
		//sr->normal *= -1;
		//sr->normal = dpdv ^ dpdu;
		sr->normal.normalize();
		//sr->normal = -sr->normal;
		//sr->normal = Vector3D(1.0f, -1.0f, 0.0f);
		//sr->hit_point = gcv;
		return true;
	}
	else //depth > 0
	{
		depth--;
		
		float um = (u0 + u1) / 2;

		Point3D default_v[4];
		BezierCurve cl(default_v), cr(default_v);
		c.split_bezier(cl, cr);

		return recursive_hit(cl, ray, t, sr, u0, um, depth, object_to_ray_mat) ||
			recursive_hit(cr, ray, t, sr, um, u1, depth, object_to_ray_mat);
	}
}

Point3D BezierCurve::project_point(const Point3D &o, const Vector3D &lx, const Vector3D &ly, const Vector3D &lz, const Point3D &q) const
{
	Vector3D p(q - o);
	return Point3D(lx * p, ly * p, lz * p);
}

Point3D BezierCurve::project_point(const Point3D &p, const Ray &ray) const
{
	Vector3D lx, ly, lz;
	get_project_coordinate(ray, lx, ly, lz);

	return project_point(ray.o, lx, ly, lz, p);	
}

BezierCurve BezierCurve::project(const Ray &ray, Matrix &in_mat) const
{	
	//Matrix mat = get_project_matrix(ray);
	Vector3D up = ray.d ^ Vector3D(p[3] - p[0]);
	Matrix mat = get_project_matrix(ray, up);
	in_mat = mat;

	Point3D t_p[4];
	for (int i = 0; i < 4; ++i)
	{
		t_p[i] = mat * p[i];
	}


	//From tungsten
	/*Vector3D lx, ly, lz;
	Point3D t_p[4];
	get_project_coordinate(ray, lx, ly, lz);

	for (int i = 0; i < 4; ++i)
	{
		t_p[i] = project_point(ray.o, lx, ly, lz, p[i]);
	}*/

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

	Vector3D tangent = (1/3.0f) * (b[1] - b[0]);
	//tangent.normalize();
	return tangent;
}

BBox BezierCurve::get_bbox() const
{
	BBox b1(p[0] - kEpsilon, p[1] + kEpsilon);
	BBox b2(p[2] - kEpsilon, p[3] + kEpsilon);
	
	return union_box(b1, b2);
	//return BBox(-2, 2, -2, 2, -10, 10);
	//return BBox();
}

int BezierCurve::max_depth() const
{
	/*int L0 = std::max(
		std::max(std::abs(p[0].x - p[1].x * 2 + p[2].x), std::abs(p[0].y - p[1].y * 2 + p[2].y)),
		std::max(std::abs(p[1].x - p[2].x * 2 + p[3].x), std::abs(p[1].y - p[2].y * 2 + p[3].y))
	);

	float epsilon = 1 / 20.0f * width;

	int n = 4;
	float r0 = std::log(std::sqrt(2.0) * n * (n - 1) * L0 / (8 * epsilon)) / std::log(4.0);

	r0 = clamp(r0, 5, 15);*/

	float L0 = 0;
	for (int i = 0; i < 2; ++i)
		L0 = std::max(
			L0, std::max(
				std::max(std::fabsf(p[i].x - 2 * p[i + 1].x + p[i + 2].x),
					std::fabsf(p[i].y - 2 * p[i + 1].y + p[i + 2].y)),
				std::fabsf(p[i].z - 2 * p[i + 1].z + p[i + 2].z)));

	auto float_to_bits = [](float f) -> unsigned int
	{
		unsigned int ui;
		memcpy(&ui, &f, sizeof(float));
		return ui;
	};

	float eps = (width * 2) * .05f;  // width / 20
	auto Log2 = [=](float v) -> int {
		if (v < 1) return 0;
		unsigned int bits = float_to_bits(v);
		// https://graphics.stanford.edu/~seander/bithacks.html#IntegerLog
		// (With an additional add so get round-to-nearest rather than
		// round down.)
		return (bits >> 23) - 127 + (bits & (1 << 22) ? 1 : 0);
	};
	// Compute log base 4 by dividing log2 in half.
	int r0 = Log2(1.41421356237f * 6.f * L0 / (8.f * eps)) / 2;
	int maxDepth = clamp(r0, 0, 10);
	return maxDepth;
	//return (int)r0;
}

void BezierCurve::get_project_coordinate(const Ray &ray, Vector3D &lx, Vector3D &ly, Vector3D &lz) const
{
	lz = ray.d;
	float d = std::sqrt(lz.x*lz.x + lz.z*lz.z);
	if (d == 0.0f) {
		lx = Vector3D(1.0f, 0.0f, 0.0f);
		ly = Vector3D(0.0f, 0.0f, -lz.y);
	}
	else {
		lx = Vector3D(lz.z / d, 0.0f, -lz.x / d);
		ly = Vector3D(lx.z*lz.y, d, -lz.y*lx.x);
	}
}

Matrix BezierCurve::get_project_matrix(const Ray &ray) const
{
	Matrix ray_pos_mat = Matrix();
	ray_pos_mat.m[0][3] = -ray.o.x;
	ray_pos_mat.m[1][3] = -ray.o.y;
	ray_pos_mat.m[2][3] = -ray.o.z;

	Matrix ray_rotate_mat = Matrix();
	float d = std::sqrt(ray.d.x * ray.d.x + ray.d.z * ray.d.z);

	bool is_needed_rotate_x_90 = false;
	if (d < kEpsilon)
	{
		printf("Error! Bezider curve hit d == 0!!\n");
		is_needed_rotate_x_90 = true;
	}
	ray_rotate_mat.m[0][0] = ray.d.z / d;
	ray_rotate_mat.m[1][1] = d;
	ray_rotate_mat.m[2][2] = ray.d.z;

	ray_rotate_mat.m[0][2] = -ray.d.x / d;

	ray_rotate_mat.m[1][0] = -ray.d.x * ray.d.y / d;
	ray_rotate_mat.m[1][2] = -ray.d.y * ray.d.z / d;

	ray_rotate_mat.m[2][0] = ray.d.x;
	ray_rotate_mat.m[2][1] = ray.d.y;

	if (is_needed_rotate_x_90)
	{
		ray_rotate_mat = rotate_to_matrix(90, Vector3D(1.0f, 0.0f, 0.0f)) * ray_rotate_mat;
	}

	Matrix mat = ray_rotate_mat * ray_pos_mat;

	return mat;
}

Matrix BezierCurve::get_project_matrix(const Ray &ray, const Vector3D &up) const
{
	return look_at_mat(ray, up);
}

Point3D BezierCurve::blossom_bezier(float u0, float u1, float u2) const
{
	Point3D a[3] = { lerp(p[0], p[1], u0), lerp(p[1], p[2], u0), lerp(p[2], p[3], u0) };
	Point3D b[2] = { lerp(a[0], a[1], u1), lerp(a[1], a[2], u1) };

	return lerp(b[0], b[1], u2);
}
