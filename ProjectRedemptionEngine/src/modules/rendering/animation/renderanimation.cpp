#include <modules/rendering/animation/renderanimation.h>

#include <cmath>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include <modules/rendering/animation/renderanimationboneconfig.h>
#include <modules/rendering/animation/renderanimationbone.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::string;
		using std::unordered_map;

		RenderAnimation::Impl& RenderAnimation::Impl::MakeImpl(
			float ticksPerSecond,
			float duration,
			RenderAnimationBoneConfig& rootBoneConfig
		)
		{
			auto pRootBone = new RenderAnimationBone(rootBoneConfig);
			return *(new Impl(ticksPerSecond, duration, *pRootBone));
		}

		RenderAnimation::Impl::Impl(
			float ticksPerSecond,
			float duration,
			RenderAnimationBone& rootBone
		)
			:
			ticksPerSecond(ticksPerSecond),
			duration(duration),
			rootBone(rootBone) {}

		RenderAnimation::Impl::~Impl()
		{
			delete &rootBone;
			// do not delete boneMap; rootBone will
			// recursively take care of all of its children
		}


		void RenderAnimation::GetBlendedStateAt(
			const RenderAnimation& a,
			const RenderAnimation& b,
			float timeA,
			float timeB,
			float blendFactor,
			unordered_map<string, glm::mat4>& result
		)
		{
			timeA = std::fmod(timeA * a._impl.ticksPerSecond, a._impl.duration);
			timeB = std::fmod(timeB * a._impl.ticksPerSecond, b._impl.duration);
			blendFactor = std::fmod(blendFactor, 1.0f);

			RenderAnimationBone::GetBlendedStateAt(
				a._impl.rootBone,
				b._impl.rootBone,
				timeA,
				timeB,
				blendFactor,
				glm::mat4(),
				result
			);
		}

		float RenderAnimation::GetDuration() const
		{
			return _impl.duration / _impl.ticksPerSecond;
		}

		void RenderAnimation::GetStateAt(
			float time,
			unordered_map<string, glm::mat4>& result
		) const
		{
			time = std::fmod(time * _impl.ticksPerSecond, _impl.duration);
			_impl.rootBone.GetStateAt(time, glm::mat4(), result);
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