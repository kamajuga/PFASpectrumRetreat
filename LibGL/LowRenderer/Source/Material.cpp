#include "Material.h"

void Material::uploadMaterialToGPU(GLuint shaderProgram, const std::string& uniformName)
{

	GLint loc = glGetUniformLocation(shaderProgram, "material.ambient");
	if (loc != -1)
		glUniform4f(loc, m_ambient.getX(), m_ambient.getY(), m_ambient.getZ(), m_ambient.getK());

	loc = glGetUniformLocation(shaderProgram, "material.diffuse");
	if (loc != -1)
		glUniform4f(loc, m_diffuse.getX(), m_diffuse.getY(), m_diffuse.getZ(), m_diffuse.getK());

	loc = glGetUniformLocation(shaderProgram, "material.specular");
	if (loc != -1)
		glUniform4f(loc, m_specular.getX(), m_specular.getY(), m_specular.getZ(), m_specular.getK());

	loc = glGetUniformLocation(shaderProgram, "material.shininess");
	if (loc != -1)
		glUniform1f(loc, m_shininess);

}

void Material::setAlpha(const float& materialAlpha)
{
	m_ambient.getK() = materialAlpha;
	m_diffuse.getK() = materialAlpha;
	m_specular.getK() = materialAlpha;
}
