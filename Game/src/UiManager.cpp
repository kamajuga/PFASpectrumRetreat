#include "UiManager.h"
#include "Application.h"
#include <algorithm>

void    UiManager::drawPhone()
{
    m_draw_list = ImGui::GetBackgroundDrawList();
    m_screen_size = ImGui::GetIO().DisplaySize;

    ImVec2 center = ImVec2(m_screen_size.x - 100.0f, m_screen_size.y - 100.0f);

    float radius = 90.0f;
    
    LibMath::Vector4 approximatedColor = m_circleColor.m_ambient * 0.5f +
        m_circleColor.m_diffuse * 0.4f +
        m_circleColor.m_specular * 0.1f;

    ImVec4 circleColor = ImVec4(
        std::clamp(approximatedColor.getX(), 0.0f, 1.0f),
        std::clamp(approximatedColor.getY(), 0.0f, 1.0f),
        std::clamp(approximatedColor.getZ(), 0.0f, 1.0f),
        1.0f
    );

    m_draw_list->AddCircleFilled(center, radius, ImGui::ColorConvertFloat4ToU32(circleColor));
}

void    UiManager::drawCrossHair()
{
    m_draw_list = ImGui::GetBackgroundDrawList();
    m_screen_size = ImGui::GetIO().DisplaySize;

    ImVec2 center = ImVec2(m_screen_size.x / 2.0f, m_screen_size.y / 2.0f);
    float radius = 3.0f;
    ImVec4 circleColor = ImVec4(1.0, 1.0, 1.0, 1.0);
    m_draw_list->AddCircleFilled(center, radius, ImGui::ColorConvertFloat4ToU32(circleColor));
}

void    UiManager::drawPauseMenu(bool& isPaused, GLFWwindow* window, Camera& camera, float& lastFrame)
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilled(
        ImVec2(0, 0),
        ImVec2((float)windowWidth, (float)windowHeight),
        IM_COL32(0, 0, 0, 160)
    );

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)windowWidth, (float)windowHeight));
    ImGui::Begin("##PauseOverlay", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoBringToFrontOnFocus
    );

    const char* title = "PAUSE";
    float textWidth = ImGui::CalcTextSize(title).x;
    ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) * 0.5f, windowHeight * 0.4f));
    ImGui::Text("%s", title);

    float buttonWidth = 200.0f;
    float buttonHeight = 40.0f;
    float centerX = (windowWidth - buttonWidth) * 0.5f;

    ImGui::SetCursorPos(ImVec2(centerX, windowHeight * 0.5f));
    if (ImGui::Button("Resume", ImVec2(buttonWidth, buttonHeight))) 
    {
        isPaused = false;
        camera.m_firstMouse = true;
        lastFrame = static_cast<float>(glfwGetTime());
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    ImGui::SetCursorPos(ImVec2(centerX, windowHeight * 0.5f + 100));
    if (ImGui::Button("Quit", ImVec2(buttonWidth, buttonHeight))) 
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    ImGui::End();
}

void UiManager::drawWinScreen(GLFWwindow* window)
{
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    drawList->AddRectFilled(
        ImVec2(0, 0),
        ImVec2((float)windowWidth, (float)windowHeight),
        IM_COL32(0, 0, 0, 160)
    );

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)windowWidth, (float)windowHeight));
    ImGui::Begin("##WinOverlay", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoBringToFrontOnFocus
    );

    const char* title = "You Won !!!";
    float textWidth = ImGui::CalcTextSize(title).x;
    ImGui::SetCursorPos(ImVec2((windowWidth - textWidth) * 0.5f, windowHeight * 0.4f));
    ImGui::Text("%s", title);

    float buttonWidth = 200.0f;
    float buttonHeight = 40.0f;
    float centerX = (windowWidth - buttonWidth) * 0.5f;

    ImGui::SetCursorPos(ImVec2(centerX, windowHeight * 0.5f));
    if (ImGui::Button("Quit", ImVec2(buttonWidth, buttonHeight)))
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    ImGui::End();
}
