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
		class PRETexture;
		class PREMaterial;

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

			PRETexture& CreateTexture();
			void DestroyTexture(PRETexture& texture);

			PREMaterial& CreateMaterial();
			void DestroyMaterial(PREMaterial& material);

			PREMesh& CreateMesh();
			void DestroyMesh(PREMesh& mesh);

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