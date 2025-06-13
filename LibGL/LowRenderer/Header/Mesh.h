#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "LibMath/Matrix.h"
#include "Shader.h"
#include "Model.h"
#include "Material.h"

class Mesh
{
public:
	Mesh() = default;
	//Mesh(Model* Model);
	Mesh(std::shared_ptr<Model> model);

	void setModelMatrix(LibMath::Vector3 translate = LibMath::Vector3(0.0f, 0.0f, 0.0f), // returns T . R . S
						LibMath::Radian rotation = LibMath::Radian(0.0f),
						LibMath::Vector3 scale = LibMath::Vector3(1.0f, 1.0f, 1.0f),
						LibMath::Vector3 axis = LibMath::Vector3(0.0f, 1.0f, 0.0f));

	void setModelMatrix(LibMath::Matrix4 const& modelMatrix)
	{
		m_modelMatrix = modelMatrix * m_modelMatrix;
	}

	LibMath::Matrix4 getModelMatrix(void) const
	{
		return m_modelMatrix;
	}
	void draw(Shader& shader, LibMath::Matrix4 VPMatrix);
	
	void uploadModelMatrixToGPU(GLuint shaderProgram, const std::string& uniformName);

	void uploadMaterialToGPU(GLuint shaderProgram, const std::string& uniformName)
	{
		m_material.uploadMaterialToGPU(shaderProgram, uniformName);
	}

	void SetMaterial(LibMath::Vector4 ambient, LibMath::Vector4 diffuse, LibMath::Vector4 specular, float shininess);

	void setMaterial(const Material& material)
	{
		m_material = material;
	}

	Material& getMaterial(void)
	{
		return m_material;
	}

	std::vector<Vertex>& getModelVertices(void) { return m_model->getVertices(); };
	std::vector<uint32_t>& getModelIndices(void) { return m_model->getIndices(); };

	std::shared_ptr<Model> getModel(void) const
	{
		return m_model;
	}


	~Mesh() = default;

private:
	std::shared_ptr<Model> m_model = nullptr;
	Material m_material;
	LibMath::Matrix4 m_modelMatrix;
	LibMath::Matrix4 m_MVPMatrix;

};
