#include <core/subsystems/rendering/preanimator.h>

#include <string>
#include <unordered_map>
#include <utility>

#include <core/components/preanimatorcomponent.h>

#include <core/subsystems/rendering/preanimation.h>

namespace PRE
{
	namespace Core
	{
		using std::pair;
		using std::string;
		using std::unordered_map;

		void PREAnimator::AddState(
			const string& stateName,
			OnStateUpdate onStateUpdate,
			const PREAnimation& animation
		)
		{
			_states.insert(
				std::make_pair(
					stateName,
					std::make_pair(
						onStateUpdate,
						&animation
					)
				)
			);
		}

		PREAnimator::PREAnimator() {}

		PREAnimator::~PREAnimator() {}
	} // namespace Core
} // namespace PRE