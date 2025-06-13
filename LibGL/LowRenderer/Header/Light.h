#pragma once
#include "LibMath/Vector.h"
#include "LibMath/Matrix/Matrix4.h"
#include "LibMath/Vector/Vector3.h"
#include "LibMath/Matrix4Vector4Operation.h"
#include "LibMath/Angle/Radian.h"

#ifndef __gl_h_
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#endif // __gl_h_

#define _USE_MATH_DEFINES 
#include <math.h>

class Light
{
public:
	Light() = default;
	Light(const LibMath::Matrix4& transform, const LibMath::Vector4& ambient, const LibMath::Vector4& diffuse, const LibMath::Vector4& specular);
	~Light() = default;

	LibMath::Vector4 m_ambient;
	LibMath::Vector4 m_diffuse;
	LibMath::Vector4 m_specular;

	LibMath::Matrix4 m_transform;

	static void uploadLightCountToGPU(GLuint shaderProgram, const std::string& uniformName, int count);

	virtual void uploadLightToGPU(GLuint shaderProgram, const std::string& uniformName, int index)
	{
		return;
	}
private:

};

class DirectionalLight : public Light
{
public:
	DirectionalLight() = default;

	DirectionalLight(const LibMath::Radian& rotation,
		const LibMath::Vector4& ambient = LibMath::Vector4(0.4f, 0.4f, 0.4f, 1.f),
		const LibMath::Vector4& diffuse = LibMath::Vector4(0.6f, 0.6f, 0.6f, 1.f),
		const LibMath::Vector4& specular = LibMath::Vector4(0.5f, 0.5f, 0.5f, 1.f));

	DirectionalLight(const LibMath::Matrix4& rotationMatrix, 
		const LibMath::Vector4& ambient = LibMath::Vector4(0.4f, 0.4f, 0.4f, 1.f),
		const LibMath::Vector4& diffuse = LibMath::Vector4(0.6f, 0.6f, 0.6f, 1.f),
		const LibMath::Vector4& specular = LibMath::Vector4(0.5f, 0.5f, 0.5f, 1.f));
	~DirectionalLight() = default;

	LibMath::Vector3 m_direction;

	void updateLightData(const LibMath::Matrix4& rotationMat, 
		const LibMath::Vector4& ambient = LibMath::Vector4(0.4f, 0.4f, 0.4f, 1.f),
		const LibMath::Vector4& diffuse = LibMath::Vector4(0.6f, 0.6f, 0.6f, 1.f),
		const LibMath::Vector4& specular = LibMath::Vector4(0.5f, 0.5f, 0.5f, 1.f));

	void uploadLightToGPU(GLuint shaderProgram, const std::string& uniformName, int index) override;
private:

};

class PointLight : public Light
{
public:
	PointLight() = default;
	PointLight(const LibMath::Vector3& position, 
		const LibMath::Vector4& ambient = LibMath::Vector4(0.4f, 0.4f, 0.4f, 1.f),
		const LibMath::Vector4& diffuse = LibMath::Vector4(0.6f, 0.6f, 0.6f, 1.f),
		const LibMath::Vector4& specular = LibMath::Vector4(0.5f, 0.5f, 0.5f, 1.f),
		const float& constant = 1.f, const float& linear = 0.14, const float& quadratic = 0.07f);

	PointLight(const LibMath::Matrix4& translationMatrix,
		const LibMath::Vector4& ambient = LibMath::Vector4(0.4f, 0.4f, 0.4f, 1.f),
		const LibMath::Vector4& diffuse = LibMath::Vector4(0.6f, 0.6f, 0.6f, 1.f),
		const LibMath::Vector4& specular = LibMath::Vector4(0.5f, 0.5f, 0.5f, 1.f),
		const float& constant = 1.f, const float& linear = 0.14, const float& quadratic = 0.07f);

	void updateLightData(const LibMath::Matrix4& translateMat,
		const LibMath::Vector4& ambient = LibMath::Vector4(0.4f, 0.4f, 0.4f, 1.f),
		const LibMath::Vector4& diffuse = LibMath::Vector4(0.6f, 0.6f, 0.6f, 1.f),
		const LibMath::Vector4& specular = LibMath::Vector4(0.5f, 0.5f, 0.5f, 1.f));

	void updateLightData(
		const LibMath::Vector4& ambient = LibMath::Vector4(0.4f, 0.4f, 0.4f, 1.f),
		const LibMath::Vector4& diffuse = LibMath::Vector4(0.6f, 0.6f, 0.6f, 1.f),
		const LibMath::Vector4& specular = LibMath::Vector4(0.5f, 0.5f, 0.5f, 1.f));

	void updatePointLightData(const float& constant, const float& linear, const float& quadratic);

	~PointLight() = default;

	LibMath::Vector3 m_position;
	float m_constant;
	float m_linear;
	float m_quadratic;

	void uploadLightToGPU(GLuint shaderProgram, const std::string& uniformName, int index) override;
private:

};

class SpotLight : public Light
{
public:
	SpotLight() = default;
	SpotLight(const LibMath::Vector3& position, const LibMath::Radian& rotation,
		const LibMath::Vector4& ambient = LibMath::Vector4(0.4f, 0.4f, 0.4f, 1.f),
		const LibMath::Vector4& diffuse = LibMath::Vector4(0.6f, 0.6f, 0.6f, 1.f),
		const LibMath::Vector4& specular = LibMath::Vector4(0.5f, 0.5f, 0.5f, 1.f),
		const float& constant = 1.f, const float& linear = 0.09f, const float& quadratic = 0.032f,
		const float& innerCutOff = static_cast<float>(cos(M_PI / 4)),
		const float& outerCutOff = static_cast<float>(cos(M_PI / 3)));

	SpotLight(const LibMath::Matrix4& transformMatrix, 
		const LibMath::Vector4& ambient = LibMath::Vector4(0.4f, 0.4f, 0.4f, 1.f),
		const LibMath::Vector4& diffuse = LibMath::Vector4(0.6f, 0.6f, 0.6f, 1.f),
		const LibMath::Vector4& specular = LibMath::Vector4(0.5f, 0.5f, 0.5f, 1.f),
		const float& constant = 1.f, const float& linear = 0.09f, const float& quadratic = 0.032f,
		const float& innerCutOff = static_cast<float>(cos(M_PI / 4)),
		const float& outerCutOff = static_cast<float>(cos(M_PI / 3)));

	void updateLightData(const LibMath::Matrix4& transformMatrix,
		const LibMath::Vector4& ambient = LibMath::Vector4(0.4f, 0.4f, 0.4f, 1.f),
		const LibMath::Vector4& diffuse = LibMath::Vector4(0.6f, 0.6f, 0.6f, 1.f),
		const LibMath::Vector4& specular = LibMath::Vector4(0.5f, 0.5f, 0.5f, 1.f));

	void updateLightData(
		const LibMath::Vector4& ambient = LibMath::Vector4(0.4f, 0.4f, 0.4f, 1.f),
		const LibMath::Vector4& diffuse = LibMath::Vector4(0.6f, 0.6f, 0.6f, 1.f),
		const LibMath::Vector4& specular = LibMath::Vector4(0.5f, 0.5f, 0.5f, 1.f));

	void updateSpotLightData(const float& constant, const float& linear, const float& quadratic, const float& innerCutOff, const float& outerCutOff);

	~SpotLight() = default;

	LibMath::Vector3 m_position;
	LibMath::Vector3 m_direction;
	float m_constant;
	float m_linear;
	float m_quadratic;
	float m_cutOff;
	float m_outerCutOff;


	void uploadLightToGPU(GLuint shaderProgram, const std::string& uniformName, int index) override;
private:
};