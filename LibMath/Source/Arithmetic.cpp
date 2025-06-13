#define _USE_MATH_DEFINES
#include <cmath>
#include <LibMath/Arithmetic.h>

#define EPSILON 1e-6f

bool LibMath::almostEqual(float num1, float num2)
{

    return std::abs(num1 - num2) <= EPSILON ;
}

float LibMath::ceiling(float num)
{
    return std::ceilf(num);
}

float LibMath::clamp(float num, float range_s, float range_e)
{
    return std::fmaxf(range_s, std::fminf(num, range_e));
}

float LibMath::floor(float num)
{
    return std::floorf(num);
}

float LibMath::squareRoot(float num)
{
    return std::sqrtf(num);
}

float LibMath::wrap(float num, float range_s, float range_e)
{
	float range = range_e - range_s;

	while (num < range_s)
	{ // increase until in interval
		num += range;
	}

	while (num >= range_e)
	{ // decrease until in interval
		num -= range;
	}

	return num;
}
