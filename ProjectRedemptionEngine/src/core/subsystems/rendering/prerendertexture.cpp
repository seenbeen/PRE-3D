#include <core/subsystems/rendering/prerendertexture.h>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::RenderCompositingNode;

		PRERenderTexture::PRERenderTexture(RenderCompositingNode& compositingNode)
			:
			_compositingNode(compositingNode) {}

		PRERenderTexture::~PRERenderTexture() {}
	} // namespace Core
} // namespace PRE