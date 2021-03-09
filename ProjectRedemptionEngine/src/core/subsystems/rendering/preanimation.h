#pragma once
#include <include/modules/animation.h>

namespace PRE
{
	namespace Core
	{
		class PREAnimatorComponent;

		class PRERendering;

		using PRE::AnimationModule::Animation;

		class PREAnimation
		{
			PREAnimation& operator=(const PREAnimation&) = delete;
			PREAnimation(const PREAnimation&) = delete;

			friend class PRERendering;
			friend class PREAnimatorComponent;

		private:
			Animation& _animation;

			PREAnimation(Animation& animation);
			~PREAnimation();
		};
	} // namespace Core
} // namespace PRE