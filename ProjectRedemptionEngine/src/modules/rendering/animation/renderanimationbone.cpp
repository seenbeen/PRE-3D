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
				[&](const RenderAnimationBone* a, const RenderAnimationBone* b)
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

		// TODO: BINARY-SEARCH FOR TIME KEYS.

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
			auto itPositionKeyFrameA = std::find_if(
				a._impl.positionKeyFrames.begin(),
				a._impl.positionKeyFrames.end(),
				[&timeA](const pair<float, glm::vec3>& keyPair)
				{
					return keyPair.first > timeA;
				}
			);
			auto itRotationKeyFrameA = std::find_if(
				a._impl.rotationKeyFrames.begin(),
				a._impl.rotationKeyFrames.end(),
				[&timeA](const pair<float, glm::fquat>& keyPair)
				{
					return keyPair.first > timeA;
				}
			);
			auto itScaleKeyFrameA = std::find_if(
				a._impl.scaleKeyFrames.begin(),
				a._impl.scaleKeyFrames.end(),
				[&timeA](const pair<float, glm::vec3>& keyPair)
				{
					return keyPair.first >= timeA;
				}
			);

			// calculate keyframe properties
			glm::vec3 positionVectorA;
			{
				auto tA = itPositionKeyFrameA->first;
				auto vA = itPositionKeyFrameA->second;
				++itPositionKeyFrameA;
				auto tB = itPositionKeyFrameA->first;
				auto vB = itPositionKeyFrameA->second;
				positionVectorA = glm::mix(
					vA,
					vB,
					(timeA - tA) / (tB - tA)
				);
			}
			glm::fquat rotationQuatA;
			{
				auto tA = itRotationKeyFrameA->first;
				auto qA = itRotationKeyFrameA->second;
				++itRotationKeyFrameA;
				auto tB = itRotationKeyFrameA->first;
				auto qB = itRotationKeyFrameA->second;
				rotationQuatA = glm::slerp(
					qA,
					qB,
					(timeA - tA) / (tB - tA)
				);
			}
			glm::vec3 scaleVectorA;
			{
				auto tA = itScaleKeyFrameA->first;
				auto vA = itScaleKeyFrameA->second;
				++itScaleKeyFrameA;
				auto tB = itScaleKeyFrameA->first;
				auto vB = itScaleKeyFrameA->second;
				scaleVectorA = glm::mix(
					vA,
					vB,
					(timeA - tA) / (tB - tA)
				);
			}

			// animation B
			auto itPositionKeyFrameB = std::find_if(
				b._impl.positionKeyFrames.begin(),
				b._impl.positionKeyFrames.end(),
				[&timeB](const pair<float, glm::vec3>& keyPair)
				{
					return keyPair.first >= timeB;
				}
			);
			auto itRotationKeyFrameB = std::find_if(
				b._impl.rotationKeyFrames.begin(),
				b._impl.rotationKeyFrames.end(),
				[&timeB](const pair<float, glm::fquat>& keyPair)
				{
					return keyPair.first >= timeB;
				}
			);
			auto itScaleKeyFrameB = std::find_if(
				b._impl.scaleKeyFrames.begin(),
				b._impl.scaleKeyFrames.end(),
				[&timeB](const pair<float, glm::vec3>& keyPair)
				{
					return keyPair.first >= timeB;
				}
			);

			// calculate keyframe properties
			glm::vec3 positionVectorB;
			{
				auto tA = itPositionKeyFrameB->first;
				auto vA = itPositionKeyFrameB->second;
				++itPositionKeyFrameB;
				auto tB = itPositionKeyFrameB->first;
				auto vB = itPositionKeyFrameB->second;
				positionVectorB = glm::mix(
					vA,
					vB,
					(timeB - tA) / (tB - tA)
				);
			}
			glm::fquat rotationQuatB;
			{
				auto tA = itRotationKeyFrameB->first;
				auto qA = itRotationKeyFrameB->second;
				++itRotationKeyFrameB;
				auto tB = itRotationKeyFrameB->first;
				auto qB = itRotationKeyFrameB->second;
				rotationQuatB = glm::slerp(
					qA,
					qB,
					(timeB - tA) / (tB - tA)
				);
			}
			glm::vec3 scaleVectorB;
			{
				auto tA = itScaleKeyFrameB->first;
				auto vA = itScaleKeyFrameB->second;
				++itScaleKeyFrameB;
				auto tB = itScaleKeyFrameB->first;
				auto vB = itScaleKeyFrameB->second;
				scaleVectorB = glm::mix(
					vA,
					vB,
					(timeB - tA) / (tB - tA)
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
			auto itPositionKeyFrame = std::find_if(
				_impl.positionKeyFrames.begin(),
				_impl.positionKeyFrames.end(),
				[&time](const pair<float, glm::vec3>& keyPair)
				{
					return keyPair.first >= time;
				}
			);
			auto itRotationKeyFrame = std::find_if(
				_impl.rotationKeyFrames.begin(),
				_impl.rotationKeyFrames.end(),
				[&time](const pair<float, glm::fquat>& keyPair)
				{
					return keyPair.first >= time;
				}
			);
			auto itScaleKeyFrame = std::find_if(
				_impl.scaleKeyFrames.begin(),
				_impl.scaleKeyFrames.end(),
				[&time](const pair<float, glm::vec3>& keyPair)
				{
					return keyPair.first >= time;
				}
			);

			// calculate keyframe properties
			glm::vec3 positionVector;
			{
				auto tA = itPositionKeyFrame->first;
				auto vA = itPositionKeyFrame->second;
				++itPositionKeyFrame;
				auto tB = itPositionKeyFrame->first;
				auto vB = itPositionKeyFrame->second;
				positionVector = glm::mix(
					vA,
					vB,
					(time - tA) / (tB - tA)
				);
			}
			glm::fquat rotationQuat;
			{
				auto tA = itRotationKeyFrame->first;
				auto qA = itRotationKeyFrame->second;
				++itRotationKeyFrame;
				auto tB = itRotationKeyFrame->first;
				auto qB = itRotationKeyFrame->second;
				rotationQuat = glm::slerp(
					qA,
					qB,
					(time - tA) / (tB - tA)
				);
			}
			glm::vec3 scaleVector;
			{
				auto tA = itScaleKeyFrame->first;
				auto vA = itScaleKeyFrame->second;
				++itScaleKeyFrame;
				auto tB = itScaleKeyFrame->first;
				auto vB = itScaleKeyFrame->second;
				scaleVector = glm::mix(
					vA,
					vB,
					(time - tA) / (tB - tA)
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