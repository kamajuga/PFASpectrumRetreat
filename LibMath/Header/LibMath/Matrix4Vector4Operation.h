#ifndef LIBMATH_MATRIX4VECTOR4OPERATION_H_
#define LIBMATH_MATRIX4VECTOR4OPERATION_H_

#include "Vector/Vector4.h"
#include "Matrix/Matrix4.h"

namespace LibMath
{
	Vector4 operator*(const Matrix4& m, const Vector4& vec);
}

#endif // !LIBMATH_MATRIX4VECTOR4OPERATION_H_
