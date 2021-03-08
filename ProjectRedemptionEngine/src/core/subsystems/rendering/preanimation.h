#pragma once
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PREAnimatorComponent;

		class PRERendering;
		class PREAnimator;

		using PRE::RenderingModule::RenderAnimation;

		class PREAnimation
		{
			PREAnimation& operator=(const PREAnimation&) = delete;
			PREAnimation(const PREAnimation&) = delete;

			friend class PRERendering;
			friend class PREAnimatorComponent;

		private:
			RenderAnimation& _animation;

			PREAnimation(RenderAnimation& animation);
			~PREAnimation();
		};
	} // namespace Core
} // namespace PRE