#pragma once
#include <string>

#include <glad/glad.h>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		struct PRERenderingConfig;

		class PREApplicationContext;
		class PREModelRendererComponent;
		class PRECameraComponent;

		class PRERenderTexture;
		class PREShader;
		class PREMesh;
		class PRESkeleton;
		class PREBoneConfig;
		class PRETexture;
		class PREMaterial;

		class PREAnimation;
		class PREAnimationConfig;
		class PREAnimator;
		class PREAnimatorConfig;

		using std::string;
		using PRE::RenderingModule::Renderer;
		using PRE::RenderingModule::RenderCamera;
		using PRE::RenderingModule::RenderModel;

		class PRERendering
		{
			PRERendering& operator=(const PRERendering&) = delete;
			PRERendering(const PRERendering&) = delete;

			friend class PREApplicationContext;
			friend class PREModelRendererComponent;
			friend class PRECameraComponent;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class PRERendering;

			private:
				static Impl& MakeImpl(PREApplicationContext& applicationContext, Renderer& renderer);

				unsigned int tagGroupCounter;
				PREApplicationContext& applicationContext;
				Renderer& renderer;

				Impl(PREApplicationContext& applicationContext, Renderer& renderer);
				~Impl();
			};

		public:
			PRERenderTexture& rootRenderTexture;

			PRERenderTexture& CreateRenderTexture(unsigned int width, unsigned int height);
			void DestroyRenderTexture(PRERenderTexture& renderTexture);

			PREShader& CreateShader(const string& vertex, const string& fragment);
			void DestroyShader(PREShader& shader);

			PRETexture& CreateTexture(
				unsigned int width,
				unsigned int height,
				const unsigned char* data
			);
			void DestroyTexture(PRETexture& texture);

			PREMaterial& CreateMaterial();
			void DestroyMaterial(PREMaterial& material);

			PREMesh& CreateMesh(
				unsigned int nVertices,
				const glm::vec3* vertices,
				const glm::vec3* normals,
				const glm::vec3* tangents,
				const glm::vec3* biTangents,
				const glm::vec2* uvs,
				const glm::ivec4* vertexBoneIds,
				const glm::vec4* vertexBoneWeights,
				unsigned int nTriangleElements,
				const unsigned int* const triangleElements
			);
			void DestroyMesh(PREMesh& mesh);

			PRESkeleton& CreateSkeleton(
				const PREBoneConfig& rootBoneConfig
			);
			void DestroySkeleton(PRESkeleton& skeleton);

			PREAnimation& CreateAnimation(
				const PREAnimationConfig& animationConfig
			);
			void DestroyAnimation(PREAnimation& animation);

			PREAnimator& CreateAnimator(
				const PREAnimatorConfig& animatorConfig
			);
			void DestroyAnimator(PREAnimator& animator);

		private:
			static PRERendering& MakePRERendering(
				const PRERenderingConfig& renderingConfig,
				PREApplicationContext& applicationContext
			);

			Impl& _impl;

			PRERendering(
				PREApplicationContext& applicationContext,
				Renderer& renderer
			);
			~PRERendering();

			void Initialize();
			void Update();
			void Shutdown();

			void AllocateCamera(PRECameraComponent& cameraComponent);
			void UpdateCamera(PRECameraComponent& cameraComponent);
			void DeallocateCamera(PRECameraComponent& cameraComponent);

			void AllocateModel(PREModelRendererComponent& modelRendererComponent);
			void UpdateModel(PREModelRendererComponent& modelRendererComponent);
			void DeallocateModel(PREModelRendererComponent& modelRendererComponent);
		};
	} // namespace Core
} // namespace PRE