#include "Application.h"

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

GLFWwindow* Application::initializeGLFWAndCreateWindow()
{
    // glfw: initialize and configure
   // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    /*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);*/

    /*GLFWwindow* window = glfwCreateWindow(
        mode->width, mode->height, "Plein écran", monitor, NULL
    );*/

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "regeer", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}

bool Application::initializeGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
}

void Application::initializeImGui(GLFWwindow* window)
{
    // Initialisation de Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Lier ImGui avec GLFW et OpenGL3
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Application::configureOpenGLState()
{
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::setupSkybox(GameManager& gameManager)
{
    gameManager.m_skyBox.loadShaders("../../Assets/Shaders/SkyBoxVert.vert", "../../Assets/Shaders/SkyBoxFrag.frag");
    gameManager.m_skyBox.generateBuffers();
}

void Application::setupSkyboxTextures(SkyBox& skyBox)
{
    std::vector<std::string> faces = { "../../Assets/Textures/right.jpg",
                            "../../Assets/Textures/left.jpg",
                            "../../Assets/Textures/top.jpg",
                            "../../Assets/Textures/bottom.jpg",
                            "../../Assets/Textures/front.jpg",
                            "../../Assets/Textures/back.jpg" };

    skyBox.loadTextures(faces);
    skyBox.use();
    skyBox.setInt("skybox", 0);
}

void Application::setupShaders(GameManager& gameManager)
{
    gameManager.m_shader.setVertexShader("../../Assets/Shaders/VertexShader.vert");
    gameManager.m_shader.setFragmentShader("../../Assets/Shaders/FragmentShader.frag");
    gameManager.m_shader.link();

    gameManager.m_rayShader.setVertexShader("../../Assets/Shaders/RayVertex.vert");
    gameManager.m_rayShader.setFragmentShader("../../Assets/Shaders/RayFragment.frag");
    gameManager.m_rayShader.link();
}

void Application::setupCamera(GameManager& gameManager)
{
    gameManager.m_camera = Camera(
        LibMath::Vector3(34.0f, -50.0f, -108.0f),   // eye (position)
        LibMath::Vector3(0.0f, 0.0f, 0.0f),   // center
        LibMath::Vector3(0.0f, 1.0f, 0.0f),   // up,
        45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

void Application::setupRay(LibMath::Geometry3D::Line& line, Camera& camera, float* rayVertices, unsigned int& rayVAO, unsigned int& rayVBO)
{
    line.m_origin.getX() = camera.m_position.getX();
    line.m_origin.getY() = camera.m_position.getY();
    line.m_origin.getZ() = camera.m_position.getZ();

    line.m_direction = camera.m_front; // Set the direction to the camera's front vector

    line.m_direction.normalize();

    line.m_length = 20.0f; // Set the length of the line

    m_rayEnd.getX() = line.m_origin.getX() + line.m_direction.getX() * line.m_length;
    m_rayEnd.getY() = line.m_origin.getY() + line.m_direction.getY() * line.m_length;
    m_rayEnd.getZ() = line.m_origin.getZ() + line.m_direction.getZ() * line.m_length;

    rayVertices[0] = line.m_origin.getX(); rayVertices[1] = line.m_origin.getY(); rayVertices[2] = line.m_origin.getZ();
    rayVertices[3] = m_rayEnd.getX(); rayVertices[4] = m_rayEnd.getY(); rayVertices[5] = m_rayEnd.getZ();

    glGenVertexArrays(1, &rayVAO);
    glGenBuffers(1, &rayVBO);

    glBindVertexArray(rayVAO);                 // VAO d'abord
    glBindBuffer(GL_ARRAY_BUFFER, rayVBO);     // puis VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(rayVertices), rayVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

bool Application::initializeAudio(GameManager& gameManager)
{
    gameManager.gameMusic = irrklang::createIrrKlangDevice();
    if (!gameManager.gameMusic)
    {
        std::cout << "Failed to create sound engine" << std::endl;
        return false;
    }
    gameManager.gameMusic->play2D("../../Assets/Sounds/Track3.ogg", true);

    return true;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Application::processInput(GLFWwindow* window, Camera& camera, LibMath::Geometry3D::Line& line, Graph<GameObjectNode> gameObjects, GameManager& gameManager)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !gameManager.m_escapePressedLastFrame)
        gameManager.m_isPaused = !gameManager.m_isPaused;

    gameManager.m_escapePressedLastFrame = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (camera.m_firstMouse) {
        camera.m_lastX = static_cast<float>(xpos);
        camera.m_lastY = static_cast<float>(ypos);
        camera.m_firstMouse = false;
    }
    float xoffset = static_cast<float>(xpos) - camera.m_lastX;
    float yoffset = camera.m_lastY - static_cast<float>(ypos);
    camera.m_lastX = static_cast<float>(xpos);
    camera.m_lastY = static_cast<float>(ypos);

    xoffset *= camera.m_mouseSensitivity;
    yoffset *= camera.m_mouseSensitivity;

    camera.m_yaw += LibMath::Degree(xoffset);
    camera.m_pitch += LibMath::Degree(yoffset);

    camera.updateCameraVectors();
    camera.m_viewMatrix = camera.lookAt(camera.m_position, camera.m_position + camera.m_front, camera.m_up);
}

unsigned int Application::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Application::UpdateRay(LibMath::Geometry3D::Line& line, Camera const& camera, LibMath::Vector3& rayEnd, unsigned int vbo)
{
    line.m_origin.getX() = camera.m_position.getX();
    line.m_origin.getY() = camera.m_position.getY();
    line.m_origin.getZ() = camera.m_position.getZ();

    line.m_direction = camera.m_front;
    rayEnd.getX() = line.m_origin.getX() + line.m_direction.getX() * 100.0f;
    rayEnd.getY() = line.m_origin.getY() + line.m_direction.getY() * 100.0f;
    rayEnd.getZ() = line.m_origin.getZ() + line.m_direction.getZ() * 100.0f;

    float rayVertices[] = {
        line.m_origin.getX(), line.m_origin.getY(), line.m_origin.getZ(),
        rayEnd.getX(), rayEnd.getY(), rayEnd.getZ()
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(rayVertices), rayVertices, GL_STATIC_DRAW);
}

void Application::startNewImGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Application::updateDeltaTime()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
}

void Application::handleGameUpdate(GameManager& gameManager, Player& player, LibMath::Geometry3D::Line& line)
{
    gameManager.m_uiManager.drawPhone();
    gameManager.m_uiManager.drawCrossHair();

    player.checkInput(m_window, gameManager, m_deltaTime);
    processInput(m_window, gameManager.m_camera, line, gameManager.m_animatedScene, gameManager);

    gameManager.getNearbyDoors(gameManager.m_camera);

    gameManager.m_shader.use();

    gameManager.m_lightManager.uploadLightCountToGPU(gameManager.m_shader);
    gameManager.m_lightManager.uploadToGPU(gameManager.m_shader, m_deltaTime);

    gameManager.checkRay(gameManager.m_animatedScene, line, m_window);
    gameManager.m_camera.uploadPositionToGPU(gameManager.m_shader.m_program, "CameraPos");
}

void Application::renderScene(GameManager& gameManager)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw scene as normal

    LibMath::Matrix4 view = gameManager.m_camera.getViewMatrix();
    LibMath::Matrix4 projection = LibMath::Matrix4::perspective(gameManager.m_camera.m_fov, gameManager.m_camera.m_aspectRatio, 0.1f, 1000.f);
    LibMath::Matrix4 vp = projection * view;

    gameManager.updateAnimation(m_deltaTime);

    gameManager.m_animatedScene.drawAll(vp);
    gameManager.m_goScene.drawAll(vp);

    gameManager.m_skyBox.draw(view, projection);

    gameManager.m_doorsScene.drawAll(vp);
}

void Application::renderRay(GameManager& gameManager)
{
    gameManager.m_rayShader.use();
    LibMath::Matrix4 view = gameManager.m_camera.getViewMatrix();
    LibMath::Matrix4 projection = LibMath::Matrix4::perspective(
        gameManager.m_camera.m_fov, gameManager.m_camera.m_aspectRatio, 0.1f, 1000.f);
    LibMath::Matrix4 vp = projection * view;

    gameManager.m_rayShader.setMat4("uViewProj", vp);
    gameManager.m_rayShader.setVec3("uColor", LibMath::Vector3(1.0f, 0.0f, 0.0f));

    glBindVertexArray(m_rayVAO);
    glDrawArrays(GL_LINES, 0, 2);
}

void Application::handlePauseMenu(GameManager& gameManager)
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    gameManager.m_uiManager.drawPauseMenu(gameManager.m_isPaused, m_window, gameManager.m_camera, m_lastFrame);
}

void Application::finalizeFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}







int Application::windowLoop(void)
{
    m_window = initializeGLFWAndCreateWindow();
    if (!m_window) return -1;

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!initializeGLAD()) return -1;

    initializeImGui(m_window);

    registerCollisions();

    // configure global opengl state
    // -----------------------------
    configureOpenGLState();

    GameManager gameManager;
    setupSkybox(gameManager);
    UiManager gui;
    gameManager.m_uiManager = gui; // Assign the GUI to the GameManager

    setupShaders(gameManager);

    gameManager.generateLevelFile(&gameManager.m_shader);

    setupSkyboxTextures(gameManager.m_skyBox);

    // camera
    setupCamera(gameManager);

    LibMath::Geometry3D::Line line;
    float rayVertices[6];
    
    setupRay(line, gameManager.m_camera, rayVertices, m_rayVAO, m_rayVBO);

    // create the player
    Player player(&gameManager.m_camera, nullptr, ColliderType::PLAYER);
    player.stickColliderToCam();

    //Music manager
    if (!initializeAudio(gameManager)) return -1;

    runMainLoop(gameManager, player, line);

    gameManager.gameMusic->drop();

    glfwTerminate();
    return 0;
}

void Application::runMainLoop(GameManager& gameManager, Player& player, LibMath::Geometry3D::Line line)
{
    // render loop
   // -----------
    while (!glfwWindowShouldClose(m_window))
    {
        startNewImGuiFrame();
        updateDeltaTime();         

        if (!gameManager.m_isPaused)
        {
            handleGameUpdate(gameManager, player, line);
        }

        if (gameManager.m_restart)
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            gameManager.m_isPaused = true;
            gameManager.m_uiManager.drawWinScreen(m_window);
        }

        UpdateRay(line, gameManager.m_camera, m_rayEnd, m_rayVBO);

        // render
        // ------   
        renderScene(gameManager);

        //Update the ray's origin and direction
        renderRay(gameManager);

        //check the player collision
        player.stickColliderToCam();

        if (gameManager.m_isPaused && !gameManager.m_restart)
        {
            handlePauseMenu(gameManager);
        }

        finalizeFrame();
    }
}

