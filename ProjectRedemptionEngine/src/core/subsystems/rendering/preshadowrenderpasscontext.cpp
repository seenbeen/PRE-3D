#include <core/subsystems/rendering/preshadowrenderpasscontext.h>

#include <unordered_map>
#include <unordered_set>

#include <include/modules/rendering.h>

#include <core/components/premodelrenderercomponent.h>
#include <core/components/prepointlightcomponent.h>
#include <core/components/prespotlightcomponent.h>
#include <core/components/predirectionallightcomponent.h>

#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/rendering/preshader.h>
#include <core/subsystems/rendering/prerendertexture.h>

namespace PRE
{
	namespace Core
	{
		using std::unordered_map;
		using std::unordered_set;

		using PRE::RenderingModule::RenderCompositingTarget;
		using PRE::RenderingModule::RenderCamera;

		PREShadowRenderPassContext::PREShadowRenderPassContext(
			RenderCamera& lightPOVCamera,
			unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
			RenderCompositingTarget& target,
			PREShader& shadowShader,
			PREPointLightComponent& pointLightComponent
		)
			:
			_lightPOVCamera(lightPOVCamera),
			_modelTagMap(modelTagMap),
			_target(target),
			_shadowShader(shadowShader),
			_pPointLightComponent(&pointLightComponent),
			_pSpotLightComponent(nullptr),
			_pDirectionalLightComponent(nullptr) {}

		PREShadowRenderPassContext::PREShadowRenderPassContext(
			RenderCamera& lightPOVCamera,
			unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
			RenderCompositingTarget& target,
			PREShader& shadowShader,
			PRESpotLightComponent& spotLightComponent
		)
			:
			_lightPOVCamera(lightPOVCamera),
			_modelTagMap(modelTagMap),
			_target(target),
			_shadowShader(shadowShader),
			_pPointLightComponent(nullptr),
			_pSpotLightComponent(&spotLightComponent),
			_pDirectionalLightComponent(nullptr) {}

		PREShadowRenderPassContext::PREShadowRenderPassContext(
			RenderCamera& lightPOVCamera,
			unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
			RenderCompositingTarget& target,
			PREShader& shadowShader,
			PREDirectionalLightComponent& directionalLightComponent
		)
			:
			_lightPOVCamera(lightPOVCamera),
			_modelTagMap(modelTagMap),
			_target(target),
			_shadowShader(shadowShader),
			_pPointLightComponent(nullptr),
			_pSpotLightComponent(nullptr),
			_pDirectionalLightComponent(&directionalLightComponent) {}

		PREShadowRenderPassContext::~PREShadowRenderPassContext() {}
	} // namespace PRE
} // namespace Core