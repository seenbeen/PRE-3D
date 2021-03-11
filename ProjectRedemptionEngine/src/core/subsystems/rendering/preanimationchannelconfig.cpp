#include <core/subsystems/rendering/preanimationchannelconfig.h>

#include <glm/glm.hpp>

#include <include/modules/animation.h>

namespace PRE
{
	namespace Core
	{
		using PRE::AnimationModule::AnimationChannelConfig;

		void PREAnimationChannelConfig::AddPositionKeyFrame(
			float time,
			const glm::vec3& position
		)
		{
			_channelConfig.AddPositionKeyFrame(time, position);
		}

		void PREAnimationChannelConfig::AddRotationKeyFrame(
			float time,
			const glm::fquat& rotation
		)
		{
			_channelConfig.AddRotationKeyFrame(time, rotation);
		}

		void PREAnimationChannelConfig::AddScaleKeyFrame(
			float time,
			const glm::vec3& scale
		)
		{
			_channelConfig.AddScaleKeyFrame(time, scale);
		}

		PREAnimationChannelConfig::PREAnimationChannelConfig(
			AnimationChannelConfig& channelConfig
		)
			:
			_channelConfig(channelConfig) {}

		PREAnimationChannelConfig::~PREAnimationChannelConfig() {}
	} // namespace Core
} // namespace PRE