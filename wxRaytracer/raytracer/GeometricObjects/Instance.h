#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "GeometricObject.h"
#include "Matrix.h"

class Instance : public GeometricObject
{
public:
	Instance();
	Instance(GeometricObject *obj);
	~Instance();

	virtual Instance *clone() const;

	void
		set_object(GeometricObject* obj_ptr);

	virtual void
		compute_bounding_box(void);

	virtual BBox
		get_bounding_box(void);

	virtual Material*
		get_material(void) const;

	virtual void
		set_material(Material* materialPtr);

	virtual bool
		hit(const Ray& ray, double& t, ShadeRec *sr = 0) const;

	void set_bounding_box();


	// affine tranformation functions

	void
		translate(const Vector3D& trans);

	void
		translate(const double dx, const double dy, const double dz);

	void
		scale(const Vector3D& s);

	void
		scale(const double a, const double b, const double c);

	virtual void
		rotate_x(const double r);

	virtual void
		rotate_y(const double r);

	virtual void
		rotate_z(const double r);

	void
		shear(const Matrix& m);

	virtual bool
		shadow_hit(const Ray& ray, double& tmin) const;

	void
		cast_shadow(bool cast_shadows);

private:
	GeometricObject *object_ptr;
	Matrix inv_matrix;
	static Matrix forward_matrix;
	BBox bbox;
	bool shadows;
};

#endif