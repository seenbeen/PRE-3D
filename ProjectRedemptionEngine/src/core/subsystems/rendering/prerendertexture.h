#pragma once
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRECameraComponent;

		class PRERendering;
		class PREMaterial;

		using PRE::RenderingModule::RenderCompositingTarget;
		using PRE::RenderingModule::RenderCompositingNode;

		class PRERenderTexture
		{
			PRERenderTexture& operator=(const PRERenderTexture&) = delete;
			PRERenderTexture(const PRERenderTexture&) = delete;
			
			friend class PRERendering;
			friend class PREMaterial;

		private:
			static void OnRender(
				RenderCompositingNode::RenderComposition& composition,
				void* vContext
			);

			PRERendering& _rendering;
			RenderCompositingTarget* const _pCompositingTarget;
			RenderCompositingNode& _compositingNode;

			PRECameraComponent* _pAssociatedCameraComponent;

			PRERenderTexture(
				PRERendering& rendering,
				RenderCompositingTarget* pCompositingTarget
			);
			~PRERenderTexture();
		};
	} // namespace Core
} // namespace PRE