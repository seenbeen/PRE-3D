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
		class Animation;
		class AnimationChannelConfig;

		using std::pair;
		using std::string;
		using std::vector;

		class AnimationChannel
		{
			AnimationChannel& operator=(const AnimationChannel&) = delete;
			AnimationChannel(const AnimationChannel&) = delete;

			friend class Animation;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class AnimationChannel;

			private:
				static const glm::mat4 MAT4_IDENTITY;

				static glm::mat4 ComputeTransformMatrix(
					const glm::vec3& position,
					const glm::fquat& rotation,
					const glm::vec3& scale
				);

				static glm::vec3 GetInterpolatedVec(
					const vector<pair<float, glm::vec3>>& keyFrames,
					float time
				);

				static glm::fquat GetInterpolatedQuat(
					const vector<pair<float, glm::fquat>>& keyFrames,
					float time
				);

				static bool TimeVecKeyFrameCmp(
					const pair<float, glm::vec3>& keyPairA,
					const pair<float, glm::vec3>& keyPairB
				);

				static bool TimeQuatKeyFrameCmp(
					const pair<float, glm::fquat>& keyPairA,
					const pair<float, glm::fquat>& keyPairB
				);

				static Impl& MakeImpl(const AnimationChannelConfig& animationChannelConfig);

				// TODO: Can optimize for locality by storing current frame index
				//       for position, rotation, scale keyframes
				const vector<pair<float, glm::vec3>> positionKeyFrames;
				const vector<pair<float, glm::fquat>> rotationKeyFrames;
				const vector<pair<float, glm::vec3>> scaleKeyFrames;

				Impl(
					const vector<pair<float, glm::vec3>>& positionKeyFrames,
					const vector<pair<float, glm::fquat>>& rotationKeyFrames,
					const vector<pair<float, glm::vec3>>& scaleKeyFrames
				);
				~Impl();
			};

		private:
			static glm::mat4 GetBlendedStateAt(
				const AnimationChannel& a,
				const AnimationChannel& b,
				float timeA,
				float timeB,
				float blendFactor
			);

			const string _channelName;
			Impl& _impl;

			AnimationChannel(const AnimationChannelConfig& animationChannelConfig);
			~AnimationChannel();

			glm::mat4 GetStateAt(float time) const;
		};
	} // namespace AnimationModule
} // namespace PRE