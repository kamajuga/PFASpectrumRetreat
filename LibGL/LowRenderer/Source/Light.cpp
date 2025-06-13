#define _USE_MATH_DEFINES
#include <cmath>
#include "Light.h"

Light::Light(const LibMath::Matrix4& transform, const LibMath::Vector4& ambient, const LibMath::Vector4& diffuse, const LibMath::Vector4& specular)
{
	m_transform = transform;
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
}

void Light::uploadLightCountToGPU(GLuint shaderProgram, const std::string& uniformName, int count)
{
	GLint location = glGetUniformLocation(shaderProgram, uniformName.c_str());
	if (location == -1) {
		std::cerr << "Warning: uniform '" << uniformName << "' not found in shader program." << std::endl;
		//throw std::runtime_error("Model matrix uniform not found: " + uniformName);
		//return;
	}
	glUniform1i(location, count);
}

DirectionalLight::DirectionalLight(const LibMath::Radian& rotation, 
	const LibMath::Vector4& ambient, 
	const LibMath::Vector4& diffuse, 
	const LibMath::Vector4& specular)
	:	Light(LibMath::Matrix4::createRotationX(rotation), ambient, diffuse, specular)
{
	m_direction = LibMath::Matrix4::createRotationX(rotation) * LibMath::Vector4(0, 0, -1, 0);
}

DirectionalLight::DirectionalLight(const LibMath::Matrix4& rotationMatrix, 
	const LibMath::Vector4& ambient, 
	const LibMath::Vector4& diffuse, 
	const LibMath::Vector4& specular)
	:	Light(rotationMatrix, ambient, diffuse, specular)
{
	m_direction = rotationMatrix * LibMath::Vector4(0, 0, -1, 0);
	
}

void DirectionalLight::updateLightData(const LibMath::Matrix4& rotationMat,
	const LibMath::Vector4& ambient,
	const LibMath::Vector4& diffuse,
	const LibMath::Vector4& specular)
{
	m_transform = rotationMat;
	m_direction = rotationMat * LibMath::Vector4(0, 0, -1, 0);
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
}

void DirectionalLight::uploadLightToGPU(GLuint shaderProgram, const std::string& uniformName, int index)
{
	// world‐space direction = transform * (0,0,-1,0)
	//LibMath::Vector4 direction = m_transform * LibMath::Vector4(0, 0, -1, 0);
	//direction.nor;

	//LibMath::Vector3 direction (0.f, -10.f, 0.f);

	GLint dirLoc = glGetUniformLocation(shaderProgram, (uniformName + ".direction").c_str());
	if (dirLoc != -1)
		glUniform3f(dirLoc, m_direction.getX(), m_direction.getY(), m_direction.getZ());

	GLint ambLoc = glGetUniformLocation(shaderProgram, (uniformName + ".ambient").c_str());
	if (ambLoc != -1)
		glUniform4f(ambLoc, m_ambient.getX(), m_ambient.getY(), m_ambient.getZ(), m_ambient.getK());

	GLuint diffLoc = glGetUniformLocation(shaderProgram, (uniformName + ".diffuse").c_str());
	if (diffLoc != -1)
		glUniform4f(diffLoc, m_diffuse.getX(), m_diffuse.getY(), m_diffuse.getZ(), m_diffuse.getK());

	GLuint specLoc = glGetUniformLocation(shaderProgram, (uniformName + ".specular").c_str());
	if (specLoc != -1)
		glUniform4f(specLoc, m_specular.getX(), m_specular.getY(), m_specular.getZ(), m_specular.getK());
}

PointLight::PointLight(const LibMath::Vector3& position, 
	const LibMath::Vector4& ambient, 
	const LibMath::Vector4& diffuse, 
	const LibMath::Vector4& specular, 
	const float& constant, const float& linear, const float& quadratic)
	: Light(LibMath::Matrix4::createTranslate(position), ambient, diffuse, specular)
{
	m_position = position;
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}

PointLight::PointLight(const LibMath::Matrix4& translationMatrix, 
	const LibMath::Vector4& ambient, 
	const LibMath::Vector4& diffuse, 
	const LibMath::Vector4& specular, 
	const float& constant, const float& linear, const float& quadratic)
	: Light(translationMatrix, ambient, diffuse, specular)
{
	m_position = translationMatrix * LibMath::Vector4(0, 0, 0, 1);
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}

void PointLight::updateLightData(const LibMath::Matrix4& translateMat,
	const LibMath::Vector4& ambient,
	const LibMath::Vector4& diffuse,
	const LibMath::Vector4& specular)
{
	m_transform = translateMat;
	m_position = translateMat * LibMath::Vector4(0, 0, 0, 1);
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
}

void PointLight::updateLightData(
	const LibMath::Vector4& ambient,
	const LibMath::Vector4& diffuse,
	const LibMath::Vector4& specular)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
}

void PointLight::updatePointLightData(const float& constant, const float& linear, const float& quadratic)
{
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}

void PointLight::uploadLightToGPU(GLuint shaderProgram, const std::string& uniformName, int index)
{
	std::string base = uniformName + "[" + std::to_string(index) + "]";
	//LibMath::Vector4 position = m_transform * LibMath::Vector4(0, 0, 0, 1);

	GLint loc = glGetUniformLocation(shaderProgram, (base + ".position").c_str());
	if (loc != -1) 
		glUniform3f(loc, m_position.getX(), m_position.getY(), m_position.getZ());

	loc = glGetUniformLocation(shaderProgram, (base + ".ambient").c_str());
	if (loc != -1)
		glUniform4f(loc, m_ambient.getX(), m_ambient.getY(), m_ambient.getZ(), m_ambient.getK());

	loc = glGetUniformLocation(shaderProgram, (base + ".diffuse").c_str());
	if (loc != -1)
		glUniform4f(loc, m_diffuse.getX(), m_diffuse.getY(), m_diffuse.getZ(), m_diffuse.getK());

	loc = glGetUniformLocation(shaderProgram, (base + ".specular").c_str());
	if (loc != -1)
		glUniform4f(loc, m_specular.getX(), m_specular.getY(), m_specular.getZ(), m_specular.getK());

	loc = glGetUniformLocation(shaderProgram, (base + ".constant").c_str());
	if (loc != -1)
		glUniform1f(loc, m_constant);

	loc = glGetUniformLocation(shaderProgram, (base + ".linear").c_str());
	if (loc != -1)
		glUniform1f(loc, m_linear);

	loc = glGetUniformLocation(shaderProgram, (base + ".quadratic").c_str());
	if (loc != -1)
		glUniform1f(loc, m_quadratic);
}

SpotLight::SpotLight(const LibMath::Vector3& position, const LibMath::Radian& rotation,
	const LibMath::Vector4& ambient,
	const LibMath::Vector4& diffuse,
	const LibMath::Vector4& specular,
	const float& constant, const float& linear, const float& quadratic,
	const float& innerCutOff, const float& outerCutOff)
	: Light(LibMath::Matrix4::createTranslate(position) * LibMath::Matrix4::createRotationY(rotation),
		ambient, diffuse, specular)
{ 
	LibMath::Matrix4 transformMat = LibMath::Matrix4::createTranslate(position) * LibMath::Matrix4::createRotationY(rotation);
	m_direction = transformMat * LibMath::Vector4(0.f, 0.f, -1.f, 0.f);
	m_position = position;
	
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
	m_cutOff = innerCutOff;
	m_outerCutOff = outerCutOff;
}

SpotLight::SpotLight(const LibMath::Matrix4& transformMatrix,
	const LibMath::Vector4& ambient,
	const LibMath::Vector4& diffuse,
	const LibMath::Vector4& specular,
	const float& constant, const float& linear, const float& quadratic,
	const float& innerCutOff, const float& outerCutOff)
	: Light(transformMatrix, ambient, diffuse, specular)
{
	m_direction = transformMatrix * LibMath::Vector4(0.f, 0.f, -1.f, 0.f);
	m_position = transformMatrix * LibMath::Vector4(0.f, 0.f, 0.f, 1.f);

	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
	m_cutOff = innerCutOff;
	m_outerCutOff = outerCutOff;
}

void SpotLight::updateLightData(const LibMath::Matrix4& transformMatrix,
	const LibMath::Vector4& ambient,
	const LibMath::Vector4& diffuse,
	const LibMath::Vector4& specular)
{
	m_direction = transformMatrix * LibMath::Vector4(0.f, 0.f, -1.f, 0.f);
	m_position = transformMatrix * LibMath::Vector4(0.f, 0.f, 0.f, 1.f);
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
}

void SpotLight::updateLightData(
	const LibMath::Vector4& ambient,
	const LibMath::Vector4& diffuse,
	const LibMath::Vector4& specular)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
}

void SpotLight::updateSpotLightData(const float& constant, const float& linear, const float& quadratic, const float& innerCutOff, const float& outerCutOff)
{
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
	m_cutOff = innerCutOff;
	m_outerCutOff = outerCutOff;
}

void SpotLight::uploadLightToGPU(GLuint shaderProgram, const std::string& uniformName, int index)
{
	std::string base = "sLight[" + std::to_string(index) + "]";

	// World-space position = transform * (0,0,0,1)
	//LibMath::Vector4 position = m_transform * LibMath::Vector4(0, 0, 0, 1);
	GLint loc = glGetUniformLocation(shaderProgram, (base + ".position").c_str());
	if (loc != -1)
		glUniform3f(loc, m_position.getX(), m_position.getY(), m_position.getZ());

	// World-space direction = transform * (0,0,-1,0)
	//LibMath::Vector4 direction = m_transform * LibMath::Vector4(0, 0, -1, 0);

	loc = glGetUniformLocation(shaderProgram, (base + ".direction").c_str());
	if (loc != -1)
		glUniform3f(loc, m_direction.getX(), m_direction.getY(), m_direction.getZ());

	// Ambient color
	loc = glGetUniformLocation(shaderProgram, (base + ".ambient").c_str());
	if (loc != -1)
		glUniform4f(loc, m_ambient.getX(), m_ambient.getY(), m_ambient.getZ(), m_ambient.getK());

	// Diffuse color
	loc = glGetUniformLocation(shaderProgram, (base + ".diffuse").c_str());
	if (loc != -1)
		glUniform4f(loc, m_diffuse.getX(), m_diffuse.getY(), m_diffuse.getZ(), m_diffuse.getK());

	// Specular color
	loc = glGetUniformLocation(shaderProgram, (base + ".specular").c_str());
	if (loc != -1)
		glUniform4f(loc, m_specular.getX(), m_specular.getY(), m_specular.getZ(), m_specular.getK());

	// Attenuation - Constant
	loc = glGetUniformLocation(shaderProgram, (base + ".constant").c_str());
	if (loc != -1)
		glUniform1f(loc, m_constant);

	// Attenuation - Linear
	loc = glGetUniformLocation(shaderProgram, (base + ".linear").c_str());
	if (loc != -1)
		glUniform1f(loc, m_linear);

	// Attenuation - Quadratic
	loc = glGetUniformLocation(shaderProgram, (base + ".quadratic").c_str());
	if (loc != -1)
		glUniform1f(loc, m_quadratic);

	// Inner cutoff angle
	loc = glGetUniformLocation(shaderProgram, (base + ".cutOff").c_str());
	if (loc != -1)
		glUniform1f(loc, m_cutOff);

	// Outer cutoff angle
	loc = glGetUniformLocation(shaderProgram, (base + ".outerCutOff").c_str());
	if (loc != -1)
		glUniform1f(loc, m_outerCutOff);
}

