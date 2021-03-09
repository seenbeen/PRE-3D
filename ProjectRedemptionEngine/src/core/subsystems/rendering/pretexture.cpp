#include <core/subsystems/rendering/pretexture.h>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		PRETexture::PRETexture(RenderTexture& texture)
			: _texture(texture) {}

		PRETexture::~PRETexture() {}
	} // namespace Core
} // namespace PRE