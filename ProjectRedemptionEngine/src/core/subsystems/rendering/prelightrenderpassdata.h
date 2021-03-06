#pragma once
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PRELightRenderPassContext;

		using PRE::RenderingModule::RenderCompositingNode;

		class PRELightRenderPassData
		{
			PRELightRenderPassData& operator=(const PRELightRenderPassData&) = delete;
			PRELightRenderPassData(const PRELightRenderPassData&) = delete;

			friend class PRERendering;

		private:
			RenderCompositingNode* const pNode;
			PRELightRenderPassContext* const pRenderPassContext;

			// used for the screen-display compositing node
			PRELightRenderPassData(RenderCompositingNode& node);

			PRELightRenderPassData(
				RenderCompositingNode& node,
				PRELightRenderPassContext& renderPassContext
			);
			~PRELightRenderPassData();
		};
	} // namespace Core
} // namespace PRE