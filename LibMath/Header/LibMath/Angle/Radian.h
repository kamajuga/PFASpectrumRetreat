#ifndef LIBMATH_ANGLE_RADIAN_H_
#define LIBMATH_ANGLE_RADIAN_H_

#include "LibMath/Angle/Degree.h" 

namespace LibMath
{
	class Degree;

	class Radian
	{
		public:
						Radian();
			explicit	Radian(float val);				// explicit so no ambiguous / implicit conversion from float to angle can happen
						Radian(Radian const& other);
						~Radian() = default;

			operator	LibMath::Degree() const;				// Degree angle = Radian{0.5};		// implicit conversion from Radian to Degree

			Radian&		operator=(const Radian& other) = default;
			Radian&		operator+=(Radian other);			// Radian angle += Radian{0.5};
			Radian&		operator-=(Radian other);			// Radian angle -= Radian{0.5};
			Radian&		operator*=(float val);			// Radian angle *= 3;
			Radian&		operator/=(float val);			// Radian angle /= 3;

			void		wrap(bool _wrap = false);			// true -> limit m_value to range [-pi, pi[		// false -> limit m_value to range [0, 2 pi[

			float		degree(bool _deg = false) const;	// return angle in degree	// true -> return value in range [-180, 180[	// false -> return value in range [0, 360[
			float		radian(bool _rad = true) const;	// return angle in radian	// true -> return value in range [-pi, pi[		// false -> return value in range [0, 2 pi[
			float		raw() const;				// return m_angle

			friend		bool	operator==(Radian _rad1, Radian _rad2);
			friend		bool	operator==(Radian _rad, float _val);
			friend		bool	operator==(Radian _rad, Degree const& _deg);
			friend		Radian	operator-(Radian _rad);
			friend		Radian	operator+(Radian _rad1, Radian _rad2);
			friend		Radian	operator-(Radian _rad1, Radian _rad2);
			friend		Radian	operator*(Radian _rad1, float _val);
			friend		Radian	operator/(Radian _rad1, float _val);

		private:
			float m_value;
	};

	bool	operator==(Radian _rad1, Radian _rad2);			// bool isEqual = Radian{0.5} == Radian{0.5};	// true
	bool	operator==(Radian _rad, float _val);
	bool	operator==(Radian _rad, Degree const& _deg);	// bool isEqual = Radian{0.5} == Degree{60};	// false

	Radian	operator-(Radian _rad);					// Degree angle = - Radian{0.5};				// Radian{-0.5}

	Radian	operator+(Radian _rad1, Radian _rad2);			// Radian angle = Radian{0.5} + Radian{0.5};	// Radian{1}
	Radian	operator-(Radian _rad1, Radian _rad2);			// Radian angle = Radian{0.5} - Radian{0.5};	// Radian{0}
	Radian	operator*(Radian _rad1, float _val);			// Radian angle = Radian{0.5} * 3;				// Radian{1.5}
	Radian	operator/(Radian _rad1, float _val);			// Radian angle = Radian{0.5} / 3;				// Radian{0.166...}

	inline namespace Literal
	{
		LibMath::Radian operator""_rad(long double _val);			// Radian angle = 0.5_rad;
		LibMath::Radian operator""_rad(unsigned long long int _val);	// Radian angle = 1_rad;
	}
}

#endif // !LIBMATH_ANGLE_RADIAN_H_
