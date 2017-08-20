// This file contains the definition of the class Matrix

#include "Matrix.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <algorithm>

// ----------------------------------------------------------------------- default constructor
// a default matrix is an identity matrix

Matrix::Matrix(void) {	
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			if (x == y)
				m[x][y] = 1.0;
			else
				m[x][y] = 0.0;
		}
}


// ----------------------------------------------------------------------- copy constructor

Matrix::Matrix (const Matrix& mat) {
	for (int x = 0; x < 4; x++)				
		for (int y = 0; y < 4; y++)			
			m[x][y] = mat.m[x][y];	
}


// ----------------------------------------------------------------------- destructor

Matrix::~Matrix (void) {}   




// ----------------------------------------------------------------------- assignment operator

Matrix& 
Matrix::operator= (const Matrix& rhs) {
	if (this == &rhs)
		return (*this);

	for (int x = 0; x < 4; x++)				
		for (int y = 0; y < 4; y++)			
			m[x][y] = rhs.m[x][y];	

	return (*this);
}


// ----------------------------------------------------------------------- operator*
// multiplication of two matrices

Matrix 
Matrix::operator* (const Matrix& mat) const {
	Matrix 	product;
	
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++) {
			double sum = 0.0;

			for (int j = 0; j < 4; j++)
				sum += m[x][j] * mat.m[j][y];
 
			product.m[x][y] = sum;			
		}
	
	return (product);
}


//Vector3D Matrix::operator*(const Vector3D &v) const
//{
//	float x = v.x, y = v.y, z = v.z;
//	return Vector3D(m[0][0] * x + m[0][1] * y + m[0][2] * z,
//		m[1][0] * x + m[1][1] * y + m[1][2] * z,
//		m[2][0] * x + m[2][1] * y + m[2][2] * z);
//}

// ----------------------------------------------------------------------- operator/
// division by a scalar

Matrix 
Matrix::operator/ (const double d) {
	for (int x = 0; x < 4; x++)				
		for (int y = 0; y < 4; y++)			
			m[x][y] = m[x][y] / d;	

	return (*this);
}



// ----------------------------------------------------------------------- set_identity
// set matrix to the identity matrix

void											
Matrix::set_identity(void) {
    for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			if (x == y)
				m[x][y] = 1.0;
			else
				m[x][y] = 0.0;
		}
}

Matrix Matrix::inverse() const
{
	int indxc[4], indxr[4];
	int ipiv[4] = { 0, 0, 0, 0 };
	double minv[4][4];
	memcpy(minv, m, 4 * 4 * sizeof(double));
	for (int i = 0; i < 4; i++) {
		int irow = 0, icol = 0;
		float big = 0.f;
		// Choose pivot
		for (int j = 0; j < 4; j++) {
			if (ipiv[j] != 1) {
				for (int k = 0; k < 4; k++) {
					if (ipiv[k] == 0) {
						if (std::abs(minv[j][k]) >= big) {
							big = float(std::abs(minv[j][k]));
							irow = j;
							icol = k;
						}
					}
					else if (ipiv[k] > 1)
						printf("Singular matrix in MatrixInvert");
				}
			}
		}
		++ipiv[icol];
		// Swap rows _irow_ and _icol_ for pivot
		if (irow != icol) {
			for (int k = 0; k < 4; ++k) std::swap(minv[irow][k], minv[icol][k]);
		}
		indxr[i] = irow;
		indxc[i] = icol;
		if (minv[icol][icol] == 0.f) printf("Singular matrix in MatrixInvert");

		// Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
		float pivinv = 1. / minv[icol][icol];
		minv[icol][icol] = 1.;
		for (int j = 0; j < 4; j++) minv[icol][j] *= pivinv;

		// Subtract this row from others to zero out their columns
		for (int j = 0; j < 4; j++) {
			if (j != icol) {
				float save = minv[j][icol];
				minv[j][icol] = 0;
				for (int k = 0; k < 4; k++) minv[j][k] -= minv[icol][k] * save;
			}
		}
	}
	// Swap columns to reflect permutation
	for (int j = 3; j >= 0; j--) {
		if (indxr[j] != indxc[j]) {
			for (int k = 0; k < 4; k++)
				std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
		}
	}

	Matrix ret_mat;
	memcpy(ret_mat.m, minv, 4 * 4 * sizeof(double));
	return ret_mat;
}
