#pragma once
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;

		using PRE::RenderingModule::RenderShaderProgram;
		using PRE::RenderingModule::RenderMesh;
		using PRE::RenderingModule::RenderTexture;
		using PRE::RenderingModule::RenderMaterial;
		using PRE::RenderingModule::RenderModel;

		class PRESkyBox
		{
			PRESkyBox& operator=(const PRESkyBox&) = delete;
			PRESkyBox(const PRESkyBox&) = delete;

			friend class PRERendering;

		private:
			RenderShaderProgram& _shaderProgram;
			RenderMesh& _mesh;
			RenderTexture& _texture;
			RenderMaterial& _material;
			RenderModel& _model;

			PRESkyBox(
				RenderShaderProgram& shaderProgram,
				RenderMesh& mesh,
				RenderTexture& texture,
				RenderMaterial& material,
				RenderModel& model
			);
			~PRESkyBox();
		};
	} // namespace Core
} // namespace PRE