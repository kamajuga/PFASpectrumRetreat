#pragma once
#include "SceneGraph.h"
#include "Shader.h"
#include "LibMath/Vector.h"
#include "vector"
#include "GameObject.h"
#include "LibMath/GeometricObject3.h"
#include "iostream"
#include "UiManager.h"
#include "LightManager.h"
#include "Camera.h"
#include "SkyBox.h"
#include <GLFW/glfw3.h>
#include <irrKlang.h>

struct AnimationData 
{
	float time = 0.0f;
	float amplitude = 0.05f;  // Distance du va-et-vient
	float speed = 1.f;      // Vitesse
};

struct ModelSet 
{
	std::shared_ptr<Model> wall, door, floor, cube;
};

struct          ObjectTransform 
{
	std::string			type;
	LibMath::Vector3	position;
	LibMath::Vector3	scale;
	LibMath::Vector3	rotation;
	LibMath::Vector3	animationAxe;
	AnimationData		animationData;
	std::unordered_map<std::string, std::string> extras;
};

class GameManager
{
public:
	GameManager() = default;
	~GameManager() = default;

	void														generateLevelFile(Shader* shader);
	void														checkRay(Graph<GameObjectNode> gameObjects, LibMath::Geometry3D::Line& line, GLFWwindow* window);
	void														swapColorBetweenObjects(GameObject& gameObject);
	void														updateAnimation(float deltaTime);
	void 														teleportePlayer(std::shared_ptr<GameObject> gameObject, Camera& camera);

	void														getNearbyDoors(Camera& Camera);

	Graph<GameObjectNode>										m_doorsScene;
	Graph<GameObjectNode>										m_goScene;
	std::vector<std::shared_ptr<GameObjectNode>>				m_collisionsTocheck;

	Graph<GameObjectNode>										m_animatedScene;
	std::vector<std::pair<AnimationData, LibMath::Vector3>>		m_animationDatas;

	std::vector<std::shared_ptr<GameObject>>					m_nearbyObjects;
	
	bool														m_ctrlPressedLastFrame = false;
	bool														m_clickedLastFrame = false;
	bool														m_escapePressedLastFrame = false;
	bool														m_isPaused = false;
	bool														m_restart = false;
	UiManager													m_uiManager;
	LightManager												m_lightManager;

	Camera														m_camera; 
	irrklang::ISoundEngine*										gameMusic = nullptr;
	Shader														m_shader;
	Shader														m_rayShader;
	SkyBox														m_skyBox;

private:
	std::shared_ptr<GameObjectNode>								createSceneRoot(Shader* shader);
	ModelSet													loadModels();
	void														bindModels(const ModelSet& models);
	ObjectTransform												parseObjectLine(const std::string& line);
	void														handleWall(const ObjectTransform& obj, std::shared_ptr<GameObjectNode> sceneNode, 
																		   Shader* shader, std::shared_ptr<Model> model);
	void														handleFloor(const ObjectTransform& obj, std::shared_ptr<GameObjectNode> sceneNode, 
																			Shader* shader, std::shared_ptr<Model> floor);
	void														handleCube(ObjectTransform& obj, Shader* shader, std::shared_ptr<Model> cube);
	void														handleDoor(const ObjectTransform& obj, Shader* shader, std::shared_ptr<Model> door);
	void														collectCollisionObjects(const GameObjectNode& node);
	void														collectCollisionObjects();

};

inline float getOffset(AnimationData& anim, float deltaTime) {
	anim.time += deltaTime;
	return sin(anim.time * anim.speed) * anim.amplitude;
}