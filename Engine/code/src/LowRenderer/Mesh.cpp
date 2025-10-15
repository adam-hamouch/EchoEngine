#include "LowRenderer/Mesh.h"
#include "LowRenderer/Camera.h"

#include "Core/UI/UI.h"

#include "Resources/Shader.h"
#include "Resources/Model.h"
#include "Resources/Material.h"
#include "Resources/ResourceManager.h"

void MeshGL::SetModelMatrix(Mat4 matrix)
{
	modelMatrix = matrix;
}

Mat4 MeshGL::GetModelMatrix()
{
	return modelMatrix;
}

void MeshGL::DisplayComponentInInspector(InspectorUI* inspector)
{
	inspector->MeshRenderer();
	inspector->MeshFilter();
	inspector->MaterialComponent();
	inspector->RenderModeComponent(); 
}

void MeshGL::Draw(Camera* camera, std::map<std::string, IShader*> shaders)
{
	if (!model)
		return;

	if (HasFlag(renderMode, RenderMode::Outline))
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDisable(GL_DEPTH_TEST); 

		ShaderGL* outlineShader = shaders["Outline"]->GetGL();
		glUseProgram(outlineShader->GetProgram());

		outlineShader->SetUniformMatrix4x4("vp", camera->GetViewProjectionMatrix());
		outlineShader->SetUniformMatrix4x4("model", modelMatrix);
		outlineShader->SetUniformFloat("outlineThickness", 0.05f);

		glBindVertexArray(model->GetGL()->vertexAttributes.VAO);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model->GetGL()->index.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShaderGL* shaderGL = shaders["Standard"]->GetGL();
	glUseProgram(shaderGL->GetProgram());

	Mat4 mvpMatrix = camera->GetViewProjectionMatrix() * modelMatrix;
	shaderGL->SetUniformMatrix4x4("mvp", mvpMatrix);
	shaderGL->SetUniformMatrix4x4("model", modelMatrix);
	shaderGL->SetUniformVector3D("viewPos", camera->eye);

	if (HasFlag(renderMode, RenderMode::Toon))
		shaderGL->SetUniformBool("useToonShading", true);
	else
		shaderGL->SetUniformBool("useToonShading", false);

	if (material)
	{
		int slot = 0;

		auto SetTexture = [&](const std::string& name, ITexture* tex) {
			bool hasMap = (tex != nullptr);
			shaderGL->SetUniformBool("has" + name, hasMap);
			if (hasMap)
				shaderGL->SetUniformTexture(name, tex->GetTextureID(), slot++);
			};

		SetTexture("AlbedoMap", material->GetAlbedoMap());
		SetTexture("MetallicMap", material->GetMetallicMap());
		SetTexture("RoughnessMap", material->GetRoughnessMap());
		SetTexture("AOMap", material->GetAOMap());
		SetTexture("NormalMap", material->GetNormalMap());

		shaderGL->SetUniformVector3D("material.albedo", material->GetAlbedo());
		shaderGL->SetUniformFloat("material.metallic", material->GetMetallic());
		shaderGL->SetUniformFloat("material.roughness", material->GetRoughness());
		shaderGL->SetUniformFloat("material.ao", material->GetAO());
	}

	glBindVertexArray(model->GetGL()->vertexAttributes.VAO);

	if (model->GetGL()->index.size() <= std::numeric_limits<GLsizei>::max()) {
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model->GetGL()->index.size()), GL_UNSIGNED_INT, 0);
	}
	else {
		std::cerr << "Error: index size exceeds GLsizei limit!" << std::endl;
	}

	glBindVertexArray(0);
}


void MeshGL::Create(IModel* _model, IMaterial* _material)
{
	material = _material;
	model = _model;
}

