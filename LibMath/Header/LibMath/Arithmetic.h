#ifndef LIBMATH_ARITHMETIC_H_
#define LIBMATH_ARITHMETIC_H_

namespace LibMath
{
	bool almostEqual(float num1, float num2);		// Return if two floating value are similar enought to be considered equal

	float ceiling(float num);				// Return lowest integer value higher or equal to parameter
	float clamp(float num, float range_s, float range_e);	// Return parameter limited by the given range
	float floor(float num);					// Return highest integer value lower or equal to parameter
	float squareRoot(float num);			// Return square root of parameter
	float wrap(float num, float range_s, float range_e);	// Return parameter as value inside the given range
}

#endif // !LIBMATH_ARITHMETIC_H_
