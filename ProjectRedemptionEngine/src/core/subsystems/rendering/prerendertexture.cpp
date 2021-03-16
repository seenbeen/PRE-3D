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
			RenderCompositingTarget& bufferA,
			RenderCompositingTarget& bufferB,
			list<PRELightRenderPassData*>::iterator front
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