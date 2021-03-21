#pragma once
#include <unordered_map>
#include <unordered_set>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PREModelRendererComponent;

		class PRERendering;
		class PREShader;
		class PRERenderTexture;
		class PREPointLightComponent;
		class PRESpotLightComponent;
		class PREDirectionalLightComponent;

		using std::unordered_map;
		using std::unordered_set;

		using PRE::RenderingModule::RenderCompositingTarget;
		using PRE::RenderingModule::RenderCamera;

		class PREShadowRenderPassContext
		{
			PREShadowRenderPassContext& operator=(const PREShadowRenderPassContext&) = delete;
			PREShadowRenderPassContext(const PREShadowRenderPassContext&) = delete;

			friend class PRERendering;

		private:
			// this is the camera component we'll use to simulate
			// the light's view transform
			RenderCamera& _lightPOVCamera;

			unordered_map<int, unordered_set<PREModelRendererComponent*>>& _modelTagMap;

			// this is the shadow depth frame buffer
			RenderCompositingTarget& _target;

			PREPointLightComponent* const _pPointLightComponent;
			PRESpotLightComponent* const _pSpotLightComponent;
			PREDirectionalLightComponent* const _pDirectionalLightComponent;

			PREShadowRenderPassContext(
				RenderCamera& lightPOVCamera,
				unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
				RenderCompositingTarget& target,
				PREPointLightComponent& pointLightComponent
			);

			PREShadowRenderPassContext(
				RenderCamera& lightPOVCamera,
				unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
				RenderCompositingTarget& target,
				PRESpotLightComponent& spotLightComponent
			);

			PREShadowRenderPassContext(
				RenderCamera& lightPOVCamera,
				unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
				RenderCompositingTarget& target,
				PREDirectionalLightComponent& directionalLightComponent
			);

			~PREShadowRenderPassContext();
		};
	} // namespace PRE
} // namespace Core