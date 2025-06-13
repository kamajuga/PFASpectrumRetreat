#pragma once
#ifndef __gl_h_
#include "glad/glad.h"

#include "GLFW/glfw3.h"
#endif // __gl_h_
#include "LibMath/Vector.h"

class Material
{
public:
	Material() = default;
	~Material() = default;

	Material(LibMath::Vector4 ambient, LibMath::Vector4 diffuse, LibMath::Vector4 specular, float shininess)
		: m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {
	}

	void uploadMaterialToGPU(GLuint shaderProgram, const std::string& uniformName);

	LibMath::Vector4 m_ambient = LibMath::Vector4(1.0);
	LibMath::Vector4 m_diffuse = LibMath::Vector4(1.0);
	LibMath::Vector4 m_specular = LibMath::Vector4(1.0);
	void setAlpha(const float& materialAlpha);
	float m_shininess = 32.0f;

	
private:


};



