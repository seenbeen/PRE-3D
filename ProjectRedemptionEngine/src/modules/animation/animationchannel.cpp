#include <modules/animation/animationchannel.h>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <modules/animation/animationchannelconfig.h>

namespace PRE
{
	namespace AnimationModule
	{
		using std::pair;
		using std::string;
		using std::vector;

		const glm::mat4 AnimationChannel::Impl::MAT4_IDENTITY = glm::mat4();

		glm::mat4 AnimationChannel::Impl::ComputeTransformMatrix(
			const glm::vec3& position,
			const glm::fquat& rotation,
			const glm::vec3& scale
		)
		{
			auto currentMatrix = glm::translate(MAT4_IDENTITY, position);
			currentMatrix = currentMatrix * glm::mat4_cast(rotation);
			currentMatrix = glm::scale(currentMatrix, scale);
			return currentMatrix;
		}

		glm::vec3 AnimationChannel::Impl::GetInterpolatedVec(
			const vector<pair<float, glm::vec3>>& keyFrames,
			float time
		)
		{
			auto timeNeedle = std::make_pair(time, glm::vec3());
			auto itKeyFrame = std::upper_bound(
				keyFrames.begin(),
				keyFrames.end(),
				timeNeedle,
				Impl::TimeVecKeyFrameCmp
			);

			auto tEnd = itKeyFrame->first;
			auto vEnd = itKeyFrame->second;
			--itKeyFrame;
			auto tStart = itKeyFrame->first;
			auto vStart = itKeyFrame->second;
			return glm::mix(
				vStart,
				vEnd,
				(time - tStart) / (tEnd - tStart)
			);
		}

		glm::fquat AnimationChannel::Impl::GetInterpolatedQuat(
			const vector<pair<float, glm::fquat>>& keyFrames,
			float time
		)
		{
			auto timeNeedle = std::make_pair(time, glm::fquat());
			auto itKeyFrame = std::upper_bound(
				keyFrames.begin(),
				keyFrames.end(),
				timeNeedle,
				Impl::TimeQuatKeyFrameCmp
			);
			auto tEnd = itKeyFrame->first;
			auto qEnd = itKeyFrame->second;
			--itKeyFrame;
			auto tStart = itKeyFrame->first;
			auto qStart = itKeyFrame->second;
			return glm::slerp(
				qStart,
				qEnd,
				(time - tStart) / (tEnd - tStart)
			);
		}

		bool AnimationChannel::Impl::TimeVecKeyFrameCmp(
			const pair<float, glm::vec3>& keyPairA,
			const pair<float, glm::vec3>& keyPairB
		)
		{
			return keyPairA.first < keyPairB.first;
		}

		bool AnimationChannel::Impl::TimeQuatKeyFrameCmp(
			const pair<float, glm::fquat>& keyPairA,
			const pair<float, glm::fquat>& keyPairB
		)
		{
			return keyPairA.first < keyPairB.first;
		}

		AnimationChannel::Impl& AnimationChannel::Impl::MakeImpl(
			const AnimationChannelConfig& animationChannelConfig
		)
		{
			return *(new Impl(
				animationChannelConfig._positionKeyFrames,
				animationChannelConfig._rotationKeyFrames,
				animationChannelConfig._scaleKeyFrames
			));
		}

		AnimationChannel::Impl::Impl(
			const vector<pair<float, glm::vec3>>& positionKeyFrames,
			const vector<pair<float, glm::fquat>>& rotationKeyFrames,
			const vector<pair<float, glm::vec3>>& scaleKeyFrames
		)
			:
			positionKeyFrames(positionKeyFrames),
			rotationKeyFrames(rotationKeyFrames),
			scaleKeyFrames(scaleKeyFrames) {}

		AnimationChannel::Impl::~Impl() {}

		glm::mat4 AnimationChannel::GetBlendedStateAt(
			const AnimationChannel& a,
			const AnimationChannel& b,
			float timeA,
			float timeB,
			float blendFactor
		)
		{
			// animation A
			auto positionVectorA = Impl::GetInterpolatedVec(
				a._impl.positionKeyFrames,
				timeA
			);
			auto rotationQuaternionA = Impl::GetInterpolatedQuat(
				a._impl.rotationKeyFrames,
				timeA
			);
			auto scaleVectorA = Impl::GetInterpolatedVec(
				a._impl.scaleKeyFrames,
				timeA
			);

			// animation B
			auto positionVectorB = Impl::GetInterpolatedVec(
				b._impl.positionKeyFrames,
				timeB
			);
			auto rotationQuaternionB = Impl::GetInterpolatedQuat(
				b._impl.rotationKeyFrames,
				timeB
			);
			auto scaleVectorB = Impl::GetInterpolatedVec(
				b._impl.scaleKeyFrames,
				timeB
			);

			// return blended values
			return Impl::ComputeTransformMatrix(
				glm::mix(positionVectorA, positionVectorB, blendFactor),
				glm::slerp(rotationQuaternionA, rotationQuaternionB, blendFactor),
				glm::mix(scaleVectorA, scaleVectorB, blendFactor)
			);
		}

		AnimationChannel::AnimationChannel(
			const AnimationChannelConfig& animationChannelConfig
		)
			:
			_impl(Impl::MakeImpl(animationChannelConfig)) {}

		AnimationChannel::~AnimationChannel()
		{
			delete &_impl;
		}

		glm::mat4 AnimationChannel::GetStateAt(float time) const
		{
			auto positionVector = Impl::GetInterpolatedVec(
				_impl.positionKeyFrames,
				time
			);
			auto rotationQuaternion = Impl::GetInterpolatedQuat(
				_impl.rotationKeyFrames,
				time
			);
			auto scaleVector = Impl::GetInterpolatedVec(
				_impl.scaleKeyFrames,
				time
			);

			return Impl::ComputeTransformMatrix(
				positionVector,
				rotationQuaternion,
				scaleVector
			);
		}
	} // namespace AnimationModule
} // namespace PRE