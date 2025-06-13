#pragma once
#include "SkyBox.h"
#include "GameManager.h"
#include "LibMath/Angle.h"
#include "Application.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "Mesh.h"
#include "Light.h"
#include "LibMath/GeometricObject3.h"
#include "GameObject.h"
#include "CollisionDispatcher.h"
#include "Collider.h"
#include "player.h"
#include "LibMath/Arithmetic.h"
#include "UiManager.h"
#include "LightManager.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
//#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#include <irrKlang.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class  Application
{
public:
	 Application() = default;
	~ Application() = default;

	GLFWwindow*		initializeGLFWAndCreateWindow();
	bool			initializeGLAD();
	void			initializeImGui(GLFWwindow* window);
	void			configureOpenGLState();
	void			setupSkybox(GameManager& gameManager);
	void			setupSkyboxTextures(SkyBox& skyBox);
	void			setupShaders(GameManager& gameManager);
	void			setupCamera(GameManager& gameManager);
	void			setupRay(LibMath::Geometry3D::Line& line, Camera& camera, float* rayVertices, unsigned int& rayVAO, unsigned int& rayVBO);
	bool			initializeAudio(GameManager& gameManager);
	static void		framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void			processInput(GLFWwindow* window, Camera& camera, LibMath::Geometry3D::Line& line, Graph<GameObjectNode> graph, GameManager& gameManager);
	unsigned int	loadCubemap(std::vector<std::string> faces);
	void			UpdateRay(LibMath::Geometry3D::Line& line, Camera const& camera, LibMath::Vector3& rayEnd, unsigned int vbo);


	void			startNewImGuiFrame();
	void			updateDeltaTime();
	void			handleGameUpdate(GameManager& gameManager, Player& player, LibMath::Geometry3D::Line& line);
	void			renderScene(GameManager& gameManager);
	void			renderRay(GameManager& gameManager);
	void			handlePauseMenu(GameManager& gameManager);
	void			finalizeFrame();
	int				windowLoop(void);
	void			runMainLoop(GameManager& gameManager, Player& player, LibMath::Geometry3D::Line line);

	// timing
	float			m_deltaTime = 0.0f;
	float			m_lastFrame = 0.0f;
	GLFWwindow*		m_window;
	unsigned int	m_rayVAO, m_rayVBO;
	LibMath::Vector3 m_rayEnd;

};
