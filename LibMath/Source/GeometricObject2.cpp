#define _USE_MATH_DEFINES
#include <cmath>
#include "LibMath/GeometricObject2.h"

#define COLLISION_ACCURACY	0.000001f

#pragma region Point
LibMath::Geometry2D::Point::Point(float x, float y)
{
	m_x = x;
	m_y = y;
}

LibMath::Geometry2D::Point::Point(Point const& other)
{
	m_x = other.m_x;
	m_y = other.m_y;
}

LibMath::Geometry2D::Point& LibMath::Geometry2D::Point::operator=(const Point& other)
{
	m_x = other.m_x;
	m_y = other.m_y;

	return *this;
}

bool LibMath::Geometry2D::operator==(Point point1, Point Point)
{
	return (std::abs(point1.getX() - Point.getX()) <= 1e-6f && std::abs(point1.getY() - Point.getY()) <=1e-6f);
}
#pragma endregion All functions for Point

#pragma region Line

LibMath::Geometry2D::Line::Line(Point const& p1, Point const& p2)
{
	m_p1 = p1;
	m_p2 = p2;
}

LibMath::Geometry2D::Line::Line(const Line& other)
{
	m_p1 = other.m_p1;
	m_p2 = other.m_p2;
}

LibMath::Geometry2D::Line& LibMath::Geometry2D::Line::operator=(const Line& other)
{
	m_p1 = other.m_p1;
	m_p2 = other.m_p2;

	return *this;
}

LibMath::Geometry2D::Point& LibMath::Geometry2D::Line::get_p1(void)
{
	return m_p1;
}

LibMath::Geometry2D::Point& LibMath::Geometry2D::Line::get_p2(void)
{
	return m_p2;
}

float LibMath::Geometry2D::Line::lenght(void)
{

	return sqrtf(powf(m_p1.getX() - m_p2.getX(), 2.0f) + powf(m_p1.getY() - m_p2.getY(), 2.0f));
}

float LibMath::Geometry2D::Line::lenghtSquare(void)
{
	return powf(m_p1.getX() - m_p2.getX(), 2.0f) + powf(m_p1.getY() - m_p2.getY(), 2.0f);
}

bool LibMath::Geometry2D::Line::checkCollision(Point& point)
{
	// Distance between the point and the 1st edge of the line
	float distance1 = sqrtf(powf(point.getX() - m_p1.getX(), 2) + powf(point.getY() - m_p1.getY(), 2));

	// Distance between the point and the 2nd edge of the line
	float distance2 = sqrtf(powf(point.getX() - m_p2.getX(), 2) + powf(point.getY() - m_p2.getY(), 2));

	float distSum = distance1 + distance2;



	if (abs(distSum - lenght()) <= COLLISION_ACCURACY) //
	{
		return true;
	}

	return false;
}

bool LibMath::Geometry2D::Line::checkCollision(Line& line)
{
	/*
		Parametric equation of a segment AB: AM = tAB, t [0, 1]

		Let two segment AB and CD:
			- AM = t AB
			- CM' = u CD
		If AB and CD collides, that means M = M'
	*/
	const float EPSILON = 1e-6f;

	//float denominator = ((m_p2.getX() - m_p1.getX()) * (line.m_p1.getY() - line.m_p2.getY())) -((m_p2.getY() - m_p1.getY()) * (line.m_p1.getX() - line.m_p2.getX()));
	float temp2 =       ((m_p2.getY() - m_p1.getY()) * (line.m_p1.getX() - line.m_p2.getX())) - ((m_p2.getX() - m_p1.getX()) * (line.m_p1.getY() - line.m_p2.getY()));

	// Check if lines are parallel or colinear
	if (fabs(temp2) < EPSILON) {
		// Check if the lines are colinear and overlap
		if (isPointOnSegment(m_p1, line.m_p1, line.m_p2) ||
			isPointOnSegment(m_p2, line.m_p1, line.m_p2) ||
			isPointOnSegment(line.m_p1, m_p1, m_p2) ||
			isPointOnSegment(line.m_p2, m_p1, m_p2)) {
			return true;
		}
		return false;
	}


	float temp1 = ( (m_p2.getX() - m_p1.getX()) * (line.m_p2.getY() - m_p1.getY())) - ((m_p2.getY() - m_p1.getY()) * (line.m_p2.getX() - m_p1.getX()));
	
	float temp3 = ((line.m_p1.getX() - line.m_p2.getX()) * (line.m_p2.getY() - m_p1.getY())) - ((line.m_p1.getY() - line.m_p2.getY()) * (line.m_p2.getX() - m_p1.getX()));

	float uA = temp1 / temp2;

	float uB = temp3 / temp2;

	// uA [0, 1] and uB [0, 1]
	if ((uA >= 0 && uA <= 1) && (uB >= 0 && uB <= 1))
	{
		return true;
	}

	return false;
}

bool LibMath::Geometry2D::operator==(Line line1, Line Line)
{
	return (line1.get_p1() == Line.get_p1() && Line.get_p2() == Line.get_p2());
}

bool LibMath::Geometry2D::isPointOnSegment(Point& point, Point& segStart, Point& segEnd)
{
	const float EPSILON =  1e-6f;
	float crossProduct = (point.getY() - segStart.getY()) * (segEnd.getX() - segStart.getX()) -
		(point.getX() - segStart.getX()) * (segEnd.getY() - segStart.getY());

	if (std::abs(crossProduct) > EPSILON)
	{
		return false; // Not colinear
	}

	float dotProduct = (point.getX() - segStart.getX()) * (segEnd.getX() - segStart.getX()) +
		(point.getY() - segStart.getY()) * (segEnd.getY() - segStart.getY());
	if (dotProduct < 0) {
		return false; // Outside segment
	}

	float squaredLength = (segEnd.getX() - segStart.getX()) * (segEnd.getX() - segStart.getX()) +
		(segEnd.getY() - segStart.getY()) * (segEnd.getY() - segStart.getY());
	if (dotProduct > squaredLength) {
		return false; // Outside segment
	}

	return true; // Colinear and within bounds
}
#pragma endregion All functions for Line

#pragma region AABB

LibMath::Geometry2D::AABB::AABB(Point const center, float const height, float const width)
{
	m_center = center;
	m_height = height;
	m_width = width;
}

LibMath::Geometry2D::AABB::AABB(AABB const& other)
{
	m_center = other.m_center;
	m_height = other.m_height;
	m_width = other.m_width;
}

LibMath::Geometry2D::AABB& LibMath::Geometry2D::AABB::operator=(const AABB& other)
{
	m_center = other.m_center;
	m_height = other.m_height;
	m_width = other.m_width;

	return *this;
}

LibMath::Geometry2D::Point LibMath::Geometry2D::AABB::center(void)
{
	return m_center;
}

float LibMath::Geometry2D::AABB::extentX(void) const
{
	return m_width / 2;
}

float LibMath::Geometry2D::AABB::extentY(void) const
{
	return m_height / 2;
}

bool LibMath::Geometry2D::AABB::checkCollision(AABB& rectanlge)
{
	if (abs(m_center.getX() - rectanlge.m_center.getX()) > (extentX() + rectanlge.extentX()))
	{
		return false;
	}

	if (abs(m_center.getY() - rectanlge.m_center.getY()) > (extentY() + rectanlge.extentY()))
	{
		return false;
	}

	return true;
}

bool LibMath::Geometry2D::AABB::checkCollision(Line& line)
{	
	// create the lines of the rectangle
	Line leftLine(getTopLeftCorner(), getBottomLeftCorner());
	Line topLine(getTopRightCorner(), getTopLeftCorner());
	Line botLine(getBottomRightCorner(), getBottomLeftCorner());
	Line rightLine(getTopRightCorner(), getBottomRightCorner());

	// check colllision with each line 
	bool top = line.checkCollision(topLine);
	bool bot = line.checkCollision(botLine);
	bool left = line.checkCollision(leftLine);
	bool right = line.checkCollision(rightLine);

	if (top || bot || left || right)
	{
		return true;
	}

	// Check if the entire line is inside the rectangle
	Point p1 = line.get_p1();
	Point p2 = line.get_p2();

	bool p1Inside = checkCollision(p1);

	bool p2Inside = checkCollision(p2);

	if (p1Inside && p2Inside)
	{
		return true; // The line is entirely inside the rectangle
	}

	return false; // No collision detected
}

bool LibMath::Geometry2D::AABB::checkCollision(Point& point)
{
	if (point.getX() >= getBottomLeftCorner().getX() &&
		point.getX() <= getBottomRightCorner().getX() &&
		point.getY() >= getBottomLeftCorner().getY() &&
		point.getY() <= getTopLeftCorner().getY())
	{
		return true;
	}
	return false;
}

LibMath::Geometry2D::Point LibMath::Geometry2D::AABB::getTopRightCorner(void) 
{
	Point point(m_center.getX() + extentX(), m_center.getY() + extentY());

	return point;
}

LibMath::Geometry2D::Point LibMath::Geometry2D::AABB::getTopLeftCorner(void)
{
	Point point(m_center.getX() - extentX(), m_center.getY() + extentY());

	return point;
}

LibMath::Geometry2D::Point LibMath::Geometry2D::AABB::getBottomRightCorner(void)
{
	Point point(m_center.getX() + extentX(), m_center.getY() - extentY());

	return point;
}

LibMath::Geometry2D::Point LibMath::Geometry2D::AABB::getBottomLeftCorner(void)
{
	Point point(m_center.getX() - extentX(), m_center.getY() - extentY());

	return point;
}
#pragma endregion All functions for AABB

#pragma region OBB

LibMath::Geometry2D::OBB::OBB(Point center, float height, float width)
{
	m_center = center;
	m_height = height;
	m_width = width;
	m_rotation = LibMath::Radian();
}

LibMath::Geometry2D::OBB::OBB(OBB const& other)
{
	m_center = other.m_center;
	m_height = other.m_height;
	m_width = other.m_width;
	m_rotation = other.m_rotation;
}

LibMath::Geometry2D::OBB& LibMath::Geometry2D::OBB::operator=(const OBB& other)
{
	m_center = other.m_center;
	m_height = other.m_height;
	m_width = other.m_width;

	return *this;
}

LibMath::Geometry2D::Point LibMath::Geometry2D::OBB::getCenter(void)
{
	return m_center;
}

float LibMath::Geometry2D::OBB::getHeight(void) const
{
	return m_height;
}

float LibMath::Geometry2D::OBB::getWidth(void) const
{
	return m_width;
}

LibMath::Radian LibMath::Geometry2D::OBB::getRotation(void) const
{
	return m_rotation;
}

void LibMath::Geometry2D::OBB::rotate(LibMath::Radian rad)
{
	m_rotation = rad;
}

LibMath::Geometry2D::Point LibMath::Geometry2D::OBB::getTopRightCorner(void)
{

	float halfHeight = m_height / 2;
	float halfWidth = m_width / 2;

	float cosR = LibMath::cos(m_rotation);
	float sinR = LibMath::sin(m_rotation);

	float x = m_center.getX() + (halfWidth * cosR - halfHeight * sinR);
	float y = m_center.getY() + (halfWidth * sinR + halfHeight * cosR);

	
	Point point(x, y);

	return point;
}

LibMath::Geometry2D::Point LibMath::Geometry2D::OBB::getTopLeftCorner(void)
{
	float halfHeight = m_height / 2;
	float halfWidth = m_width / 2;

	float cosR = LibMath::cos(m_rotation);
	float sinR = LibMath::sin(m_rotation);

	float x = m_center.getX() + (-halfWidth * cosR - halfHeight * sinR);
	float y = m_center.getY() + (-halfWidth * sinR + halfHeight * cosR);

	Point point(x, y);

	return point;
}

LibMath::Geometry2D::Point LibMath::Geometry2D::OBB::getBotRightCorner(void)
{
	float halfHeight = m_height / 2;
	float halfWidth = m_width / 2;

	float cosR = LibMath::cos(m_rotation);
	float sinR = LibMath::sin(m_rotation);

	float x = m_center.getX() + (halfWidth * cosR - (-halfHeight) * sinR);
	float y = m_center.getY() + (halfWidth * sinR + (-halfHeight) * cosR);

	Point point(x, y);

	return point;
}

LibMath::Geometry2D::Point LibMath::Geometry2D::OBB::getBotLeftCorner(void)
{
	float halfHeight = m_height / 2;
	float halfWidth = m_width / 2;

	float cosR = LibMath::cos(m_rotation);
	float sinR = LibMath::sin(m_rotation);

	float x = m_center.getX() + (-halfWidth * cosR - (-halfHeight) * sinR);
	float y = m_center.getY() + (-halfWidth * sinR + (-halfHeight) * cosR);

	Point point(x, y);

	return point;
}

LibMath::Vector2 LibMath::Geometry2D::normalVector(Point p1, Point p2)
{
	float x = p2.getX() - p1.getX();
	float y = p2.getY() - p1.getY();

	LibMath::Vector2 normal(-y, x);

	normal.normalize();

	return normal;
}

bool LibMath::Geometry2D::overlapOnAxis(float& min1, float& max1, float& min2, float& max2)
{
	return !(max1 < min2 || max2 < min1);
}

void LibMath::Geometry2D::projectRectangleOnAxis(OBB& rectangle, const LibMath::Vector2& axis, float& min, float& max)
{
	Point center = rectangle.getCenter();

	LibMath::Radian rotation = rectangle.getRotation();

	Point topRight = rectangle.getTopRightCorner();
	Point topLeft = rectangle.getTopLeftCorner();
	Point botRight = rectangle.getBotRightCorner();
	Point botLeft = rectangle.getBotLeftCorner();
	
	//Compute the corners of the rectangle
	LibMath::Vector2 corners[4] = {
		{topRight.getX(), topRight.getX()},
		{topLeft.getX(), topLeft.getY()},
		{botRight.getX(), botRight.getY()},
		{botLeft.getX(), botLeft.getX()}
	};



	// Project the corners onto the axis and find the min and max projections
	min = max = axis.dotProduct(corners[0]);

	for (int i = 1; i < 4; ++i)
	{
		float projection = axis.dotProduct(corners[i]);
		if (projection < min)
		{
			min = projection;
		}
		if (projection > max)
		{
			max = projection;
		}
	}

}

bool LibMath::Geometry2D::isCollision(OBB& rect1, OBB& rect2)
{
	Point center1 = rect1.getCenter();
	Point center2 = rect2.getCenter();

	// Define the axes to test (normals to the edges of the rectangles)
	LibMath::Vector2 axes[4] =
	{
		{normalVector(rect1.getTopRightCorner(), rect1.getTopLeftCorner())},
		{normalVector(rect1.getTopLeftCorner(), rect1.getBotLeftCorner())},
		{normalVector(rect2.getTopRightCorner(), rect2.getTopLeftCorner())},
		{normalVector(rect2.getTopLeftCorner(), rect2.getBotLeftCorner())}
	};

	float min1, max1, min2, max2;

	for (LibMath::Vector2& axis : axes)
	{
		axis.normalize();
		projectRectangleOnAxis(rect1, axis, min1, max1);
		projectRectangleOnAxis(rect2, axis, min2, max2);
		if (!overlapOnAxis(min1, max1, min2, max2))
		{
			return false; // Separation axis found, no collision
		}
	}
	return true; // No separating axis found, collision occurs
}

#pragma endregion All functions for OBB

#pragma region Circle

LibMath::Geometry2D::Circle::Circle(Point const center, float const radius)
{
	m_center = center;
	m_radius = radius;
}

LibMath::Geometry2D::Circle::Circle(Circle const& other)
{
	m_center = other.m_center;
	m_radius = other.m_radius;

}

LibMath::Geometry2D::Circle& LibMath::Geometry2D::Circle::operator=(const Circle& other)
{
	m_center = other.m_center;
	m_radius = other.m_radius;

	return *this;
}

float LibMath::Geometry2D::Circle::perimeter(void) const
{
	return static_cast<float>(M_PI) * 2 * m_radius;
}

float LibMath::Geometry2D::Circle::area(void) const
{
	return static_cast<float>(M_PI) * powf(m_radius, 2);
}

bool LibMath::Geometry2D::Circle::checkCollision(Circle& circle)
{
	// Distance between the center of the two circle
	float distance = sqrtf(powf(m_center.getX() - circle.m_center.getX(), 2) + powf(m_center.getY() - circle.m_center.getY(), 2));

	if (distance <= m_radius + circle.m_radius)
	{
		return true;
	}
	return false;
}

bool LibMath::Geometry2D::Circle::checkCollision(Line& line)
{
	// Check if one of the edges of the line is in the circle
	bool inside1 = checlCollision(line.get_p1());
	bool inside2 = checlCollision(line.get_p2());

	if (inside1 || inside2)
	{
		return true;
	}

	// compute the distance between the circle and it's closest point to the line
	// dot product of the vectors v1(circle - edge1)
	float dot = ((m_center.getX() - line.get_p1().getX()) * (line.get_p2().getX() - line.get_p1().getX())) +
				((m_center.getY() - line.get_p1().getY())) * (line.get_p2().getY() - line.get_p1().getY());

	dot = (dot / line.lenghtSquare());
	
	//Clamp the dot product to the range[0, 1] to keep the closest point on the segment
	dot = std::max(0.0f, std::min(1.0f, dot));

	float closestX = line.get_p1().getX() + (dot * (line.get_p2().getX() - line.get_p1().getX()));
	float closestY = line.get_p1().getY() + (dot * (line.get_p2().getY() - line.get_p1().getY()));

	//distance between the closest point and the circle
	float distX = closestX - m_center.getX();
	float distY = closestY - m_center.getY();

	float distance = sqrtf(powf(distX, 2) + powf(distY, 2));

	// check if the distance is smaller the circle's radius
	if (distance <= m_radius )
	{
		return true;
	}

	return false;

}

bool LibMath::Geometry2D::Circle::checlCollision(Point& point)
{
	// distance between circle radius and the point
	float distance  = sqrtf(powf(m_center.getX() - point.getX(), 2) + powf(m_center.getY() - point.getY(), 2));

	if (distance <= m_radius)
	{
		return true;
	}

	return false;
}
#pragma endregion 
