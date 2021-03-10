#include <modules/animation/animationconfig.h>

#ifdef __PRE_DEBUG__
#include <algorithm>
#endif

#include <string>
#include <vector>

#include <modules/animation/animationchannelconfig.h>

namespace PRE
{
	namespace AnimationModule
	{
		using std::string;
		using std::vector;

		AnimationConfig::AnimationConfig() {}

		AnimationConfig::~AnimationConfig()
		{
			for (auto it = _animationChannelConfigs.begin(); it != _animationChannelConfigs.end(); ++it)
			{
				delete *it;
			}
		}

		AnimationChannelConfig& AnimationConfig::AddAnimationChannelConfig(
			const string& channelName
		)
		{
#ifdef __PRE_DEBUG__
			auto it = std::find_if(
				_animationChannelConfigs.begin(),
				_animationChannelConfigs.end(),
				[&channelName](AnimationChannelConfig* config)
				{
					return config->channelName == channelName;
				}
			);
			assert(it == _animationChannelConfigs.end());
#endif

			auto pAnimationChannelConfig = new AnimationChannelConfig(
				channelName
			);
			_animationChannelConfigs.push_back(pAnimationChannelConfig);
			return *pAnimationChannelConfig;
		}
	} // namespace AnimationModule
} // namespace PRE