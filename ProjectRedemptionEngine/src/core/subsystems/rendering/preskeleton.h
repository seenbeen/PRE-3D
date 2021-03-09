#pragma once
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;

		using PRE::RenderingModule::RenderSkeleton;

		class PRESkeleton
		{
			PRESkeleton& operator=(const PRESkeleton&) = delete;
			PRESkeleton(const PRESkeleton&) = delete;

			friend class PRERendering;

		private:
			RenderSkeleton& _skeleton;

			PRESkeleton(RenderSkeleton& skeleton);
			~PRESkeleton();
		};
	} // namespace Core
} // namespace PRE