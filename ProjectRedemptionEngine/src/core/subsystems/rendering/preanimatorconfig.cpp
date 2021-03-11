#include <core/subsystems/rendering/preanimatorconfig.h>

#include <string>
#include <unordered_map>
#include <utility>

#include <core/components/preanimatorcomponent.h>

namespace PRE
{
	namespace Core
	{
		using std::pair;
		using std::string;
		using std::unordered_map;

		PREAnimatorConfig::PREAnimatorConfig() {}
		PREAnimatorConfig::~PREAnimatorConfig() {}

		void PREAnimatorConfig::AddState(
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
	} // namespace Core
} // namespace PRE