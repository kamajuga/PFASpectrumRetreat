#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#include "Texture.h"

Texture::Texture(const Texture& texture)
{
	m_textureID = texture.m_textureID;
	m_sampler = texture.m_sampler;
	m_textureUnit = texture.m_textureUnit;
}

void Texture::loadTexture(const std::string& path, bool flipImage)
{
	if (m_textureID) {
		glDeleteTextures(1, &m_textureID);
		m_textureID = 0;
	}
	m_width, m_height, m_channels;
	stbi_set_flip_vertically_on_load(true);
	m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
	if (!m_data)
	{
		throw std::runtime_error("Failed to load texture: " + path);
	}
	GLenum format = GL_RGB;
	if (m_channels == 1)
		format = GL_RED;
	else if (m_channels == 3)
		format = GL_RGB;
	else if (m_channels == 4)
		format = GL_RGBA;

	generateTexture();
	bind();
	generateTextureImage();
	setTextureParameters();



}

void Texture::generateTexture(void)
{
	glGenTextures(1, &m_textureID);
}

void Texture::bind(void) const
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::generateTextureImage(void)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(m_data);
	m_data = nullptr;
}

void Texture::bindTextureUnit(unsigned int unit)
{
	glBindTextureUnit(unit, m_textureID);
	glBindSampler(unit, m_sampler);

	m_textureUnit = unit;
}

void Texture::setTextureParameters(void)
{
	glGenSamplers(1, &m_sampler);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(m_sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.f);

}

void Texture::activate(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
}

void Texture::unbind(void)
{
	if (m_textureID) {
		glDeleteTextures(1, &m_textureID);
		m_textureID = 0;
	}

	if (m_sampler) {
		glDeleteSamplers(1, &m_sampler);
		m_sampler = 0;
	}

}

Texture::~Texture()
{
	unbind();
	if (m_data)
	{
		m_data = nullptr;
	}
}
