#pragma once
#include <string>
#include <unordered_map>
#include <utility>

#include <core/subsystems/rendering/preanimatorconfig.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PREAnimation;

		using std::pair;
		using std::string;
		using std::unordered_map;

		class PREAnimator
		{
			PREAnimator& operator=(const PREAnimator&) = delete;
			PREAnimator(const PREAnimator&) = delete;

			friend class PRERendering;
			friend class PREAnimatorComponent;

		private:
			const unordered_map<string, pair<PREAnimatorConfig::OnStateUpdate, const PREAnimation*>> _states;

			PREAnimator(const PREAnimatorConfig& animatorConfig);
			~PREAnimator();
		};
	} // namespace Core
} // namespace PRE