#pragma once
#include "Math/Math.h"
#include "glad/glad.h"

#include <map>

#include "Utils/Engine.h"

class IShader;
class Camera; 

struct Character
{
	unsigned int textureID;
	Vector2D size;
	Vector2D bearing;
	unsigned int advance;
};

class TextBuffer
{
public:
	TextBuffer() = default;
	~TextBuffer();

	GLuint VBO;

	void Init();
	void UnbindBuffer();
};

class TextVertexAttributes
{
public:
	TextVertexAttributes() = default;
	~TextVertexAttributes();

	GLuint VAO;

	void Bind();
	void Init();
};

class RenderTextManager
{
public :
	RenderTextManager() = default; 
	~RenderTextManager() = default;

	ENGINE_API void Render(std::string text, float x, float y, float scale, Vector3D color);
	ENGINE_API void LoadFont(std::string path);
	ENGINE_API void LoadBuffer();

	ENGINE_API void SetProjection(const Mat4& projection) const;

	std::map<char, Character> characters; 

private:
	TextVertexAttributes vertexAttributes;
	TextBuffer buffer;
};