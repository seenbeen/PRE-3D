#include <core/subsystems/rendering/preanimation.h>

#include <include/modules/animation.h>

#include <core/subsystems/rendering/preanimationconfig.h>

namespace PRE
{
	namespace Core
	{
		class PREAnimatorComponent;

		class PRERendering;

		using PRE::AnimationModule::Animation;

		PREAnimation::PREAnimation(const PREAnimationConfig& animationConfig)
			:
			_animation(animationConfig._animationConfig) {}

		PREAnimation::~PREAnimation() {}

		void PREAnimation::GetBlendedStateAt(
			const PREAnimation& a,
			const PREAnimation& b,
			float timeA,
			float timeB,
			float blendFactor,
			unordered_map<string, glm::mat4>& result
		)
		{
			Animation::GetBlendedStateAt(
				a._animation,
				b._animation,
				timeA,
				timeB,
				blendFactor,
				result
			);
		}

		float PREAnimation::GetDuration() const
		{
			return _animation.GetDuration();
		}

		void PREAnimation::GetStateAt(
			float time,
			unordered_map<string, glm::mat4>& result
		) const
		{
			_animation.GetStateAt(time, result);
		}
	} // namespace Core
} // namespace PRE