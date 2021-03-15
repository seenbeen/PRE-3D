#include <core/subsystems/rendering/prelightrenderpassdata.h>

#include <include/modules/rendering.h>

#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/rendering/prelightrenderpasscontext.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::RenderCompositingNode;

		PRELightRenderPassData::PRELightRenderPassData(
			RenderCompositingNode& node,
			PRELightRenderPassContext& renderPassContext
		)
			:
			pNode(&node),
			pRenderPassContext(&renderPassContext) {}

		PRELightRenderPassData::~PRELightRenderPassData() {}
	} // namespace Core
} // namespace PRE