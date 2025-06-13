#pragma once
#pragma once
#include "GeometricObject3.h"

namespace LibMath
{
	namespace Collisions3D
	{
		// Sphere Collisions
		bool checkCollision(const Geometry3D::Sphere& sphere1, const Geometry3D::Sphere& sphere2);
		bool checkCollision(const Geometry3D::Sphere& sphere, const Geometry3D::Point& point);
		bool checkCollision(const Geometry3D::Sphere& sphere, const Geometry3D::Line& line);
		bool checkCollision(const Geometry3D::Sphere& sphere, const Geometry3D::Plan& plan);

		// Plan Collisions
		bool checkCollision(const Geometry3D::Plan& plan1, const Geometry3D::Plan& plan2);
		bool checkCollision(const Geometry3D::Plan& plan, const Geometry3D::Line& line);
		bool checkCollision(const Geometry3D::Plan& plan, const Geometry3D::Point& point);


		// Capsule Collisions
		bool checkCollision(const Geometry3D::Capsule& capsule, const Geometry3D::Point& point);
		bool checkCollision(const Geometry3D::Capsule& capsule, const Geometry3D::Line& line);

		//AABB Collisions
		bool checkCollision(const Geometry3D::AABB& aabb, const Geometry3D::Line& line);
		bool checkCollision(const Geometry3D::AABB& aabb1, const Geometry3D::AABB& aabb2);
		bool checkCollision(const Geometry3D::Capsule& capsule, const Geometry3D::AABB& aabb);
	}
}