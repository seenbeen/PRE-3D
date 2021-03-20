#pragma once
#include <unordered_map>
#include <unordered_set>

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

		class PRELightRenderPassContext
		{
			PRELightRenderPassContext& operator=(const PRELightRenderPassContext&) = delete;
			PRELightRenderPassContext(const PRELightRenderPassContext&) = delete;

			friend class PRERendering;

		private:
			unordered_map<int, unordered_set<PREModelRendererComponent*>>& _modelTagMap;
			PRERenderTexture& _renderTexture;

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
				PREPointLightComponent& pointLightComponent
			);

			PRELightRenderPassContext(
				unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
				PRERenderTexture& renderTexture,
				PRESpotLightComponent& spotLightComponent
			);

			PRELightRenderPassContext(
				unordered_map<int, unordered_set<PREModelRendererComponent*>>& modelTagMap,
				PRERenderTexture& renderTexture,
				PREDirectionalLightComponent& directionalLightComponent
			);

			~PRELightRenderPassContext();
		};
	} // namespace PRE
} // namespace Core