#pragma once
#include <string>
#include <vector>

namespace PRE
{
	namespace AnimationModule
	{
		class Animation;
		class AnimationChannelConfig;

		using std::string;
		using std::vector;

		class AnimationConfig
		{
			AnimationConfig& operator=(const AnimationConfig&) = delete;
			AnimationConfig(const AnimationConfig&) = delete;

			friend class Animation;

		public:
			AnimationConfig();
			~AnimationConfig();

			AnimationChannelConfig& AddAnimationChannelConfig(
				const string& channelName
			);

		private:
			vector<AnimationChannelConfig*> _animationChannelConfigs;
		};
	} // namespace AnimationModule
} // namespace PRE