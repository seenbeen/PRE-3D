#pragma once
#include <vector>

namespace PRE
{
	namespace RenderingModule
	{
		class RenderAnimation;
		class RenderAnimationChannelConfig;

		using std::vector;

		class RenderAnimationConfig
		{
			RenderAnimationConfig& operator=(const RenderAnimationConfig&) = delete;
			RenderAnimationConfig(const RenderAnimationConfig&) = delete;

			friend class RenderAnimation;

		private:
			vector<RenderAnimationChannelConfig*> _animationChannelConfigs;

		public:
			RenderAnimationConfig();
			~RenderAnimationConfig();

			RenderAnimationChannelConfig& AddAnimationChannelConfig(int channelId);
		};
	} // namespace RenderingModule
} // namespace PRE