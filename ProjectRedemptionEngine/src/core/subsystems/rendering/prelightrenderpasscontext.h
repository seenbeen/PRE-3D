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
		class PREDirectionalLightComponent;

		class PRELightRenderPassContext
		{
			PRELightRenderPassContext& operator=(const PRELightRenderPassContext&) = delete;
			PRELightRenderPassContext(const PRELightRenderPassContext&) = delete;

			friend class PRERendering;

		private:
			PRERenderTexture& _renderTexture;

			PREAmbientLightComponent* const _pAmbientLightComponent;
			PREPointLightComponent* const _pPointLightComponent;
			PRESpotLightComponent* const _pSpotLightComponent;
			PREDirectionalLightComponent* const _pDirectionalLightComponent;

			PRELightRenderPassContext(
				PRERenderTexture& renderTexture,
				PREAmbientLightComponent& ambientLightComponent
			);

			PRELightRenderPassContext(
				PRERenderTexture& renderTexture,
				PREPointLightComponent& pointLightComponent
			);

			PRELightRenderPassContext(
				PRERenderTexture& renderTexture,
				PRESpotLightComponent& spotLightComponent
			);

			PRELightRenderPassContext(
				PRERenderTexture& renderTexture,
				PREDirectionalLightComponent& directionalLightComponent
			);

			~PRELightRenderPassContext();
		};
	} // namespace PRE
} // namespace Core