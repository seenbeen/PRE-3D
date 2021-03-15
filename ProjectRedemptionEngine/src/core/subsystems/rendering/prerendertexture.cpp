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
			list<PRELightRenderPassData*>::iterator front,
			RenderCompositingTarget& bufferA,
			RenderCompositingTarget& bufferB
		)
			:
			_pAssociatedCameraComponent(nullptr),
			_pBufferA(&bufferA),
			_pBufferB(&bufferB),
			_accumulatorBufferIsA(false),
			_front(front) {}

		PRERenderTexture::~PRERenderTexture() {}
	} // namespace Core
} // namespace PRE