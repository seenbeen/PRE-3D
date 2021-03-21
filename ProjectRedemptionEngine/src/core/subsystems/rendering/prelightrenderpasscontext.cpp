#include <core/subsystems/rendering/prelightrenderpasscontext.h>

#include <unordered_map>
#include <unordered_set>

#include <core/subsystems/rendering/prerendertexture.h>

#include <core/components/preambientlightcomponent.h>
#include <core/components/prepointlightcomponent.h>
#include <core/components/prespotlightcomponent.h>
#include <core/components/predirectionallightcomponent.h>
#include <core/components/premodelrenderercomponent.h>

namespace PRE
{
	namespace Core
	{
		using std::unordered_map;
		using std::unordered_set;

		PRELightRenderPassContext::PRELightRenderPassContext(
			unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
			PRERenderTexture& renderTexture,
			PREAmbientLightComponent& ambientLightComponent
		)
			:
			_modelTagMap(modelTagMap),
			_renderTexture(renderTexture),
			_pShadowMap(nullptr),
			_pAmbientLightComponent(&ambientLightComponent),
			_pPointLightComponent(nullptr),
			_pSpotLightComponent(nullptr),
			_pDirectionalLightComponent(nullptr) {}

		PRELightRenderPassContext::PRELightRenderPassContext(
			unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
			PRERenderTexture& renderTexture,
			RenderCompositingTarget& shadowMap,
			PREPointLightComponent& pointLightComponent
		)
			:
			_modelTagMap(modelTagMap),
			_renderTexture(renderTexture),
			_pShadowMap(&shadowMap),
			_pAmbientLightComponent(nullptr),
			_pPointLightComponent(&pointLightComponent),
			_pSpotLightComponent(nullptr),
			_pDirectionalLightComponent(nullptr) {}

		PRELightRenderPassContext::PRELightRenderPassContext(
			unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
			PRERenderTexture& renderTexture,
			RenderCompositingTarget& shadowMap,
			PRESpotLightComponent& spotLightComponent
		)
			:
			_modelTagMap(modelTagMap),
			_renderTexture(renderTexture),
			_pShadowMap(&shadowMap),
			_pAmbientLightComponent(nullptr),
			_pPointLightComponent(nullptr),
			_pSpotLightComponent(&spotLightComponent),
			_pDirectionalLightComponent(nullptr) {}

		PRELightRenderPassContext::PRELightRenderPassContext(
			unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
			PRERenderTexture& renderTexture,
			RenderCompositingTarget& shadowMap,
			PREDirectionalLightComponent& directionalLightComponent
		)
			:
			_modelTagMap(modelTagMap),
			_renderTexture(renderTexture),
			_pShadowMap(&shadowMap),
			_pAmbientLightComponent(nullptr),
			_pPointLightComponent(nullptr),
			_pSpotLightComponent(nullptr),
			_pDirectionalLightComponent(&directionalLightComponent) {}

		PRELightRenderPassContext::~PRELightRenderPassContext() {}
	} // namespace Core
} // namespace PRE