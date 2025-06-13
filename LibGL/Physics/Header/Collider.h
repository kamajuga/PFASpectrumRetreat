#pragma once
#include "LibMath/GeometricObject3.h"
#include "Mesh.h"
#include <memory>

namespace Geometry3D = LibMath::Geometry3D;

struct MeshDimensions {
	float	width;
	float	height;
	float	depth;
};

enum ColliderType
{
	BOX,
	CAPSULE,
	SPHERE,
	PLAYER
};

class Collider
{
public:
	Collider() = default;
	Collider(const Collider& other); 
	Collider& operator=(const Collider& other); 
	Collider(const ColliderType& CollideType, Mesh& mesh, const LibMath::Vector3& playerCenter = LibMath::Vector3(0, 0, 0));

	void					updateCollider(LibMath::Matrix4& transMat);

	~Collider();

	Geometry3D::AABB*		getAABB() const 
	{
		return dynamic_cast<Geometry3D::AABB*>(m_ColliderShape);
	}

	Geometry3D::Capsule*	getCapsule() const 
	{
		return dynamic_cast<Geometry3D::Capsule*>(m_ColliderShape);
	}

	Geometry3D::Sphere*		getSphere() const 
	{	
		return dynamic_cast<Geometry3D::Sphere*>(m_ColliderShape);
	}

	template<typename T>
	T*						getShape() const 
	{
		return dynamic_cast<T*>(m_ColliderShape);
	}

	static MeshDimensions	getboxCollider(Model& model);

	Geometry3D::Object3D*	m_ColliderShape;
private:
	
	void					playerCollider(const LibMath::Vector3& center, float height, float radius);
	void					boxCollider(Mesh& mesh);
	void					capsuleCollider(Mesh& mesh);
	void					sphereCollider(Mesh& mesh);

	
};

