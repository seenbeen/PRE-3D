#pragma once
#include <string>
#include <unordered_map>
#include <utility>

#include <core/components/preanimatorcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PREAnimation;

		using std::pair;
		using std::string;
		using std::unordered_map;

		class PREAnimator
		{
			PREAnimator& operator=(const PREAnimator&) = delete;
			PREAnimator(const PREAnimator&) = delete;

			friend class PREAnimatorComponent;

		public:
			typedef void (*OnStateUpdate)(PREAnimatorComponent::Controller& animatorComponentController);

			void AddState(
				const string& stateName,
				OnStateUpdate onStateUpdate,
				const PREAnimation& animation
			);


			// TODO: RENDERING->MAKE ANIMATOR WITH CONFIGS
			PREAnimator();
			~PREAnimator();

		private:
			unordered_map<string, pair<OnStateUpdate, const PREAnimation*>> _states;
		};
	} // namespace Core
} // namespace PRE