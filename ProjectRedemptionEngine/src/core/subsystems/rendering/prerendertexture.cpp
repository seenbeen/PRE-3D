#include <core/subsystems/rendering/prerendertexture.h>

#include <include/modules/rendering.h>

#include <core/subsystems/rendering/prerendering.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::RenderCompositingTarget;
		using PRE::RenderingModule::RenderCompositingNode;

		void PRERenderTexture::OnRender(
			RenderCompositingNode::RenderComposition& composition,
			void* vContext
		)
		{
			auto& renderTexture = *static_cast<PRERenderTexture*>(vContext);
			if (renderTexture._pAssociatedCameraComponent != nullptr)
			{
				renderTexture._rendering.ComposeRenderComposition(
					composition,
					*renderTexture._pAssociatedCameraComponent
				);
				composition.SetCompositingTarget(renderTexture._pCompositingTarget);
			}
		}

		PRERenderTexture::PRERenderTexture(
			PRERendering& rendering,
			RenderCompositingTarget* pCompositingTarget
		)
			:
			_rendering(rendering),
			_pCompositingTarget(pCompositingTarget),
			_compositingNode(
				_rendering.AllocateCompositingNode(
					OnRender,
					*this
				)
			),
			_pAssociatedCameraComponent(nullptr) {}

		PRERenderTexture::~PRERenderTexture()
		{
			_rendering.DeallocateCompositingNode(_compositingNode);
		}
	} // namespace Core
} // namespace PRE