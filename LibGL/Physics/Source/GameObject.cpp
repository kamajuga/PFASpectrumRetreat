#include "GameObject.h"

GameObject::GameObject(std::shared_ptr<Mesh> mesh, const ColliderType& collidertype, const LibMath::Vector3& playerCenter)
{
	m_mesh = mesh;
	m_collider = Collider(collidertype, *mesh, playerCenter);
}

GameObject::~GameObject()
{
	if (m_mesh)
	{
		m_mesh = nullptr;
		//delete m_mesh;
	}
}
