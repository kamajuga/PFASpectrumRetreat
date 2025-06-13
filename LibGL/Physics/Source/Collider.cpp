#include "Collider.h"
#include "LibMath/Matrix4Vector4Operation.h"
#include <LibMath/Collisions.h>

Collider::Collider(const Collider& other)
{
	if (other.m_ColliderShape) 
	{
        if (auto* aabb = dynamic_cast<Geometry3D::AABB*>(other.m_ColliderShape)) {
            m_ColliderShape = new Geometry3D::AABB(*aabb);
        }
        else if (auto* capsule = dynamic_cast<Geometry3D::Capsule*>(other.m_ColliderShape)) {
            m_ColliderShape = new Geometry3D::Capsule(*capsule);
        }
        else if (auto* sphere = dynamic_cast<Geometry3D::Sphere*>(other.m_ColliderShape)) {
            m_ColliderShape = new Geometry3D::Sphere(*sphere);
        }
		else
		{
			m_ColliderShape = nullptr;
		}
    }
	else
	{
		m_ColliderShape = nullptr;
	}
}

Collider&		Collider::operator=(const Collider& other)
{
	if (this != &other) 
	{
		//delete m_ColliderShape;
		m_ColliderShape = nullptr;

		if (other.m_ColliderShape)
		{
			if (auto* aabb = dynamic_cast<Geometry3D::AABB*>(other.m_ColliderShape)) 
			{
				m_ColliderShape = new Geometry3D::AABB(*aabb);
			}
			else if (auto* capsule = dynamic_cast<Geometry3D::Capsule*>(other.m_ColliderShape)) 
			{
				m_ColliderShape = new Geometry3D::Capsule(*capsule);
			}
			else if (auto* sphere = dynamic_cast<Geometry3D::Sphere*>(other.m_ColliderShape)) 
			{
				m_ColliderShape = new Geometry3D::Sphere(*sphere);
			}
		}
	}
	return *this;
}

Collider::Collider(const ColliderType& Type, Mesh& mesh, const LibMath::Vector3& playerCenter)
{
	switch (Type)
	{
	case ColliderType::BOX:
		boxCollider(mesh);
		break;

	case ColliderType::CAPSULE:
		capsuleCollider(mesh);
		break;

	case ColliderType::SPHERE:
		sphereCollider(mesh);
		break;
	case ColliderType::PLAYER:
		playerCollider(playerCenter, 4, 2);
		break;

	default:
		m_ColliderShape = nullptr;
		break;
	}
}

Collider::~Collider()
{
	if (m_ColliderShape)
	{
		delete m_ColliderShape;
	}
}

MeshDimensions	Collider::getboxCollider(Model& model)
{
	std::vector<Vertex> vertices = model.getVertices();
	MeshDimensions dims = { 0.0f, 0.0f, 0.0f };

	if (vertices.empty()) return MeshDimensions();

	if (!vertices.empty()) 
	{
		float minX = vertices[0].m_position.getX(), maxX = vertices[0].m_position.getX();
		float minY = vertices[0].m_position.getY(), maxY = vertices[0].m_position.getY();
		float minZ = vertices[0].m_position.getZ(), maxZ = vertices[0].m_position.getZ();

		for (const auto& vertex : vertices) 
		{
			minX = std::min(minX, vertex.m_position.getX());
			maxX = std::max(maxX, vertex.m_position.getX());
			minY = std::min(minY, vertex.m_position.getY());
			maxY = std::max(maxY, vertex.m_position.getY());
			minZ = std::min(minZ, vertex.m_position.getZ());
			maxZ = std::max(maxZ, vertex.m_position.getZ());
		}

		dims.width = maxX - minX;
		dims.height = maxY - minY;
		dims.depth = maxZ - minZ;
	}

	return dims;
}

void			Collider::updateCollider(LibMath::Matrix4& transMat)
{
	m_ColliderShape->update(transMat);
}

void			Collider::playerCollider(const LibMath::Vector3& center, float height, float radius)
{
	// Point du bas de la capsule (pied du joueur)
	LibMath::Vector3 bottomPoint = center;
	bottomPoint.getY() -= (height / 2.0f - radius); // Soustraire la demi-hauteur moins le rayon

	// Point du haut de la capsule (tête du joueur)  
	LibMath::Vector3 topPoint = center;
	topPoint.getY() += (height / 2.0f - radius); // Ajouter la demi-hauteur moins le rayon

	// Créer les objets Point à partir des Vector3
	Geometry3D::Point pointA(bottomPoint.getX(), bottomPoint.getY(), bottomPoint.getZ());
	Geometry3D::Point pointB(topPoint.getX(), topPoint.getY(), topPoint.getZ());

	m_ColliderShape = new Geometry3D::Capsule(pointA, pointB, radius);
}

void			Collider::boxCollider(Mesh& mesh)
{
	std::vector<Vertex> vertices = mesh.getModelVertices();
	if (vertices.empty()) return;

	LibMath::Matrix4 modelMatrix = mesh.getModelMatrix();

	// Reste de votre code...
	LibMath::Vector3 min = { std::numeric_limits<float>::max(),
							std::numeric_limits<float>::max(),
							std::numeric_limits<float>::max() };
	LibMath::Vector3 max = { std::numeric_limits<float>::lowest(),
							std::numeric_limits<float>::lowest(),
							std::numeric_limits<float>::lowest() };

	for (const auto& vertex : vertices) 
	{
		LibMath::Vector4 worldPos = modelMatrix * LibMath::Vector4(
			vertex.m_position.getX(),
			vertex.m_position.getY(),
			vertex.m_position.getZ(),
			1.0f);

		min.getX() = std::min(min.getX(), worldPos.getX());
		min.getY() = std::min(min.getY(), worldPos.getY());
		min.getZ() = std::min(min.getZ(), worldPos.getZ());
		max.getX() = std::max(max.getX(), worldPos.getX());
		max.getY() = std::max(max.getY(), worldPos.getY());
		max.getZ() = std::max(max.getZ(), worldPos.getZ());
	}

	// DEBUG: Afficher les dimensions calculées
	float width = max.getX() - min.getX();
	float height = max.getY() - min.getY();
	float depth = max.getZ() - min.getZ();


	Geometry3D::Point center = 
	{
		(min.getX() + max.getX()) / 2.0f,
		(min.getY() + max.getY()) / 2.0f,
		(min.getZ() + max.getZ()) / 2.0f
	};

	m_ColliderShape = new Geometry3D::AABB(center, width, height, depth);
}

void			Collider::capsuleCollider(Mesh& mesh)
{
	std::vector<Vertex> vertices = mesh.getModelVertices();
	if (vertices.size() < 2) return;

	// Get the mesh's model matrix
	LibMath::Matrix4 modelMatrix = mesh.getModelMatrix(); // or mesh.getTransform().getMatrix()

	// Transform all vertices and store them
	std::vector<LibMath::Vector3> transformedVertices;
	transformedVertices.reserve(vertices.size());

	for (const auto& vertex : vertices)
	{
		// Transform vertex by model matrix
		LibMath::Vector4 worldPos = /*modelMatrix **/ LibMath::Vector4(vertex.m_position.getX(),
			vertex.m_position.getY(),
			vertex.m_position.getZ(),
			1.0f);

		// Store transformed position
		transformedVertices.push_back({ worldPos.getX(), worldPos.getY(), worldPos.getZ() });
	}

	// Find the two farthest points using transformed vertices
	LibMath::Vector3 farthest_p1 = transformedVertices[0];
	LibMath::Vector3 farthest_p2 = transformedVertices[1];
	float max_dist_sq = 0.0f;

	for (size_t i = 0; i < transformedVertices.size(); ++i)
	{
		for (size_t j = i + 1; j < transformedVertices.size(); ++j)
		{
			float dist_sq = (transformedVertices[i].getX() - transformedVertices[j].getX()) * (transformedVertices[i].getX() - transformedVertices[j].getX()) +
				(transformedVertices[i].getY() - transformedVertices[j].getY()) * (transformedVertices[i].getY() - transformedVertices[j].getY()) +
				(transformedVertices[i].getZ() - transformedVertices[j].getZ()) * (transformedVertices[i].getZ() - transformedVertices[j].getZ());
			if (dist_sq > max_dist_sq) {
				max_dist_sq = dist_sq;
				farthest_p1 = transformedVertices[i];
				farthest_p2 = transformedVertices[j];
			}
		}
	}

	// Calculate approximate radius based on transformed distance
	float approximate_radius = std::sqrt(max_dist_sq) / 2.0f;

	m_ColliderShape = new Geometry3D::Capsule(Geometry3D::Point{ farthest_p1.getX(), farthest_p1.getY(), farthest_p1.getZ() },
		Geometry3D::Point{ farthest_p2.getX(), farthest_p2.getY(), farthest_p2.getZ() },
		approximate_radius);
}

void			Collider::sphereCollider(Mesh& mesh)
{
	std::vector<Vertex> vertices = mesh.getModelVertices();
	if (vertices.empty()) return;

	// Get the mesh's model matrix
	LibMath::Matrix4 modelMatrix = mesh.getModelMatrix(); // or mesh.getTransform().getMatrix()

	LibMath::Vector3 min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
	LibMath::Vector3 max = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
	LibMath::Vector3 sum = { 0.0f, 0.0f, 0.0f };

	// First pass: transform vertices and calculate bounds + sum
	std::vector<LibMath::Vector3> transformedVertices;
	transformedVertices.reserve(vertices.size());

	for (const auto& vertex : vertices)
	{
		// Transform vertex by model matrix
		LibMath::Vector4 worldPos = /*modelMatrix **/ LibMath::Vector4(vertex.m_position.getX(),
			vertex.m_position.getY(),
			vertex.m_position.getZ(),
			1.0f);

		// Store transformed position
		LibMath::Vector3 transformedPos = { worldPos.getX(), worldPos.getY(), worldPos.getZ() };
		transformedVertices.push_back(transformedPos);

		// Calculate bounds with transformed positions
		min.getX() = std::min(min.getX(), transformedPos.getX());
		min.getY() = std::min(min.getY(), transformedPos.getY());
		min.getZ() = std::min(min.getZ(), transformedPos.getZ());
		max.getX() = std::max(max.getX(), transformedPos.getX());
		max.getY() = std::max(max.getY(), transformedPos.getY());
		max.getZ() = std::max(max.getZ(), transformedPos.getZ());

		// Calculate sum for center computation
		sum.getX() += transformedPos.getX();
		sum.getY() += transformedPos.getY();
		sum.getZ() += transformedPos.getZ();
	}

	// Calculate center with transformed positions
	LibMath::Vector3 center_vec = { sum.getX() / vertices.size(), sum.getY() / vertices.size(), sum.getZ() / vertices.size() };

	// Calculate maximum radius with transformed positions
	float max_radius_sq = 0.0f;
	for (const auto& transformedPos : transformedVertices)
	{
		float dist_sq = (transformedPos.getX() - center_vec.getX()) * (transformedPos.getX() - center_vec.getX()) +
			(transformedPos.getY() - center_vec.getY()) * (transformedPos.getY() - center_vec.getY()) +
			(transformedPos.getZ() - center_vec.getZ()) * (transformedPos.getZ() - center_vec.getZ());
		max_radius_sq = std::max(max_radius_sq, dist_sq);
	}

	float radius = std::sqrt(max_radius_sq);
	m_ColliderShape = new Geometry3D::Sphere(Geometry3D::Point{ center_vec.getX(), center_vec.getY(), center_vec.getZ() }, radius);
}
