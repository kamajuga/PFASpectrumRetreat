#include "SceneGraph.h"

void SceneNode::addChild(std::shared_ptr<SceneNode> child)
{
	child->m_parent = this;
	m_children.push_back(child);
}

void SceneNode::addChild(std::shared_ptr<Mesh> mesh, Shader* shader, const LibMath::Matrix4& transformMat)
{
	std::shared_ptr<SceneNode> childNode = std::make_shared<SceneNode>();
	childNode->m_mesh = mesh;
	childNode->m_shader = shader;
	childNode->m_localTransform = transformMat;

	addChild(childNode);
}

void SceneNode::initialize(std::shared_ptr<Mesh> mesh, Shader* shader, const LibMath::Matrix4& transformMat)
{
	m_mesh = mesh;
	m_shader = shader;
	m_localTransform = transformMat;
}

void SceneNode::update(LibMath::Matrix4 const& parentTransform)
{
	m_worldTransform = parentTransform * m_localTransform;

	if (m_mesh) m_mesh->setModelMatrix(m_worldTransform);

	

	for (std::shared_ptr<SceneNode> child : m_children)
	{
		child->update(m_worldTransform);
	}

}

void SceneNode::draw(const LibMath::Matrix4& VPMatrix)
{
	if (m_mesh && m_shader)
	{
		m_mesh->draw(*m_shader, VPMatrix);
	}

	for(std::shared_ptr<SceneNode> child : m_children)
	{
		child->draw(VPMatrix);
	}
}

void SceneNode::deleteRecursive(void)
{
	if (m_children.empty()) return;


}

SceneNode::~SceneNode()
{
	m_children.clear();
}


void GameObjectNode::addChild(std::shared_ptr<GameObjectNode> child)
{
	child->m_parent = this;
	m_children.push_back(child);
}

void GameObjectNode::addChild(std::shared_ptr<GameObject> gameObject, Shader* shader, const LibMath::Matrix4& transformMat)
{
	std::shared_ptr<GameObjectNode> childNode = std::make_shared<GameObjectNode>();
	childNode->m_gameobject = gameObject;
	childNode->m_shader = shader;
	childNode->m_localTransform = transformMat;

	addChild(childNode);
}

void GameObjectNode::initialize(std::shared_ptr<GameObject> gameObject, Shader* shader, const LibMath::Matrix4& transformMat)
{
	m_gameobject = gameObject;
	m_shader = shader;
	m_localTransform = transformMat;
}

void GameObjectNode::update(LibMath::Matrix4 const& parentTransform)
{
	m_worldTransform = parentTransform * m_localTransform;

	if (m_gameobject && m_gameobject->getMesh())
	{
		
		m_gameobject->getMesh()->setModelMatrix(m_worldTransform);
		m_gameobject->getCollider().updateCollider(m_worldTransform);
		//std::cout << m_gameobject->getCollider().m_ColliderShape->getCenter() << std::endl;
	}

	for (std::shared_ptr<GameObjectNode> child : m_children)
	{
		child->update(m_worldTransform);
	}
}

void GameObjectNode::draw(const LibMath::Matrix4& VPMatrix)
{
	if (m_gameobject && m_gameobject->getMesh() && m_shader)
	{
		m_gameobject->getMesh()->draw(*m_shader, VPMatrix);
	}

	for (std::shared_ptr<GameObjectNode> child : m_children)
	{
		child->draw(VPMatrix);
	}
}

void GameObjectNode::deleteRecursive(void)
{
	if (m_children.empty()) return;

}

GameObjectNode::~GameObjectNode()
{
	m_children.clear();
}