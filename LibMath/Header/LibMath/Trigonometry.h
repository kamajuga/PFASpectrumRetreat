#ifndef LIBMATH_TRIGONOMETRY_H_
#define LIBMATH_TRIGONOMETRY_H_

#include "Angle/Radian.h"

namespace LibMath
{
	extern float const g_pi;	 // useful constant pi -> 3.141592...

	float	sin(Radian rad);		// float result = sin(Radian{0.5});		// 0.479426
	float	cos(Radian rad);		// float result = sin(Degree{45});		// 0.707107			// this make use implicit conversion
	float	tan(Radian rad);		// float result = sin(0.5_rad);			// 0.479426			// this make use user defined litteral
	Radian	asin(float val);		// Radian angle = asin(0.479426);		// Radian{0.500001}
	Radian	acos(float val);		// Degree angle = acos(0.707107);		// Degree{44.99998}	// this make use implicit conversion
	Radian	atan(float val);		// Radian angle = atan(0.546302);		// Radian{0.500000}
	Radian	atan(float val1, float val2); // Radian angle = atan(1, -2);			// Radian{2.677945}
}

#endif // !LIBMATH_TRIGONOMETRY_H_
