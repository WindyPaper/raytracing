// This builds a scene that consists of 35 shaded spheres and a plane.
// The objects are illuminated by a directional light and rendered with
// ambient and diffuse shading.
// Perspective viewing is used with a pinhole camera.
// Jittered sampling for antialiasing is hardwired into the PinHole::render_scene function.
// There are no sampler classes in this project.
// These are in the Chapter 5 download file.
// The spheres are the same as those in the Chapter 14 page one image. 

#include "World.h"
#include "Plane.h"
#include "Directional.h"
#include "Pinhole.h"
#include "Matte.h"
#include "Point3D.h"
#include "RayCast.h"
#include "MultiJittered.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "ThinLens.h"
#include "FishEye.h"
#include "SolidCylinder.h"
#include "ConvexPartCylinder.h"
#include "Box.h"
#include "PointLight.h"
#include "Phong.h"
#include "AmbientOccluder.h"
#include "ConvexPartCylinder.h"
#include "AreaLighting.h"
#include "Rectangle.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "EnvironmentLight.h"
#include "ConcaveSphere.h"
#include "Instance.h"
#include "Grid.h"
#include "Mesh.h"
#include "Reflective.h"
#include "Whitted.h"
#include "GlossyReflector.h"
#include "PathTrace.h"
#include "Transparent.h"

void 												
World::build(void) {

	int num_samples = 16;

	vp.set_hres(600);
	vp.set_vres(400);
	vp.set_samples(num_samples);
	//vp.set_max_depth(0);				// for Figure 26.6(a)
		//vp.set_max_depth(1);				// for Figure 26.6(b)
		vp.set_max_depth(10);				// for Figure 26.6(c)

	tracer_ptr = new PathTrace(this);

	//Ambient* ambient_ptr = new Ambient;
	//ambient_ptr->scale_radiance(1.0);
	//set_ambient_light(ambient_ptr);


	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(100, 45, 100);
	pinhole_ptr->set_lookat(-10, 40, 0);
	pinhole_ptr->set_view_distance(400);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);


	Emissive* emissive_ptr = new Emissive;
	emissive_ptr->set_ce(white);
	emissive_ptr->scale_radiance(1.5);


	ConcaveSphere* sphere_ptr = new ConcaveSphere;		// centered on the origin
	sphere_ptr->set_radius(1000000.0);
	sphere_ptr->set_shadows(false);
	sphere_ptr->set_material(emissive_ptr);
	add_object(sphere_ptr);


	float ka = 0.2;  // common ambient reflection coefficient	


	// large sphere

	Matte* matte_ptr1 = new Matte;
	matte_ptr1->set_ka(ka);
	matte_ptr1->set_kd(0.60);
	matte_ptr1->set_cd(white);
	matte_ptr1->set_sampler(new MultiJittered(num_samples));

	Sphere* sphere_ptr1 = new Sphere(Point3D(38, 20, -24), 20);
	sphere_ptr1->set_material(matte_ptr1);
	add_object(sphere_ptr1);


	// small sphere

	Matte* matte_ptr2 = new Matte;
	matte_ptr2->set_ka(ka);
	matte_ptr2->set_kd(0.5);
	matte_ptr2->set_cd(0.85);				// gray
	matte_ptr2->set_sampler(new MultiJittered(num_samples));

	Sphere* sphere_ptr2 = new Sphere(Point3D(34, 12, 13), 12);
	sphere_ptr2->set_material(matte_ptr2);
	add_object(sphere_ptr2);


	// medium sphere

	Matte* matte_ptr3 = new Matte;
	matte_ptr3->set_ka(ka);
	matte_ptr3->set_kd(0.75);
	matte_ptr3->set_cd(0.73, 0.22, 0.0);    // orange
	matte_ptr3->set_sampler(new MultiJittered(num_samples));
	Transparent *glass_ptr = new Transparent;
	glass_ptr->set_ks(0.5);
	glass_ptr->set_exp(2000);
	glass_ptr->set_ior(1.5);
	glass_ptr->set_kr(0.1);
	glass_ptr->set_kt(0.9);

	Sphere* sphere_ptr3 = new Sphere(Point3D(-7, 15, 42), 16);
	sphere_ptr3->set_material(glass_ptr);
	add_object(sphere_ptr3);


	// cylinder

	Matte* matte_ptr4 = new Matte;
	matte_ptr4->set_ka(ka);
	matte_ptr4->set_kd(0.75);
	matte_ptr4->set_cd(0.60);				// gray
	matte_ptr4->set_sampler(new MultiJittered(num_samples));

	double bottom = 0.0;
	double top = 85.0;
	double radius = 22.0;
	ConvexPartCylinder* cylinder_ptr = new ConvexPartCylinder(bottom, top, radius, 0, 360);
	cylinder_ptr->set_material(matte_ptr4);
	add_object(cylinder_ptr);


	// box

	Matte* matte_ptr5 = new Matte;
	matte_ptr5->set_ka(ka);
	matte_ptr5->set_kd(0.75);
	matte_ptr5->set_cd(0.95);				// gray
	matte_ptr5->set_sampler(new MultiJittered(num_samples));

	Box* box_ptr = new Box(Point3D(-55, 0, -110), Point3D(-25, 60, 65));  // thicker
	box_ptr->set_material(matte_ptr5);
	add_object(box_ptr);


	// ground plane

	//MultiJittered* sampler_ptr6 = new MultiJittered(num_samples);

	Matte* matte_ptr6 = new Matte;
	matte_ptr6->set_ka(0.15);
	matte_ptr6->set_kd(0.95);
	matte_ptr6->set_cd(0.37, 0.43, 0.08);     // olive green
	matte_ptr6->set_sampler(new MultiJittered(num_samples));

	Plane* plane_ptr = new Plane(Point3D(0, 0.01, 0), Normal(0, 1, 0));
	plane_ptr->set_material(matte_ptr6);
	add_object(plane_ptr);
}

