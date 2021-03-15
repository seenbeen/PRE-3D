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

			const list<PRERenderTexture*>::iterator _it;
			RenderCompositingTarget* const _pBufferA;
			RenderCompositingTarget* const _pBufferB;

			bool _accumulatorBufferIsA; // if false, b is accumulator buffer

			list<PRELightRenderPassData*> _lightPasses;

			PRERenderTexture(
				list<PRERenderTexture*>::iterator it,
				RenderCompositingTarget& bufferA,
				RenderCompositingTarget& bufferB
			);
			~PRERenderTexture();
		};
	} // namespace Core
} // namespace PRE