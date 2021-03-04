#include <core/subsystems/rendering/pretexture.h>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		void PRETexture::SetData(
			unsigned int width,
			unsigned int height,
			const unsigned char* const data
		)
		{
			_texture.SetData(width, height, data);
		}

		PRETexture::PRETexture(RenderTexture& texture)
			: _texture(texture) {}

		PRETexture::~PRETexture() {}
	} // namespace Core
} // namespace PRE