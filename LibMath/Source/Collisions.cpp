#include "LibMath/Collisions.h"
#include "LibMath/Arithmetic.h"

#pragma region Sphere Collision 3D
bool LibMath::Collisions3D::checkCollision(const Geometry3D::Sphere& sphere1, const Geometry3D::Sphere& sphere2)
{
	// Distance between the center of the two sphere
	float distance = sqrtf(powf(sphere1.center().getX() - sphere2.center().getX(), 2) +
		powf(sphere1.center().getY() - sphere2.center().getY(), 2) +
		powf(sphere1.center().getZ() - sphere2.center().getZ(), 2));

	// Check if the distance is less than the sum of the radius of the two spheres
	if (distance <= (sphere1.radius() + sphere2.radius()))
		return true;

	// No collision
	return false;
}

bool LibMath::Collisions3D::checkCollision(const Geometry3D::Sphere& sphere, const Geometry3D::Point& point)
{
	// Distance between the center of the sphere and the point
	float distance = sqrtf(powf(sphere.center().getX() - point.getX(), 2) +
		powf(sphere.center().getY() - point.getY(), 2) +
		powf(sphere.center().getZ() - point.getZ(), 2));

	// Check if the distance is less than the radius of the sphere
	if (distance <= sphere.radius())
		return true;

	// No collision
	return false;
}

bool LibMath::Collisions3D::checkCollision(const Geometry3D::Sphere& sphere, const Geometry3D::Line& line)
{
	Vector3 lineToSphere = sphere.center().toVector3() - line.getOrigin().toVector3();
	Vector3 lineDirection = line.getDirection();
	lineDirection.normalize();

	Vector3 center = sphere.center().toVector3();

	// Calculate the discriminant

	float d = lineToSphere.dot(lineDirection); 
	
	float c = lineToSphere.dot(lineToSphere) - (sphere.radius() * sphere.radius());

	float discriminant = d * d - c;

	// Check if the discriminant is less than 0
	if (discriminant < 0)
		// No collision
		return false;

	return true;
}

bool LibMath::Collisions3D::checkCollision(const Geometry3D::Sphere& sphere, const Geometry3D::Plan& plan)
{
	Vector3 planNormal = plan.normal();
	float planDistance = plan.distance();

	planNormal.normalize();

	// Calculate the distance between the center of the sphere and the plan

	// Find a point on the plan
	// Let a point of coordinate = (0, 0, z) z = -d/nz
	float z = - planDistance / planNormal.getZ();
	Vector3 pointOnPlan(0, 0, z);

	// Calculate the distancec from the sphere center to plan 
	Vector3 sphereCenter = sphere.center().toVector3();
	float distance = std::abs(planNormal.dot(sphereCenter) + planDistance);

	// Check if the distance is less than the radius of the sphere
	if (distance <= sphere.radius())
		return true;

	// No collision
	return false;
}

#pragma endregion All functions Sphere 3D


#pragma region Plan Collision 3D

bool LibMath::Collisions3D::checkCollision(const Geometry3D::Plan& plan1, const Geometry3D::Plan& plan2)
{
	Vector3 plan1Normal = plan1.normal();
	Vector3 plan2Normal = plan2.normal();

	Vector3 crossProduct = plan1Normal.cross(plan2Normal);

	// Check if the two plans are parallel
	if (crossProduct.magnitude() == 0.f)
	{
		// Check if the two plans are overlapping
		if (std::abs(plan1.distance()) - std::abs(plan2.distance()) == 0)
			return true;
		else
			return false;
	}	

	// Collision
	return true;
}

bool LibMath::Collisions3D::checkCollision(const Geometry3D::Plan& plan, const Geometry3D::Line& line)
{
	Vector3 planNormal = plan.normal();

	
	// Calculate the dot product between the plan normal and the line direction

	float dotProduct = planNormal.dot(line.getDirection());

	// Check if the dot product is equal to 0
	if (dotProduct == 0.f)
	{
		return checkCollision(plan, line.getOrigin());
		// The line is parallel to the plan

	}
	 
	return true;
}

bool LibMath::Collisions3D::checkCollision(const Geometry3D::Plan& plan, const Geometry3D::Point& point)
{
	Vector3 planNormal = plan.normal();

	planNormal.normalize();

	float distance = planNormal.dot(point.toVector3()) - plan.distance();

	return distance == 0.f;
}
#pragma endregion All functions Plan 3D


#pragma region Capsule Collision 3D

bool LibMath::Collisions3D::checkCollision(const Geometry3D::Capsule& capsule, const Geometry3D::Point& point)
{
	// Calculate the vector from point A to point B of the capsule's central axis
	LibMath::Vector3 capsuleAxisDirection(capsule.m_pointB.toVector3() - capsule.m_pointA.toVector3());

	// Calculate the vector from point A to the test point
	LibMath::Vector3 pointToAxisStart(point.toVector3() - capsule.m_pointA.toVector3());

	// Project the point onto the capsule's central axis using dot product
	float projectionParameter = pointToAxisStart.dot(capsuleAxisDirection) / capsuleAxisDirection.magnitudeSquared();

	// Clamp t to [0,1] to ensure the projection stays within the capsule's cylinder part
	projectionParameter = LibMath::clamp(projectionParameter, 0.f, 1.f);

	// Calculate the closest point on the capsule's central axis to our test point
	// This is the projected point using the clamped parametric value
	Geometry3D::Point closestPointOnAxis(capsule.m_pointA.getX() + (projectionParameter * capsuleAxisDirection.getX()),
		capsule.m_pointA.getY() + (projectionParameter * capsuleAxisDirection.getY()),
		capsule.m_pointA.getZ() + (projectionParameter * capsuleAxisDirection.getZ()));

	// Calculate the distance from the test point to the closest point on the capsule's axis
	float distanceToAxis = (point.toVector3() - closestPointOnAxis.toVector3()).magnitude();

	// Check if the distance is within the capsule's radius
	if (distanceToAxis <= capsule.m_radius)
	{
		return true;  // Point is inside or on the capsule
	}
	return false;     // Point is outside the capsule
}

bool LibMath::Collisions3D::checkCollision(const Geometry3D::Capsule& capsule, const Geometry3D::Line& line)
{
	// Calculate the vector from point A to point B of the capsule's central axis
	LibMath::Vector3 capsuleAxisDirection(capsule.m_pointB.toVector3() - capsule.m_pointA.toVector3());

	// Calculate the vector from capsule's point A to the line's origin
	LibMath::Vector3 lineOriginToAxisStart(line.m_origin.toVector3() - capsule.m_pointA.toVector3());

	// Project the line origin onto the capsule's central axis using dot product
	// This gives us the parametric value along the capsule's axis
	float capsuleAxisProjectionParameter = lineOriginToAxisStart.dot(capsuleAxisDirection) / capsuleAxisDirection.magnitudeSquared();

	// Clamp parameter to [0,1] to ensure the projection stays within the capsule's cylinder part
	capsuleAxisProjectionParameter = LibMath::clamp(capsuleAxisProjectionParameter, 0.f, 1.f);

	// Calculate the closest point on the capsule's axis to the line's origin
	Geometry3D::Point closestPointOnCapsuleAxis(
		capsule.m_pointA.getX() + (capsuleAxisProjectionParameter * capsuleAxisDirection.getX()),
		capsule.m_pointA.getY() + (capsuleAxisProjectionParameter * capsuleAxisDirection.getY()),
		capsule.m_pointA.getZ() + (capsuleAxisProjectionParameter * capsuleAxisDirection.getZ()));

	// Normalize the line direction for proper projection calculations
	LibMath::Vector3 normalizedDirection = line.m_direction;
	normalizedDirection.normalize();

	// Project the closest capsule point onto the line to find the closest point on the line
	float lineProjectionParameter = (closestPointOnCapsuleAxis.toVector3() - line.m_origin.toVector3()).dot(normalizedDirection);

	// Calculate the closest point on the line to the capsule's axis
	Geometry3D::Point closestPointOnLine(
		line.m_origin.getX() + (lineProjectionParameter * normalizedDirection.getX()),
		line.m_origin.getY() + (lineProjectionParameter * normalizedDirection.getY()),
		line.m_origin.getZ() + (lineProjectionParameter * normalizedDirection.getZ()));

	// Calculate the shortest distance between the capsule's axis and the line
	float shortestDistance = (closestPointOnCapsuleAxis.toVector3() - closestPointOnLine.toVector3()).magnitude();

	// If the distance is greater than the capsule's radius, check the end caps (spheres)
	if (shortestDistance > capsule.m_radius)
	{
		// Check collision with the first end cap (sphere at point A)
		Geometry3D::Sphere firstEndCapSphere(capsule.m_pointA, capsule.m_radius);
		if (!checkCollision(firstEndCapSphere, line))
		{
			// Check collision with the second end cap (sphere at point B)
			Geometry3D::Sphere secondEndCapSphere(capsule.m_pointB, capsule.m_radius);

			if (!checkCollision(secondEndCapSphere, line))
			{
				return false; // No collision with either end cap
			}
		}
	}

	return true; // Collision detected either with cylinder part or end caps
}

#include <algorithm>

bool LibMath::Collisions3D::checkCollision(const Geometry3D::Capsule& capsule, const Geometry3D::AABB& aabb)
{
	// Find the closest point on the capsule's segment to the AABB's center
	Geometry3D::Point segmentPointToConsider = getClosestToSegment(capsule.m_pointA, capsule.m_pointB, aabb.center());

	// Find the closest point on the AABB to that segment point
	Geometry3D::Point aabbClosestPoint = getClosestToAABB(aabb, segmentPointToConsider);

	// Find the closest point on the segment to the AABB's closest point
	Geometry3D::Point finalSegmentClosestPoint = getClosestToSegment(capsule.m_pointA, capsule.m_pointB, aabbClosestPoint);

	// Calculate the squared distance between these two final closest points
	float distanceSquared = finalSegmentClosestPoint.getDistanceSquared(aabbClosestPoint);

	// Return true if the squared distance is less than or equal to the capsule's squared radius
	return distanceSquared <= (capsule.m_radius * capsule.m_radius);
}
#pragma endregion 

#pragma region AABB Collision 3D

// Implement AABB collision detection functions here
bool LibMath::Collisions3D::checkCollision(const Geometry3D::AABB& aabb1, const Geometry3D::AABB& aabb2)
{
	// Check if the AABBs overlap on all axes
	return (aabb1.center().getX() - aabb1.extentX() <= aabb2.center().getX() + aabb2.extentX() &&
		aabb1.center().getX() + aabb1.extentX() >= aabb2.center().getX() - aabb2.extentX() &&
		aabb1.center().getY() - aabb1.extentY() <= aabb2.center().getY() + aabb2.extentY() &&
		aabb1.center().getY() + aabb1.extentY() >= aabb2.center().getY() - aabb2.extentY() &&
		aabb1.center().getZ() - aabb1.extentZ() <= aabb2.center().getZ() + aabb2.extentZ() &&
		aabb1.center().getZ() + aabb1.extentZ() >= aabb2.center().getZ() - aabb2.extentZ());
}

bool LibMath::Collisions3D::checkCollision(const Geometry3D::AABB& aabb, const Geometry3D::Line& line)
{
	LibMath::Vector3 origin = line.m_origin.toVector3();
	LibMath::Vector3 direction = line.m_direction;
	float maxLength = line.m_length;

	LibMath::Vector3 invDir(
		(direction.getX() != 0.0f) ? 1.0f / direction.getX() : std::numeric_limits<float>::max(),
		(direction.getY() != 0.0f) ? 1.0f / direction.getY() : std::numeric_limits<float>::max(),
		(direction.getZ() != 0.0f) ? 1.0f / direction.getZ() : std::numeric_limits<float>::max()
	);

	LibMath::Vector3 min;
	LibMath::Vector3 max;
	min.getX() = aabb.center().getX() - aabb.extentX();
	min.getY() = aabb.center().getY() - aabb.extentY();
	min.getZ() = aabb.center().getZ() - aabb.extentZ();

	max.getX() = aabb.center().getX() + aabb.extentX();
	max.getY() = aabb.center().getY() + aabb.extentY();
	max.getZ() = aabb.center().getZ() + aabb.extentZ();

	float t1 = (min.getX() - origin.getX()) * invDir.getX();
	float t2 = (max.getX() - origin.getX()) * invDir.getX();
	float tMinX = std::min(t1, t2);
	float tMaxX = std::max(t1, t2);

	t1 = (min.getY() - origin.getY()) * invDir.getY();
	t2 = (max.getY() - origin.getY()) * invDir.getY();
	float tMinY = std::min(t1, t2);
	float tMaxY = std::max(t1, t2);

	t1 = (min.getZ() - origin.getZ()) * invDir.getZ();
	t2 = (max.getZ() - origin.getZ()) * invDir.getZ();
	float tMinZ = std::min(t1, t2);
	float tMaxZ = std::max(t1, t2);

	float tEnter = std::max({ tMinX, tMinY, tMinZ });
	float tExit = std::min({ tMaxX, tMaxY, tMaxZ });

	if (tEnter <= tExit && tExit >= 0.0f && tEnter <= maxLength)
		return true;

	return false;
}

#pragma endregion AABB Collision 3D