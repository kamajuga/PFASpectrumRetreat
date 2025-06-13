#ifndef LIBMATH_VECTOR_VECTOR3_H_
#define LIBMATH_VECTOR_VECTOR3_H_

#include <iostream>
#include <string>

#include "LibMath/Angle/Radian.h"
#include "LibMath/Vector/Vector4.h"
#include "LibMath/Trigonometry.h"

namespace LibMath
{
	class Vector4;

	class Vector3
	{
	public:
						Vector3() = default;										// set all component to 0
		explicit		Vector3(float val);									// set all component to the same value
						Vector3(float val_x, float val_y, float val_z);					// set all component individually
						Vector3(Vector3 const& other);						// copy all component
						Vector3(Vector4 const& vec4);
						~Vector3() = default;

		float			getX(void) const { return m_x; };
		float&			getX(void) { return m_x; };

		float			getY(void) const { return m_y; };
		float&			getY(void) { return m_y; };

		float			getZ(void) const { return m_z; };
		float&			getZ(void) { return m_z; };

		static Vector3	zero(void);											// return a vector with all its component set to 0
		static Vector3	one(void);											// return a vector with all its component set to 1
		static Vector3	up(void);											// return a unit vector pointing upward
		static Vector3	down(void);											// return a unit vector pointing downward
		static Vector3	left(void);											// return a unit vector pointing left
		static Vector3	right(void);										// return a unit vector pointing right
		static Vector3	front(void);										// return a unit vector pointing forward
		static Vector3	back(void);											// return a unit vector pointing backward

		Vector3&		operator=(Vector3 const& other) = default;

		float&			operator[](int n);								// return this vector component value
		float			operator[](int n) const;							// return this vector component value

		Radian			angleFrom(Vector3 const& vec) const;				// return smallest angle between 2 vector

		Vector3			cross(Vector3 const& vec) const;					// return a copy of the cross product result

		float			distanceFrom(Vector3 const& vec) const;				// return distance between 2 points
		float			distanceSquaredFrom(Vector3 const& vec) const;		// return square value of the distance between 2 points
		float			distance2DFrom(Vector3 const& vec) const;			// return the distance between 2 points on the X-Y axis only
		float			distance2DSquaredFrom(Vector3 const& vec) const;	// return the square value of the distance between 2 points points on the X-Y axis only

		float			dot(Vector3 const& vec) const;						// return dot product result

		bool			isLongerThan(Vector3 const& vec) const;				// return true if this vector magnitude is greater than the other
		bool			isShorterThan(Vector3 const& vec) const;			// return true if this vector magnitude is less than the other

		bool			isUnitVector(void) const;							// return true if this vector magnitude is 1

		float			magnitude(void) const;								// return vector magnitude
		float			magnitudeSquared(void) const;						// return square value of the vector magnitude

		void			normalize(void);									// scale this vector to have a magnitude of 1

		void			projectOnto(Vector3 const& vec);					// project this vector onto an other

		void			reflectOnto(Vector3 const& vec);					// reflect this vector by an other

		void			rotate(Radian rad_x, Radian rad_y, Radian rad_z);					// rotate this vector using Euler angle apply in the z, x, y order
		void			rotate(Radian angle, Vector3 const& vec);					// rotate this vector around an arbitrary axis
		//void			rotate(Quaternion const&); todo quaternion		// rotate this vector using a quaternion rotor

		void			scale(Vector3 const& vec);							// scale this vector by a given factor

		std::string		string(void) const;									// return a string representation of this vector
		std::string		stringLong(void) const;								// return a verbose string representation of this vector

		void			translate(Vector3 const& vec);						// offset this vector by a given distance

	private:
		float m_x = 0.0f;
		float m_y = 0.0f;
		float m_z = 0.0f;
	};

	Vector3			rotateArroundAxis(Vector3 const& vector, Vector3 const& axis, Radian angle);
	std::string		formatNumber(float value);

	bool			operator==(Vector3 const& vec1, Vector3 const& vec2);			// Vector3{ 1 } == Vector3::one()				// true					// return if 2 vectors have the same component
	bool			operator!=(Vector3 const& vec1, Vector3 const& vec2);			// Vector3{ 1 } != Vector3::one()				// false				// return if 2 vectors differ by at least a component

	Vector3			operator-(Vector3 vec);									// - Vector3{ .5, 1.5, -2.5 }					// { -.5, -1.5, 2.5 }	// return a copy of a vector with all its component inverted

	Vector3			operator+(Vector3 vec1, Vector3 const& vec2);					// Vector3{ .5, 1.5, -2.5 } + Vector3::one()	// { 1.5, 2.5, -1.5 }	// add 2 vectors component wise
	Vector3			operator-(Vector3 vec1, Vector3 const& vec2);					// Vector3{ .5, 1.5, -2.5 } - Vector3{ 1 }		// { -.5, .5, -3.5 }	// subtract 2 vectors component wise
	Vector3			operator+(LibMath::Vector3& lhs, LibMath::Vector3& rhs);
	Vector3			operator-(LibMath::Vector3& lhs, LibMath::Vector3& rhs);
	Vector3			operator-(Vector3 vec1, float val);						// Vector3{ .5, 1.5, -2.5 } - 1.0f				// { -.5, .5, -3.5 }	// subtract a value from all vector component
	Vector3			operator*(Vector3 vec1, Vector3 const& vec2);					// Vector3{ .5, 1.5, -2.5 } * Vector3::zero()	// { 0, 0, 0 }			// multiply 2 vectors component wise
	Vector3			operator*(Vector3 vec, float val);
	Vector3			operator/(Vector3 vec1, Vector3 const& vec2);					// Vector3{ .5, 1.5, -2.5 } / Vector3{ 2 }		// { .25, .75, -1.25 }	// divide 2 vectors component wise

	Vector3&		operator+=(Vector3& vec1, Vector3 const& vec2);				// addition component wise
	Vector3&		operator-=(Vector3& vec1, Vector3 const& vec2);				// subtraction component wise
	Vector3&		operator*=(Vector3& vec1, Vector3 const& vec2);				// multiplication component wise
	Vector3&		operator*=(Vector3& vec, float val);
	Vector3&		operator/=(Vector3& vec1, Vector3 const& vec2);				// division component wise

	std::ostream&	operator<<(std::ostream& os, Vector3 const& vec);			// cout << Vector3{ .5, 1.5, -2.5 }				// add a vector string representation to an output stream
	std::istream&	operator>>(std::istream& is, Vector3& vec);				// ifstream file{ save.txt }; file >> vector;	// parse a string representation from an input stream into a vector
}

#endif // !LIBMATH_VECTOR_VECTOR3_H_
