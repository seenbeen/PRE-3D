#pragma once
#include <unordered_map>

#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		class Camera;

		class RenderShaderProgram;
		class RenderTexture;

		class RenderModel;
		class Renderer;
		
		using std::unordered_map;

		class RenderMaterial
		{
			RenderMaterial& operator=(const RenderMaterial&) = delete;
			RenderMaterial(const RenderMaterial&) = delete;

			friend class RenderModel;
			friend class Renderer;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderMaterial;

			private:
				RenderShaderProgram* shaderProgram;
				unordered_map<unsigned int, RenderTexture*> textureBindings;

				static Impl& MakeImpl(RenderShaderProgram& shaderProgram);

				Impl(RenderShaderProgram& shaderProgram);
				~Impl();
			};

		private:
			Impl& _impl;

			RenderMaterial(RenderShaderProgram& shaderProgram);
			~RenderMaterial();

			void SetShaderProgram(RenderShaderProgram& shaderProgram);
			void SetTextureBinding(RenderTexture* renderTexture, GLenum bindUnit);

			void Bind(const Camera& camera);
		};
	} // namespace RenderingModule
} // namespace PRE