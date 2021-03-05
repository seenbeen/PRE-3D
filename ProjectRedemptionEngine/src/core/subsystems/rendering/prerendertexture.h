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
			const unsigned int _tagGroup;
			RenderCompositingNode& _compositingNode;

			PRERenderTexture(unsigned int tagGroup, RenderCompositingNode& compositingNode);
			~PRERenderTexture();
		};
	} // namespace Core
} // namespace PRE