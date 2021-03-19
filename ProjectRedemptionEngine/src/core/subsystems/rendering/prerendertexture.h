#pragma once
#include <list>
#include <unordered_map>

#include <glm/glm.hpp>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRECameraComponent;
		class PREAmbientLightComponent;
		class PREPointLightComponent;
		class PRESpotLightComponent;
		class PREDirectionalLightComponent;

		class PRERendering;
		class PREMaterial;

		class PRELightRenderPassData;

		using std::list;
		using std::unordered_map;

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

			const int _layer;
			const glm::vec2 _size;
			RenderCompositingTarget* const _pBufferA;
			RenderCompositingTarget* const _pBufferB;

			// accumulator: buffer to be written to
			// !accumulator: buffer to be read from
			bool _accumulatorBufferIsA;

			// tells you which light pass we're on,
			// reset at the beginning of each frame
			int _passCounter;

			// "shortcut" to avoid making a map per light type
			unordered_map<void*, list<PRELightRenderPassData*>::iterator> _lightMap;

			PRERenderTexture(
				int layer,
				const glm::vec2& size,
				RenderCompositingTarget& bufferA,
				RenderCompositingTarget& bufferB
			);
			~PRERenderTexture();
		};
	} // namespace Core
} // namespace PRE