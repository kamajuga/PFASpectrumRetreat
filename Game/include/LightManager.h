#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Light.h"
#include "Shader.h"

class LightManager
{
public:
	LightManager();

	void	initLights(void);
	void	uploadLightCountToGPU(Shader& shader);
	void	uploadToGPU(Shader& shader, float deltaTime);
	void	addPointLight(PointLight pointLight);
	void	addSpotLight(SpotLight spotLight);
	void	loadLightsFromFile(const std::string& file);
	void	parsePointLight(std::istringstream& iss);
	void	parseSpotLight(std::istringstream& iss);
	void	parseColor(const std::string& colorStr, Light& light, const float& lightItensity);

	~LightManager() = default;

	DirectionalLight		m_directionalLight;
	std::vector<PointLight> m_pointLights;
	std::vector<SpotLight>	m_spotLights;
private:
	

};


