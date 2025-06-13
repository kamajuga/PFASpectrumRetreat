#pragma once
#include "imgui.h"
#include "LibMath/Vector.h"
#include "Material.h"
#include <GLFW/glfw3.h>
#include "Camera.h"

class UiManager
{
public:
	UiManager() = default;
	~UiManager() = default;

	void				drawPhone();
	void				drawCrossHair();
	void				drawPauseMenu(bool& isPaused, GLFWwindow* window, Camera& camera, float& lastFrame);
	void				drawWinScreen(GLFWwindow* window);

	Material			m_circleColor;
private:
	ImDrawList*			m_draw_list = nullptr;
	ImVec2				m_screen_size;
};