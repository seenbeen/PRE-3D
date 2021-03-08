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
		class RenderAnimation;
		class RenderAnimationChannelConfig;

		using std::pair;
		using std::string;
		using std::vector;

		class RenderAnimationChannel
		{
			RenderAnimationChannel& operator=(const RenderAnimationChannel&) = delete;
			RenderAnimationChannel(const RenderAnimationChannel&) = delete;

			friend class RenderAnimation;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderAnimationChannel;

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

				static Impl& MakeImpl(const RenderAnimationChannelConfig& animationChannelConfig);

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
				const RenderAnimationChannel& a,
				const RenderAnimationChannel& b,
				float timeA,
				float timeB,
				float blendFactor
			);

			const string _channelName;
			Impl& _impl;

			RenderAnimationChannel(const RenderAnimationChannelConfig& animationChannelConfig);
			~RenderAnimationChannel();

			glm::mat4 GetStateAt(float time) const;
		};
	} // namespace RenderingModule
} // namespace PRE