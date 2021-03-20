#include <core/subsystems/rendering/preshadowrenderpassdata.h>

#include <include/modules/rendering.h>

#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/rendering/prelightrenderpassdata.h>
#include <core/subsystems/rendering/preshadowrenderpasscontext.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::RenderCompositingNode;

		PREShadowRenderPassData::PREShadowRenderPassData(
			RenderCompositingNode& node,
			PREShadowRenderPassContext& renderPassContext
		)
			:
			pNode(&node),
			pRenderPassContext(&renderPassContext),
			pLastLightData(nullptr) {}

		PREShadowRenderPassData::~PREShadowRenderPassData() {}
	} // namespace Core
} // namespace PRE