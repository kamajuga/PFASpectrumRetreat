#ifndef LIBMATH_ANGLE_DEGREE_H_
#define LIBMATH_ANGLE_DEGREE_H_

namespace LibMath
{
	class Radian;

	class Degree
	{
		public:
			friend		class Radian;
						Degree();
			explicit	Degree(float value);				// explicit so no ambiguous / implicit conversion from float to angle can happen
						Degree(Degree const& other);
						~Degree() = default;

			operator	LibMath::Radian() const;				// Radian angle = Degree{45};		// implicit conversion from Degree to Radian

			Degree&		operator=(const Degree& other) = default;
			Degree&		operator+=(Degree other);			// Degree angle += Degree{45};
			Degree&		operator-=(Degree other);			// Degree angle -= Degree{45};
			Degree&		operator*=(float value);			// Degree angle *= 3;
			Degree&		operator/=(float value);			// Degree angle /= 3;

			void		wrap(bool _wrap = false);			// true -> limit m_value to range [-180, 180[	// false -> limit m_value to range [0, 360[

			float		degree(bool _deg = false) const;	// return angle in degree	// true -> return value in range [-180, 180[	// false -> return value in range [0, 360[
			float		radian(bool _rad = true) const;	// return angle in radian	// true -> return value in range [-pi, pi[		// false -> return value in range [0, 2 pi[
			float		raw() const;				// return m_angle

			
			friend		bool	operator==(Degree _deg1, Degree _deg2);
			friend		bool	operator==(Degree _deg, float _val);
			friend		bool	operator==(Degree _deg, Radian const& _rad);
			friend		Degree	operator-(Degree _deg);
			friend		Degree	operator+(Degree _deg1, Degree _deg2);
			friend		Degree	operator-(Degree _deg1, Degree _deg2); 
			friend		Degree	operator*(Degree _deg, float _val);
			friend		Degree	operator/(Degree _deg, float _val);

		private:
			float m_value;
	};

	bool	operator==(Degree _deg1, Degree _deg2);			// bool isEqual = Degree{45} == Degree{45};		// true
	bool	operator==(Degree _deg, float _val);
	bool	operator==(Degree _deg, Radian const& _rad);	// bool isEqual = Degree{60} == Radian{0.5};	// false

	Degree	operator-(Degree _deg);					// Degree angle = - Degree{45};					// Degree{-45}

	Degree	operator+(Degree _deg1, Degree _deg2);			// Degree angle = Degree{45} + Degree{45};		// Degree{90}
	Degree	operator-(Degree _deg1, Degree _deg2);			// Degree angle = Degree{45} - Degree{45};		// Degree{0}
	Degree	operator*(Degree _deg, float _val);			// Degree angle = Degree{45} * 3;				// Degree{135}
	Degree	operator/(Degree _deg, float _val);			// Degree angle = Degree{45} / 3;				// Degree{15}

	inline namespace Literal
	{
		LibMath::Degree operator""_deg(long double _val);			// Degree angle = 7.5_deg;
		LibMath::Degree operator""_deg(unsigned long long int _val);	// Degree angle = 45_deg;
	}
}

#endif // !LIBMATH_ANGLE_DEGREE_H_
