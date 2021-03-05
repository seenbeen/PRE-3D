#include <core/subsystems/rendering/prerendertexture.h>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::RenderCompositingNode;

		PRERenderTexture::PRERenderTexture(
			unsigned int tagGroup,
			RenderCompositingNode& compositingNode
		)
			:
			_tagGroup(tagGroup),
			_compositingNode(compositingNode) {}

		PRERenderTexture::~PRERenderTexture() {}
	} // namespace Core
} // namespace PRE