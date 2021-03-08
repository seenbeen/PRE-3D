#include <modules/rendering/animation/renderanimationchannelconfig.h>

#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		using std::pair;
		using std::vector;

		RenderAnimationChannelConfig::RenderAnimationChannelConfig(int id)
			:
			id(id) {}

		RenderAnimationChannelConfig::~RenderAnimationChannelConfig() {}

		void RenderAnimationChannelConfig::AddPositionKeyFrame(
			float time,
			const glm::vec3& position
		)
		{
			_positionKeyFrames.push_back(
				std::make_pair(time, position)
			);
		}

		void RenderAnimationChannelConfig::AddRotationKeyFrame(
			float time,
			const glm::fquat& rotation
		)
		{
			_rotationKeyFrames.push_back(
				std::make_pair(time, rotation)
			);
		}

		void RenderAnimationChannelConfig::AddScaleKeyFrame(
			float time,
			const glm::vec3& scale
		)
		{
			_scaleKeyFrames.push_back(
				std::make_pair(time, scale)
			);
		}
	} // namespace RenderingModule
} // namespace PRE