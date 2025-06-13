#include "LibMath/Trigonometry.h"
#include <cmath>

float LibMath::sin(Radian rad)
{
	return std::sin(rad.radian());
}

float LibMath::cos(Radian rad)
{
	return std::cos(rad.radian()) ;
}

float LibMath::tan(Radian rad)
{
	return std::tan(rad.radian());
}

LibMath::Radian LibMath::asin(float val)
{
	return Radian(std::asin(val));
}

LibMath::Radian LibMath::acos(float val)
{
	return Radian(std::acos(val));
}

LibMath::Radian LibMath::atan(float val)
{
	return Radian(std::atan(val));
}

LibMath::Radian LibMath::atan(float val1, float val2)
{
	return Radian(std::atan2(val1, val2));
}
