#pragma once
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PRERenderTexture;
		class PREShader;
		class PRETexture;

		using PRE::RenderingModule::RenderMaterial;

		class PREMaterial
		{
			PREMaterial& operator=(const PREMaterial&) = delete;
			PREMaterial(const PREMaterial&) = delete;

			friend class PRERendering;

		public:
			enum class CompositingAttachment { POSITIONS, NORMALS, ALBEDO_SPECULAR };

			void SetShaderProgram(PREShader* pShader);
			void SetTextureBinding(PRETexture* pTexture, unsigned int bindUnit);
			void SetTextureBinding(
				PRERenderTexture& renderTexture,
				const CompositingAttachment& attachment,
				unsigned int bindUnit
			);

		private:
			RenderMaterial& _material;

			PREMaterial(RenderMaterial& material);
			~PREMaterial();
		};
	} // namespace Core
} // namespace PRE