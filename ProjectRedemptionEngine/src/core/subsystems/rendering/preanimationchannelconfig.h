#pragma once
#include <glm/glm.hpp>

#include <include/modules/animation.h>

namespace PRE
{
	namespace Core
	{
		class PREAnimationConfig;

		using PRE::AnimationModule::AnimationChannelConfig;

		class PREAnimationChannelConfig
		{
			PREAnimationChannelConfig& operator=(const PREAnimationChannelConfig&) = delete;
			PREAnimationChannelConfig(const PREAnimationChannelConfig&) = delete;

			friend class PREAnimationConfig;

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
			AnimationChannelConfig& _channelConfig;

			PREAnimationChannelConfig(
				AnimationChannelConfig& channelConfig
			);
			~PREAnimationChannelConfig();
		};
	} // namespace Core
} // namespace PRE