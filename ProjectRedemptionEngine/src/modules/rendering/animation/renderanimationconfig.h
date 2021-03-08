#pragma once
#include <string>
#include <vector>

namespace PRE
{
	namespace RenderingModule
	{
		class RenderAnimation;
		class RenderAnimationChannelConfig;

		using std::string;
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

			RenderAnimationChannelConfig& AddAnimationChannelConfig(
				const string& channelName
			);
		};
	} // namespace RenderingModule
} // namespace PRE