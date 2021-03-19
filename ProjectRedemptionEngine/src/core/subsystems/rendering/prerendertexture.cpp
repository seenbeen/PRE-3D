#include <core/subsystems/rendering/prerendertexture.h>

#include <list>
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		using std::list;

		using PRE::RenderingModule::RenderCompositingTarget;
		using PRE::RenderingModule::RenderCompositingNode;

		PRERenderTexture::PRERenderTexture(
			int layer,
			const glm::vec2& size,
			RenderCompositingTarget& bufferA,
			RenderCompositingTarget& bufferB
		)
			:
			_layer(layer),
			_size(size),
			_pAssociatedCameraComponent(nullptr),
			_pBufferA(&bufferA),
			_pBufferB(&bufferB),
			_accumulatorBufferIsA(false),
			_passCounter(0) {}

		PRERenderTexture::~PRERenderTexture() {}
	} // namespace Core
} // namespace PRE