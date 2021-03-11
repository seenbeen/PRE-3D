#pragma once
#include <string>
#include <vector>

#include <include/modules/animation.h>

namespace PRE
{
	namespace Core
	{
		class PREAnimation;
		class PREAnimationChannelConfig;

		using std::string;
		using std::vector;

		using PRE::AnimationModule::AnimationConfig;

		class PREAnimationConfig
		{
			PREAnimationConfig& operator=(const PREAnimationConfig&) = delete;
			PREAnimationConfig(const PREAnimationConfig&) = delete;

			friend class PREAnimation;
		public:
			PREAnimationConfig(float ticksPerSecond, float duration);
			~PREAnimationConfig();

			PREAnimationChannelConfig& AddAnimationChannelConfig(
				const string& channelName
			);

		private:
			vector<PREAnimationChannelConfig*> _channelConfigs;
			AnimationConfig _animationConfig;
		};
	} // namespace Core
} // namespace PRE