#include <modules/rendering/animation/renderanimation.h>

#include <cmath>
#include <unordered_map>

#include <glm/glm.hpp>

#include <modules/rendering/animation/renderanimationconfig.h>
#include <modules/rendering/animation/renderanimationchannelconfig.h>
#include <modules/rendering/animation/renderanimationchannel.h>

namespace PRE
{
	namespace RenderingModule
	{
		using std::unordered_map;

		RenderAnimation::Impl& RenderAnimation::Impl::MakeImpl(
			float ticksPerSecond,
			float duration,
			const RenderAnimationConfig& animationConfig
		)
		{
			unordered_map<int, RenderAnimationChannel*> channels;
			for (
				auto it = animationConfig._animationChannelConfigs.begin();
				it != animationConfig._animationChannelConfigs.end();
				++it
			)
			{
				channels[(*it)->id] = new RenderAnimationChannel(**it);
			}
			return *(new Impl(ticksPerSecond, duration, channels));
		}

		RenderAnimation::Impl::Impl(
			float ticksPerSecond,
			float duration,
			unordered_map<int, RenderAnimationChannel*>& channels
		)
			:
			ticksPerSecond(ticksPerSecond),
			duration(duration),
			channels(std::move(channels)) {}

		RenderAnimation::Impl::~Impl()
		{
			for (auto it = channels.begin(); it != channels.end(); ++it)
			{
				delete it->second;
			}
		}

		void RenderAnimation::GetBlendedStateAt(
			const RenderAnimation& a,
			const RenderAnimation& b,
			float timeA,
			float timeB,
			float blendFactor,
			unordered_map<int, glm::mat4>& result
		)
		{
			result.clear();

			timeA = std::fmod(timeA * a._impl.ticksPerSecond, a._impl.duration);
			timeB = std::fmod(timeB * a._impl.ticksPerSecond, b._impl.duration);
			blendFactor = std::fmod(blendFactor, 1.0f);

			// arbitrarily use a as baseline
			for (
				auto itA = a._impl.channels.begin();
				itA != a._impl.channels.end();
				++itA
			)
			{
				auto itB = b._impl.channels.find(itA->first);
				if (itB != b._impl.channels.end())
				{
					result[itA->first] = RenderAnimationChannel::GetBlendedStateAt(
						*itA->second,
						*itB->second,
						timeA,
						timeB,
						blendFactor
					);
				}
				else
				{
					// take 100% of A's state if channel doesn't exist in B
					result[itA->first] = itA->second->GetStateAt(timeA);
				}
			}

			// fill in all leftover channels from b
			for (
				auto itB = b._impl.channels.begin();
				itB != b._impl.channels.end();
				++itB
			)
			{
				auto itA = a._impl.channels.find(itB->first);
				// this time we're only interested in channels only in B
				if (itA == a._impl.channels.end())
				{
					result[itB->first] = itB->second->GetStateAt(timeB);
				}
			}
		}

		float RenderAnimation::GetDuration() const
		{
			return _impl.duration / _impl.ticksPerSecond;
		}

		void RenderAnimation::GetStateAt(
			float time,
			unordered_map<int, glm::mat4>& result
		) const
		{
			result.clear();
			time = std::fmod(time * _impl.ticksPerSecond, _impl.duration);
			for (auto it = _impl.channels.begin(); it != _impl.channels.end(); ++it)
			{
				result[it->first] = it->second->GetStateAt(time);
			}
		}

		RenderAnimation::RenderAnimation(
			float ticksPerSecond,
			float duration,
			RenderAnimationConfig& animationConfig
		)
			:
			_impl(Impl::MakeImpl(ticksPerSecond, duration, animationConfig)) {}

		RenderAnimation::~RenderAnimation()
		{
			delete &_impl;
		}
	} // namespace RenderingModule
} // namespace PRE