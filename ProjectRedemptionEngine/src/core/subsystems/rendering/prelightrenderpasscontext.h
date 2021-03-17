#pragma once

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PRERenderTexture;
		class PREAmbientLightComponent;
		class PREPointLightComponent;
		class PRESpotLightComponent;

		class PRELightRenderPassContext
		{
			PRELightRenderPassContext& operator=(const PRELightRenderPassContext&) = delete;
			PRELightRenderPassContext(const PRELightRenderPassContext&) = delete;

			friend class PRERendering;

		private:
			const bool _isFirstPass;
			PRERenderTexture& _renderTexture;

			PREAmbientLightComponent* const _pAmbientLightComponent;
			PREPointLightComponent* const _pPointLightComponent;
			PRESpotLightComponent* const _pSpotLightComponent;

			PRELightRenderPassContext(
				bool isFirstPass,
				PRERenderTexture& renderTexture,
				PREAmbientLightComponent& ambientLightComponent
			);

			PRELightRenderPassContext(
				bool isFirstPass,
				PRERenderTexture& renderTexture,
				PREPointLightComponent& pointLightComponent
			);

			PRELightRenderPassContext(
				bool isFirstPass,
				PRERenderTexture& renderTexture,
				PRESpotLightComponent& spotLightComponent
			);

			~PRELightRenderPassContext();
		};
	} // namespace PRE
} // namespace Core