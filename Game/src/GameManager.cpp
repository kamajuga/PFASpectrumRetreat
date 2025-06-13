#include "GameManager.h"
#include "CollisionDispatcher.h"
#include "GameObject.h"
#include "Collider.h"
#include "Material.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

static const    std::unordered_map<std::string, LibMath::Vector4> namedColors = {
        {"red",    {1.0f, 0.0f, 0.0f, 1.0f}},
        {"green",  {0.0f, 1.0f, 0.0f, 1.0f}},
        {"blue",   {0.0f, 0.0f, 1.0f, 1.0f}},
        {"white",  {1.0f, 1.0f, 1.0f, 1.0f}},
        {"black",  {0.0f, 0.0f, 0.0f, 1.0f}},
        {"yellow", {1.0f, 1.0f, 0.0f, 1.0f}},
        {"purple", {0.5f, 0.0f, 0.5f, 1.0f}},
        {"orange", {1.0f, 0.647f, 0.0f, 1.0f}},
        {"cyan",   {0.0f, 1.0f, 1.0f, 1.0f}},
        {"magenta",{1.0f, 0.0f, 1.0f, 1.0f}}
};

void            GameManager::generateLevelFile(Shader* shader)
{
    std::shared_ptr<GameObjectNode>sceneNode = createSceneRoot(shader); 

    auto models = loadModels();

    bindModels(models);
    
    std::ifstream file("../../Assets/Levels/Level.txt");

    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier.\n";
        return;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        ObjectTransform obj = parseObjectLine(line);

        if (obj.type == "Wall") handleWall(obj, sceneNode, shader, models.wall);
        else if (obj.type == "Floor" || obj.type == "Ceiling") handleFloor(obj, sceneNode, shader, models.floor);
        else if (obj.type == "Cube" || obj.type == "CubeTP" || obj.type == "CubeWin") handleCube(obj, shader, models.cube);
        else if (obj.type == "Door") handleDoor(obj, shader, models.door);
    }

    m_lightManager.loadLightsFromFile("../../Assets/Levels/Lights.txt");
    sceneNode->update();

    collectCollisionObjects(*sceneNode);

    collectCollisionObjects();   

    m_goScene.addNode(std::move(sceneNode));
}

void            GameManager::checkRay(Graph<GameObjectNode> gameObjects, LibMath::Geometry3D::Line& line, GLFWwindow* window)
{
	for (auto& gameObject : gameObjects.m_nodes) 
	{
        if (checkCollision(*gameObject->m_gameobject->getCollider().m_ColliderShape, line))
        {
            bool isCtrlPressed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;

            if (isCtrlPressed && !m_ctrlPressedLastFrame)
            {
                swapColorBetweenObjects(*gameObject->m_gameobject);
            }

            m_ctrlPressedLastFrame = isCtrlPressed;
        }

        LibMath::Geometry3D::Line line10 = line;
        line10.m_length *= 5;
        LibMath::Vector3 gameObjectColor = gameObject->m_gameobject->getMesh()->getMaterial().m_ambient;

        LibMath::Vector3 circleColor = m_uiManager.m_circleColor.m_ambient;

        bool canTP = (gameObject->m_gameobject->getTag() == "CanTeleporte") && gameObjectColor == circleColor;

        if (canTP && checkCollision(*gameObject->m_gameobject->getCollider().m_ColliderShape, line10))
        {
            bool isLeftClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

            if (isLeftClicked && !m_clickedLastFrame)
            {
                teleportePlayer(gameObject->m_gameobject , m_camera);
            }

            m_clickedLastFrame = isLeftClicked;
        }
       

        if (gameObject->m_children.empty())
            continue;

        for (auto& go : gameObject->m_children) // maybe remove this
        {
            if (checkCollision(*go->m_gameobject->getCollider().m_ColliderShape, line))
            {
                swapColorBetweenObjects(*go->m_gameobject);
                return;
            }
        }	
	}
}

void            GameManager::swapColorBetweenObjects(GameObject & gameObject)
{
    gameMusic->play2D("../../Assets/Sounds/swap_color.ogg");
    Material tempColor = gameObject.getMesh()->getMaterial();
    gameObject.getMesh()->getMaterial() = m_uiManager.m_circleColor;
    m_uiManager.m_circleColor = tempColor; // Swap the color of the circle with the object's color
}

void 			GameManager::teleportePlayer(std::shared_ptr<GameObject> gameObject, Camera& camera)
{
    gameMusic->play2D("../../Assets/Sounds/teleport.ogg");
    MeshDimensions dimensions = Collider::getboxCollider(*gameObject->getMesh()->getModel());
    LibMath::Vector3 cubePos = gameObject->getCollider().getAABB()->center().toVector3();
    LibMath::Vector3 position = -(gameObject->getCollider().getAABB()->center().toVector3() - camera.m_position);
    position.normalize(); // Normalize the direction vector

    LibMath::Vector3 newPos(
        cubePos.getX() + position.getX() * dimensions.width,
        cubePos.getY() + 5.f + position.getY() * dimensions.height,
        cubePos.getZ() + position.getZ() * dimensions.depth + 2.f 
    );

    camera.m_position = newPos;
}

void            GameManager::getNearbyDoors(Camera& camera)
{
    LibMath::Vector3 cameraPos = camera.m_position;

    for (std::shared_ptr<GameObjectNode>& doorNode : m_doorsScene.m_nodes)
    {
        LibMath::Vector3 gameObjectPosition = doorNode->m_localTransform * LibMath::Vector4(0.f, 0.f, 0.f, 1.f);

        if (cameraPos.distanceFrom(gameObjectPosition) <= 100.f)
        {
            if ((LibMath::Vector3)m_uiManager.m_circleColor.m_ambient == (LibMath::Vector3)doorNode->m_gameobject->getMesh()->getMaterial().m_ambient)
            {
                doorNode->m_gameobject->getMesh()->getMaterial().setAlpha(0.05f);
            }
            else
            {
                doorNode->m_gameobject->getMesh()->getMaterial().setAlpha(0.1f);
            }
            if (std::find(m_nearbyObjects.begin(), m_nearbyObjects.end(), doorNode->m_gameobject) != m_nearbyObjects.end())
                continue;
            m_nearbyObjects.push_back(doorNode->m_gameobject);
        }
        else
        {
            std::vector<std::shared_ptr<GameObject>>::iterator it = std::find(m_nearbyObjects.begin(), m_nearbyObjects.end(), doorNode->m_gameobject);
            if (it != m_nearbyObjects.end())
                m_nearbyObjects.erase(it);
        }
    }
}

void            GameManager::updateAnimation(float deltaTime)
{
    size_t size = m_animationDatas.size();

    for (size_t i = 0; i < size; i++)
    {
        LibMath::Vector3 axis = m_animationDatas[i].second;
        if (axis.magnitude() == 0)
            continue;

        float Offset = getOffset(m_animationDatas[i].first, deltaTime);
        //std::cout << deltaTime << std::endl;
        //std::cout << Offset << std::endl;
        
        LibMath::Matrix4 transMat = LibMath::Matrix4::createTranslate({ Offset * axis.getX(), Offset * axis.getY(), Offset * axis.getZ() });
        m_animatedScene.m_nodes[i]->update(transMat);
    }

    m_animatedScene.updateAll();
}



std::shared_ptr<GameObjectNode> GameManager::createSceneRoot(Shader* shader)
{
    auto root = std::make_shared<GameObjectNode>();
    root->initialize(nullptr, shader, LibMath::Matrix4::identity());
    return root;
}

ModelSet GameManager::loadModels()
{
    return {
        std::make_shared<Model>(Parser::parse3DObject("../../Assets/Meshes/TriDoor.obj")),
        std::make_shared<Model>(Parser::parse3DObject("../../Assets/Meshes/DoorR.obj")),
        std::make_shared<Model>(Parser::parse3DObject("../../Assets/Meshes/floor.obj")),
        std::make_shared<Model>(Parser::parse3DObject("../../Assets/Meshes/cube.obj"))
    };
}

void GameManager::bindModels(const ModelSet& models)
{
    models.wall->bind();
    models.door->bind();
    models.floor->bind();
    models.cube->bind();
}

ObjectTransform GameManager::parseObjectLine(const std::string& line)
{
    std::istringstream iss(line);
    ObjectTransform obj;
    iss >> obj.type;

    iss >> obj.position.getX() >> obj.position.getY() >> obj.position.getZ();
    iss >> obj.scale.getX() >> obj.scale.getY() >> obj.scale.getZ();
    iss >> obj.rotation.getX() >> obj.rotation.getY() >> obj.rotation.getZ();
    iss >> obj.animationAxe.getX() >> obj.animationAxe.getY() >> obj.animationAxe.getZ();
    iss >> obj.animationData.amplitude >> obj.animationData.speed;

    std::string token;
    while (iss >> token) {
        auto eq = token.find('=');
        if (eq != std::string::npos) {
            std::string key = token.substr(0, eq);
            std::string value = token.substr(eq + 1);
            obj.extras[key] = value;
        }
    }

    return obj;
}

void GameManager::handleWall(const ObjectTransform& obj, std::shared_ptr<GameObjectNode> sceneNode, Shader* shader, std::shared_ptr<Model> model)
{
    auto transMat = LibMath::Matrix4::createTranslate(obj.position) *
        LibMath::Matrix4::createRotationXYZ(obj.rotation) *
        LibMath::Matrix4::createScale(obj.scale);

    auto mesh = std::make_shared<Mesh>(model);
    mesh->SetMaterial(
        LibMath::Vector4(1.f, 0.05f, 0.05f, 1.f), // Légère ambient
        LibMath::Vector3(0.4f, 0.4f, 0.4f),              // diffuse
        LibMath::Vector3(0.774597f, 0.774597f, 0.774597f), // specular
        50.0f
    );
    mesh->setModelMatrix(transMat);

    auto go = std::make_shared<GameObject>(mesh, ColliderType::BOX);
    go->setTag("Wall");
    sceneNode->addChild(go, shader, LibMath::Matrix4::identity());
}

void GameManager::handleFloor(const ObjectTransform& obj, std::shared_ptr<GameObjectNode> sceneNode, Shader* shader, std::shared_ptr<Model> floor)
{
    auto transMat = LibMath::Matrix4::createTranslate(obj.position) *
        LibMath::Matrix4::createRotationXYZ(obj.rotation) *
        LibMath::Matrix4::createScale(obj.scale);

    auto mesh = std::make_shared<Mesh>(floor);
    mesh->SetMaterial({ 0.01f, 0.5f, 0.5f, 1.f }, { 0.1f, 0.1f, 0.1f, 1.f }, { 0.f, 0.f, 0.f, 1.f }, 1.0f);
    mesh->setModelMatrix(transMat);

    auto go = std::make_shared<GameObject>(mesh, ColliderType::BOX);
    go->setTag("Floor");
    sceneNode->addChild(go, shader, LibMath::Matrix4::identity());
}

void GameManager::handleCube(ObjectTransform& obj, Shader* shader, std::shared_ptr<Model> cube)
{
    auto transMat = LibMath::Matrix4::createTranslate(obj.position) *
        LibMath::Matrix4::createRotationXYZ(obj.rotation) *
        LibMath::Matrix4::createScale(obj.scale);

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(cube);
    mesh->SetMaterial(
        namedColors.at(obj.extras["color"]), // Légère ambient
        LibMath::Vector3(0.6f, 0.6f, 0.6f),              // diffuse
        LibMath::Vector3(0.8f, 0.8f, 0.8f),              // specular
        32.0f
    );
    mesh->setModelMatrix(transMat);

    auto go = std::make_shared<GameObject>(mesh, ColliderType::BOX);
    if (obj.type == "CubeTP") go->setTag("CanTeleporte");

    auto node = std::make_shared<GameObjectNode>();
    node->initialize(go, shader, LibMath::Matrix4::identity());

    if (obj.type == "CubeWin")
    {
        go->setTag("WinCube");
        m_collisionsTocheck.push_back(node);
    }

    m_animationDatas.emplace_back(AnimationData{ obj.animationData.time, obj.animationData.amplitude, obj.animationData.speed }, obj.animationAxe);
    m_animatedScene.addNode(std::move(node));
}

void GameManager::handleDoor(const ObjectTransform& obj, Shader* shader, std::shared_ptr<Model> door)
{
    auto transMat = LibMath::Matrix4::createTranslate(obj.position) *
        LibMath::Matrix4::createRotationXYZ(obj.rotation) *
        LibMath::Matrix4::createScale(obj.scale);

    auto mesh = std::make_shared<Mesh>(door);
    mesh->SetMaterial(namedColors.at(obj.extras.at("color")), { 0.1f, 0.1f, 0.1f, 1.f }, { 0.f, 0.f, 0.f, 1.f }, 1.0f);
    mesh->setModelMatrix(transMat);

    auto go = std::make_shared<GameObject>(mesh, ColliderType::BOX);
    go->setTag("Door");

    auto node = std::make_shared<GameObjectNode>();
    node->initialize(go, shader, transMat);
    m_doorsScene.addNode(std::move(node));
}

void GameManager::collectCollisionObjects(const GameObjectNode& node)
{
    // Ajouter les objets de m_goScene
    for (auto& obj : node.m_children)
    {
        m_collisionsTocheck.push_back(obj);
    }
}

void GameManager::collectCollisionObjects()
{
    // Ajouter les objets de m_doorsScene
    for (auto& obj : m_doorsScene.m_nodes)
    {
        m_collisionsTocheck.push_back(obj);
    }
}
