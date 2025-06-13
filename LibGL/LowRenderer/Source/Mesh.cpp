#include "Mesh.h"
#include <iostream>

//Mesh::Mesh(Model* model)
//{
//	m_model = std::make_shared<Model>(*model);
//	m_modelMatrix = LibMath::Matrix4::identity();
//	m_MVPMatrix = LibMath::Matrix4::identity();
//}

Mesh::Mesh(std::shared_ptr<Model> model)
{
	m_model = model;
	m_material = Material();
	m_modelMatrix = LibMath::Matrix4::identity();
	m_MVPMatrix = LibMath::Matrix4::identity();
}
void Mesh::setModelMatrix(LibMath::Vector3 translate, LibMath::Radian rotation, LibMath::Vector3 scale, LibMath::Vector3 axis)
{
	m_modelMatrix = LibMath::Matrix4::createTransform(translate, rotation, scale);
}

void Mesh::draw(Shader& shader, LibMath::Matrix4 VPMatrix)
{
	// calculate the MVP matrix
	m_MVPMatrix = VPMatrix * m_modelMatrix;
	

	size_t size = m_model->getIndices().size();

	shader.use();

	uploadMaterialToGPU(shader.m_program, "material");

	// set the Model Matrix
	shader.setMat4("v_Model", m_modelMatrix);

	// set the MVP matrix in the shader
	shader.setMat4("v_MVP", m_MVPMatrix);

	// 3) Upload normal matrix as full 4×4 inverse-transpose
	LibMath::Matrix4 normalMat4 = m_modelMatrix.inverse().transpose();
	shader.setMat4("v_NormalMatrix", normalMat4);


	if (m_model->m_texture)
	{
		GLuint textureUnit = m_model->m_texture->getTextureUnit();
		m_model->m_texture->activate();
		m_model->m_texture->bind();

		shader.setInt("u_Texture", textureUnit);

		//glUniform1i(glGetUniformLocation(shader.m_program, "u_Texture"), textureUnit);
	}
	
	// Draw the model
	glBindVertexArray(m_model->m_VAO);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

	// This will be unboud in main loop


}

void Mesh::uploadModelMatrixToGPU(GLuint shaderProgram, const std::string& uniformName)
{
	GLint location = glGetUniformLocation(shaderProgram, uniformName.c_str());
	if (location == -1) {
		std::cerr << "Warning: uniform '" << uniformName << "' not found in shader program." << std::endl;
		//throw std::runtime_error("Model matrix uniform not found: " + uniformName);
		//return;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, m_modelMatrix.getPointer());
}

void Mesh::SetMaterial(LibMath::Vector4 ambient, LibMath::Vector4 diffuse, LibMath::Vector4 specular, float shininess)
{
	m_material = Material(ambient, diffuse, specular, shininess);
}

