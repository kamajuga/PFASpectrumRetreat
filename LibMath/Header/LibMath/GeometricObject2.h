#ifndef GEOMERTIC_OBJECT2_H_
#define GEOMERTIC_OBJECT2_H_
#include <iostream>
#include "LibMath/Vector/Vector2.h"
#include "LibMath/Angle/Radian.h"
#include "LibMath/Trigonometry.h"


namespace LibMath
{
	namespace Geometry2D
	{
		class Point
		{
		public:
							Point() = default;
							Point(float x, float y);
							Point(Point const& other);
							~Point() = default;

			Point&			operator=(const Point& other);

			float			getX(void) { return m_x; };
			float const		getX(void) const { return m_x; }

			float			getY(void) { return m_y; };
			float const		getY(void) const { return m_y; }


		private:
			float			m_x = 0.f;
			float			m_y = 0.f;

		};

		bool				operator==(Point point1, Point Point);


		class Line
		{
		public:
							Line() = default;
							Line(Point const& p1, Point const& p2);
							Line(const Line& other);
							~Line() = default;

			Line&			operator=(const Line& other);

			Point&			get_p1(void);
			Point&			get_p2(void);
			float			lenght(void);
			float			lenghtSquare(void);

			bool			checkCollision(Point& point);
			bool			checkCollision(Line& line);

		private:
			Point			m_p1;
			Point			m_p2;

		};
		bool				operator==(Line line1, Line Line);
		bool				isPointOnSegment(Point& point, Point& segStart, Point& segEnd);

		class AABB
		{
		public:
							AABB() = default;
			explicit		AABB(Point const center, float const height, float const width);
							AABB(AABB const& other);
							~AABB() = default;

			AABB&			operator=(const AABB& other);

			Point			center(void);
			float			extentX(void) const;
			float			extentY(void) const;

			bool			checkCollision(AABB& rectanlge);
			bool			checkCollision(Line& line);
			bool			checkCollision(Point& point);

			Point			getTopRightCorner(void);
			Point			getTopLeftCorner(void);
			Point			getBottomRightCorner(void);
			Point			getBottomLeftCorner(void);

		private:
			Point			m_center;
			float			m_height = 0.f;
			float			m_width = 0.f;


		};

		class OBB
		{
		public:
								OBB() = default;
			explicit			OBB(Point center, float height, float width);
								OBB(OBB const& other);
								~OBB() = default;

			OBB&				operator=(const OBB& other);

			Point				getCenter(void);
			float				getHeight(void) const;
			float				getWidth(void) const;
			LibMath::Radian		getRotation(void) const;

			void				rotate(LibMath::Radian rad);

			Point				getTopRightCorner(void);
			Point				getTopLeftCorner(void);
			Point				getBotRightCorner(void);
			Point				getBotLeftCorner(void);

		private:
			Point				m_center;
			float				m_height = 0.f;
			float				m_width = 0.f;
			LibMath::Radian		m_rotation;

		};

		LibMath::Vector2		normalVector(Point p1, Point p2);
		bool					overlapOnAxis(float& min1, float& max1, float& min2, float& max2);
		void					projectRectangleOnAxis(OBB& rectangle, const LibMath::Vector2& axis, float& min, float& max);
		bool					isCollision(OBB& rect1, OBB& rect2);

		class Circle
		{
		public:
			friend			class Point;
							Circle() = default;
			explicit		Circle(Point const center, float const radius);
							Circle(Circle const& other);
							~Circle() = default;

			Circle&			operator=(const Circle& other);

			Point			getCenter(void) { return m_center; };
			float			getRadius(void) { return m_radius; };

			float			perimeter(void) const;
			float			area(void) const;

			bool			checkCollision(Circle& circle);
			bool			checkCollision(Line& line);
			bool			checlCollision(Point& point);

		private:
			Point			m_center;
			float			m_radius = 0.f;

		};
	}
}

#endif // !GEOMERTIC_OBJECT2_H_
