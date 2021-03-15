#pragma once
#include <list>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRECameraComponent;

		class PRERendering;
		class PREMaterial;

		class PRELightComponent;
		class PRELightRenderPassData;

		using std::list;

		using PRE::RenderingModule::RenderCompositingTarget;
		using PRE::RenderingModule::RenderCompositingNode;

		class PRERenderTexture
		{
			PRERenderTexture& operator=(const PRERenderTexture&) = delete;
			PRERenderTexture(const PRERenderTexture&) = delete;
			
			friend class PRERendering;
			friend class PREMaterial;

		private:
			PRECameraComponent* _pAssociatedCameraComponent;

			RenderCompositingTarget* const _pBufferA;
			RenderCompositingTarget* const _pBufferB;

			// accumulator: buffer to be written to
			// !accumulator: buffer to be read from
			bool _accumulatorBufferIsA;

			list<PRELightRenderPassData*>::iterator _front;

			PRERenderTexture(
				list<PRELightRenderPassData*>::iterator front,
				RenderCompositingTarget& bufferA,
				RenderCompositingTarget& bufferB
			);
			~PRERenderTexture();
		};
	} // namespace Core
} // namespace PRE