#include <core/subsystems/rendering/prelightrenderpasscontext.h>

#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/rendering/prerendertexture.h>
#include <core/components/prepointlightcomponent.h>

namespace PRE
{
	namespace Core
	{
		PRELightRenderPassContext::PRELightRenderPassContext(
			PRERendering& rendering,
			PRERenderTexture& renderTexture,
			PREPointLightComponent& pointLightComponent
		)
			:
			_rendering(rendering),
			_renderTexture(renderTexture),
			_pPointLightComponent(&pointLightComponent) {}

		PRELightRenderPassContext::~PRELightRenderPassContext() {}
	} // namespace Core
} // namespace PRE