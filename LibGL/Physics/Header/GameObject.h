#pragma once
#include "Collider.h"
#include "Mesh.h"
#include "LibMath/Matrix.h"
#include "string"

class GameObject
{
public:
	GameObject() = default;
	GameObject(std::shared_ptr<Mesh> mesh, const ColliderType& collidertype, const LibMath::Vector3& playerCenter = LibMath::Vector3(0.f, 0.f, 0.f));
	~GameObject();

	// Getters
	Collider&						getCollider() { return m_collider; }
	const Collider&					getCollider() const { return m_collider; }
	std::shared_ptr<Mesh>			getMesh() { return m_mesh; }
	const std::shared_ptr<Mesh>		getMesh() const { return m_mesh; }

	void							setTag(const std::string& tag) { m_tag = tag; }
	const std::string&				getTag() const { return m_tag; }

private:
	std::shared_ptr<Mesh>			m_mesh;
	Collider						m_collider;
	std::string						m_tag;
};