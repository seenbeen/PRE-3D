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
			list<PRERenderTexture*>::iterator it,
			RenderCompositingTarget& bufferA,
			RenderCompositingTarget& bufferB
		)
			:
			_it(it),
			_pAssociatedCameraComponent(nullptr),
			_pBufferA(&bufferA),
			_pBufferB(&bufferB),
			_accumulatorBufferIsA(false) {}

		PRERenderTexture::~PRERenderTexture() {}
	} // namespace Core
} // namespace PRE