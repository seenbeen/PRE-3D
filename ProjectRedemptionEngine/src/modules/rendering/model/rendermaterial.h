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
		class RenderCompositingNode;
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
			void SetTextureBinding(RenderTexture* pRenderTexture, GLenum bindUnit);
			void SetTextureBinding(
				RenderCompositingNode& compositingNode,
				GLenum bindUnit
			);

		private:
			Impl& _impl;

			RenderMaterial();
			~RenderMaterial();

			void Bind(
				const glm::mat4& modelViewMatrix,
				const glm::mat4& projectionMatrix
			);

			void Bind(
				const glm::mat4& modelViewMatrix,
				const glm::mat4& projectionMatrix,
				const vector<glm::mat4> boneTransforms
			);
		};
	} // namespace RenderingModule
} // namespace PRE