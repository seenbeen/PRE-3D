#include <modules/animation/animationconfig.h>

#ifdef __PRE_DEBUG__
#include <algorithm>
#endif

#include <string>
#include <unordered_map>

#include <modules/animation/animationchannelconfig.h>

namespace PRE
{
	namespace AnimationModule
	{
		using std::string;
		using std::unordered_map;

		AnimationConfig::AnimationConfig(float ticksPerSecond, float duration)
			:
			_ticksPerSecond(ticksPerSecond),
			_duration(duration) {}

		AnimationConfig::~AnimationConfig()
		{
			for (auto it = _animationChannelConfigs.begin(); it != _animationChannelConfigs.end(); ++it)
			{
				delete it->second;
			}
		}

		AnimationChannelConfig& AnimationConfig::AddAnimationChannelConfig(
			const string& channelName
		)
		{
#ifdef __PRE_DEBUG__
			assert(_animationChannelConfigs.find(channelName) == _animationChannelConfigs.end());
#endif

			auto pAnimationChannelConfig = new AnimationChannelConfig();
			_animationChannelConfigs[channelName] = pAnimationChannelConfig;
			return *pAnimationChannelConfig;
		}
	} // namespace AnimationModule
} // namespace PRE