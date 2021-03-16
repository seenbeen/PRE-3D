#pragma once

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PRERenderTexture;
		class PREPointLightComponent;

		// class PRESpotLightComponent;
		// class PREDirectionalLightComponent;
		// class PREAmbientLightComponent;

		class PRELightRenderPassContext
		{
			PRELightRenderPassContext& operator=(const PRELightRenderPassContext&) = delete;
			PRELightRenderPassContext(const PRELightRenderPassContext&) = delete;

			friend class PRERendering;

		private:
			PRERenderTexture& _renderTexture;

			PREPointLightComponent* _pPointLightComponent;

			PRELightRenderPassContext(
				PRERenderTexture& renderTexture,
				PREPointLightComponent& pointLightComponent
			);
			~PRELightRenderPassContext();
		};
	} // namespace PRE
} // namespace Core