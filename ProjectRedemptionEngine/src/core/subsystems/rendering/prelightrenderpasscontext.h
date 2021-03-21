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
		class PRERenderTexture;
		class PREAmbientLightComponent;
		class PREPointLightComponent;
		class PRESpotLightComponent;
		class PREDirectionalLightComponent;

		using std::unordered_map;
		using std::unordered_set;

		using PRE::RenderingModule::RenderCompositingTarget;

		class PRELightRenderPassContext
		{
			PRELightRenderPassContext& operator=(const PRELightRenderPassContext&) = delete;
			PRELightRenderPassContext(const PRELightRenderPassContext&) = delete;

			friend class PRERendering;

		private:
			unordered_map<int, unordered_set<PREModelRendererComponent*>>& _modelTagMap;
			PRERenderTexture& _renderTexture;

			RenderCompositingTarget* _pShadowMap;

			PREAmbientLightComponent* const _pAmbientLightComponent;
			PREPointLightComponent* const _pPointLightComponent;
			PRESpotLightComponent* const _pSpotLightComponent;
			PREDirectionalLightComponent* const _pDirectionalLightComponent;

			PRELightRenderPassContext(
				unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
				PRERenderTexture& renderTexture,
				PREAmbientLightComponent& ambientLightComponent
			);

			PRELightRenderPassContext(
				unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
				PRERenderTexture& renderTexture,
				RenderCompositingTarget& shadowMap,
				PREPointLightComponent& pointLightComponent
			);

			PRELightRenderPassContext(
				unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
				PRERenderTexture& renderTexture,
				RenderCompositingTarget& shadowMap,
				PRESpotLightComponent& spotLightComponent
			);

			PRELightRenderPassContext(
				unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
				PRERenderTexture& renderTexture,
				RenderCompositingTarget& shadowMap,
				PREDirectionalLightComponent& directionalLightComponent
			);

			~PRELightRenderPassContext();
		};
	} // namespace PRE
} // namespace Core