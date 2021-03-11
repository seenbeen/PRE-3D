#include <modules/animation/animationchannelconfig.h>

#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PRE
{
	namespace AnimationModule
	{
		using std::pair;
		using std::string;
		using std::vector;

		void AnimationChannelConfig::AddPositionKeyFrame(
			float time,
			const glm::vec3& position
		)
		{
			_positionKeyFrames.push_back(
				std::make_pair(time, position)
			);
		}

		void AnimationChannelConfig::AddRotationKeyFrame(
			float time,
			const glm::fquat& rotation
		)
		{
			_rotationKeyFrames.push_back(
				std::make_pair(time, rotation)
			);
		}

		void AnimationChannelConfig::AddScaleKeyFrame(
			float time,
			const glm::vec3& scale
		)
		{
			_scaleKeyFrames.push_back(
				std::make_pair(time, scale)
			);
		}

		AnimationChannelConfig::AnimationChannelConfig() {}

		AnimationChannelConfig::~AnimationChannelConfig() {}
	} // namespace AnimationModule
} // namespace PRE