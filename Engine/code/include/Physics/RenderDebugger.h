#pragma once
#include "Math/Math.h"

#ifdef APIENTRY
#undef APIENTRY
#endif
#include <glad/glad.h>

class IShader; 
class Camera;

class DebugVertex 
{
public:
	DebugVertex() = default;

	Vector3D position = { 0, 0, 0 };
	Vector4D color = { 0, 0, 0, 0 };
};

class DebugVertexAttributes
{
public:
	DebugVertexAttributes() = default;

	GLuint VAO;

	void Bind();
	void Init();
};

class DebugBuffer
{
public:
	DebugBuffer() = default;
	~DebugBuffer() = default;

	GLuint VBO;
	GLuint EBO;

	void Init(std::vector<DebugVertex>& vertices, std::vector<uint32_t>& index);
	void UnbindBuffer();
};

class RenderDebugger
{
public:
	RenderDebugger() = default; 

	void DrawTriangle(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2, const Vector4D& color);
	void DrawLine(const Vector3D& start, const Vector3D& end, const Vector4D& color);

	void DrawSphere(const Vector3D& center, float radius, const Vector3D& scale, const Vector4D& color);
	void DrawCube(const Vector3D& center, const Vector3D& halfSize, const Vector3D& scale, const Vector4D& color);
	void DrawCapsule(const Vector3D& center, float radius, float height, const Vector3D& scale, const Vector4D& color); 

	void DrawLines() const; 

	void SetCamera(Camera* _camera) { camera = _camera; }

	void Clear();

private:
	void LoadBuffer();
	std::vector<uint32_t> GenerateIndices(const std::vector<DebugVertex>& vertices);

	Camera* camera = nullptr;

	std::vector<DebugVertex> mVertices;
	std::vector<uint32_t> mIndices;

	DebugVertexAttributes vertexAttributes;
	DebugBuffer buffer;
};