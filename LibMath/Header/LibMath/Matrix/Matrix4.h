#ifndef LIBMATH_MATRIX_MATRIX4_H_
#define LIBMATH_MATRIX_MATRIX4_H_

#include "LibMath/Vector/Vector4.h"
#include "LibMath/Vector/Vector3.h"

namespace LibMath
{
	class Matrix4
	{

	public:
							Matrix4() = default;
		explicit			Matrix4(float const diagonal);
							Matrix4(float const a00, float const a01, float const a02, float const a03,
									float const a10, float const a11, float const a12, float const a13,
									float const a20, float const a21, float const a22, float const a23,
									float const a30, float const a31, float const a32, float const a33);
							Matrix4(Matrix4 const& other);
							~Matrix4() = default;

		Matrix4&			operator=(Matrix4 const& other);

		class RowProxy {
		private:
			float* row;

		public:

			RowProxy(float* rowRef) {
				row =  rowRef;
			}

			RowProxy(float const* rowRef) {
				row = const_cast<float *>(rowRef);
			}

			float& operator[](size_t col) 
			{
				if (col > 3)
				{
					throw(std::out_of_range("Error: column out of range"));
				}

				return row[col];
			}

			float operator[](size_t col) const
			{
				if (col > 3)
				{
					throw(std::out_of_range("Error: column out of range"));
				}
				return row[col];
			}
		};

		RowProxy			operator[](size_t const row) const;					// alternative for operator[][] overload
		RowProxy			operator[](size_t const row);

		Matrix4				transpose(void) const;
		float				determinant(void) const;
		Matrix4				minors(void) const;
		Matrix4				cofators(void) const;
		Matrix4				adjugate(void) const;
		Matrix4				inverse(void) const;
		float*				getPointer(void) { return &m_elements[0][0]; };

		static		Matrix4				perspective(float const fov, float const aspectRatio, float const near, float const far);

		static		Matrix4			createTransform(LibMath::Vector3 const translate = LibMath::Vector3(0.0f, 0.0f, 0.0f), // returns T . R . S
													LibMath::Radian const rotation = LibMath::Radian(0.0f),
													LibMath::Vector3 const scale = LibMath::Vector3(1.0f, 1.0f, 1.0f));

		static		Matrix4			createTranslate(LibMath::Vector3 const translate);
		static		Matrix4			createRotationX(LibMath::Radian const angle);
		static		Matrix4			createRotationY(LibMath::Radian const angle);
		static		Matrix4			createRotationZ(LibMath::Radian const angle);

		static		Matrix4			createRotationXYZ(LibMath::Radian const angleX, LibMath::Radian const angleY, LibMath::Radian const angleZ);
		static		Matrix4			createRotationXYZ(const Vector3& angles);

		static		Matrix4			RemoveTranslationComponent(const LibMath::Matrix4& matrix);

		static		Matrix4			createScale(LibMath::Vector3 const scale);

		static		Matrix4			identity(void);



	private:
		float m_elements[4][4] = { 0.0f };
	};

	Matrix4					operator+(Matrix4 const mat1, Matrix4 const mat2);
	Matrix4					operator*(Matrix4 const mat, float const val);
	//LibMath::Vector4		operator*(Matrix4 mat, LibMath::Vector4 vec);
	Matrix4				operator*(Matrix4 const mat1, Matrix4 const mat2);
}



#endif // !LIBMATH_MATRIX_MATRIX4_H_
