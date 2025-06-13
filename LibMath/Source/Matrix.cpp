#include "LibMath/Matrix.h"
#include "LibMath/Trigonometry.h"
#include "LibMath/Matrix4Vector4Operation.h"
#include "LibMath/Arithmetic.h"

#pragma region Matrix 2D

#pragma region Matrix2Dx2
LibMath::Matrix2Dx2::Matrix2Dx2(float const diagonal)
{

	m_elements[0][0] = diagonal;
	m_elements[0][1] = 0;
	m_elements[1][0] = 0;
	m_elements[1][1] = diagonal;
}

LibMath::Matrix2Dx2::Matrix2Dx2(float const a1, float const a2, float const a3, float const a4)
{
	m_elements[0][0] = a1;		m_elements[0][1] = a2;
	m_elements[1][0] = a3;		m_elements[1][1] = a4;
}

LibMath::Matrix2Dx2::Matrix2Dx2(Matrix2Dx2 const& other)
{
	m_elements[0][0] = other.m_elements[0][0];		m_elements[0][1] = other.m_elements[0][1];
	m_elements[1][0] = other.m_elements[1][0];		m_elements[1][1] = other.m_elements[1][1];
}

LibMath::Matrix2Dx2& LibMath::Matrix2Dx2::operator=(Matrix2Dx2 const& other)
{
	m_elements[0][0] = other.m_elements[0][0];		m_elements[0][1] = other.m_elements[0][1];
	m_elements[1][0] = other.m_elements[1][0];		m_elements[1][1] = other.m_elements[1][1];

	return *this;
}

bool LibMath::Matrix2Dx2::operator==(Matrix2Dx2 const& other) const
{
	return (m_elements[0][0] == other.m_elements[0][0] &&
			m_elements[0][1] == other.m_elements[0][1] &&
			m_elements[1][0] == other.m_elements[1][0] &&
			m_elements[1][1] == other.m_elements[1][1]	);
}

LibMath::Matrix2Dx2::RowProxy LibMath::Matrix2Dx2::operator[](size_t const row)
{
	if (row > 1)
	{
		throw(std::out_of_range("Error: row out of range"));
	}
	return RowProxy(m_elements[row]);
}

float LibMath::Matrix2Dx2::determinant(void) const
{
	return m_elements[0][0] * m_elements[1][1] - m_elements[0][1] * m_elements[1][0];
}

LibMath::Matrix2Dx2 LibMath::Matrix2Dx2::minors(void) const
{
	// Element m(i,j) represent the minor of the element n_(i,j) in the original patrix
	float m1 = m_elements[1][1];
	float m2 = m_elements[0][1];
	float m3 = m_elements[1][0];
	float m4 = m_elements[0][0];
	return Matrix2Dx2(m1, m2, m3, m4);
}

LibMath::Matrix2Dx2 LibMath::Matrix2Dx2::cofactors(void) const
{
	float m1 = m_elements[1][1];
	float m2 = -m_elements[0][1];
	float m3 = -m_elements[1][0];
	float m4 = m_elements[0][0];
	return Matrix2Dx2(m1, m2, m3, m4);
}

LibMath::Matrix2Dx2 LibMath::Matrix2Dx2::adjugate(void) const
{
	float m1 = m_elements[1][1];
	float m2 = -m_elements[0][1];
	float m3 = -m_elements[1][0];
	float m4 = m_elements[0][0];

	//transpose cofactor matrix
	return Matrix2Dx2(m1, m2, m3, m4);
}

LibMath::Matrix2Dx2 LibMath::Matrix2Dx2::inverse(void) const
{
	Matrix2Dx2 adjugate = this->adjugate();
	float determinant = this->determinant();

	float coef = 1 / determinant;

	return Matrix2Dx2(coef * adjugate.m_elements[0][0],
					coef * adjugate.m_elements[0][1],
					coef * adjugate.m_elements[1][0],
					coef * adjugate.m_elements[1][1]);
}

LibMath::Matrix2Dx2 LibMath::Matrix2Dx2::transpose(void) const
{
	float m1 = m_elements[0][0];
	float m2 = m_elements[0][1];
	float m3 = m_elements[1][0];
	float m4 = m_elements[1][1];

	return Matrix2Dx2(m1, m3, m2, m4);
}

LibMath::Matrix2Dx2 LibMath::Matrix2Dx2::createRotation(Radian const& rad_) const
{

	// Create the basic 2x2 rotation matrix
	float cosTheta = LibMath::cos(rad_);
	float sinTheta = LibMath::sin(rad_);

	// Set the rotation matrix elements
	return Matrix2Dx2(cosTheta, sinTheta,
		-sinTheta, cosTheta);
}

LibMath::Matrix2Dx2 LibMath::Matrix2Dx2::createScale(LibMath::Vector2 vec) const
{
	Matrix2Dx2 scale(vec.getX(), 0, 0, vec.getY());

	return scale;
}

LibMath::Matrix2Dx2 LibMath::Matrix2Dx2::createTransform(LibMath::Radian rad, LibMath::Vector2 scale) const
{

	// Create the basic 2x2 rotation matrix
	float cosTheta = LibMath::cos(rad);
	float sinTheta = LibMath::sin(rad);


	return Matrix2Dx2(
		scale.getX() * cosTheta, -sinTheta,
		sinTheta, scale.getY() * cosTheta
	);
}

LibMath::Matrix2Dx2 LibMath::Matrix2Dx2::identity(void) const
{
	return Matrix2Dx2(1, 0, 0, 1);
}

LibMath::Matrix2Dx2 LibMath::operator+(LibMath::Matrix2Dx2  m1, LibMath::Matrix2Dx2 m2)
{
	float a1 = m1[0][0] + m2[0][0];
	float a2 = m1[0][1] + m2[0][1];
	float a3 = m1[1][0] + m2[1][0];
	float a4 = m1[1][1] + m2[1][1];

	return LibMath::Matrix2Dx2(a1, a2, a3, a4);
}

LibMath::Matrix2Dx2 LibMath::operator*(LibMath::Matrix2Dx2 m1, LibMath::Matrix2Dx2 m2)
{
	float a1 = m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1];
	float a2 = m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1];
	float a3 = m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1];
	float a4 = m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1];

	return LibMath::Matrix2Dx2(a1, a2, a3, a4);
}

LibMath::Matrix2Dx2 LibMath::operator*(LibMath::Matrix2Dx2 m1, float scalair)
{
	float a1 = scalair * m1[0][0];
	float a2 = scalair * m1[0][1];
	float a3 = scalair * m1[1][0];
	float a4 = scalair * m1[1][1];

	return LibMath::Matrix2Dx2(a1, a2, a3, a4);
}

LibMath::Vector2 LibMath::operator*(LibMath::Matrix2Dx2 m, LibMath::Vector2 vec)
{
	float a1 = m[0][0] * vec.getX() + m[1][0] * vec.getY();
	float a2 = m[0][1] * vec.getX() + m[1][1] * vec.getY();

	return LibMath::Vector2(a1, a2);
}
#pragma endregion Matrix2D->2x2

#pragma region Matrix2Dx3

LibMath::Matrix2Dx3::Matrix2Dx3(float const diagonal)
{
	m_elements[0][0] = diagonal;
	m_elements[0][1] = 0;
	m_elements[0][2] = 0;

	m_elements[1][0] = 0;
	m_elements[1][1] = diagonal;
	m_elements[1][2] = 0;

	m_elements[2][0] = 0;
	m_elements[2][1] = 0;
	m_elements[2][2] = diagonal;
}

LibMath::Matrix2Dx3::Matrix2Dx3(float const a00, float const a01, float const a02, 
								float const a10, float const a11, float const a12, 
								float const a20, float const a21, float const a22)
{
	m_elements[0][0] = a00;
	m_elements[0][1] = a01;
	m_elements[0][2] = a02;

	m_elements[1][0] = a10;
	m_elements[1][1] = a11;
	m_elements[1][2] = a12;

	m_elements[2][0] = a20;
	m_elements[2][1] = a21;
	m_elements[2][2] = a22;
}

LibMath::Matrix2Dx3::Matrix2Dx3(Matrix2Dx3 const& other)
{
	m_elements[0][0] = other.m_elements[0][0];
	m_elements[0][1] = other.m_elements[0][1];
	m_elements[0][2] = other.m_elements[0][2];

	m_elements[1][0] = other.m_elements[1][0];
	m_elements[1][1] = other.m_elements[1][1];
	m_elements[1][2] = other.m_elements[1][2];

	m_elements[2][0] = other.m_elements[2][0];
	m_elements[2][1] = other.m_elements[2][1];
	m_elements[2][2] = other.m_elements[2][2];
}

LibMath::Matrix2Dx3& LibMath::Matrix2Dx3::operator=(Matrix2Dx3 const& other)
{
	m_elements[0][0] = other.m_elements[0][0];
	m_elements[0][1] = other.m_elements[0][1];
	m_elements[0][2] = other.m_elements[0][2];

	m_elements[1][0] = other.m_elements[1][0];
	m_elements[1][1] = other.m_elements[1][1];
	m_elements[1][2] = other.m_elements[1][2];

	m_elements[2][0] = other.m_elements[2][0];
	m_elements[2][1] = other.m_elements[2][1];
	m_elements[2][2] = other.m_elements[2][2];

	return *this;
}

bool LibMath::Matrix2Dx3::operator==(LibMath::Matrix2Dx3 const& m)
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (m_elements[i][j] != m[i][j])
			{
				return false;
			}
		}
	}

	return true;
}

LibMath::Matrix2Dx3::RowProxy LibMath::Matrix2Dx3::operator[](size_t row)
{
	if (row > 2)
	{
		throw(std::out_of_range("Error: row not in range"));
	}

	return RowProxy(m_elements[row]);

}

LibMath::Matrix2Dx3::RowProxy LibMath::Matrix2Dx3::operator[](size_t const row) const
{
	if (row > 2)
	{
		throw(std::out_of_range("Error: row not in range"));
	}

	return RowProxy(m_elements[row]);
}

LibMath::Matrix2Dx3 LibMath::Matrix2Dx3::createTranslation(LibMath::Vector2 const translation)
{
	Matrix2Dx3 translation_m(      1,               0,              0,
							       0,               1,              0, 
							translation.getX(), translation.getY(), 1);

	return translation_m;
}

LibMath::Matrix2Dx3 LibMath::Matrix2Dx3::createRotation(LibMath::Geometry2D::Point
	const point, LibMath::Radian  rad)
{

	float cosR = LibMath::cos(rad);
	float sinR = LibMath::sin(rad);

	Matrix2Dx3 rotation(                        cosR,                                            sinR,                    0,
						                       -sinR,                                            cosR,                    0,
						point.getX() * (1 - cosR) + point.getY() * sinR, point.getY() * (1 - cosR) - point.getY() * sinR, 1);
	return rotation;
}

LibMath::Matrix2Dx3 LibMath::Matrix2Dx3::createScale(LibMath::Vector2 const scale)
{
	Matrix2Dx3 scale_m(scale.getX(), 0, 0,
		0, scale.getY(), 0,
		0, 0, 1);
	return scale_m;
}

LibMath::Matrix2Dx3 LibMath::Matrix2Dx3::createTransform(LibMath::Vector2 const translation, LibMath::Radian const rotation, LibMath::Vector2 const scale) 
{
	float cosR = LibMath::cos(rotation);
	float sinR = LibMath::sin(rotation);

	LibMath::Matrix2Dx3 translate = createTranslation(translation);
	LibMath::Matrix2Dx3 rotate = createRotation(LibMath::Geometry2D::Point(0.f, 0.f), rotation);
	LibMath::Matrix2Dx3 scaleM = createScale(scale);

	return Matrix2Dx3(scale.getX() * cosR, sinR * scale.getX(),  0,
					  -sinR * scale.getY(), scale.getY() * cosR, 0,
					  translation.getX(), translation.getY(),    1
	);
	
}

LibMath::Matrix2Dx3 LibMath::Matrix2Dx3::identity(void)
{
	return Matrix2Dx3(1, 0, 0, 0, 1, 0, 0, 0, 1);
}

LibMath::Matrix2Dx3 LibMath::operator+(Matrix2Dx3 const m1, Matrix2Dx3 const m2)
{

	return Matrix2Dx3(
		m1[0][0] + m2[0][0], m1[0][1] + m2[0][1], m1[0][2] + m2[0][2],
		m1[1][0] + m2[1][0], m1[1][1] + m2[1][1], m1[1][2] + m2[1][2],
		m1[2][0] + m2[2][0], m1[2][1] + m2[2][1], m1[2][2] + m2[2][2]
	);
}

LibMath::Matrix2Dx3 LibMath::operator*(Matrix2Dx3 const m, float const scalair)
{
	return Matrix2Dx3(
		scalair * m[0][0], scalair * m[0][1], scalair * m[0][2],
		scalair * m[1][0], scalair * m[1][1], scalair * m[1][2],
		scalair * m[2][0], scalair * m[2][1], scalair * m[2][2]
	);
}

LibMath::Matrix2Dx3 LibMath::operator*(Matrix2Dx3 const m1, Matrix2Dx3 const m2)
{
	// Access elements in column-major order: m[column][row]

	float a1 = (m1[0][0] * m2[0][0]) + (m1[1][0] * m2[0][1]) + (m1[2][0] * m2[0][2]);
	float a2 = (m1[0][1] * m2[0][0]) + (m1[1][1] * m2[0][1]) + (m1[2][1] * m2[0][2]);
	float a3 = (m1[0][2] * m2[0][0]) + (m1[1][2] * m2[0][1]) + (m1[2][2] * m2[0][2]);

	float a4 = (m1[0][0] * m2[1][0]) + (m1[1][0] * m2[1][1]) + (m1[2][0] * m2[1][2]);
	float a5 = (m1[0][1] * m2[1][0]) + (m1[1][1] * m2[1][1]) + (m1[2][1] * m2[1][2]);
	float a6 = (m1[0][2] * m2[1][0]) + (m1[1][2] * m2[1][1]) + (m1[2][2] * m2[1][2]);

	float a7 = (m1[0][0] * m2[2][0]) + (m1[1][0] * m2[2][1]) + (m1[2][0] * m2[2][2]);
	float a8 = (m1[0][1] * m2[2][0]) + (m1[1][1] * m2[2][1]) + (m1[2][1] * m2[2][2]);
	float a9 = (m1[0][2] * m2[2][0]) + (m1[1][2] * m2[2][1]) + (m1[2][2] * m2[2][2]);

	return Matrix2Dx3(a1, a2, a3, a4, a5, a6, a7, a8, a9);


}
#pragma endregion Matrix2d->3x3

#pragma endregion Matrix2D

#pragma region Matrix3D

LibMath::Matrix3Dx3::Matrix3Dx3(float const diagonal)
{
	m_elements[0][0] = diagonal;
	m_elements[0][1] = 0;
	m_elements[0][2] = 0;

	m_elements[1][0] = 0;
	m_elements[1][1] = diagonal;
	m_elements[1][2] = 0;

	m_elements[2][0] = 0;
	m_elements[2][1] = 0;
	m_elements[2][2] = diagonal;
}

LibMath::Matrix3Dx3::Matrix3Dx3(float const a00, float const a01, float const a02, 
								float const a10, float const a11, float const a12, 
								float const a20, float const a21, float const a22)
{
	m_elements[0][0] = a00;
	m_elements[0][1] = a01;
	m_elements[0][2] = a02;

	m_elements[1][0] = a10;
	m_elements[1][1] = a11;
	m_elements[1][2] = a12;

	m_elements[2][0] = a20;
	m_elements[2][1] = a21;
	m_elements[2][2] = a22;
}

LibMath::Matrix3Dx3::Matrix3Dx3(Matrix3Dx3 const& other)
{
	m_elements[0][0] = other.m_elements[0][0];
	m_elements[0][1] = other.m_elements[0][1];
	m_elements[0][2] = other.m_elements[0][2];

	m_elements[1][0] = other.m_elements[1][0];
	m_elements[1][1] = other.m_elements[1][1];
	m_elements[1][2] = other.m_elements[1][2];

	m_elements[2][0] = other.m_elements[2][0];
	m_elements[2][1] = other.m_elements[2][1];
	m_elements[2][2] = other.m_elements[2][2];
}

LibMath::Matrix3Dx3& LibMath::Matrix3Dx3::operator=(Matrix3Dx3 const& other)
{
	m_elements[0][0] = other.m_elements[0][0];
	m_elements[0][1] = other.m_elements[0][1];
	m_elements[0][2] = other.m_elements[0][2];

	m_elements[1][0] = other.m_elements[1][0];
	m_elements[1][1] = other.m_elements[1][1];
	m_elements[1][2] = other.m_elements[1][2];

	m_elements[2][0] = other.m_elements[2][0];
	m_elements[2][1] = other.m_elements[2][1];
	m_elements[2][2] = other.m_elements[2][2];

	return *this;
}

LibMath::Matrix3Dx3::RowProxy LibMath::Matrix3Dx3::operator[](size_t const row)
{
	if (row > 2)
	{
		throw(std::out_of_range("Error: row not in range"));
	}

	return RowProxy(m_elements[row]);
}

LibMath::Matrix3Dx3::RowProxy LibMath::Matrix3Dx3::operator[](size_t const row) const
{
	if (row > 2)
	{
		throw(std::out_of_range("Error: row not in range"));
	}

	return RowProxy(m_elements[row]);
}

LibMath::Matrix3Dx3 LibMath::Matrix3Dx3::transpose(void) const
{
	return Matrix3Dx3(
		m_elements[0][0], m_elements[1][0], m_elements[2][0],
		m_elements[0][1], m_elements[1][1], m_elements[2][1],
		m_elements[0][2], m_elements[1][2], m_elements[2][2]
	);
}

float LibMath::Matrix3Dx3::determinant(void) const
{
	float component1 = m_elements[0][0] * ((m_elements[1][1] * m_elements[2][2]) - (m_elements[1][2] * m_elements[2][1]));
	float component2 = m_elements[0][1] * ((m_elements[1][0] * m_elements[2][2]) - (m_elements[1][2] * m_elements[2][0]));
	float component3 = m_elements[0][2] * ((m_elements[1][0] * m_elements[2][1]) - (m_elements[2][0] * m_elements[1][1]));

	return component1 - component2 + component3;
}

LibMath::Matrix3Dx3 LibMath::Matrix3Dx3::minors(void) const
{
	float minor00 = (m_elements[1][1] * m_elements[2][2]) - (m_elements[2][1] * m_elements[1][2]);
	float minor01 = (m_elements[1][0] * m_elements[2][2]) - (m_elements[1][2] * m_elements[2][0]);
	float minor02 = (m_elements[1][0] * m_elements[2][1]) - (m_elements[2][0] * m_elements[1][1]);

	float minor10 = (m_elements[0][1] * m_elements[2][2]) - (m_elements[2][1] * m_elements[0][2]);
	float minor11 = (m_elements[0][0] * m_elements[2][2]) - (m_elements[2][0] * m_elements[0][2]);
	float minor12 = (m_elements[0][0] * m_elements[2][1]) - (m_elements[2][0] * m_elements[0][1]);

	float minor20 = (m_elements[0][1] * m_elements[1][2]) - (m_elements[1][1] * m_elements[0][2]);
	float minor21 = (m_elements[0][0] * m_elements[1][2]) - (m_elements[1][0] * m_elements[0][2]);
	float minor22 = (m_elements[0][0] * m_elements[1][1]) - (m_elements[1][0] * m_elements[0][1]);

	return Matrix3Dx3(
		minor00, minor01, minor02,
		minor10, minor11, minor12,
		minor20, minor21, minor22
	);
}

LibMath::Matrix3Dx3 LibMath::Matrix3Dx3::cofators(void) const
{
	Matrix3Dx3 minors_ = minors();

	return Matrix3Dx3(
		minors_[0][0], -minors_[0][1], minors_[0][2],
		-minors_[1][0], minors_[1][1], -minors_[1][2],
		minors_[2][0], -minors_[2][1], minors_[2][2]
	);
}

LibMath::Matrix3Dx3 LibMath::Matrix3Dx3::adjugate(void) const
{
	Matrix3Dx3 cofactors_ = cofators();

	return cofactors_.transpose();
}

LibMath::Matrix3Dx3 LibMath::Matrix3Dx3::inverse(void) const
{
	float det = determinant();

	float opDet = 1 / det;

	Matrix3Dx3 adj = adjugate();
	return Matrix3Dx3(
		opDet * adj[0][0], opDet * adj[0][1], opDet * adj[0][2],
		opDet * adj[1][0], opDet * adj[1][1], opDet * adj[1][2],
		opDet * adj[2][0], opDet * adj[2][1], opDet * adj[2][2]
	);
}

LibMath::Matrix3Dx3 LibMath::Matrix3Dx3::createTransform(LibMath::Vector2 const translate, LibMath::Radian const rotation, LibMath::Vector2 const scale)
{
	
	float cosR = LibMath::cos(rotation);
	float sinR = LibMath::sin(rotation);
	
	return Matrix3Dx3(
		scale.getX() * cosR, scale.getX() * sinR, 0.f,
		-scale.getY() * sinR, scale.getY() * cosR, 0.f,
		translate.getX(), translate.getY(), 1.f
	);
}

LibMath::Matrix3Dx3 LibMath::Matrix3Dx3::createRotationX(LibMath::Radian const angle)
{
	float cosR = LibMath::cos(angle);
	float sinR = LibMath::sin(angle);

	return Matrix3Dx3(
		1.f, 0.f, 0.f,
		0.f, cosR, sinR,
		0.f, -sinR, cosR
	);
}

LibMath::Matrix3Dx3 LibMath::Matrix3Dx3::createRotationY(LibMath::Radian const angle)
{
	float cosR = LibMath::cos(angle);
	float sinR = LibMath::sin(angle);

	return Matrix3Dx3(
		cosR, 0.f, -sinR, 
		0.f, 1.f, 0.f,
		sinR, 0.f, cosR
	);
}

LibMath::Matrix3Dx3 LibMath::Matrix3Dx3::createRotationZ(LibMath::Radian const angle)
{
	float cosR = LibMath::cos(angle);
	float sinR = LibMath::sin(angle);

	return Matrix3Dx3(
		cosR, sinR, 0.f,
		-sinR, cosR, 0.f,
		0.f, 0.f, 1.f
	);
}

LibMath::Matrix3Dx3 LibMath::Matrix3Dx3::createScale(LibMath::Vector3 const scale)
{
	return Matrix3Dx3(
		scale.getX(), 0.0f, 0.f,
		0.f, scale.getY(), 0.f,
		0.f, 0.f, scale.getZ()
	);
}

LibMath::Matrix3Dx3 LibMath::Matrix3Dx3::identity(void)
{
	return Matrix3Dx3(
		1.f, 0.f, 0.f, 
		0.f, 1.f, 0.f, 
		0.f, 0.f, 1.f
	);
}

LibMath::Matrix3Dx3	LibMath::operator+(Matrix3Dx3 const m1, Matrix3Dx3 const m2)
{
	return Matrix3Dx3(
		m1[0][0] + m2[0][0], m1[0][1] + m2[0][1], m1[0][2] + m2[0][2],
		m1[1][0] + m2[1][0], m1[1][1] + m2[1][1], m1[1][2] + m2[1][2],
		m1[2][0] + m2[2][0], m1[2][1] + m2[2][1], m1[2][2] + m2[2][2]
	);
}

LibMath::Matrix3Dx3 LibMath::operator*(Matrix3Dx3 const m, float const scalair)
{
	return Matrix3Dx3(
		scalair * m[0][0], scalair * m[0][1], scalair * m[0][2],
		scalair * m[1][0], scalair * m[1][1], scalair * m[1][2],
		scalair * m[2][0], scalair * m[2][1], scalair * m[2][2]
	);
}

LibMath::Vector3 LibMath::operator*(Matrix3Dx3 const mat, LibMath::Vector3 const vec)
{
	// Column-Major
	return LibMath::Vector3(
		mat[0][0] * vec.getX() + mat[1][0] * vec.getY() + mat[2][0] * vec.getZ(),
		mat[0][1] * vec.getX() + mat[1][1] * vec.getY() + mat[2][1] * vec.getZ(),
		mat[0][2] * vec.getX() + mat[1][2] * vec.getY() + mat[2][2] * vec.getZ()
	);
}

LibMath::Matrix3Dx3 LibMath::operator*(Matrix3Dx3 const m1, Matrix3Dx3 const m2)
{

	float a1 = (m1[0][0] * m2[0][0]) + (m1[1][0] * m2[0][1]) + (m1[2][0] * m2[0][2]);
	float a2 = (m1[0][1] * m2[0][0]) + (m1[1][1] * m2[0][1]) + (m1[2][1] * m2[0][2]);
	float a3 = (m1[0][2] * m2[0][0]) + (m1[1][2] * m2[0][1]) + (m1[2][2] * m2[0][2]);

	float a4 = (m1[0][0] * m2[1][0]) + (m1[1][0] * m2[1][1]) + (m1[2][0] * m2[1][2]);
	float a5 = (m1[0][1] * m2[1][0]) + (m1[1][1] * m2[1][1]) + (m1[2][1] * m2[1][2]);
	float a6 = (m1[0][2] * m2[1][0]) + (m1[1][2] * m2[1][1]) + (m1[2][2] * m2[1][2]);

	float a7 = (m1[0][0] * m2[2][0]) + (m1[1][0] * m2[2][1]) + (m1[2][0] * m2[2][2]);
	float a8 = (m1[0][1] * m2[2][0]) + (m1[1][1] * m2[2][1]) + (m1[2][1] * m2[2][2]);
	float a9 = (m1[0][2] * m2[2][0]) + (m1[1][2] * m2[2][1]) + (m1[2][2] * m2[2][2]);
	return Matrix3Dx3(
		a1, a2, a3,
		a4, a5, a6,
		a7, a8, a9
	);
}

#pragma endregion 

#pragma region Matrix4

LibMath::Matrix4::Matrix4(float const diagonal)
{
	m_elements[0][0] = diagonal;
	m_elements[0][1] = 0.f;
	m_elements[0][2] = 0.f;
	m_elements[0][3] = 0.f;

	m_elements[1][0] = 0.f;
	m_elements[1][1] = diagonal;
	m_elements[1][2] = 0.f;
	m_elements[1][3] = 0.f;

	m_elements[2][0] = 0.f;
	m_elements[2][1] = 0.f;
	m_elements[2][2] = diagonal;
	m_elements[2][3] = 0.f;

	m_elements[3][0] = 0.f;
	m_elements[3][1] = 0.f;
	m_elements[3][2] = 0.f;
	m_elements[3][3] = diagonal;
}

LibMath::Matrix4::Matrix4(float const a00, float const a01, float const a02, float const a03,
	float const a10, float const a11, float const a12, float const a13,
	float const a20, float const a21, float const a22, float const a23,
	float const a30, float const a31, float const a32, float const a33)
{
	m_elements[0][0] = a00;
	m_elements[0][1] = a01;
	m_elements[0][2] = a02;
	m_elements[0][3] = a03;

	m_elements[1][0] = a10;
	m_elements[1][1] = a11;
	m_elements[1][2] = a12;
	m_elements[1][3] = a13;

	m_elements[2][0] = a20;
	m_elements[2][1] = a21;
	m_elements[2][2] = a22;
	m_elements[2][3] = a23;

	m_elements[3][0] = a30;
	m_elements[3][1] = a31;
	m_elements[3][2] = a32;
	m_elements[3][3] = a33;
}

LibMath::Matrix4::Matrix4(Matrix4 const& other)
{
	m_elements[0][0] = other.m_elements[0][0];
	m_elements[0][1] = other.m_elements[0][1];
	m_elements[0][2] = other.m_elements[0][2];
	m_elements[0][3] = other.m_elements[0][3];

	m_elements[1][0] = other.m_elements[1][0];
	m_elements[1][1] = other.m_elements[1][1];
	m_elements[1][2] = other.m_elements[1][2];
	m_elements[1][3] = other.m_elements[1][3];

	m_elements[2][0] = other.m_elements[2][0];
	m_elements[2][1] = other.m_elements[2][1];
	m_elements[2][2] = other.m_elements[2][2];
	m_elements[2][3] = other.m_elements[2][3];

	m_elements[3][0] = other.m_elements[3][0];
	m_elements[3][1] = other.m_elements[3][1];
	m_elements[3][2] = other.m_elements[3][2];
	m_elements[3][3] = other.m_elements[3][3];
}

LibMath::Matrix4& LibMath::Matrix4::operator=(Matrix4 const& other)
{
	m_elements[0][0] = other.m_elements[0][0];
	m_elements[0][1] = other.m_elements[0][1];
	m_elements[0][2] = other.m_elements[0][2];
	m_elements[0][3] = other.m_elements[0][3];

	m_elements[1][0] = other.m_elements[1][0];
	m_elements[1][1] = other.m_elements[1][1];
	m_elements[1][2] = other.m_elements[1][2];
	m_elements[1][3] = other.m_elements[1][3];

	m_elements[2][0] = other.m_elements[2][0];
	m_elements[2][1] = other.m_elements[2][1];
	m_elements[2][2] = other.m_elements[2][2];
	m_elements[2][3] = other.m_elements[2][3];

	m_elements[3][0] = other.m_elements[3][0];
	m_elements[3][1] = other.m_elements[3][1];
	m_elements[3][2] = other.m_elements[3][2];
	m_elements[3][3] = other.m_elements[3][3];

	return *this;
}

LibMath::Matrix4::RowProxy LibMath::Matrix4::operator[](size_t const row) const
{
	if (row > 3)
	{
		throw(std::out_of_range("Error: row not in range"));
	}

	return RowProxy(m_elements[row]);
}

LibMath::Matrix4::RowProxy LibMath::Matrix4::operator[](size_t row)
{
	if (row > 3)
	{
		throw(std::out_of_range("Error: row not in range"));
	}

	return RowProxy(m_elements[row]);
}

LibMath::Matrix4 LibMath::Matrix4::transpose(void) const
{
	return Matrix4(
		m_elements[0][0], m_elements[1][0], m_elements[2][0], m_elements[3][0],
		m_elements[0][1], m_elements[1][1], m_elements[2][1], m_elements[3][1],
		m_elements[0][2], m_elements[1][2], m_elements[2][2], m_elements[3][2],
		m_elements[0][3], m_elements[1][3], m_elements[2][3], m_elements[3][3]
	);
}

float LibMath::Matrix4::determinant(void) const
{
	LibMath::Matrix3Dx3 subMatrix00(m_elements[1][1], m_elements[1][2], m_elements[1][3], 
									m_elements[2][1], m_elements[2][2], m_elements[2][3], 
									m_elements[3][1], m_elements[3][2], m_elements[3][3]);
	float minor00 = subMatrix00.determinant();

	LibMath::Matrix3Dx3 subMatrix01(m_elements[1][0], m_elements[1][2], m_elements[1][3],
									m_elements[2][0], m_elements[2][2], m_elements[2][3],
									m_elements[3][0], m_elements[3][2], m_elements[3][3]);
	float minor01 = subMatrix01.determinant();

	LibMath::Matrix3Dx3 subMatrix02(m_elements[1][0], m_elements[1][1], m_elements[1][3],
									m_elements[2][0], m_elements[2][1], m_elements[2][3],
									m_elements[3][0], m_elements[3][1], m_elements[3][3]);
	float minor02 = subMatrix02.determinant();

	LibMath::Matrix3Dx3 subMatrix03(m_elements[1][0], m_elements[1][1], m_elements[1][2],
									m_elements[2][0], m_elements[2][1], m_elements[2][2],
									m_elements[3][0], m_elements[3][1], m_elements[3][2]);
	float minor03 = subMatrix03.determinant();


	return (m_elements[0][0] * minor00) - (m_elements[0][1] * minor01) + (m_elements[0][2] * minor02) - (m_elements[0][3] * minor03);
}

LibMath::Matrix4 LibMath::Matrix4::minors(void) const
{
	LibMath::Matrix3Dx3 subMatrix00(m_elements[1][1], m_elements[1][2], m_elements[1][3],
									m_elements[2][1], m_elements[2][2], m_elements[2][3],
									m_elements[3][1], m_elements[3][2], m_elements[3][3]);
	float minor00 = subMatrix00.determinant();

	LibMath::Matrix3Dx3 subMatrix01(m_elements[1][0], m_elements[1][2], m_elements[1][3],
									m_elements[2][0], m_elements[2][2], m_elements[2][3],
									m_elements[3][0], m_elements[3][2], m_elements[3][3]);
	float minor01 = subMatrix01.determinant();

	LibMath::Matrix3Dx3 subMatrix02(m_elements[1][0], m_elements[1][1], m_elements[1][3],
									m_elements[2][0], m_elements[2][1], m_elements[2][3],
									m_elements[3][0], m_elements[3][1], m_elements[3][3]);
	float minor02 = subMatrix02.determinant();

	LibMath::Matrix3Dx3 subMatrix03(m_elements[1][0], m_elements[1][1], m_elements[1][2],
									m_elements[2][0], m_elements[2][1], m_elements[2][2],
									m_elements[3][0], m_elements[3][1], m_elements[3][2]);
	float minor03 = subMatrix03.determinant();

	/////////////////////////////////////////////////

	LibMath::Matrix3Dx3 subMatrix10(m_elements[0][1], m_elements[0][2], m_elements[0][3],
									m_elements[2][1], m_elements[2][2], m_elements[2][3],
									m_elements[3][1], m_elements[3][2], m_elements[3][3]);
	float minor10 = subMatrix10.determinant();

	LibMath::Matrix3Dx3 subMatrix11(m_elements[0][0], m_elements[0][2], m_elements[0][3],
									m_elements[2][0], m_elements[2][2], m_elements[2][3],
									m_elements[3][0], m_elements[3][2], m_elements[3][3]);
	float minor11 = subMatrix11.determinant();

	LibMath::Matrix3Dx3 subMatrix12(m_elements[0][0], m_elements[0][1], m_elements[0][3],
									m_elements[2][0], m_elements[2][1], m_elements[2][3],
									m_elements[3][0], m_elements[3][1], m_elements[3][3]);
	float minor12 = subMatrix12.determinant();

	LibMath::Matrix3Dx3 subMatrix13(m_elements[0][0], m_elements[0][1], m_elements[0][2],
									m_elements[2][0], m_elements[2][1], m_elements[2][2],
									m_elements[3][0], m_elements[3][1], m_elements[3][2]);
	float minor13 = subMatrix13.determinant();

	/////////////////////////////////////////////////

	LibMath::Matrix3Dx3 subMatrix20(m_elements[0][1], m_elements[0][2], m_elements[0][3],
									m_elements[1][1], m_elements[1][2], m_elements[1][3],
									m_elements[3][1], m_elements[3][2], m_elements[3][3]);
	float minor20 = subMatrix20.determinant();

	LibMath::Matrix3Dx3 subMatrix21(m_elements[0][0], m_elements[0][2], m_elements[0][3],
									m_elements[1][0], m_elements[1][2], m_elements[1][3],
									m_elements[3][0], m_elements[3][2], m_elements[3][3]);
	float minor21 = subMatrix21.determinant();

	LibMath::Matrix3Dx3 subMatrix22(m_elements[0][0], m_elements[0][1], m_elements[0][3],
									m_elements[1][0], m_elements[1][1], m_elements[1][3],
									m_elements[3][0], m_elements[3][1], m_elements[3][3]);
	float minor22 = subMatrix22.determinant();

	LibMath::Matrix3Dx3 subMatrix23(m_elements[0][0], m_elements[0][1], m_elements[0][2],
									m_elements[1][0], m_elements[1][1], m_elements[1][2],
									m_elements[3][0], m_elements[3][1], m_elements[3][2]);
	float minor23 = subMatrix23.determinant();

	/////////////////////////////////////////////////

	LibMath::Matrix3Dx3 subMatrix30(m_elements[0][1], m_elements[0][2], m_elements[0][3],
									m_elements[1][1], m_elements[1][2], m_elements[1][3],
									m_elements[2][1], m_elements[2][2], m_elements[2][3]);
	float minor30 = subMatrix30.determinant();

	LibMath::Matrix3Dx3 subMatrix31(m_elements[0][0], m_elements[0][2], m_elements[0][3],
									m_elements[1][0], m_elements[1][2], m_elements[1][3],
									m_elements[2][0], m_elements[2][2], m_elements[2][3]);
	float minor31 = subMatrix31.determinant();

	LibMath::Matrix3Dx3 subMatrix32	(m_elements[0][0], m_elements[0][1], m_elements[0][3],
									m_elements[1][0], m_elements[1][1], m_elements[1][3],
									m_elements[2][0], m_elements[2][1], m_elements[2][3]);
	float minor32 = subMatrix32.determinant();

	LibMath::Matrix3Dx3 subMatrix33(m_elements[0][0], m_elements[0][1], m_elements[0][2],
									m_elements[1][0], m_elements[1][1], m_elements[1][2],
									m_elements[2][0], m_elements[2][1], m_elements[2][2]);
	float minor33 = subMatrix33.determinant();
	
	return Matrix4(
		minor00, minor01, minor02, minor03,
		minor10, minor11, minor12, minor13,
		minor20, minor21, minor22, minor23,
		minor30, minor31, minor32, minor33 
	);
}

LibMath::Matrix4 LibMath::Matrix4::cofators(void) const
{
	Matrix4 minors_ = minors();

	return Matrix4(
		minors_[0][0], -minors_[0][1], minors_[0][2], -minors_[0][3],
		-minors_[1][0], minors_[1][1], -minors_[1][2], minors_[1][3],
		minors_[2][0], -minors_[2][1], minors_[2][2], -minors_[2][3],
		-minors_[3][0], minors_[3][1], -minors_[3][2], minors_[3][3]
	);
}

LibMath::Matrix4 LibMath::Matrix4::adjugate(void) const
{
	Matrix4 cofactor_ = cofators();

	return cofactor_.transpose();
}

LibMath::Matrix4 LibMath::Matrix4::inverse(void) const
{
	float det = determinant();

	if (LibMath::almostEqual(det, 0))
	{
		throw std::runtime_error("Matrix is not invertible.\n");
	}

	Matrix4 adj = adjugate();

	return adj * (1.0f / det);
}

LibMath::Matrix4 LibMath::Matrix4::perspective(float const fov, float const aspectRatio, float const near, float const far)
{
	// Ensure valid input
	if (fov <= 0 || aspectRatio <= 0 || near <= 0 || far <= 0 || near >= far)
	{
		throw std::invalid_argument("Invalid arguments for perspective projection.");
	}

	// Compute the tangent of half the vertical field of view
	float tanHalfFovY = tan(Radian(fov / 2.0f));

	// Initialize the perspective projection matrix
	Matrix4 result;

	// Set the elements of the perspective matrix
	result[0][0] = 1.0f / (aspectRatio * tanHalfFovY);
	result[1][1] = 1.0f / tanHalfFovY;
	result[2][2] = -(far + near) / (far - near);
	result[2][3] = -1.0f;
	result[3][2] = -(2.0f * far * near) / (far - near);
	result[3][3] = 0.0f;

	return result;
}

LibMath::Matrix4 LibMath::Matrix4::createTransform(LibMath::Vector3 const translate, LibMath::Radian const rotation, LibMath::Vector3 const scale)
{
	// Create translation, rotation, and scale matrices
	Matrix4 translationMatrix = createTranslate(translate);
	Matrix4 rotationMatrix = createRotationZ(rotation); // Assuming rotation around Z-axis
	Matrix4 scaleMatrix = createScale(scale);

	// Combine matrices: T * R * S
	return translationMatrix * rotationMatrix * scaleMatrix;
}

LibMath::Matrix4 LibMath::Matrix4::createTranslate(LibMath::Vector3 const  translate)
{
	return Matrix4(
				1.f,			  0.f,				0.f,	  0.f,
				0.f,			  1.f,				0.f,	  0.f,
				0.f,			  0.f,				1.f,	  0.f,
		translate.getX(), translate.getY(), translate.getZ(), 1.f
	);
}

LibMath::Matrix4 LibMath::Matrix4::createRotationX(LibMath::Radian const angle)
{
	float const cosR = LibMath::cos(angle);
	float const sinR = LibMath::sin(angle);

	return Matrix4(
		1.f, 0.f, 0.f, 0.f,
		0.f, cosR, sinR, 0.0f, 
		0.f, -sinR, cosR, 0.0f, 
		0.f, 0.f, 0.f, 1.f
	);
}

LibMath::Matrix4 LibMath::Matrix4::createRotationY(LibMath::Radian const angle)
{
	float const cosR = LibMath::cos(angle);
	float const sinR = LibMath::sin(angle);

	return Matrix4(
		cosR, 0.f, -sinR, 0.f,
		0.f, 1.f, 0.f, 0.f, 
		sinR, 0.f, cosR, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

LibMath::Matrix4 LibMath::Matrix4::createRotationZ(LibMath::Radian const angle)
{
	float const cosR = LibMath::cos(angle);
	float const sinR = LibMath::sin(angle);

	return Matrix4(
		cosR, sinR, 0.f, 0.f,
		-sinR, cosR, 0.f, 0.f, 
		0.f, 0.f, 1.f, 0.f, 
		0.f, 0.f, 0.f, 1.f
	);
}

LibMath::Matrix4 LibMath::Matrix4::createRotationXYZ(LibMath::Radian const angleX, LibMath::Radian const angleY, LibMath::Radian const angleZ)
{
	Matrix4 rotX = createRotationX(angleX);
	Matrix4 rotY = createRotationY(angleY);
	Matrix4 rotZ = createRotationZ(angleZ);

	// Ordre YXZ (le plus courant) : rotation Y d'abord, puis X, puis Z
	return rotZ * rotX * rotY;
}

LibMath::Matrix4 LibMath::Matrix4::createRotationXYZ(const Vector3& angles)
{
	LibMath::Radian x = LibMath::Degree(angles.getX());
	LibMath::Radian y = LibMath::Degree(angles.getY());
	LibMath::Radian z = LibMath::Degree(angles.getZ());
	return createRotationXYZ(x, y, z);
}

LibMath::Matrix4 LibMath::Matrix4::RemoveTranslationComponent(const LibMath::Matrix4& matrix)
{
	LibMath::Matrix4 temp = matrix;

	for (size_t i = 0; i < 3; ++i)
	{
		temp[3][i] = 0.f;
		temp[i][3] = 0.f;
	}

	temp[3][3] = 1.f;

	return temp;
}

LibMath::Matrix4 LibMath::Matrix4::createScale(LibMath::Vector3 const scale)
{

	return Matrix4(
		scale.getX(), 0.f, 0.f, 0.f, 
		0.f, scale.getY(), 0.f, 0.f,
		0.f, 0.f, scale.getZ(), 0.f, 
		0.f, 0.f, 0.f, 1.f
	);
}

LibMath::Matrix4 LibMath::Matrix4::identity(void)
{
	return Matrix4(
		1.f, 0.f, 0.f, 0.f, 
		0.f, 1.f, 0.f, 0.f, 
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

LibMath::Matrix4 LibMath::operator+(Matrix4 const m1, Matrix4 const m2)
{
	return Matrix4(
		m1[0][0] + m2[0][0], m1[0][1] + m2[0][1], m1[0][2] + m2[0][2], m1[0][3] + m2[0][3],
		m1[1][0] + m2[1][0], m1[1][1] + m2[1][1], m1[1][2] + m2[1][2], m1[1][3] + m2[1][3],
		m1[2][0] + m2[2][0], m1[2][1] + m2[2][1], m1[2][2] + m2[2][2], m1[2][3] + m2[2][3],
		m1[3][0] + m2[3][0], m1[3][1] + m2[3][1], m1[3][2] + m2[3][2], m1[3][3] + m2[3][3]
	);
}

LibMath::Matrix4 LibMath::operator*(Matrix4 const m, float const scalair)
{
	return Matrix4(
		scalair * m[0][0], scalair * m[0][1], scalair * m[0][2], scalair * m[0][3],
		scalair * m[1][0], scalair * m[1][1], scalair * m[1][2], scalair * m[1][3],
		scalair * m[2][0], scalair * m[2][1], scalair * m[2][2], scalair * m[2][3],
		scalair * m[3][0], scalair * m[3][1], scalair * m[3][2], scalair * m[3][3]
	);
}

LibMath::Vector4 LibMath::operator*(const Matrix4 & m, const LibMath::Vector4 & vec)
{
	return LibMath::Vector4(
		(m[0][0] * vec.getX()) + (m[1][0] * vec.getY()) + (m[2][0] * vec.getZ()) + (m[3][0] * vec.getK()),
		(m[0][1] * vec.getX()) + (m[1][1] * vec.getY()) + (m[2][1] * vec.getZ()) + (m[3][1] * vec.getK()),
		(m[0][2] * vec.getX()) + (m[1][2] * vec.getY()) + (m[2][2] * vec.getZ()) + (m[3][2] * vec.getK()),
		(m[0][3] * vec.getX()) + (m[1][3] * vec.getY()) + (m[2][3] * vec.getZ()) + (m[3][3] * vec.getK())
	);
}

LibMath::Matrix4 LibMath::operator*(Matrix4 const m1, Matrix4 const m2)
{
	float comp00 = (m2[0][0] * m1[0][0]) + (m2[0][1] * m1[1][0]) + (m2[0][2] * m1[2][0]) + (m2[0][3] * m1[3][0]);
	float comp01 = (m2[0][0] * m1[0][1]) + (m2[0][1] * m1[1][1]) + (m2[0][2] * m1[2][1]) + (m2[0][3] * m1[3][1]);
	float comp02 = (m2[0][0] * m1[0][2]) + (m2[0][1] * m1[1][2]) + (m2[0][2] * m1[2][2]) + (m2[0][3] * m1[3][2]);
	float comp03 = (m2[0][0] * m1[0][3]) + (m2[0][1] * m1[1][3]) + (m2[0][2] * m1[2][3]) + (m2[0][3] * m1[3][3]);

	float comp10 = (m2[1][0] * m1[0][0]) + (m2[1][1] * m1[1][0]) + (m2[1][2] * m1[2][0]) + (m2[1][3] * m1[3][0]);
	float comp11 = (m2[1][0] * m1[0][1]) + (m2[1][1] * m1[1][1]) + (m2[1][2] * m1[2][1]) + (m2[1][3] * m1[3][1]);
	float comp12 = (m2[1][0] * m1[0][2]) + (m2[1][1] * m1[1][2]) + (m2[1][2] * m1[2][2]) + (m2[1][3] * m1[3][2]);
	float comp13 = (m2[1][0] * m1[0][3]) + (m2[1][1] * m1[1][3]) + (m2[1][2] * m1[2][3]) + (m2[1][3] * m1[3][3]);

	float comp20 = (m2[2][0] * m1[0][0]) + (m2[2][1] * m1[1][0]) + (m2[2][2] * m1[2][0]) + (m2[2][3] * m1[3][0]);
	float comp21 = (m2[2][0] * m1[0][1]) + (m2[2][1] * m1[1][1]) + (m2[2][2] * m1[2][1]) + (m2[2][3] * m1[3][1]);
	float comp22 = (m2[2][0] * m1[0][2]) + (m2[2][1] * m1[1][2]) + (m2[2][2] * m1[2][2]) + (m2[2][3] * m1[3][2]);
	float comp23 = (m2[2][0] * m1[0][3]) + (m2[2][1] * m1[1][3]) + (m2[2][2] * m1[2][3]) + (m2[2][3] * m1[3][3]);

	float comp30 = (m2[3][0] * m1[0][0]) + (m2[3][1] * m1[1][0]) + (m2[3][2] * m1[2][0]) + (m2[3][3] * m1[3][0]);
	float comp31 = (m2[3][0] * m1[0][1]) + (m2[3][1] * m1[1][1]) + (m2[3][2] * m1[2][1]) + (m2[3][3] * m1[3][1]);
	float comp32 = (m2[3][0] * m1[0][2]) + (m2[3][1] * m1[1][2]) + (m2[3][2] * m1[2][2]) + (m2[3][3] * m1[3][2]);
	float comp33 = (m2[3][0] * m1[0][3]) + (m2[3][1] * m1[1][3]) + (m2[3][2] * m1[2][3]) + (m2[3][3] * m1[3][3]);

	return Matrix4(
		comp00, comp01, comp02, comp03,
		comp10, comp11, comp12, comp13,
		comp20, comp21, comp22, comp23,
		comp30, comp31, comp32, comp33
	);
}

#pragma endregion