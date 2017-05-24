#ifndef __BRDF__
#define __BRDF__

// This file contains the declaration of the base class BRDF

#include <math.h>

#include "BxDF.h"

class BRDF : public BxDF
{
	public:
	
		BRDF(void);						
		
		BRDF(const BRDF& object);
		
		virtual BRDF*
		clone(void) const = 0;
		
		~BRDF(void);
								
		virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
		
		virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
		
		virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;
		
		virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo) const;
		
		void
			set_sampler(Sampler* sPtr);
			
	protected:
	
		BRDF&							
		operator= (const BRDF& rhs);

protected:

	Sampler* sampler_ptr;		// for indirect illumination
};

#endif