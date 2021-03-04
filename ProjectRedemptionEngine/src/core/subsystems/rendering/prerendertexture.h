#pragma once
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PREMaterial;

		using PRE::RenderingModule::RenderCompositingNode;

		class PRERenderTexture
		{
			PRERenderTexture& operator=(const PRERenderTexture&) = delete;
			PRERenderTexture(const PRERenderTexture&) = delete;
			
			friend class PRERendering;
			friend class PREMaterial;

		private:
			RenderCompositingNode& _compositingNode;

			PRERenderTexture(RenderCompositingNode& compositingNode);
			~PRERenderTexture();
		};
	} // namespace Core
} // namespace PRE