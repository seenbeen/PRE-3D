#include <modules/rendering/animation/renderanimation.h>

#include <cmath>
#include <queue>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include <modules/rendering/animation/renderanimationboneconfig.h>
#include <modules/rendering/animation/renderanimationbone.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::queue;
		using std::string;
		using std::unordered_map;

		RenderAnimation::Impl& RenderAnimation::Impl::MakeImpl(
			float ticksPerSecond,
			float duration,
			RenderAnimationBoneConfig& rootBoneConfig
		)
		{
			auto pRootBone = new RenderAnimationBone(rootBoneConfig);
			
			unordered_map<string, RenderAnimationBone*> boneMap;

			// bfs through pRootBone's descendants and map them via name
			queue<RenderAnimationBone*> boneQueue;
			boneQueue.push(pRootBone);
			while (!boneQueue.empty())
			{
				auto pCurrent = boneQueue.front();
				boneQueue.pop();

				boneMap[pCurrent->_name] = pCurrent;

				for (
					auto it = pCurrent->_children.begin();
					it != pCurrent->_children.end();
					++it
				)
				{
					boneQueue.push(*it);
				}
			}
		}

		RenderAnimation::Impl::Impl(
			float ticksPerSecond,
			float duration,
			RenderAnimationBone& rootBone,
			unordered_map<string, RenderAnimationBone*>& boneMap
		)
			:
			ticksPerSecond(ticksPerSecond),
			duration(duration),
			rootBone(rootBone),
			boneMap(std::move(boneMap)) {}

		RenderAnimation::Impl::~Impl()
		{
			delete &rootBone;
			// do not delete boneMap; rootBone will
			// recursively take care of all of its children
		}

		void RenderAnimation::SetTime(float time)
		{
			_impl.rootBone.SetTime(std::fmod(time * _impl.ticksPerSecond, _impl.duration));
		}

		float RenderAnimation::GetDuration()
		{
			return _impl.duration / _impl.ticksPerSecond;
		}

		const glm::mat4& RenderAnimation::GetCurrentBoneMatrix(const string& boneName) const
		{
#ifdef __PRE_DEBUG__
			if (_impl.boneMap.find(boneName) == _impl.boneMap.end())
			{
				throw "Non-existant AnimationBone matrix requested";
			}
#endif

			return _impl.boneMap.find(boneName)->second->GetCurrentMatrix();
		}

		RenderAnimation::RenderAnimation(
			float ticksPerSecond,
			float duration,
			RenderAnimationBoneConfig& rootBoneConfig
		)
			:
			_impl(Impl::MakeImpl(ticksPerSecond, duration, rootBoneConfig)) {}

		RenderAnimation::~RenderAnimation()
		{
			delete &_impl;
		}
	} // namespace RenderingModule
} // namespace PRE