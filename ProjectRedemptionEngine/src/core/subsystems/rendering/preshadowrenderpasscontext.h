#pragma once
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PRERenderTexture;
		class PREPointLightComponent;
		class PRESpotLightComponent;
		class PREDirectionalLightComponent;

		using PRE::RenderingModule::RenderCompositingTarget;
		using PRE::RenderingModule::RenderCamera;

		class PREShadowRenderPassContext
		{
			PREShadowRenderPassContext& operator=(const PREShadowRenderPassContext&) = delete;
			PREShadowRenderPassContext(const PREShadowRenderPassContext&) = delete;

			friend class PRERendering;

		private:
			// this is the shadow depth frame buffer
			RenderCompositingTarget& _target;
			// this is the texture this shadow map will be used for
			PRERenderTexture& _renderTexture;

			PREPointLightComponent* const _pPointLightComponent;
			PRESpotLightComponent* const _pSpotLightComponent;
			PREDirectionalLightComponent* const _pDirectionalLightComponent;

			PREShadowRenderPassContext(
				RenderCompositingTarget& target,
				PRERenderTexture& renderTexture,
				PREPointLightComponent& pointLightComponent
			);

			PREShadowRenderPassContext(
				RenderCompositingTarget& target,
				PRERenderTexture& renderTexture,
				PRESpotLightComponent& spotLightComponent
			);

			PREShadowRenderPassContext(
				RenderCompositingTarget& target,
				PRERenderTexture& renderTexture,
				PREDirectionalLightComponent& directionalLightComponent
			);

			~PREShadowRenderPassContext();
		};
	} // namespace PRE
} // namespace Core