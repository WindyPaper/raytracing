// 	Copyright (C) Mp77 2012
//	Original from Kevin Suffern 2000-2007
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#pragma once

#include "Point3D.h"
#include "Vector3D.h"

class Ray {
	public:
	
		Point3D			o;  	// origin 
		Vector3D		d; 		// direction 
		
		int depth;

		Ray(void);			
		
		Ray(const Point3D& origin, const Vector3D& dir);	
		
		Ray(const Ray& ray); 		
		
		Ray& 						
		operator= (const Ray& rhs);
		 								
		~Ray(void);
};

Matrix look_at_mat(const Ray &ray, const Vector3D &up);
