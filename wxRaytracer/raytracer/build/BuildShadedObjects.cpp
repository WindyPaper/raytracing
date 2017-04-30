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
#include "Dielectric.h"
#include "ImageTexture.h"
#include "SV_Matte.h"
#include "RectangularMap.h"
#include "Instance.h"
#include "SolidCylinder.h"
#include "Checker3D.h"
#include "TInstance.h"
#include "Rectangle.h"

void 												
World::build(void) {

	int num_samples = 16;

	int sample_type = 5;

	switch (sample_type)
	{
	case  0:
	{
		vp.set_hres(600);
		vp.set_vres(400);
		vp.set_samples(num_samples);
		//vp.set_max_depth(0);				// for Figure 26.6(a)
		//vp.set_max_depth(1);				// for Figure 26.6(b)
		vp.set_max_depth(10);				// for Figure 26.6(c)

		background_color = white;

		tracer_ptr = new Whitted(this);

		Ambient* ambient_ptr = new Ambient;
		ambient_ptr->scale_radiance(0.25);
		set_ambient_light(ambient_ptr);

		// point light 

		PointLight* light_ptr1 = new PointLight;
		light_ptr1->set_position(Point3D(40, 50, 0));
		light_ptr1->set_radiance(4.5);
		light_ptr1->set_cast_shadows(true);
		add_light(light_ptr1);


		// point light 

		PointLight* light_ptr2 = new PointLight;
		light_ptr2->set_position(Point3D(-10, 20, 10));
		light_ptr2->set_radiance(4.5);
		light_ptr2->set_cast_shadows(true);
		add_light(light_ptr2);


		// directional light 

		Directional* light_ptr3 = new Directional;
		light_ptr3->set_direction(-1, 0, 0);
		light_ptr3->scale_radiance(4.5);
		light_ptr3->set_cast_shadows(true);
		add_light(light_ptr3);

		Pinhole* pinhole_ptr = new Pinhole;
		pinhole_ptr->set_eye(100, 45, 100);
		pinhole_ptr->set_lookat(-10, 40, 0);
		pinhole_ptr->set_view_distance(400);
		pinhole_ptr->compute_uvw();
		set_camera(pinhole_ptr);


		Emissive* emissive_ptr = new Emissive;
		emissive_ptr->set_ce(white);
		emissive_ptr->scale_radiance(1.5);

		Dielectric* dielectric_ptr = new Dielectric;
		dielectric_ptr->set_eta_in(1.5);		// glass
		dielectric_ptr->set_eta_out(1.0);		// air
		dielectric_ptr->set_cf_in(white);
		dielectric_ptr->set_cf_out(white);


		//ConcaveSphere* sphere_ptr = new ConcaveSphere;		// centered on the origin
		//sphere_ptr->set_radius(1000000.0);
		//sphere_ptr->set_shadows(false);
		//sphere_ptr->set_material(emissive_ptr);
		//add_object(sphere_ptr);


		float ka = 0.2;  // common ambient reflection coefficient	


						 // large sphere

						 /*Matte* matte_ptr1 = new Matte;
						 matte_ptr1->set_ka(ka);
						 matte_ptr1->set_kd(0.60);
						 matte_ptr1->set_cd(white);
						 matte_ptr1->set_sampler(new MultiJittered(num_samples));*/

		Sphere* sphere_ptr1 = new Sphere(Point3D(38, 20, -24), 20);
		sphere_ptr1->set_material(dielectric_ptr);
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
		matte_ptr4->set_cd(1);				// gray
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
		break;
	case 1:
	{
		//int num_samples = 16;

		vp.set_hres(600);
		vp.set_vres(400);
		vp.set_samples(num_samples);
		vp.set_max_depth(0);

		background_color = RGBColor(0.0);

		tracer_ptr = new PathTrace(this);

		Pinhole* pinhole_ptr = new Pinhole;
		pinhole_ptr->set_eye(5, 1.5, 8);
		pinhole_ptr->set_lookat(0.25, 0.0, 0.0);
		pinhole_ptr->set_view_distance(1000.0);
		pinhole_ptr->compute_uvw();
		set_camera(pinhole_ptr);

		Directional* light_ptr = new Directional;
		light_ptr->set_direction(-15, 20, 25);
		light_ptr->scale_radiance(2.5);
		light_ptr->set_cast_shadows(true);
		add_light(light_ptr);

		// image:

		Image* image_ptr = new Image;
		image_ptr->read_ppm_file("TextureFiles/ppm/Lightlace.ppm");

		// mapping:

		//SquareMap* map_ptr = new SquareMap;   
		RectangularMap *map_ptr = new RectangularMap;	//I can't find SquareMap class in original code, Here we may try to use another similar class to simulate this effect 

														// image texture:

		ImageTexture* texture_ptr = new ImageTexture(image_ptr);
		texture_ptr->set_mapping(map_ptr);

		// spatially varying material:

		SV_Matte* sv_matte_ptr = new SV_Matte;
		sv_matte_ptr->set_ka(0.40);
		sv_matte_ptr->set_kd(0.95);
		sv_matte_ptr->set_cd(texture_ptr);
		MultiJittered *p_sv_multi_jitter = new MultiJittered(num_samples);
		p_sv_multi_jitter->map_samples_to_hemisphere(1.0);
		sv_matte_ptr->set_sampler(p_sv_multi_jitter);

		// generic rectangle:

		TRectangle* rectangle_ptr = new TRectangle;
		rectangle_ptr->set_material(sv_matte_ptr);

		// transformed rectangle:

		Instance* lightlace_ptr = new Instance(rectangle_ptr);
		lightlace_ptr->scale(1.0, 1.0, 1.5);
		lightlace_ptr->rotate_z(90);
		lightlace_ptr->rotate_y(90);
		add_object(lightlace_ptr);

		// ground plane

		Matte* matte_ptr1 = new Matte;
		matte_ptr1->set_ka(0.25);
		matte_ptr1->set_kd(0.5);
		matte_ptr1->set_cd(1.0);
		MultiJittered *p_matte_jitter = new MultiJittered(num_samples);
		p_matte_jitter->map_samples_to_hemisphere(1.0);
		matte_ptr1->set_sampler(p_matte_jitter);

		Plane* plane_ptr = new Plane(Point3D(0.0, -1.0, 0.0), Normal(0.0, 1.0, 0.0));
		plane_ptr->set_material(matte_ptr1);
		add_object(plane_ptr);
	}
		break;
	case 2:
	{
		int num_samples = 16;

		vp.set_hres(600);
		vp.set_vres(400);
		vp.set_samples(num_samples);

		tracer_ptr = new RayCast(this);
		background_color = RGBColor(0.5);

		Pinhole* pinhole_ptr = new Pinhole;
		pinhole_ptr->set_eye(0, 0, 100);
		pinhole_ptr->set_lookat(0.0);
		pinhole_ptr->set_view_distance(7500.0);
		pinhole_ptr->compute_uvw();
		set_camera(pinhole_ptr);

		Directional* light_ptr = new Directional;
		light_ptr->set_direction(0, 0, 1);
		light_ptr->scale_radiance(3.0);
		add_light(light_ptr);

		Checker3D* checker_ptr = new Checker3D;
		checker_ptr->set_size(0.3);
		checker_ptr->set_color1(white);
		checker_ptr->set_color2(black);

		TInstance* scaled_checker_ptr = new TInstance(checker_ptr);
		scaled_checker_ptr->scale(2, 1, 1);

		SV_Matte* sv_matte_ptr = new SV_Matte;
		sv_matte_ptr->set_ka(0.8);
		sv_matte_ptr->set_kd(0.4);
		sv_matte_ptr->set_cd(scaled_checker_ptr);

		Box* box_ptr = new Box(Point3D(-1.0), Point3D(1.0));
		box_ptr->set_material(sv_matte_ptr);

		Instance* transformed_box_ptr = new Instance(box_ptr);
		transformed_box_ptr->scale(2, 1, 1);
		transformed_box_ptr->rotate_z(45);
		add_object(transformed_box_ptr);
	}
	break;
	case 3:
	{
		//int num_samples = 256;

		vp.set_hres(600);
		vp.set_vres(400);
		vp.set_samples(num_samples);
		vp.set_max_depth(0);

		tracer_ptr = new AreaLighting(this);

		AmbientOccluder* ambient_occluder_ptr = new AmbientOccluder;
		ambient_occluder_ptr->set_sampler(new MultiJittered(num_samples));
		ambient_occluder_ptr->set_min_amount(0.5);
		set_ambient_light(ambient_occluder_ptr);


		Pinhole* pinhole_ptr = new Pinhole;
		pinhole_ptr->set_eye(100, 45, 100);
		pinhole_ptr->set_lookat(-10, 40, 0);
		pinhole_ptr->set_view_distance(400);
		pinhole_ptr->compute_uvw();
		set_camera(pinhole_ptr);


		Emissive* emissive_ptr = new Emissive;
		emissive_ptr->set_ce(1.0, 1.0, 0.5); 	// lemon
		emissive_ptr->scale_radiance(0.9);



		float ka = 0.2;  // commom ambient reflection coefficient

						 // large sphere

		Matte* matte_ptr1 = new Matte;
		matte_ptr1->set_ka(ka);
		matte_ptr1->set_kd(0.60);
		matte_ptr1->set_cd(0.75);

		Sphere* sphere_ptr1 = new Sphere(Point3D(38, 20, -24), 20);
		sphere_ptr1->set_material(matte_ptr1);
		add_object(sphere_ptr1);


		// small sphere

		Matte* matte_ptr2 = new Matte;
		matte_ptr2->set_ka(ka);
		matte_ptr2->set_kd(0.4);
		matte_ptr2->set_cd(0.25, 1.0, 0.35);       // green

		Sphere* sphere_ptr2 = new Sphere(Point3D(34, 12, 13), 12);
		sphere_ptr2->set_material(matte_ptr2);
		add_object(sphere_ptr2);


		// medium sphere

		Matte* matte_ptr3 = new Matte;
		matte_ptr3->set_ka(ka);
		matte_ptr3->set_kd(0.5);
		matte_ptr3->set_cd(0.75);

		Sphere* sphere_ptr3 = new Sphere(Point3D(-7, 15, 42), 16);
		sphere_ptr3->set_material(matte_ptr3);
		add_object(sphere_ptr3);


		// cylinder

		Phong* phong_ptr = new Phong;
		phong_ptr->set_ka(ka);
		phong_ptr->set_kd(0.25);
		phong_ptr->set_cd(0.60);
		phong_ptr->set_ks(0.5);
		phong_ptr->set_exp(100);

		float bottom = 0.0;
		float top = 85;
		float radius = 22;
		SolidCylinder* cylinder_ptr = new SolidCylinder(bottom, top, radius);
		cylinder_ptr->set_material(phong_ptr);
		add_object(cylinder_ptr);


		// box

		Matte* matte_ptr4 = new Matte;
		matte_ptr4->set_ka(ka);
		matte_ptr4->set_kd(0.5);
		matte_ptr4->set_cd(0.95);

		Box* box_ptr = new Box(Point3D(-35, 0, -110), Point3D(-25, 60, 65));
		box_ptr->set_material(matte_ptr4);
		add_object(box_ptr);


		// ground plane 

		Matte* matte_ptr5 = new Matte;
		matte_ptr5->set_ka(0.15);
		matte_ptr5->set_kd(0.5);
		matte_ptr5->set_cd(0.7);

		Plane* plane_ptr = new Plane(Point3D(0, 0.01, 0), Normal(0, 1, 0));
		plane_ptr->set_material(matte_ptr5);
		add_object(plane_ptr);		
	}
	break;
	case 4:
	{
		//18.04a
		//int num_samples = 1;   		// for Figure 18.4(a)
									//	int num_samples = 100;   	// for Figure 18.4(b) & (c)

		vp.set_hres(600);
		vp.set_vres(400);
		vp.set_samples(num_samples);
		vp.set_max_depth(5);				// for Figure 26.6(a)
											//	vp.set_max_depth(1);				// for Figure 26.6(b)
											//	vp.set_max_depth(5);				// for Figure 26.6(c)

		tracer_ptr = new PathTrace(this);
		//tracer_ptr = new Whitted(this);
		//tracer_ptr = new AreaLighting(this);

		//Ambient* ambient_ptr = new Ambient;
		//ambient_ptr->scale_radiance(0.0);
		//set_ambient_light(ambient_ptr);


		Pinhole* pinhole_ptr = new Pinhole;
		pinhole_ptr->set_eye(100, 200, 100);
		pinhole_ptr->set_lookat(-10, 40, 0);
		pinhole_ptr->set_view_distance(400);
		pinhole_ptr->compute_uvw();
		set_camera(pinhole_ptr);


		Emissive* emissive_ptr = new Emissive;
		emissive_ptr->set_ce(white);
		emissive_ptr->scale_radiance(1.0);


		//ConcaveSphere* sphere_ptr = new ConcaveSphere;		// centered on the origin
		//sphere_ptr->set_radius(1000000.0);
		//sphere_ptr->set_shadows(false);
		//sphere_ptr->set_material(emissive_ptr);
		//add_object(sphere_ptr);

		Point3D p0 = Point3D(-300, 0, 30);
		Vector3D a = Vector3D(0.0, 0.0, -60);
		Vector3D b = Vector3D(0.0, 60, 0.0);
		Normal normal = Normal(1.0, 0.0, 0.0);
		TRectangle *pLightRectangle = new TRectangle(p0, a, b, normal);
		pLightRectangle->set_sampler(new MultiJittered(num_samples));

		Matte* matte_rectangle_ptr = new Matte;
		matte_rectangle_ptr->set_ka(0.5);
		matte_rectangle_ptr->set_kd(0.60);
		matte_rectangle_ptr->set_cd(red);
		matte_rectangle_ptr->set_sampler(new MultiJittered(num_samples));
		pLightRectangle->set_material(matte_rectangle_ptr);
		//add_object(pLightRectangle);
		
		AreaLight* ceiling_light_ptr = new AreaLight;
		ceiling_light_ptr->set_object(pLightRectangle);
		ceiling_light_ptr->set_cast_shadows(true);
		ceiling_light_ptr->set_material(emissive_ptr);
		add_light(ceiling_light_ptr);

		//Point light
		/*PointLight* light_ptr1 = new PointLight;
		light_ptr1->set_position(Point3D(1, 5, 0));
		light_ptr1->set_radiance(3.0);
		light_ptr1->set_cast_shadows(true);
		add_light(light_ptr1);*/


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

		Sphere* sphere_ptr3 = new Sphere(Point3D(-7, 15, 42), 16);
		sphere_ptr3->set_material(matte_ptr3);
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
		SolidCylinder* cylinder_ptr = new SolidCylinder(bottom, top, radius);
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

		MultiJittered* sampler_ptr6 = new MultiJittered(num_samples);

		Matte* matte_ptr6 = new Matte;
		matte_ptr6->set_ka(0.15);
		matte_ptr6->set_kd(0.95);
		matte_ptr6->set_cd(0.37, 0.43, 0.08);     // olive green
		matte_ptr6->set_sampler(new MultiJittered(num_samples));

		Plane* plane_ptr = new Plane(Point3D(0, 0.01, 0), Normal(0, 1, 0));
		plane_ptr->set_material(matte_ptr6);
		add_object(plane_ptr);
	}
	break;
	case 5: //area lighting
	{
		Sampler* sampler_ptr = new MultiJittered(num_samples);

		vp.set_hres(600);
		vp.set_vres(600);
		vp.set_sampler(sampler_ptr);
		vp.set_max_depth(5);

		//background_color = RGBColor(0.5);
		background_color = black;

		//tracer_ptr = new AreaLighting(this);
		tracer_ptr = new PathTrace(this);

		Pinhole* camera = new Pinhole;
		camera->set_eye(-20, 10, 20);
		camera->set_lookat(0, 2, 0);
		camera->set_view_distance(1080);
		camera->compute_uvw();
		set_camera(camera);


		Emissive* emissive_ptr = new Emissive;
		emissive_ptr->scale_radiance(40.0);
		emissive_ptr->set_ce(white);


		// define a rectangle for the rectangular light

		float width = 4.0;				// for Figure 18.4(a) & (b)
		float height = 4.0;
		//	float width = 2.0;				// for Figure 18.4(c)
		//	float height = 2.0;
		Point3D center(0.0, 7.0, -7.0);	// center of each area light (rectangular, disk, and spherical)
		Point3D p0(-0.5 * width, center.y - 0.5 * height, center.z);
		Vector3D a(width, 0.0, 0.0);
		Vector3D b(0.0, height, 0.0);
		Normal normal(0, 0, 1);

		TRectangle* rectangle_ptr = new TRectangle(p0, a, b, normal);
		rectangle_ptr->set_material(emissive_ptr);
		rectangle_ptr->set_sampler(sampler_ptr);
		rectangle_ptr->set_shadows(false);
		add_object(rectangle_ptr);


		AreaLight* area_light_ptr = new AreaLight;
		area_light_ptr->set_object(rectangle_ptr);
		area_light_ptr->set_cast_shadows(true);
		area_light_ptr->set_material(emissive_ptr);
		add_light(area_light_ptr);


		// Four axis aligned boxes

		float box_width = 1.0; 		// x dimension
		float box_depth = 1.0; 		// z dimension
		float box_height = 4.5; 		// y dimension
		float gap = 3.0;

		Matte* matte_ptr1 = new Matte;
		matte_ptr1->set_ka(0.25);
		matte_ptr1->set_kd(0.75);
		matte_ptr1->set_cd(0.4, 0.7, 0.4);     // green
		matte_ptr1->set_sampler(new MultiJittered(num_samples));

		Box* box_ptr0 = new Box(Point3D(-1.5 * gap - 2.0 * box_width, 0.0, -0.5 * box_depth),
			Point3D(-1.5 * gap - box_width, box_height, 0.5 * box_depth));
		box_ptr0->set_material(matte_ptr1);
		add_object(box_ptr0);

		Box* box_ptr1 = new Box(Point3D(-0.5 * gap - box_width, 0.0, -0.5 * box_depth),
			Point3D(-0.5 * gap, box_height, 0.5 * box_depth));
		box_ptr1->set_material(matte_ptr1);
		add_object(box_ptr1);

		Box* box_ptr2 = new Box(Point3D(0.5 * gap, 0.0, -0.5 * box_depth),
			Point3D(0.5 * gap + box_width, box_height, 0.5 * box_depth));
		box_ptr2->set_material(matte_ptr1);
		add_object(box_ptr2);

		Box* box_ptr3 = new Box(Point3D(1.5 * gap + box_width, 0.0, -0.5 * box_depth),
			Point3D(1.5 * gap + 2.0 * box_width, box_height, 0.5 * box_depth));
		box_ptr3->set_material(matte_ptr1);
		add_object(box_ptr3);


		// ground plane

		Matte* matte_ptr2 = new Matte;
		matte_ptr2->set_ka(0.1);
		matte_ptr2->set_kd(0.90);
		matte_ptr2->set_cd(white);
		matte_ptr2->set_sampler(new MultiJittered(num_samples));

		Plane* plane_ptr = new Plane(Point3D(0.0), Normal(0, 1, 0));
		plane_ptr->set_material(matte_ptr2);
		add_object(plane_ptr);
	}
	break;
	default:
		break;
	}
	
}

