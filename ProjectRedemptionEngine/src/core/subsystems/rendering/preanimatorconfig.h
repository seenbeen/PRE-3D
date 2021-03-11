#pragma once
#include <string>
#include <unordered_map>
#include <utility>

#include <core/components/preanimatorcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PREAnimator;
		class PREAnimation;

		using std::pair;
		using std::string;
		using std::unordered_map;

		class PREAnimatorConfig
		{
			PREAnimatorConfig& operator=(const PREAnimatorConfig&) = delete;
			PREAnimatorConfig(const PREAnimatorConfig&) = delete;

			friend class PREAnimator;

		public:
			typedef void (*OnStateUpdate)(PREAnimatorComponent::Controller& animatorComponentController);

			PREAnimatorConfig();
			~PREAnimatorConfig();

			void AddState(
				const string& stateName,
				OnStateUpdate onStateUpdate,
				const PREAnimation& animation
			);

		private:
			unordered_map<string, pair<OnStateUpdate, const PREAnimation*>> _states;
		};
	} // namespace Core
} // namespace PRE