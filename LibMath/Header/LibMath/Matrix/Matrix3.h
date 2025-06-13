#ifndef LIBMATH_MATRIX_MATRIX3_H_
#define LIBMATH_MATRIX_MATRIX3_H_

#include "LibMath/Vector/Vector3.h"

namespace LibMath
{
	class Matrix3Dx3
	{
	public:
							Matrix3Dx3() = default;
		explicit			Matrix3Dx3(float const diagonal);
							Matrix3Dx3(float const a00, float const a01, float const a02, 
								float const a10, float const a11, float const a12, 
								float const a20, float const a21, float const a22);
							Matrix3Dx3(Matrix3Dx3 const& other);
							~Matrix3Dx3() = default;

		Matrix3Dx3&			operator=(Matrix3Dx3 const& other);

		class RowProxy {
		private:
			float* row;

		public:
			RowProxy(float* rowRef) {
				row = rowRef;
			}

			RowProxy(float const* rowRef) {

				row = const_cast<float*>(rowRef);

			}

			float& operator[](size_t const col) {

				if (col > 2)
				{
					throw(std::out_of_range("Error: column out of range"));
				}

				return row[col];
			}

			float operator[](size_t const col) const {

				if (col > 2)
				{
					throw(std::out_of_range("Error: column out of range"));
				}

				return row[col];
			}
		};

		RowProxy			operator[](size_t const row);					// alternative for operator[][] overload
		RowProxy			operator[](size_t const row) const;

		Matrix3Dx3			transpose(void) const;
		float				determinant(void) const;
		Matrix3Dx3			minors(void) const;
		Matrix3Dx3			cofators(void) const;
		Matrix3Dx3			adjugate(void) const;
		Matrix3Dx3			inverse(void) const;

		static Matrix3Dx3			createTransform(LibMath::Vector2 const translate = LibMath::Vector2(0.0f, 0.0f), // returns T . R . S
			LibMath::Radian const rotation = LibMath::Radian(0.0f),
			LibMath::Vector2 const scale = LibMath::Vector2(1.0f, 1.0f));
	
		static Matrix3Dx3			createRotationX(LibMath::Radian const angle);
		static Matrix3Dx3			createRotationY(LibMath::Radian const angle);
		static Matrix3Dx3			createRotationZ(LibMath::Radian const angle);

		static Matrix3Dx3			createScale(LibMath::Vector3 const scale);

		static Matrix3Dx3			identity(void);



	private:
		float m_elements[3][3] = { 0.0f }; // row major matrix

	};
	Matrix3Dx3				operator+(Matrix3Dx3 const mat1, Matrix3Dx3 const mat2);
	Matrix3Dx3				operator*(Matrix3Dx3 const mat, float const val);
	LibMath::Vector3		operator*(Matrix3Dx3 const mat, LibMath::Vector3 const vec);
	Matrix3Dx3				operator*(Matrix3Dx3 const mat1, Matrix3Dx3 const mat2);
}

#endif // !LIBMATH_MATRIX_MATRIX3_H_
