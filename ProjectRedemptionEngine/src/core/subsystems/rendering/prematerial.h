#pragma once
#include <unordered_map>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PRERenderTexture;
		class PREShader;
		class PRETexture;

		using std::unordered_map;
		using PRE::RenderingModule::RenderMaterial;

		class PREMaterial
		{
			PREMaterial& operator=(const PREMaterial&) = delete;
			PREMaterial(const PREMaterial&) = delete;

			friend class PRERendering;

		public:
			void SetShader(PREShader* pShader);
			void SetTextureBinding(PRETexture* pTexture, unsigned int bindUnit);
			void SetTextureBinding(
				PRERenderTexture& renderTexture,
				unsigned int bindUnit
			);

		private:
			RenderMaterial& _material;
			PREShader* _pShader;

			unordered_map<unsigned int, PRERenderTexture*> _renderTextureBindings;

			PREMaterial(RenderMaterial& material);
			~PREMaterial();

			void BindRenderTextureAccumulatorBindings();
		};
	} // namespace Core
} // namespace PRE