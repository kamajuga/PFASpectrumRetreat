#include "Model.h"


Model::Model(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	m_VAO = 0;
	m_VBO = 0;
	m_EBO = 0;
	m_vertices = vertices;
	m_indices = indices;
	m_texture = nullptr;
}

std::vector<Vertex>& Model::getVertices(void)
{
	return m_vertices;
}

std::vector<uint32_t>& Model::getIndices(void) 
{
	return m_indices;
}

void Model::bind(void)
{
	// Vertex buffer object
	VBO.generateBuffer(m_VBO);
	VBO.bind(m_VBO, m_vertices.size(), m_vertices.data());

	// Vertex array object
	VAO.generateBuffer(m_VAO);
	VAO.bind(m_VAO);

	// Element buffer object
	EBO.generateBuffer(m_EBO);
	EBO.bind(m_EBO, m_indices.size(), m_indices.data());
}

void Model::setTexture(Texture* texture)
{
	Texture* text = new Texture(*texture);
	m_texture = text;
}


Model Parser::parse3DObject(const std::string& path)
{
	std::ifstream file = loadFile(path);
	std::string line;
	TempBuffers buffers;
	FinalData data;

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "v") {
			parseVertexLine(iss, buffers);
		}
		else if (prefix == "vt") {
			parseUvLine(iss, buffers);
		}
		else if (prefix == "vn") {
			parseNormalLine(iss, buffers);
		}
		else if (prefix == "f") {
			parseFaceLine(iss, buffers, data);
		}
	}

	file.close();
	return Model(data.m_vertices, data.m_indices);

	////std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	//std::vector<LibMath::Vector3> temp_position;
	//std::vector<LibMath::Vector2 > temp_uvs;
	//std::vector<LibMath::Vector3> temp_normals;

	//std::vector<Vertex> modelVertices;
	//std::vector<uint32_t> modelIndices;
	//std::unordered_map<std::string, uint32_t> uniqueVertexMap;


	//while (std::getline(file, line))
	//{
	//	// Convert to string stream
	//	std::istringstream iss(line);
	//	std::string prefix;
	//	iss >> prefix; // Read the first word of the line

	//	if (prefix == "v")
	//	{
	//		LibMath::Vector3 vertex;

	//		// Try to read three floating numbers, and store them in the vertex
	//		iss >> vertex.getX() >> vertex.getY() >> vertex.getZ();
	//		temp_position.push_back(vertex);
	//	}
	//	else 
	//	if (prefix == "vt")
	//	{
	//		LibMath::Vector2 uv;

	//		 // Try to read two floating numbers, and store them in the uv
	//		iss >> uv.getX() >> uv.getY();
	//		temp_uvs.push_back(uv);
	//	}
	//	else if (prefix == "vn")
	//	{
	//		LibMath::Vector3 normal;

	//		// Try to read three floating numbers, and store them in the normal
	//		iss >> normal.getX() >> normal.getY() >> normal.getZ();
	//		temp_normals.push_back(normal);
	//	}
	//	else if (prefix == "f")
	//	{
	//		std::string vertexTextureNormal[3]; // Index 0 for vertex, 1 for uv, 2 for normal
	//		int vertexIndex, uvIndex, normalIndex;

	//		iss >> vertexTextureNormal[0] >> vertexTextureNormal[1] >> vertexTextureNormal[2];

	//		for (const std::string& token : vertexTextureNormal)
	//		{
	//			// Check if the vertex is already in the map
	//			if (uniqueVertexMap.find(token) != uniqueVertexMap.end())
	//			{
	//				modelIndices.push_back(uniqueVertexMap[token]);
	//				continue;
	//			}

	//			// If not, add it to the map and the vertex vector
	//			extractIndices(token, vertexIndex, uvIndex, normalIndex);

	//			handleNegativeIndex(vertexIndex, temp_position.size());
	//			handleNegativeIndex(uvIndex, temp_uvs.size());
	//			handleNegativeIndex(normalIndex, temp_normals.size());


	//			Vertex finalVertex;
	//			finalVertex.m_position = (vertexIndex >= 0 && vertexIndex < temp_position.size()) ? temp_position[vertexIndex] : LibMath::Vector3();
	//			finalVertex.m_textureUV = (uvIndex >= 0 && uvIndex < temp_uvs.size()) ? temp_uvs[uvIndex] : LibMath::Vector2();
	//			finalVertex.m_normal = (normalIndex >= 0 && normalIndex < temp_normals.size()) ? temp_normals[normalIndex] : LibMath::Vector3();

	//			uint32_t newIndex = static_cast<uint32_t>(modelVertices.size());
	//			modelVertices.push_back(finalVertex);
	//			modelIndices.push_back(newIndex);
	//			uniqueVertexMap[token] = newIndex;
	//				
	//		}
	//	}

	//} 
	
	/*file.close();

	return Model(modelVertices, modelIndices);*/
}

void Parser::parseVertexLine(const std::istringstream& iss, TempBuffers& buffers) {
	LibMath::Vector3 vertex;
	const_cast<std::istringstream&>(iss) >> vertex.getX() >> vertex.getY() >> vertex.getZ();
	buffers.m_tempPosition.push_back(vertex);
}

void Parser::parseUvLine(const std::istringstream& iss, TempBuffers& buffers) {
	LibMath::Vector2 uv;
	const_cast<std::istringstream&>(iss) >> uv.getX() >> uv.getY();
	buffers.m_tempUV.push_back(uv);
}

void Parser::parseNormalLine(const std::istringstream& iss, TempBuffers& buffers) {
	LibMath::Vector3 normal;
	const_cast<std::istringstream&>(iss) >> normal.getX() >> normal.getY() >> normal.getZ();
	buffers.m_tempNormal.push_back(normal);
}

Vertex Parser::createVertex(const std::string& token, const TempBuffers& buffers) {
	int vertexIndex, uvIndex, normalIndex;
	extractIndices(token, vertexIndex, uvIndex, normalIndex);

	handleNegativeIndex(vertexIndex, buffers.m_tempPosition.size());
	handleNegativeIndex(uvIndex, buffers.m_tempUV.size());
	handleNegativeIndex(normalIndex, buffers.m_tempNormal.size());

	Vertex vertex;
	vertex.m_position = (vertexIndex >= 0 && vertexIndex < buffers.m_tempPosition.size())
		? buffers.m_tempPosition[vertexIndex] : LibMath::Vector3();
	vertex.m_textureUV = (uvIndex >= 0 && uvIndex < buffers.m_tempUV.size())
		? buffers.m_tempUV[uvIndex] : LibMath::Vector2();
	vertex.m_normal = (normalIndex >= 0 && normalIndex < buffers.m_tempNormal.size())
		? buffers.m_tempNormal[normalIndex] : LibMath::Vector3();

	return vertex;
}

void Parser::processFaceToken(const std::string& token, const TempBuffers& buffers, FinalData& data) {
	if (data.m_vertexMap.find(token) != data.m_vertexMap.end()) {
		data.m_indices.push_back(data.m_vertexMap[token]);
		return;
	}

	Vertex vertex = createVertex(token, buffers);
	uint32_t newIndex = static_cast<uint32_t>(data.m_vertices.size());
	data.m_vertices.push_back(vertex);
	data.m_indices.push_back(newIndex);
	data.m_vertexMap[token] = newIndex;
}

void Parser::parseFaceLine(const std::istringstream& iss, const TempBuffers& buffers, FinalData& data) {
	std::string tokens[3];
	const_cast<std::istringstream&>(iss) >> tokens[0] >> tokens[1] >> tokens[2];

	for (const std::string& token : tokens) {
		processFaceToken(token, buffers, data);
	}
}


std::ifstream Parser::loadFile(const std::string& path)
{
	std::ifstream file (path.data());
	if (!file.is_open()) {
		std::cerr << "Erreur d'ouverture du fichier" << std::endl;
		return file;
	}

	return file;
}

void Parser::handleNegativeIndex(int& index, size_t size)
{
	if (index < 0)
		index += size;
	else
		index -= 1;
}	

void Parser::extractIndices(const std::string& line, int& vertexIndex, int& uvIndex, int& normalIndex)
{
	// initialize the indices
	vertexIndex = uvIndex = normalIndex = 0;
	 
	// Convert to string stream
	std::stringstream ss(line);
	std::string indexHolder;
	int index = 0; //counter

	while (std::getline(ss, indexHolder, '/'))
	{
		int value = indexHolder.empty() ? 0 : std::stoi(indexHolder);
		switch (index)
		{
			
		case 0:
			vertexIndex = value;
			break;
		case 1:
			uvIndex =  value;
			break;
		case 2:
			normalIndex = value;
			break;
		default:
			break;
		}
		index++;
	}


}

void Model::VertexBuffer::generateBuffer(GLuint& VBO)
{

	glGenBuffers(1, &VBO);
}

void Model::VertexBuffer::bind(GLuint& VBO, size_t bufferSize, Vertex* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(Vertex), data, GL_STATIC_DRAW);
}

void Model::VertexBuffer::unbind(void) const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::VertexAttributes::generateBuffer(GLuint& VAO)
{
	glGenVertexArrays(1, &VAO);
}

void Model::VertexAttributes::bind(GLuint& VAO) const
{
	glBindVertexArray(VAO);


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);    // tex
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));    // pos
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));    // normal
	glEnableVertexAttribArray(2);

}

void Model::VertexAttributes::unbind(void) const
{
	glBindVertexArray(0);
}

void Model::ElementBuffer::generateBuffer(GLuint& EBO)
{
	glGenBuffers(1, &EBO);
}

void Model::ElementBuffer::bind(GLuint& EBO, size_t bufferSize, uint32_t* data)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize * sizeof(uint32_t),
										  data, GL_STATIC_DRAW);
}

void Model::ElementBuffer::unbind(void) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Model::~Model()
{
	// delete buffers
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	if (m_texture)
	{
		delete m_texture;
		m_texture = nullptr;
	}
}