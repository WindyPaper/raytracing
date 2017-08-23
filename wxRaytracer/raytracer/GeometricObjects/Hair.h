#ifndef _HAIR_H_
#define _HAIR_H_

#pragma once

#include "GeometricObject.h"
#include "cyHairFile.h"

class BezierCurve
{
public:
	BezierCurve(const Point3D p[4]);
	~BezierCurve();

	void set_control_point(const Point3D p[4]);
	void set_control_point(const Point3D &p0, const Point3D &p1, const Point3D &p2, const Point3D &p3);

	bool hit(const Ray &ray, double& t, ShadeRec *sr);
	bool recursive_hit(const BezierCurve &c, const Ray &ray, double &t, ShadeRec *sr, float u0, float u1, int depth, const Matrix& object_to_ray_mat);

	Point3D project_point(const Point3D &o, const Vector3D &lx, const Vector3D &ly, const Vector3D &lz, const Point3D &q) const;
	Point3D project_point(const Point3D &p, const Ray &ray) const;
	BezierCurve project(const Ray &ray, Matrix &in_mat) const;
	void split_bezier(BezierCurve &c0, BezierCurve &c1) const;
	Point3D eval(float v) const;
	Vector3D dp(float v) const;
	BBox get_bbox() const;
	int max_depth() const;

protected:
	void get_project_coordinate(const Ray &ray, Vector3D &lx, Vector3D &ly, Vector3D &lz) const;
	Matrix get_project_matrix(const Ray &ray) const;
	Matrix get_project_matrix(const Ray &ray, const Vector3D &up) const;
	Point3D blossom_bezier(float u0, float u1, float u2) const;

private:
	Point3D p[4];
	float width;
};

class Hair : public GeometricObject
{
public:
	Hair();
	virtual ~Hair();

	virtual Hair*
		clone(void) const;

	virtual bool
		hit(const Ray& ray, double& t, ShadeRec *sr = 0) const;

	virtual bool
		shadow_hit(const Ray& ray, double& tmin) const;

	void load_hair(const char *filename);

private:
	cy::HairFile hair_file;
};

#endif


