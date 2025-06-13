#ifndef LIBMATH_MATRIX_MATRIX2_H_
#define LIBMATH_MATRIX_MATRIX2_H_

#include "LibMath/GeometricObject2.h"

namespace LibMath
{
	class Matrix2Dx2
	{
	public:

							Matrix2Dx2() = default;
		explicit			Matrix2Dx2(float const diagonal);
							Matrix2Dx2(float const a1, float const a2, float const a3, float const  a4);
							Matrix2Dx2(Matrix2Dx2 const& other);
							~Matrix2Dx2() = default;
		

		class RowProxy { 
			private:
				float row[2];

			public:
				RowProxy(float rowRef[2]) {
					row[0] = rowRef[0];
					row[1] = rowRef[1];
				}

				float& operator[](size_t const col) {
					if (col > 1)
					{
						throw(std::out_of_range("Error: column out of range"));
					}
					return row[col];
				}
		};

		Matrix2Dx2&			operator=(Matrix2Dx2 const& other);
	

		bool				operator==(Matrix2Dx2 const& other) const;
		RowProxy			operator[](size_t const row);					// alternative for operator[][] overload

		float				determinant(void) const;
		Matrix2Dx2			minors(void) const;
		Matrix2Dx2			cofactors(void) const;
		Matrix2Dx2			adjugate(void) const;
		Matrix2Dx2			inverse(void) const;
		Matrix2Dx2			transpose(void) const;


		Matrix2Dx2			createRotation(Radian const& rad) const;
		Matrix2Dx2			createScale(LibMath::Vector2 const vec) const;
		Matrix2Dx2			createTransform(LibMath::Radian const rad = 0_rad, LibMath::Vector2 const scale = Vector2(1, 1)) const;

		Matrix2Dx2			identity(void) const;
		

	private:
		float m_elements[2][2] = {0};

	};
	Matrix2Dx2			operator+(Matrix2Dx2 const m1, Matrix2Dx2 const m2);
	Matrix2Dx2			operator*(Matrix2Dx2 const m1, Matrix2Dx2 const m2);
	Matrix2Dx2			operator*(Matrix2Dx2 const m1, float const scalair);
	LibMath::Vector2			operator*(Matrix2Dx2 const m, LibMath::Vector2 const vec);

	class Matrix2Dx3
	{
	public:
		Matrix2Dx3() = default;
		Matrix2Dx3(float const diagonal);
		Matrix2Dx3(float const a00, float const a01, float const a02, 
				   float const a10, float const a11, float const a12, 
				   float const a20, float const a21, float const a22);
		Matrix2Dx3(Matrix2Dx3 const& other);

		Matrix2Dx3& operator=(Matrix2Dx3 const& other);

		class RowProxy {
		private:
			float* row;

		public:
			RowProxy(float* rowRef) {
				row = rowRef;
			}

			RowProxy(float const * rowRef) {
				row = const_cast<float*>(rowRef);
			}

			float& operator[](size_t const col) {

				if (col >2)
				{
					throw(std::out_of_range("Error: column out of range"));
				}

				return row[col];
			}

			float operator[](size_t col) const {
				if (col > 2) {
					throw std::out_of_range("Error: column out of range");
				}
				return row[col]; // Retourne la valeur réelle
			}
		};

		RowProxy	operator[](size_t const row);
		RowProxy	operator[](size_t const row) const;

		bool		operator==(Matrix2Dx3 const& m);

		static Matrix2Dx3	createTranslation(LibMath::Vector2 const translation);
		static Matrix2Dx3	createRotation(LibMath::Geometry2D::Point const point, LibMath::Radian const rad);
		static Matrix2Dx3	createScale(LibMath::Vector2 const scale);

		static Matrix2Dx3	createTransform(LibMath::Vector2 const translation = LibMath::Vector2(0, 0),
			LibMath::Radian const rotation = LibMath::Radian(0), LibMath::Vector2 const scale = LibMath::Vector2(1, 1));

		static Matrix2Dx3	identity(void);
		


		~Matrix2Dx3() = default;
		

	private:
		float m_elements[3][3] = { 0 };

	};
	Matrix2Dx3			operator+(Matrix2Dx3 const m1, Matrix2Dx3 const m2);
	Matrix2Dx3			operator*(Matrix2Dx3 const m, float const scalair);
	//Matrix2Dx3			operator*(Matrix2Dx3 m, LibMath::Vector3 vec);
	Matrix2Dx3			operator*(Matrix2Dx3 const m1, Matrix2Dx3 const m2);
}

#endif // !LIBMATH_MATRIX_MATRIX2_H_