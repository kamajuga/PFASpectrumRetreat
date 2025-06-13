#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#ifndef __gl_h_
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#endif // __gl_h_

#include "IRessource.h"
#include "Texture.h"
#include "LibMath/Vector/Vector2.h"
#include "LibMath/Vector/Vector3.h"


struct Vertex
{
	LibMath::Vector2 m_textureUV;
	LibMath::Vector3 m_position;
	LibMath::Vector3 m_normal;
};

struct TempBuffers
{
    std::vector<LibMath::Vector2> m_tempUV;
    std::vector<LibMath::Vector3> m_tempPosition;
    std::vector < LibMath::Vector3> m_tempNormal;
};

struct FinalData
{
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::unordered_map<std::string, uint32_t> m_vertexMap;
};

class Model : public IRessource
{
public:
    Model() : m_VAO(0), m_VBO(0), m_EBO(0), m_vertices(), m_indices(), m_texture(nullptr) {} // Initialize all members  
    Model(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

    std::vector<Vertex>& getVertices(void);
    std::vector<uint32_t>& getIndices(void);
    
    void bind(void);

    void setTexture(Texture* texture);

    class VertexBuffer
    {
    public:
        VertexBuffer() = default;
        ~VertexBuffer() { glBindBuffer(GL_ARRAY_BUFFER, 0); };

        void generateBuffer(GLuint& VBO);
        void bind(GLuint& VBO, size_t bufferSize, Vertex* data);
        void unbind(void) const;

    private:

    };

    class VertexAttributes
    {
    public:
        VertexAttributes() = default;
        ~VertexAttributes() { glBindVertexArray(0); };

        void generateBuffer(GLuint& VAO);
        void bind(GLuint& VAO) const;
        void unbind(void) const;

    private:

    };

    class ElementBuffer
    {
    public:
        ElementBuffer() = default;
        ~ElementBuffer() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };

        void generateBuffer(GLuint& EBO);
        void bind(GLuint& EBO, size_t bufferSize, uint32_t* data);
        void unbind(void) const;

    private:
    };

    ~Model();

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    Texture* m_texture;

private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    VertexBuffer VBO;
    VertexAttributes VAO;
    ElementBuffer EBO;
    
};

class Parser
{
public:
	Parser() = default;

	~Parser() = default;

	static Model parse3DObject(const std::string& path);

private:

	static std::ifstream loadFile(const std::string& path);
	static void handleNegativeIndex(int& index, size_t size);
	static void extractIndices(const std::string& line, int& vertexIndex, int& uvIndex, int& normalIndex);
    static void parseVertexLine(const std::istringstream& iss, TempBuffers& buffers);
    static void parseUvLine(const std::istringstream& iss, TempBuffers& buffers);
    static void parseNormalLine(const std::istringstream& iss, TempBuffers& buffers);
    static Vertex createVertex(const std::string& token, const TempBuffers& buffers);
    static void processFaceToken(const std::string& token, const TempBuffers& buffers, FinalData& data);
    static void parseFaceLine(const std::istringstream& iss, const TempBuffers& buffers, FinalData& data);


};

