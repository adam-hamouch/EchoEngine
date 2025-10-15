#include "Resources/Model.h"
#include <fstream>
#include <limits>

void ModelGL::LoadResource(const char* path)
{
	_path = path;

	Assimp::Importer importer;
	const aiScene* scene = scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials | aiProcess_FindDegenerates | aiProcess_FindInvalidData | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_GenSmoothNormals | aiProcess_FixInfacingNormals); 

	if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		std::cerr << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
		return; 
	}

	ProcessNode(scene->mRootNode, scene);
	LoadBuffer(); 
}

void ModelGL::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void ModelGL::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> localVertices;
	std::vector<uint32_t> localIndices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.position = Vector3D(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		if (mesh->HasNormals())
		{
			vertex.normal = Vector3D(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);
		}
		else
		{
			vertex.normal = Vector3D(0, 0, 0);
		}

		if (mesh->mTextureCoords[0]) 
		{
			vertex.textureUV = Vector2D(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else
		{
			vertex.textureUV = Vector2D(0, 0);
		}

		localVertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			localIndices.push_back(face.mIndices[j]);
		}
	}

	uint32_t indexOffset = static_cast<uint32_t>(vertices.size());
	for (uint32_t i : localIndices)
		index.push_back(i + indexOffset);

	vertices.insert(vertices.end(), localVertices.begin(), localVertices.end());
}

void Buffer::Init(std::vector<Vertex>& vertices, std::vector<uint32_t>& index)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * index.size(), index.data(), GL_STATIC_DRAW);
}

void Buffer::UnbindBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexAttributes::Bind()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

void VertexAttributes::Init()
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(5 * sizeof(GLfloat)));

	glBindVertexArray(0);
}

void ModelGL::LoadBuffer()
{
	vertexAttributes.Bind();
	buffer.Init(vertices, index);
	vertexAttributes.Init();
	buffer.UnbindBuffer();
}

