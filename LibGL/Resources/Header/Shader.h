#pragma once
#include <fstream>
#include <sstream>
#include <iostream>	

#ifndef __gl_h_
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#endif // __gl_h_

#include "LibMath/Matrix/Matrix4.h"
#include "string"


#include "IRessource.h"

class Shader : public IRessource
{
public:
	Shader() : m_program(0), m_vertexShader(0), m_fragmentShader(0) {}

	bool setVertexShader(std::string const& filepath);
	bool setFragmentShader(std::string const& filepath);
	bool link(void);

	int  getId(void) { return m_program; };

	void use(void) { glUseProgram(m_program); };
	void setMat4(const std::string& uniformName, LibMath::Matrix4& matrix);
	void setInt(const std::string& uniformName, const int& value);
	void setVec3(const std::string& uniformName, const LibMath::Vector3& vector);

	~Shader(); // delete shaders and program

	int m_program;
private:

	int m_vertexShader;
	int m_fragmentShader;

	void loadShaderFile(std::string filepath, std::stringstream& buffer);

};