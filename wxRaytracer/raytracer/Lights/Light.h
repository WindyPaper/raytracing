#ifndef __LIGHT__
#define __LIGHT__

#include "Vector3D.h"
#include "RGBColor.h"
#include "Ray.h"

class ShadeRec;


//-------------------------------------------------------------------- class Light

class Light {	
	public:
	
		Light(void);
								
		Light(const Light& ls);			

		Light& 								
		operator= (const Light& rhs); 

		virtual Light* 						
		clone(void) const = 0;
		
		virtual 							
		~Light(void);
						
		virtual Vector3D								
		get_direction(ShadeRec& sr) = 0;				
																
		virtual RGBColor														
		L(ShadeRec& sr);	

		bool casts_shadows() const { return shadows; }
		void set_cast_shadows(bool cast_shadows) { shadows = cast_shadows; }

		virtual bool in_shadow(const Ray &ray, const ShadeRec &sr) const { return false; }

		virtual float
			G(const ShadeRec& sr) const { return 1.0; }

		virtual float
			pdf(ShadeRec& sr) const { return 1.0; }

protected:
	bool shadows;
};

#endif