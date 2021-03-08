#include <modules/rendering/animation/renderanimationconfig.h>

#ifdef __PRE_DEBUG__
#include <algorithm>
#endif

#include <string>
#include <vector>

#include <modules/rendering/animation/renderanimationchannelconfig.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::string;
		using std::vector;

		RenderAnimationConfig::RenderAnimationConfig() {}

		RenderAnimationConfig::~RenderAnimationConfig()
		{
			for (auto it = _animationChannelConfigs.begin(); it != _animationChannelConfigs.end(); ++it)
			{
				delete *it;
			}
		}

		RenderAnimationChannelConfig& RenderAnimationConfig::AddAnimationChannelConfig(
			const string& channelName
		)
		{
#ifdef __PRE_DEBUG__
			auto it = std::find_if(
				_animationChannelConfigs.begin(),
				_animationChannelConfigs.end(),
				[&channelName](RenderAnimationChannelConfig* config)
				{
					return config->channelName == channelName;
				}
			);
			if (it != _animationChannelConfigs.end())
			{
				throw "Duplicate channel id detected";
			}
#endif

			auto pAnimationChannelConfig = new RenderAnimationChannelConfig(
				channelName
			);
			_animationChannelConfigs.push_back(pAnimationChannelConfig);
			return *pAnimationChannelConfig;
		}
	} // namespace RenderingModule
} // namespace PRE