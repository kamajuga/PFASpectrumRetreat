#include "Player.h"

#define	NEAR_ZERO 0.000001f

Player::Player(Camera* camera, std::shared_ptr<Mesh> mesh, const ColliderType& colliderType)
	: GameObject(mesh, colliderType, camera->m_position),	//call the GameObject constructor
	m_cam(camera) {}

void Player::stickColliderToCam()
{
	Geometry3D::Capsule* capsule = getCollider().getCapsule();

	//check if the player capsule exist
	if (capsule != nullptr)
	{
		//set the first point of the capsule to the camera
		capsule->m_pointA.getX() = m_cam->m_position.getX();
		capsule->m_pointA.getY() = m_cam->m_position.getY();
		capsule->m_pointA.getZ() = m_cam->m_position.getZ();
		
		//set the second point of the capsule to the camera minus the height of the player
		capsule->m_pointB.getX() = m_cam->m_position.getX();
		capsule->m_pointB.getY() = m_cam->m_position.getY() - m_playerHeight;
		capsule->m_pointB.getZ() = m_cam->m_position.getZ();
	}
}

void Player::checkInput(GLFWwindow* window, GameManager& gameManager, const float& deltaTime)
{
	//get the velocity of the player
	float velocity = m_cam->m_moveSpeed * deltaTime;

	if (m_canSpawn)
	{
		m_spawnTimer += deltaTime;
		if (m_spawnTimer > 2.f) 
			m_canSpawn = false;
		m_grounded = true; 		
	}
	else
	{
		m_grounded = isGroundedGlobal(gameManager); //to change with multiple objs
	}			
	LibMath::Vector3 step;

	//check the input and modify the step in function the key pressed
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//adjust the step to move in the front of the player
		LibMath::Vector3 front(m_cam->m_front.getX(), 0.f, m_cam->m_front.getZ());
		step += front * velocity;
	};
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//adjust the step to move in the back of the player
		LibMath::Vector3 front(m_cam->m_front.getX(), 0.f, m_cam->m_front.getZ());
		step -= front * velocity;
	};
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//adjust the step to move in the left of the player
		LibMath::Vector3 right(m_cam->m_right.getX(), 0.f, m_cam->m_right.getZ());
		step -= right * velocity;
	};
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		//adjust the step to move in the right of the player
		LibMath::Vector3 right(m_cam->m_right.getX(), 0.f, m_cam->m_right.getZ());
		step += right * velocity;
	};
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS )
	{
		m_velocityY = m_jumpForce;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		//adjust the step to move in the right of the player
		LibMath::Vector3 up(0.f, m_cam->m_up.getY(), 0.f);
		step -= up * velocity;
	}

	//apply gravity if the player isn't grounded
	if (!m_grounded)
	{
		m_velocityY -= m_gravityValue * deltaTime;
	}

	//Apply vertical motion
	step.getY() += m_velocityY * deltaTime;

	//check if the player have a collision with the step and adjust the step
	getAdjutedStep(gameManager, step);

	m_cam->m_position += step;
}

bool Player::isGroundedIndividual(const GameObject* obj) const
{
	//player:
	Geometry3D::Capsule* capsule = getCollider().getCapsule();
	const float radius = capsule->m_radius;

	//lowest y point - radius
	float lowestY = capsule->m_pointB.getY() - radius;

	//get the position of the player
	float playerX = capsule->m_pointB.getX();
	float playerZ = capsule->m_pointB.getZ();

	//object:
	Geometry3D::AABB* aabb = obj->getCollider().getAABB();

	//get the min and max pos of the AABB
	float AABBMaxX = aabb->center().getX() + aabb->extentX();
	float AABBMinX = aabb->center().getX() - aabb->extentX();
	
	float AABBMaxY = aabb->center().getY() + aabb->extentY();
	float AABBMinY = aabb->center().getY() - aabb->extentY();
	

	float AABBMaxZ = aabb->center().getZ() + aabb->extentZ();
	float AABBMinZ = aabb->center().getZ() - aabb->extentZ();

	//checks if the player is on top of the AABB and touching
	bool verticalCheck = (lowestY >= AABBMaxY - m_groundTolerence && 
						  lowestY <= AABBMaxY + m_groundTolerence);

	//check if the player is in the ccoordonate of the AABB 
	bool horizontalCheck = (playerX + radius > AABBMinX &&
							playerX - radius < AABBMaxX &&
							playerZ + radius > AABBMinZ &&
							playerZ - radius < AABBMaxZ);

	return verticalCheck && horizontalCheck;
}

bool Player::isGroundedGlobal(const GameManager& gameManager) const
{
	//check if the gamemanager is nit empty
	if (gameManager.m_goScene.m_nodes.empty()) { return false; }

	//go throught all the AABB
	for (auto& obj : gameManager.m_goScene.m_nodes[0]->m_children)
	{
		//check if the AABB exiqt and have a collider
		if (!obj->m_gameobject || !obj->m_gameobject->getCollider().getAABB())
		{
			std::cout << "Objet sans collider ou gameobject nul : " << std::endl;
			continue;
		}

		//check if the player is grounded on the AABB
		if (isGroundedIndividual(obj->m_gameobject.get()))
		{
			return true;
		}
	}
	return false;
}


bool Player::checkCollisionIndividual(const LibMath::Vector3 step, Geometry3D::Object3D* obj) const
{
	//get the point of future state of the player
	Geometry3D::Point newPointA = getCollider().getCapsule()->m_pointA;
	Geometry3D::Point newPointB = getCollider().getCapsule()->m_pointB;

	//addition the step
	newPointA = Geometry3D::Point(newPointA.getX() + step.getX(), newPointA.getY() + step.getY(), newPointA.getZ() + step.getZ());
	newPointB = Geometry3D::Point(newPointB.getX() + step.getX(), newPointB.getY() + step.getY(), newPointB.getZ() + step.getZ());

	Geometry3D::Capsule newPos(newPointA, newPointB, getCollider().getCapsule()->m_radius);

	return checkCollision(newPos, *obj);
}

LibMath::Vector3 Player::getNormalAABB(const GameObject* obj) const
{
	//get the closest point on the capsule's segment to the AABB's center
	Geometry3D::Point segmentPointToConsider = Geometry3D::getClosestToSegment(this->getCollider().getCapsule()->m_pointA, 
		this->getCollider().getCapsule()->m_pointB, obj->getCollider().getAABB()->center());

	//get the closest point on the AABB to that segment point
	Geometry3D::Point aabbClosestPoint = getClosestToAABB(*obj->getCollider().getAABB(), segmentPointToConsider);

	//get the closest point on the segment to the AABB's closest point
	Geometry3D::Point finalSegmentClosestPoint = getClosestToSegment(this->getCollider().getCapsule()->m_pointA, 
		this->getCollider().getCapsule()->m_pointB, aabbClosestPoint);

	//calculate the squared distance between these two final closest points
	float distanceSquared = finalSegmentClosestPoint.getDistanceSquared(aabbClosestPoint);

	//return the normal of the AABB face that collide with the player
	
	//if the collision is not a ground or a roof -> != just Y
	if (distanceSquared > NEAR_ZERO)
	{
		LibMath::Vector3 collisionAxis = finalSegmentClosestPoint - aabbClosestPoint;

		collisionAxis.normalize();

		return collisionAxis;
	}
	//if the collision is a ground or a roof -> just Y
	else
	{
		return LibMath::Vector3().down();
	}
}

void Player::getAdjutedStep(GameManager& gameManager, LibMath::Vector3& step) const
{
	LibMath::Vector3 normal;

	//go throught all the AABB
	for (auto& obj : gameManager.m_collisionsTocheck)
	{
		if (!obj) { continue; }

		//check if there is a collision
		if (checkCollisionIndividual(step, obj->m_gameobject.get()->getCollider().m_ColliderShape))
		{
			//get the phone and door colors without alpha | rgba -> rgb
			LibMath::Vector3 gameObjectColor = obj->m_gameobject.get()->getMesh()->getMaterial().m_ambient;

			LibMath::Vector3 circleColor = gameManager.m_uiManager.m_circleColor.m_ambient;

			if (obj->m_gameobject.get()->getTag() == "WinCube")
			{
				gameManager.m_restart = true;
			}

			//check if the collider is the one of a door and if the player have the right color
			if (obj->m_gameobject.get()->getTag() == "Door" &&
				obj->m_gameobject.get()->getMesh() &&
				gameObjectColor == circleColor)
			{
				continue;
			}

			//adjust the step of the payer
			else
			{
				//get the normal of the collider
				normal = getNormalAABB(obj->m_gameobject.get());
				normal.normalize();

				//project the step onto the normal to adjust the step
				LibMath::Vector3 projection = step;
				projection.projectOnto(normal);
				step -= projection;
			}
		}
	}
}
