#include <core/subsystems/rendering/preanimator.h>

#include <string>
#include <unordered_map>
#include <utility>

#include <core/components/preanimatorcomponent.h>

#include <core/subsystems/rendering/preanimatorconfig.h>

#include <core/subsystems/rendering/preanimation.h>

namespace PRE
{
	namespace Core
	{
		using std::pair;
		using std::string;
		using std::unordered_map;

		PREAnimator::PREAnimator(const PREAnimatorConfig& animatorConfig)
			:
			_states(animatorConfig._states) {}

		PREAnimator::~PREAnimator() {}
	} // namespace Core
} // namespace PRE