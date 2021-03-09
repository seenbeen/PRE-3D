#include <core/subsystems/rendering/preanimation.h>

#include <include/modules/animation.h>

namespace PRE
{
	namespace Core
	{
		class PREAnimatorComponent;

		class PRERendering;

		using PRE::AnimationModule::Animation;

		PREAnimation::PREAnimation(Animation& animation)
			:
			_animation(animation) {}

		PREAnimation::~PREAnimation() {}
	} // namespace Core
} // namespace PRE