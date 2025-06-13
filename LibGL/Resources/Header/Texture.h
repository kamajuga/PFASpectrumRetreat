#pragma once
#include <vector>
#include <fstream>
#include <sstream>

#ifndef __gl_h_
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#endif // __gl_h_

#include "IRessource.h"
#include "LibMath/Vector/Vector2.h"
#include "LibMath/Vector/Vector3.h"

class Texture : public IRessource
{
public:
	Texture() = default;
	Texture(const Texture& texture);

	void loadTexture(const std::string& path, bool flipImage = true);

	GLuint getSampler(void) { return m_sampler; };
	GLuint getTextureID(void) { return m_textureID; };
	unsigned int getTextureUnit(void) { return m_textureUnit;  };

	void generateTexture(void);
	void bind(void) const;
	void generateTextureImage(void);
	void bindTextureUnit(unsigned int unit = 0);
	void setTextureParameters(void);
	void activate(unsigned int slot = 0) const;
	void unbind(void);

	~Texture();

private:

	GLuint m_textureID = 0;
	GLuint m_sampler = 0;

	unsigned int m_textureUnit = 0;
	unsigned char* m_data = nullptr;
	int m_width = 0;
	int m_height = 0;
	int m_channels = 0;

};