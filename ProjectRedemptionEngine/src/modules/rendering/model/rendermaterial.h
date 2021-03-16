#pragma once
#include <unordered_map>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderCompositingTarget;
		class RenderShaderProgram;
		class RenderTexture;
		class RenderModel;

		using std::unordered_map;
		using std::vector;

		class RenderMaterial
		{
			RenderMaterial& operator=(const RenderMaterial&) = delete;
			RenderMaterial(const RenderMaterial&) = delete;

			friend class Renderer;
			friend class RenderModel;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderMaterial;

			private:
				RenderShaderProgram* pShaderProgram;
				unordered_map<unsigned int, RenderTexture*> textureBindings;

				static Impl& MakeImpl();

				Impl();
				~Impl();
			};

		public:
			void SetShaderProgram(RenderShaderProgram* pShaderProgram);
			void SetTextureBinding(RenderTexture* pTexture, GLenum bindUnit);
			void SetTextureBinding(RenderCompositingTarget* pCompositingTarget, GLenum bindUnit);

		private:
			Impl& _impl;

			RenderMaterial();
			~RenderMaterial();

			void Bind(
				const glm::mat4& modelMatrix,
				const glm::mat4& viewMatrix,
				const glm::mat4& projectionMatrix
			);

			void Bind(
				const glm::mat4& modelViewMatrix,
				const glm::mat4& viewMatrix,
				const glm::mat4& projectionMatrix,
				const vector<glm::mat4> boneTransforms
			);
		};
	} // namespace RenderingModule
} // namespace PRE