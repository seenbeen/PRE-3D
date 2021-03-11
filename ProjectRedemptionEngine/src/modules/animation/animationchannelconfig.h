#pragma once
#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PRE
{
	namespace AnimationModule
	{
		class AnimationChannel;
		class AnimationConfig;

		using std::pair;
		using std::string;
		using std::vector;

		class AnimationChannelConfig
		{
			AnimationChannelConfig& operator=(const AnimationChannelConfig&) = delete;
			AnimationChannelConfig(const AnimationChannelConfig&) = delete;

			friend class AnimationChannel;
			friend class AnimationConfig;

		public:
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

		private:
			vector<pair<float, glm::vec3>> _positionKeyFrames;
			vector<pair<float, glm::fquat>> _rotationKeyFrames;
			vector<pair<float, glm::vec3>> _scaleKeyFrames;

			AnimationChannelConfig();
			~AnimationChannelConfig();
		};
	} // namespace AnimationModule
} // namespace PRE