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

		public:
			void SetData(
				unsigned int width,
				unsigned int height,
				const unsigned char* const data
			);

		private:
			RenderTexture& _texture;

			PRETexture(RenderTexture& texture);
			~PRETexture();
		};
	} // namespace Core
} // namespace PRE