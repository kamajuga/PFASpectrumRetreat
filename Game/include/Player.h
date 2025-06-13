#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "LibMath/Vector.h"
#include "CollisionDispatcher.h"
#include "GameManager.h"
#include "vector"

class Player : public GameObject
{
public:
	Player() = default;
	Player(Camera* camera, std::shared_ptr<Mesh> mesh, const ColliderType& colliderType);

	void				stickColliderToCam();

	void				checkInput(GLFWwindow*, GameManager&, const float&);

	bool				isGroundedIndividual(const GameObject*) const;
	bool				isGroundedGlobal(const GameManager&) const;

	bool				checkCollisionIndividual(const LibMath::Vector3, Geometry3D::Object3D*) const;


	LibMath::Vector3	getNormalAABB(const GameObject*) const;
	void				getAdjutedStep(GameManager&, LibMath::Vector3&) const;


private:
			Camera*			m_cam;						//the player's cam

	//player infos
	const	float			m_playerHeight = 3.f;

	//jump
	const	float			m_groundTolerence = 0.1f;	//the tolerence to know if the player is grounded
	const	float			m_jumpForce = 7.f;			//the force added to Y when the player is jumping
			float			m_velocityY = 0.f;			//the velocity on the Y axis
	const	float			m_gravityValue = 10.f;		//the value of the gravity
	bool					m_canSpawn = true;
	float					m_spawnTimer = 0.0f;
	bool					m_grounded = false;


};