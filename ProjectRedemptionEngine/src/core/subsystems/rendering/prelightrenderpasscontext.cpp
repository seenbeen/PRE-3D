#include <core/subsystems/rendering/prelightrenderpasscontext.h>

#include <core/subsystems/rendering/prerendertexture.h>
#include <core/components/prepointlightcomponent.h>

namespace PRE
{
	namespace Core
	{
		PRELightRenderPassContext::PRELightRenderPassContext(
			bool isFirstPass,
			PRERenderTexture& renderTexture,
			PREPointLightComponent& pointLightComponent
		)
			:
			_isFirstPass(isFirstPass),
			_renderTexture(renderTexture),
			_pPointLightComponent(&pointLightComponent) {}

		PRELightRenderPassContext::~PRELightRenderPassContext() {}
	} // namespace Core
} // namespace PRE