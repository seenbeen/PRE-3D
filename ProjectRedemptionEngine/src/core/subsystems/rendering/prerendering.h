#pragma once
#include <string>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		struct PRERenderingConfig;

		class PREApplicationContext;
		class PREModelRendererComponent;

		class PREShader;
		class PRETexture;
		class PREMaterial;
		class PREMesh;

		using std::string;
		using PRE::RenderingModule::Renderer;

		class PRERendering
		{
			PRERendering& operator=(const PRERendering&) = delete;
			PRERendering(const PRERendering&) = delete;

			friend class PREApplicationContext;
			friend class PREModelRendererComponent;

		public:
			PREShader& CreateShader(const string& vertex, const string& fragment);
			void DestroyShader(PREShader& shader);

			PREMaterial& CreateMaterial(PREShader& shader);
			PRETexture& CreateTexture(const unsigned char* data, unsigned int width, unsigned int height);

		private:
			static PRERendering& MakePRERendering(
				const PRERenderingConfig& renderingConfig,
				PREApplicationContext& preApplicationContext
			);

			PREApplicationContext& _preApplicationContext;
			Renderer& _renderer;

			PRERendering(
				PREApplicationContext& preApplicationContext,
				Renderer& renderer
			);
			~PRERendering();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE