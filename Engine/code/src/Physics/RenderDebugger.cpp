#include "Physics/RenderDebugger.h"

#include "Core/Engine.h"

#include "Resources/Shader.h"
#include "Resources/ResourceManager.h"

#include "LowRenderer/Camera.h"

void RenderDebugger::DrawTriangle(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2, const Vector4D& color)
{
	DebugVertex vertex0;
	vertex0.position = v0;
	vertex0.color = color;

	DebugVertex vertex1;
	vertex1.position = v1;
	vertex1.color = color;

	DebugVertex vertex2;
	vertex2.position = v2;
	vertex2.color = color;

	mVertices.push_back(vertex0);
	mVertices.push_back(vertex1);
	mVertices.push_back(vertex2);
}

void RenderDebugger::DrawLine(const Vector3D& start, const Vector3D& end, const Vector4D& color)
{
	DebugVertex vertex0;
	vertex0.position = start;
	vertex0.color = color;

	DebugVertex vertex1;
	vertex1.position = end;
	vertex1.color = color;

	mVertices.push_back(vertex0); 
	mVertices.push_back(vertex1); 
}

void RenderDebugger::DrawSphere(const Vector3D& center, float radius, const Vector3D& scale, const Vector4D& color)
{
    const int segments = 32;

    for (int i = 0; i < segments; ++i)
    {
        float theta1 = (float)i / segments * 2.0f * PI;
        float theta2 = (float)(i + 1) / segments * 2.0f * PI;

        Vector3D p1 = center + Vector3D(cos(theta1) * radius * scale.x, sin(theta1) * radius * scale.y, 0.0f);
        Vector3D p2 = center + Vector3D(cos(theta2) * radius * scale.x, sin(theta2) * radius * scale.y, 0.0f);

        DrawLine(p1, p2, color);
    }

    for (int i = 0; i < segments; ++i)
    {
        float theta1 = (float)i / segments * 2.0f * PI;
        float theta2 = (float)(i + 1) / segments * 2.0f * PI;

        Vector3D p1 = center + Vector3D(0.0f, cos(theta1) * radius * scale.y, sin(theta1) * radius * scale.z);
        Vector3D p2 = center + Vector3D(0.0f, cos(theta2) * radius * scale.y, sin(theta2) * radius * scale.z);

        DrawLine(p1, p2, color);
    }

    for (int i = 0; i < segments; ++i)
    {
        float theta1 = (float)i / segments * 2.0f * PI;
        float theta2 = (float)(i + 1) / segments * 2.0f * PI;

        Vector3D p1 = center + Vector3D(cos(theta1) * radius * scale.x, 0.0f, sin(theta1) * radius * scale.z);
        Vector3D p2 = center + Vector3D(cos(theta2) * radius * scale.x, 0.0f, sin(theta2) * radius * scale.z);

        DrawLine(p1, p2, color);
    }

    LoadBuffer();
}


void RenderDebugger::DrawCube(const Vector3D& center, const Vector3D& halfSize, const Vector3D& scale, const Vector4D& color)
{
    Vector3D scaledHalfSize = Vector3D(halfSize.x * scale.x, halfSize.y * scale.y, halfSize.z * scale.z);

    Vector3D p0 = center + Vector3D(-scaledHalfSize.x, -scaledHalfSize.y, -scaledHalfSize.z);
    Vector3D p1 = center + Vector3D(scaledHalfSize.x, -scaledHalfSize.y, -scaledHalfSize.z);
    Vector3D p2 = center + Vector3D(scaledHalfSize.x, scaledHalfSize.y, -scaledHalfSize.z);
    Vector3D p3 = center + Vector3D(-scaledHalfSize.x, scaledHalfSize.y, -scaledHalfSize.z);
    Vector3D p4 = center + Vector3D(-scaledHalfSize.x, -scaledHalfSize.y, scaledHalfSize.z);
    Vector3D p5 = center + Vector3D(scaledHalfSize.x, -scaledHalfSize.y, scaledHalfSize.z);
    Vector3D p6 = center + Vector3D(scaledHalfSize.x, scaledHalfSize.y, scaledHalfSize.z);
    Vector3D p7 = center + Vector3D(-scaledHalfSize.x, scaledHalfSize.y, scaledHalfSize.z);

    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p3, color);
    DrawLine(p3, p0, color);

    DrawLine(p4, p5, color);
    DrawLine(p5, p6, color);
    DrawLine(p6, p7, color);
    DrawLine(p7, p4, color);

    DrawLine(p0, p4, color);
    DrawLine(p1, p5, color);
    DrawLine(p2, p6, color);
    DrawLine(p3, p7, color);

    LoadBuffer();
}

void RenderDebugger::DrawCapsule(const Vector3D& center, float radius, float height, const Vector3D& scale, const Vector4D& color)
{
    const int segments = 16;

    float scaledRadius = radius * std::max(scale.x, scale.z); 
    float scaledHalfHeight = (height * 0.5f) * scale.y; 

    for (int i = 0; i < segments; ++i)
    {
        float theta1 = (float)i / segments * 2.0f * PI;
        float theta2 = (float)(i + 1) / segments * 2.0f * PI;

        Vector3D p1 = center + Vector3D(cos(theta1) * scaledRadius, -scaledHalfHeight, sin(theta1) * scaledRadius);
        Vector3D p2 = center + Vector3D(cos(theta2) * scaledRadius, -scaledHalfHeight, sin(theta2) * scaledRadius);
        Vector3D p3 = center + Vector3D(cos(theta1) * scaledRadius, scaledHalfHeight, sin(theta1) * scaledRadius);
        Vector3D p4 = center + Vector3D(cos(theta2) * scaledRadius, scaledHalfHeight, sin(theta2) * scaledRadius);

        DrawLine(p1, p2, color);
        DrawLine(p3, p4, color);
        DrawLine(p1, p3, color);
    }

    for (int i = 0; i < segments; ++i)
    {
        float theta1 = (float)i / segments * PI / 2.0f;
        float theta2 = (float)(i + 1) / segments * PI / 2.0f;

        for (int j = 0; j < segments; ++j)
        {
            float phi = (float)j / segments * 2.0f * PI;

            Vector3D p1 = center + Vector3D(
                scaledRadius * cos(phi) * sin(theta1),
                scaledHalfHeight + scaledRadius * cos(theta1),
                scaledRadius * sin(phi) * sin(theta1)
            );
            Vector3D p2 = center + Vector3D(
                scaledRadius * cos(phi) * sin(theta2),
                scaledHalfHeight + scaledRadius * cos(theta2),
                scaledRadius * sin(phi) * sin(theta2)
            );

            DrawLine(p1, p2, color);
        }
    }

    for (int i = 0; i < segments; ++i)
    {
        float theta1 = (float)i / segments * PI / 2.0f;
        float theta2 = (float)(i + 1) / segments * PI / 2.0f;

        for (int j = 0; j < segments; ++j)
        {
            float phi = (float)j / segments * 2.0f * PI;

            Vector3D p1 = center + Vector3D(
                scaledRadius * cos(phi) * sin(theta1),
                -scaledHalfHeight - scaledRadius * cos(theta1),
                scaledRadius * sin(phi) * sin(theta1)
            );
            Vector3D p2 = center + Vector3D(
                scaledRadius * cos(phi) * sin(theta2),
                -scaledHalfHeight - scaledRadius * cos(theta2),
                scaledRadius * sin(phi) * sin(theta2)
            );

            DrawLine(p1, p2, color);
        }
    }

    LoadBuffer();
}

void RenderDebugger::DrawLines() const
{
    if (!mVertices.empty())
    {
		GLuint shaderProgram = Engine::GetInstance()->GetResourceManager()->Get<IShader>("debug_shader.json")->GetProgram();
        glUseProgram(shaderProgram);

		int location = glGetUniformLocation(shaderProgram, "viewProjectionMatrix");
		glUniformMatrix4fv(location, 1, GL_TRUE, camera->GetViewProjectionMatrix().data);

        glBindVertexArray(vertexAttributes.VAO);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_LINES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glBindVertexArray(0);
    }
}


void DebugBuffer::Init(std::vector<DebugVertex>& vertices, std::vector<uint32_t>& index)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(DebugVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * index.size(), index.data(), GL_STATIC_DRAW);
}

void DebugBuffer::UnbindBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DebugVertexAttributes::Bind()
{
    glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

void DebugVertexAttributes::Init()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)0);                 
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color)); 
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void RenderDebugger::Clear()
{
	mVertices.clear();
	mIndices.clear();
}

void RenderDebugger::LoadBuffer()
{
	mIndices = GenerateIndices(mVertices);

	vertexAttributes.Bind();
	buffer.Init(mVertices, mIndices);

	vertexAttributes.Init();
	buffer.UnbindBuffer();
}

std::vector<uint32_t> RenderDebugger::GenerateIndices(const std::vector<DebugVertex>& vertices)
{
	std::vector<uint32_t> indices;
	indices.reserve(vertices.size());

	for (uint32_t i = 0; i < static_cast<uint32_t>(vertices.size()); i++)
	{
		indices.push_back(i);
	}

	return indices;
}
