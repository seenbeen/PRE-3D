#include <modules/rendering/animation/renderanimationbone.h>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <modules/rendering/animation/renderanimationboneconfig.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::string;
		using std::vector;
		using std::pair;

		RenderAnimationBone::Impl& RenderAnimationBone::Impl::MakeImpl(
			const RenderAnimationBoneConfig& animationBoneConfig
		)
		{
			vector<RenderAnimationBone*> children;
			for (
				auto it = animationBoneConfig._children.begin();
				it != animationBoneConfig._children.end();
				++it
				)
			{
				children.push_back(new RenderAnimationBone(**it));
			}
			std::sort(
				children.begin(),
				children.end(),
				[](const RenderAnimationBone* a, const RenderAnimationBone* b)
				{
					return a->name.compare(b->name);
				}
			);

			vector<pair<float, glm::vec3>> positionKeyFrames;
			vector<pair<float, glm::fquat>> rotationKeyFrames;
			vector<pair<float, glm::vec3>> scaleKeyFrames;
			for (
				auto it = animationBoneConfig._keyFrames.begin();
				it != animationBoneConfig._keyFrames.end();
				++it
			)
			{
				auto keyFrame = *it;
				if (keyFrame->kind == RenderAnimationBoneConfig::KeyFrameKind::POSITION)
				{
					positionKeyFrames.push_back(
						std::make_pair(
							keyFrame->time,
							keyFrame->vectorData
						)
					);
				}
				else if (keyFrame->kind == RenderAnimationBoneConfig::KeyFrameKind::ROTATION)
				{
					rotationKeyFrames.push_back(
						std::make_pair(
							keyFrame->time,
							keyFrame->quaternionData
						)
					);
				}
				else
				{
					scaleKeyFrames.push_back(
						std::make_pair(
							keyFrame->time,
							keyFrame->vectorData
						)
					);
				}
			}

			return *(new Impl(
				children,
				positionKeyFrames,
				rotationKeyFrames,
				scaleKeyFrames
			));
		}

		RenderAnimationBone::Impl::Impl(
			vector<RenderAnimationBone*>& children,
			vector<pair<float, glm::vec3>>& positionKeyFrames,
			vector<pair<float, glm::fquat>>& rotationKeyFrames,
			vector<pair<float, glm::vec3>>& scaleKeyFrames
		)
			:
			children(std::move(children)),
			positionKeyFrames(std::move(positionKeyFrames)),
			rotationKeyFrames(std::move(rotationKeyFrames)),
			scaleKeyFrames(std::move(scaleKeyFrames)) {}

		RenderAnimationBone::Impl::~Impl()
		{
			for (auto it = children.begin(); it != children.end(); ++it)
			{
				delete* it;
			}
		}

		bool RenderAnimationBone::TimeVecKeyFrameCmp(
			const pair<float, glm::vec3>& keyPairA,
			const pair<float, glm::vec3>& keyPairB
		)
		{
			return keyPairA.first < keyPairB.first;
		}

		bool RenderAnimationBone::TimeQuatKeyFrameCmp(
			const pair<float, glm::fquat>& keyPairA,
			const pair<float, glm::fquat>& keyPairB
		)
		{
			return keyPairA.first < keyPairB.first;
		}

		void RenderAnimationBone::GetBlendedStateAt(
			const RenderAnimationBone& a,
			const RenderAnimationBone& b,
			float timeA,
			float timeB,
			float blendFactor,
			const glm::mat4& parentMatrix,
			unordered_map<string, glm::mat4>& result
		)
		{
#ifdef __PRE_DEBUG__
			if (a.name != b.name || a._impl.children.size() != b._impl.children.size())
			{
				throw "Cannot blend animations with different skeletal structures";
			}
#endif
			// animation A
			auto timeVecNeedleA = std::make_pair(timeA, glm::vec3());
			auto timeQuatNeedleA = std::make_pair(timeA, glm::fquat());
			auto itPositionKeyFrameA = std::upper_bound(
				a._impl.positionKeyFrames.begin(),
				a._impl.positionKeyFrames.end(),
				timeVecNeedleA,
				RenderAnimationBone::TimeVecKeyFrameCmp
			);
			auto itRotationKeyFrameA = std::upper_bound(
				a._impl.rotationKeyFrames.begin(),
				a._impl.rotationKeyFrames.end(),
				timeQuatNeedleA,
				RenderAnimationBone::TimeQuatKeyFrameCmp
			);
			auto itScaleKeyFrameA = std::upper_bound(
				a._impl.scaleKeyFrames.begin(),
				a._impl.scaleKeyFrames.end(),
				timeVecNeedleA,
				RenderAnimationBone::TimeVecKeyFrameCmp
			);

			// calculate keyframe properties
			glm::vec3 positionVectorA;
			{
				auto tEnd = itPositionKeyFrameA->first;
				auto vEnd = itPositionKeyFrameA->second;
				--itPositionKeyFrameA;
				auto tStart = itPositionKeyFrameA->first;
				auto vStart = itPositionKeyFrameA->second;
				positionVectorA = glm::mix(
					vStart,
					vEnd,
					(timeA - tStart) / (tEnd - tStart)
				);
			}
			glm::fquat rotationQuatA;
			{
				auto tEnd = itRotationKeyFrameA->first;
				auto qEnd = itRotationKeyFrameA->second;
				--itRotationKeyFrameA;
				auto tStart = itRotationKeyFrameA->first;
				auto qStart = itRotationKeyFrameA->second;
				rotationQuatA = glm::slerp(
					qStart,
					qEnd,
					(timeA - tStart) / (tEnd - tStart)
				);
			}
			glm::vec3 scaleVectorA;
			{
				auto tEnd = itScaleKeyFrameA->first;
				auto vEnd = itScaleKeyFrameA->second;
				--itScaleKeyFrameA;
				auto tStart = itScaleKeyFrameA->first;
				auto vStart = itScaleKeyFrameA->second;
				scaleVectorA = glm::mix(
					vStart,
					vEnd,
					(timeA - tStart) / (tEnd - tStart)
				);
			}

			// animation B
			auto timeVecNeedleB = std::make_pair(timeB, glm::vec3());
			auto timeQuatNeedleB = std::make_pair(timeB, glm::fquat());
			auto itPositionKeyFrameB = std::upper_bound(
				b._impl.positionKeyFrames.begin(),
				b._impl.positionKeyFrames.end(),
				timeVecNeedleB,
				RenderAnimationBone::TimeVecKeyFrameCmp
			);
			auto itRotationKeyFrameB = std::upper_bound(
				b._impl.rotationKeyFrames.begin(),
				b._impl.rotationKeyFrames.end(),
				timeQuatNeedleB,
				RenderAnimationBone::TimeQuatKeyFrameCmp
			);
			auto itScaleKeyFrameB = std::upper_bound(
				b._impl.scaleKeyFrames.begin(),
				b._impl.scaleKeyFrames.end(),
				timeVecNeedleB,
				RenderAnimationBone::TimeVecKeyFrameCmp
			);

			// calculate keyframe properties
			glm::vec3 positionVectorB;
			{
				auto tEnd = itPositionKeyFrameB->first;
				auto vEnd = itPositionKeyFrameB->second;
				--itPositionKeyFrameB;
				auto tStart = itPositionKeyFrameB->first;
				auto vStart = itPositionKeyFrameB->second;
				positionVectorB = glm::mix(
					vStart,
					vEnd,
					(timeB - tStart) / (tEnd - tStart)
				);
			}
			glm::fquat rotationQuatB;
			{
				auto tEnd = itRotationKeyFrameB->first;
				auto qEnd = itRotationKeyFrameB->second;
				--itRotationKeyFrameB;
				auto tStart = itRotationKeyFrameB->first;
				auto qStart = itRotationKeyFrameB->second;
				rotationQuatB = glm::slerp(
					qStart,
					qEnd,
					(timeB - tStart) / (tEnd - tStart)
				);
			}
			glm::vec3 scaleVectorB;
			{
				auto tEnd = itScaleKeyFrameB->first;
				auto vEnd = itScaleKeyFrameB->second;
				--itScaleKeyFrameB;
				auto tStart = itScaleKeyFrameB->first;
				auto vStart = itScaleKeyFrameB->second;
				scaleVectorB = glm::mix(
					vStart,
					vEnd,
					(timeB - tStart) / (tEnd - tStart)
				);
			}

			// blend keyframes together
			auto currentMatrix = glm::translate(
				parentMatrix,
				glm::mix(positionVectorA, positionVectorB, blendFactor)
			);
			currentMatrix = currentMatrix * glm::mat4_cast(
				glm::slerp(rotationQuatA, rotationQuatB, blendFactor)
			);
			currentMatrix = glm::scale(
				currentMatrix,
				glm::mix(scaleVectorA, scaleVectorB, blendFactor)
			);

			result[a.name] = currentMatrix;

			for (auto i = 0; i < a._impl.children.size(); ++i)
			{
				GetBlendedStateAt(
					*a._impl.children[i],
					*b._impl.children[i],
					timeA,
					timeB,
					blendFactor,
					currentMatrix,
					result
				);
			}
		}

		RenderAnimationBone::RenderAnimationBone(
			const RenderAnimationBoneConfig& animationBoneConfig
		)
			:
			name(animationBoneConfig._name),
			_impl(Impl::MakeImpl(animationBoneConfig)) {}

		RenderAnimationBone::~RenderAnimationBone()
		{
			delete &_impl;
		}

		void RenderAnimationBone::GetStateAt(
			float time,
			const glm::mat4& parentMatrix,
			unordered_map<string, glm::mat4>& result
		) const
		{
			auto timeVecNeedle = std::make_pair(time, glm::vec3());
			auto timeQuatNeedle = std::make_pair(time, glm::fquat());
			auto itPositionKeyFrame = std::upper_bound(
				_impl.positionKeyFrames.begin(),
				_impl.positionKeyFrames.end(),
				timeVecNeedle,
				RenderAnimationBone::TimeVecKeyFrameCmp
			);
			auto itRotationKeyFrame = std::upper_bound(
				_impl.rotationKeyFrames.begin(),
				_impl.rotationKeyFrames.end(),
				timeQuatNeedle,
				RenderAnimationBone::TimeQuatKeyFrameCmp
			);
			auto itScaleKeyFrame = std::upper_bound(
				_impl.scaleKeyFrames.begin(),
				_impl.scaleKeyFrames.end(),
				timeVecNeedle,
				RenderAnimationBone::TimeVecKeyFrameCmp
			);

			// calculate keyframe properties
			glm::vec3 positionVector;
			{
				auto tEnd = itPositionKeyFrame->first;
				auto vEnd = itPositionKeyFrame->second;
				--itPositionKeyFrame;
				auto tStart = itPositionKeyFrame->first;
				auto vStart = itPositionKeyFrame->second;
				positionVector = glm::mix(
					vStart,
					vEnd,
					(time - tStart) / (tEnd - tStart)
				);
			}
			glm::fquat rotationQuat;
			{
				auto tEnd = itRotationKeyFrame->first;
				auto qEnd = itRotationKeyFrame->second;
				--itRotationKeyFrame;
				auto tStart = itRotationKeyFrame->first;
				auto qStart = itRotationKeyFrame->second;
				rotationQuat = glm::slerp(
					qStart,
					qEnd,
					(time - tStart) / (tEnd - tStart)
				);
			}
			glm::vec3 scaleVector;
			{
				auto tEnd = itScaleKeyFrame->first;
				auto vEnd = itScaleKeyFrame->second;
				--itScaleKeyFrame;
				auto tStart = itScaleKeyFrame->first;
				auto vStart = itScaleKeyFrame->second;
				scaleVector = glm::mix(
					vStart,
					vEnd,
					(time - tStart) / (tEnd - tStart)
				);
			}

			auto currentMatrix = glm::translate(parentMatrix, positionVector);
			currentMatrix = currentMatrix * glm::mat4_cast(rotationQuat);
			currentMatrix = glm::scale(currentMatrix, scaleVector);

			result[name] = currentMatrix;

			for (auto it = _impl.children.begin(); it != _impl.children.end(); ++it)
			{
				(*it)->GetStateAt(time, currentMatrix, result);
			}
		}
	} // namespace RenderingModule
} // namespace PRE