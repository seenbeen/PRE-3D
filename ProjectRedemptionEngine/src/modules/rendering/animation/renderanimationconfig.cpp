#include <modules/rendering/animation/renderanimationconfig.h>

#ifdef __PRE_DEBUG__
#include <algorithm>
#endif

#include <vector>

#include <modules/rendering/animation/renderanimationchannelconfig.h>

namespace PRE
{
	namespace RenderingModule
	{
		RenderAnimationConfig::RenderAnimationConfig() {}

		RenderAnimationConfig::~RenderAnimationConfig()
		{
			for (auto it = _animationChannelConfigs.begin(); it != _animationChannelConfigs.end(); ++it)
			{
				delete *it;
			}
		}

		RenderAnimationChannelConfig& RenderAnimationConfig::AddAnimationChannelConfig(int channelId)
		{
#ifdef __PRE_DEBUG__
			auto localChannelId = channelId;
			auto it = std::find_if(
				_animationChannelConfigs.begin(),
				_animationChannelConfigs.end(),
				[localChannelId](RenderAnimationChannelConfig* config)
				{
					return config->id == localChannelId;
				}
			);
			if (it != _animationChannelConfigs.end())
			{
				throw "Duplicate channel id detected";
			}
#endif

			auto pAnimationChannelConfig = new RenderAnimationChannelConfig(
				channelId
			);
			_animationChannelConfigs.push_back(pAnimationChannelConfig);
			return *pAnimationChannelConfig;
		}
	} // namespace RenderingModule
} // namespace PRE