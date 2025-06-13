#pragma once

#include <vector>
#include <string>

#ifndef __gl_h_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif // !__gl_h_

#include "LibMath/Matrix/Matrix4.h"
#include "Shader.h"

class SkyBox
{
public:
	SkyBox() = default;

	void loadTextures(std::vector<std::string>& faces);

	void loadShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	void setMat4(const std::string& uniformName, LibMath::Matrix4& matrix);

	void setInt(const std::string& uniformName, int value);

	void use(void);

	GLuint getTextureID(void) const { return m_textureID; }

	void draw(const LibMath::Matrix4& viewMat, LibMath::Matrix4& projectionMatrix);

	void generateBuffers(void);


	~SkyBox() = default;


	
private:
	GLuint m_textureID = 0;
	GLuint m_skyboxVAO = 0;
	GLuint m_skyboxVBO = 0;

	Shader m_shader;
};
