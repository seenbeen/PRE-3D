#pragma once
#include <string>
#include <unordered_map>

namespace PRE
{
	namespace AnimationModule
	{
		class Animation;
		class AnimationChannelConfig;

		using std::string;
		using std::unordered_map;

		class AnimationConfig
		{
			AnimationConfig& operator=(const AnimationConfig&) = delete;
			AnimationConfig(const AnimationConfig&) = delete;

			friend class Animation;

		public:
			AnimationConfig(float ticksPerSecond, float duration);
			~AnimationConfig();

			AnimationChannelConfig& AddAnimationChannelConfig(
				const string& channelName
			);

		private:
			const float _ticksPerSecond;
			const float _duration;
			unordered_map<string, AnimationChannelConfig*> _animationChannelConfigs;
		};
	} // namespace AnimationModule
} // namespace PRE