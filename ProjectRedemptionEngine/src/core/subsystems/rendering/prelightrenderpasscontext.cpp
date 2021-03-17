#include <core/subsystems/rendering/prelightrenderpasscontext.h>

#include <core/subsystems/rendering/prerendertexture.h>
#include <core/components/preambientlightcomponent.h>
#include <core/components/prepointlightcomponent.h>

namespace PRE
{
	namespace Core
	{
		PRELightRenderPassContext::PRELightRenderPassContext(
			bool isFirstPass,
			PRERenderTexture& renderTexture,
			PREAmbientLightComponent& ambientLightComponent
		)
			:
			_isFirstPass(isFirstPass),
			_renderTexture(renderTexture),
			_pAmbientLightComponent(&ambientLightComponent),
			_pPointLightComponent(nullptr),
			_pSpotLightComponent(nullptr) {}

		PRELightRenderPassContext::PRELightRenderPassContext(
			bool isFirstPass,
			PRERenderTexture& renderTexture,
			PREPointLightComponent& pointLightComponent
		)
			:
			_isFirstPass(isFirstPass),
			_renderTexture(renderTexture),
			_pAmbientLightComponent(nullptr),
			_pPointLightComponent(&pointLightComponent),
			_pSpotLightComponent(nullptr) {}

		PRELightRenderPassContext::PRELightRenderPassContext(
			bool isFirstPass,
			PRERenderTexture& renderTexture,
			PRESpotLightComponent& spotLightComponent
		)
			:
			_isFirstPass(isFirstPass),
			_renderTexture(renderTexture),
			_pAmbientLightComponent(nullptr),
			_pPointLightComponent(nullptr),
			_pSpotLightComponent(&spotLightComponent) {}

		PRELightRenderPassContext::~PRELightRenderPassContext() {}
	} // namespace Core
} // namespace PRE