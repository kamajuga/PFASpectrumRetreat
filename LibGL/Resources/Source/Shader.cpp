#include "Shader.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#ifndef __gl_h_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif // !__gl_h_


#define DEFAULT_ERROR_BUFFER 512

void  Shader::loadShaderFile(std::string filepath, std::stringstream& buffer)
{
	// Open the file
	std::ifstream file(filepath);
	buffer;

	// Check if the file is open
	if (!file.good())
	{
		std::cerr << "Error opening file: " << filepath << std::endl;
		return;
	}

	buffer << file.rdbuf(); // Read the file content into the buffer

 //return buffer.str().c_str();
}

bool Shader::setVertexShader(std::string const& filename)
{
	std::stringstream buffer;
	// Load the vertex shader source code from the file
	loadShaderFile(filename, buffer);
	std::string vertexShaderSourceStr = buffer.str(); // Store the stringstream content in a std::string  
	const char* vertexShaderSource = vertexShaderSourceStr.c_str();


	//std::cout << "[Shader] File loaded: " << std::endl << buffer.str() << "\n";

	

	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(m_vertexShader);

	// Check for compilation errors
	int success;
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[DEFAULT_ERROR_BUFFER];
		glGetShaderInfoLog(m_vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	return true;
}

bool Shader::setFragmentShader(std::string const& filepath)
{
	std::stringstream buffer;
	loadShaderFile(filepath, buffer );
	std::string fragmentShaderSourceStr = buffer.str(); // Store the stringstream content in a std::string  
	const char* fragmentShaderSource = fragmentShaderSourceStr.c_str();

	

	if (!fragmentShaderSource)
		return false;

	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(m_fragmentShader);

	// Check for compilation errors
	int success;
	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[DEFAULT_ERROR_BUFFER];
		glGetShaderInfoLog(m_fragmentShader, DEFAULT_ERROR_BUFFER, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}


	return true;
}

bool Shader::link(void)
{
	if (m_fragmentShader == 0 || m_vertexShader == 0)
	{
		std::cerr << "ERROR::SHADER::CANNOT CREATE PROGRAM, SHADER MISSING\n" << std::endl;
		return false;
	}
	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	glLinkProgram(m_program);


	//GLint count = 0;
	//glGetProgramiv(m_program, GL_ATTACHED_SHADERS, &count);
	//std::cout << "[Shader] after attach, ATTACHED_SHADERS=" << count << "\n";

	// Check for linking errors
	int success;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);

	if (!success)
	{
		char infoLog[DEFAULT_ERROR_BUFFER];
		glGetProgramInfoLog(m_program, DEFAULT_ERROR_BUFFER, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}

	// Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);

	return true;
}

void Shader::setMat4(const std::string& uniformName, LibMath::Matrix4& matrix)
{
	GLuint location = glGetUniformLocation(m_program, uniformName.c_str());
	if (location == -1) {
		std::cerr << "Warning: uniform '" << uniformName << "' not found in shader program." << std::endl;
		//throw std::runtime_error("Model matrix uniform not found: " + uniformName);
		return;
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, matrix.getPointer()); // set it manually
}

void Shader::setInt(const std::string& uniformName, const int& value)
{
	GLint location = glGetUniformLocation(m_program, uniformName.c_str());
	if (location == -1) 
	{
		std::cerr << "Warning: uniform '" << uniformName << "' not found in shader program." << std::endl;
		return;
	}
	glUniform1i(location, value);
}

void Shader::setVec3(const std::string& uniformName, const LibMath::Vector3& vector)
{
	GLuint location = glGetUniformLocation(m_program, uniformName.c_str());
	if (location == -1) {
		std::cerr << "Warning: uniform '" << uniformName << "' not found in shader program." << std::endl;
		return;
	}
	glUniform3f(location, vector.getX(), vector.getY(), vector.getZ()); // set it manually
}

Shader::~Shader()
{
	if (m_vertexShader)
		glDeleteShader(m_vertexShader);
	m_vertexShader = 0;

	if (m_fragmentShader)
	{
		glDeleteShader(m_fragmentShader);
		m_fragmentShader = 0;
	}

	if (m_program)
	{
		glDeleteProgram(m_program);
		m_program = 0;
	}
}
