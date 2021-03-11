#include <core/subsystems/rendering/preanimationconfig.h>

#include <string>

#include <include/modules/animation.h>

#include <core/subsystems/rendering/preanimationchannelconfig.h>

namespace PRE
{
	namespace Core
	{
		using std::string;

		PREAnimationConfig::PREAnimationConfig(float ticksPerSecond, float duration)
			:
			_animationConfig(ticksPerSecond, duration) {}
		
		PREAnimationConfig::~PREAnimationConfig()
		{
			for (auto it = _channelConfigs.begin(); it != _channelConfigs.end(); ++it)
			{
				delete *it;
			}
		}

		PREAnimationChannelConfig& PREAnimationConfig::AddAnimationChannelConfig(
			const string& channelName
		)
		{
			auto pChannelConfig = new PREAnimationChannelConfig(
				_animationConfig.AddAnimationChannelConfig(channelName)
			);
			_channelConfigs.push_back(pChannelConfig);
			return *pChannelConfig;
		}
	} // namespace Core
} // namespace PRE