#pragma once
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PREMaterial;

		using PRE::RenderingModule::RenderTexture;

		class PRETexture
		{
			PRETexture& operator=(const PRETexture&) = delete;
			PRETexture(const PRETexture&) = delete;

			friend class PRERendering;
			friend class PREMaterial;

		private:
			RenderTexture& _texture;

			PRETexture(RenderTexture& texture);
			~PRETexture();
		};
	} // namespace Core
} // namespace PRE