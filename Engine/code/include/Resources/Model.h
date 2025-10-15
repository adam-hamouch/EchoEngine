#pragma once
#include "IModel.h"

#ifdef APIENTRY
#undef APIENTRY
#endif
#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

class Vertex
{
public:
	Vertex() = default; 
	~Vertex() = default;

	Vertex(Vector3D pos, Vector2D text, Vector3D n)
	{
		position = pos;
		normal = n;
		textureUV = text;
	}

	Vector3D position = { 0, 0, 0 };
	Vector2D textureUV = { 0, 0 };
	Vector3D normal = { 0, 0, 0 };
};

class Buffer
{
public:
	Buffer() = default; 
	~Buffer() = default; 

	GLuint VBO;
	GLuint EBO;

	void Init(std::vector<Vertex>& vertices, std::vector<uint32_t>& index);
	void UnbindBuffer();
};

class VertexAttributes
{
public:
	VertexAttributes() = default; 
	~VertexAttributes() = default; 

	GLuint VAO;

	void Bind();
	void Init();
};

class ModelGL : public IModel
{ 
public:
	ModelGL() = default; 
	~ModelGL() override = default; 

	std::vector<uint32_t> index;
	VertexAttributes vertexAttributes;
	Buffer buffer;

	ModelGL* GetGL() override { return this; }

private:
	void LoadResource(const char* path) override;
	void ProcessNode(aiNode* node, const aiScene* scene); 
	void ProcessMesh(aiMesh* mesh, const aiScene* scene); 

	void LoadBuffer();

	std::vector<Vector3D> positions;
	std::vector<Vector2D> uv;
	std::vector<Vector3D> normals;
	std::vector<Vertex> vertices;
};