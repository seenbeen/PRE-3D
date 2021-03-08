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
				positionKeyFrames,
				rotationKeyFrames,
				scaleKeyFrames
			));
		}

		RenderAnimationBone::Impl::Impl(
			vector<pair<float, glm::vec3>>& positionKeyFrames,
			vector<pair<float, glm::fquat>>& rotationKeyFrames,
			vector<pair<float, glm::vec3>>& scaleKeyFrames
		)
			:
			positionKeyFrames(std::move(positionKeyFrames)),
			rotationKeyFrames(std::move(rotationKeyFrames)),
			scaleKeyFrames(std::move(scaleKeyFrames)),
			parentMatrix(),
			currentMatrix() {}

		RenderAnimationBone::Impl::~Impl() {}

		RenderAnimationBone::RenderAnimationBone(
			const RenderAnimationBoneConfig& animationBoneConfig
		)
			:
			_name(animationBoneConfig._name),
			_impl(Impl::MakeImpl(animationBoneConfig))
		{
			for (
				auto it = animationBoneConfig._children.begin();
				it != animationBoneConfig._children.end();
				++it
				)
			{
				_children.push_back(new RenderAnimationBone(**it));
			}
		}

		RenderAnimationBone::~RenderAnimationBone()
		{
			for (auto it = _children.begin(); it != _children.end(); ++it)
			{
				delete* it;
			}

			delete &_impl;
		}

		void RenderAnimationBone::SetTime(float time)
		{
			auto itPositionKeyFrame = std::find_if(
				_impl.positionKeyFrames.begin(),
				_impl.positionKeyFrames.end(),
				[&time](const pair<float, glm::vec3>& keyPair)
				{
					return keyPair.first > time;
				}
			);
			auto itRotationKeyFrame = std::find_if(
				_impl.rotationKeyFrames.begin(),
				_impl.rotationKeyFrames.end(),
				[&time](const pair<float, glm::fquat>& keyPair)
				{
					return keyPair.first > time;
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

			auto positionVector = itPositionKeyFrame != _impl.positionKeyFrames.end() ?
				itPositionKeyFrame->second :
				_impl.positionKeyFrames.rbegin()->second;
			auto rotationQuat = itRotationKeyFrame != _impl.rotationKeyFrames.end() ?
				itRotationKeyFrame->second :
				_impl.rotationKeyFrames.rbegin()->second;
			auto scaleVector = itScaleKeyFrame != _impl.scaleKeyFrames.end() ?
				itScaleKeyFrame->second :
				_impl.scaleKeyFrames.rbegin()->second;

			_impl.currentMatrix = glm::translate(_impl.parentMatrix, positionVector);
			_impl.currentMatrix = _impl.currentMatrix * glm::mat4_cast(rotationQuat);
			_impl.currentMatrix = glm::scale(_impl.currentMatrix, scaleVector);

			for (auto it = _children.begin(); it != _children.end(); ++it)
			{
				(*it)->SetTime(time);
			}
		}

		const glm::mat4& RenderAnimationBone::GetCurrentMatrix() const
		{
			return _impl.currentMatrix;
		}
	} // namespace RenderingModule
} // namespace PRE