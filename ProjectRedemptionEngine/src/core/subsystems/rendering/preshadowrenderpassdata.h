#pragma once
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PRELightRenderPassData;
		class PREShadowRenderPassContext;

		using PRE::RenderingModule::RenderCompositingNode;

		class PREShadowRenderPassData
		{
			PREShadowRenderPassData& operator=(const PREShadowRenderPassData&) = delete;
			PREShadowRenderPassData(const PREShadowRenderPassData&) = delete;

			friend class PRERendering;

		private:
			RenderCompositingNode* const pNode;
			PREShadowRenderPassContext* const pRenderPassContext;

			PRELightRenderPassData* pLastLightData;

			PREShadowRenderPassData(
				RenderCompositingNode& node,
				PREShadowRenderPassContext& renderPassContext
			);
			~PREShadowRenderPassData();
		};
	} // namespace Core
} // namespace PRE