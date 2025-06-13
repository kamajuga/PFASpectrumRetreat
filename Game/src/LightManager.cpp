#include "LightManager.h"

LightManager::LightManager()
{
    //m_directionalLight = DirectionalLight(LibMath::Radian(0.f));
}

void    LightManager::initLights(void)
{
    PointLight pointLight(LibMath::Vector3(20, 2, 60));

    SpotLight spotLight(LibMath::Vector3(20, 5, 20), LibMath::Radian(.0f));

    m_pointLights.push_back(std::move(pointLight));
    m_spotLights.push_back(std::move(spotLight));
}

void    LightManager::uploadLightCountToGPU(Shader& shader)
{
    Light::uploadLightCountToGPU(shader.m_program, "nbPointLights", static_cast<int>(m_pointLights.size()));
    Light::uploadLightCountToGPU(shader.m_program, "nbSpotLights", static_cast<int>(m_spotLights.size()));
}


void    LightManager::uploadToGPU(Shader& shader, float deltaTime)
{
	m_directionalLight.uploadLightToGPU(shader.m_program, "dLight", 0);

	for (size_t i = 0; i < m_pointLights.size(); i++)
	{
		m_pointLights[i].uploadLightToGPU(shader.m_program, "pLight", (int)i);
	}

	for (size_t i = 0; i < m_spotLights.size(); i++)
	{
		m_spotLights[i].uploadLightToGPU(shader.m_program, "sLight", (int)i);
	}
}


void    LightManager::addPointLight(PointLight pointLight)
{
	m_pointLights.push_back(std::move(pointLight));
}

void    LightManager::addSpotLight(SpotLight spotLight)
{
	m_spotLights.push_back(std::move(spotLight));
}

void    LightManager::loadLightsFromFile(const std::string& file)
{
    std::ifstream fileStream(file);
    if (!fileStream.is_open()) {
        std::cerr << "Error: Could not open lights file: " << file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(fileStream, line)) 
    {
        if (line.empty() || line[0] == '#') 
        {
            continue;
        }

        std::istringstream iss(line);
        std::string lightType;
        iss >> lightType;

        if (lightType == "PointLight") 
        {
            parsePointLight(iss);
        }
        else if (lightType == "SpotLight") 
        {
            parseSpotLight(iss);
        }
    }

    fileStream.close();
}

void    LightManager::parsePointLight(std::istringstream& iss)
{
    LibMath::Vector3 pos;
    float constant, linear, quadratic;
    float lightIntensity;
    std::string colorStr;

    // Read parameters
    iss >> pos.getX() >> pos.getY() >> pos.getZ();
    iss >> constant >> linear >> quadratic >> lightIntensity >> colorStr;

    // Create pointLight
    PointLight pointLight = PointLight(pos);
	pointLight.updateLightData({ 1.0f, 1.f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
    pointLight.updatePointLightData(constant, linear, quadratic);

    // Parse the color
    parseColor(colorStr, pointLight, lightIntensity);

    // Add light to the collection
    m_pointLights.push_back(std::move(pointLight));
}

void    LightManager::parseSpotLight(std::istringstream& iss)
{
    LibMath::Vector3 pos;
    LibMath::Vector3 rot;
    float cutOff, outerCutOff;
    float constant, linear, quadratic;
    float lightIntensity;
    std::string colorStr;

    // Read parameters
    iss >> pos.getX() >> pos.getY() >> pos.getZ();
    iss >> rot.getX() >> rot.getY() >> rot.getZ();
    iss >> cutOff >> outerCutOff >> constant >> linear >> quadratic >> lightIntensity >> colorStr;

    // Create position and rotation
    LibMath::Matrix4 transform = LibMath::Matrix4::createTranslate(pos) * LibMath::Matrix4::createRotationXYZ(rot);

    float finalCutOff = LibMath::cos(LibMath::Degree(cutOff));
    float finalOuterCutOff = LibMath::cos(LibMath::Degree(outerCutOff));

    // Create spotLight
    SpotLight spotLight = SpotLight(transform);

    // Configure properties
    spotLight.updateSpotLightData(constant, linear, quadratic, finalCutOff, finalOuterCutOff);

    // Parse color
    parseColor(colorStr, spotLight, lightIntensity);

    // Add light to collection
    m_spotLights.push_back(std::move(spotLight));
}

void    LightManager::parseColor(const std::string& colorStr, Light& light, const float& lightItensity)
{
    // Extract color from format
    size_t equalPos = colorStr.find('=');
    if (equalPos != std::string::npos) 
    {
        std::string colorName = colorStr.substr(equalPos + 1);

        if (colorName == "white") 
        {
            light.m_ambient = LibMath::Vector3(1.0f, 1.0f, 1.0f) * lightItensity;
        }
        else if (colorName == "red") 
        {
            light.m_ambient = LibMath::Vector3(1.0f, 0.0f, 0.0f) * lightItensity;
        }
        else if (colorName == "green") 
        {
            light.m_ambient = LibMath::Vector3(0.0f, 1.0f, 0.0f) * lightItensity;
        }
        else if (colorName == "blue") 
        {
            light.m_ambient = LibMath::Vector3(0.0f, 0.0f, 1.0f) * lightItensity;
        }
        else if (colorName == "orange") 
        {
            light.m_ambient = LibMath::Vector3(1.0f, 0.5f, 0.0f) * lightItensity;
        }
        else if (colorName == "yellow") 
        {
            light.m_ambient = LibMath::Vector3(1.0f, 1.0f, 0.0f) * lightItensity;
        }
        else if (colorName == "purple")
        {
            light.m_ambient = LibMath::Vector3(0.5f, 0.0f, 0.5f) * lightItensity;
        }
        else 
        {
            // Default color
            light.m_ambient = LibMath::Vector3(1.0f, 1.0f, 1.0f) * lightItensity;
        }
    }
}

