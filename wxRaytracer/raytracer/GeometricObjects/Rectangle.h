#ifndef __RECTANGLE__
#define __RECTANGLE__

// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


#include "Sampler.h"
#include "GeometricObject.h"

class MyRectangle: public GeometricObject {	
	public:
		
		MyRectangle(void);   									
				
		MyRectangle(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b);
		
		MyRectangle(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b, const Normal& n);
		
		virtual MyRectangle* 										
		clone(void) const;
	
		MyRectangle(const MyRectangle& r); 						

		virtual													
		~MyRectangle(void);   									

		MyRectangle& 												
		operator= (const MyRectangle& rhs);			
	
		virtual bool 												 
			hit(const Ray& ray, double& t, ShadeRec *sr = 0) const;
				
		
		// the following functions are used when the rectangle is a light source
		
		virtual void 								
		set_sampler(Sampler* sampler); 
				
		virtual Point3D 											
		sample(void);
		
		virtual Normal 																
		get_normal(const Point3D& p);		
		
		virtual float												
		pdf(ShadeRec& sr);

		virtual bool
			shadow_hit(const Ray& ray, double& tmin) const;
		
	private:
	
		Point3D 		p0;   			// corner vertex 
		Vector3D		a;				// side
		Vector3D		b;				// side
		double			a_len_squared;	// square of the length of side a
		double			b_len_squared;	// square of the length of side b
		Normal			normal;	
		
		float			area;			// for rectangular lights
		float			inv_area;		// for rectangular lights
		Sampler*		sampler_ptr;	// for rectangular lights 	
		
		static const double kEpsilon;   											
};

#endif
