#pragma once
#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class RenderAnimationChannel;
		class RenderAnimationConfig;

		using std::pair;
		using std::string;
		using std::vector;

		class RenderAnimationChannelConfig
		{
			RenderAnimationChannelConfig& operator=(const RenderAnimationChannelConfig&) = delete;
			RenderAnimationChannelConfig(const RenderAnimationChannelConfig&) = delete;

			friend class RenderAnimationChannel;
			friend class RenderAnimationConfig;

		private:
			vector<pair<float, glm::vec3>> _positionKeyFrames;
			vector<pair<float, glm::fquat>> _rotationKeyFrames;
			vector<pair<float, glm::vec3>> _scaleKeyFrames;

			RenderAnimationChannelConfig(const string& channelName);
			~RenderAnimationChannelConfig();

		public:
			const string channelName;

			void AddPositionKeyFrame(
				float time,
				const glm::vec3& position
			);

			void AddRotationKeyFrame(
				float time,
				const glm::fquat& rotation
			);

			void AddScaleKeyFrame(
				float time,
				const glm::vec3& scale
			);
		};
	} // namespace RenderingModule
} // namespace PRE